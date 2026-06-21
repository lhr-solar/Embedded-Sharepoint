# Host flashing tools

Two host-side tools for the resident CAN/DFU bootloader. Cross-platform
(macOS / Linux / WSL). Both run on plain Python 3 for `--help`, `--dry-run`,
and the CRC self-test; their heavy dependencies are imported/located lazily.

- `can_flash.py` — flash a `.bin` over **CAN** using the bootloader protocol
  (canonical wire format: `../contract/bl_protocol.schema.json`). Needs the
  [`python-can`](https://python-can.readthedocs.io/) package only when it opens
  the bus.
- `dfu_flash.py` — flash over **USB DFU** by wrapping `dfu-util` against the
  STM32 system bootloader (`0483:df11`). Needs the `dfu-util` binary only when
  it actually transfers.

## Prerequisites

`python-can` and `dfu-util` are provided by the dev shell:

```bash
nix develop
```

## App base addresses

| target          | slot A       | slot B       |
| --------------- | ------------ | ------------ |
| `stm32g491vet`  | `0x08004000` | `0x08040000` |
| `stm32g431cbt`  | `0x08002000` | —            |

## can_flash.py

Flow: `GET_INFO → ENTER → ERASE → SET_ADDR → WRITE_BLOCK… → VERIFY_CRC32
→ COMMIT → RESET`. The device is authoritative for CRC verification.

```bash
# CRC-32/MPEG-2 self-test (no CAN, no deps) -> prints PASS, exit 0
python3 bootloader/tools/can_flash.py --selftest

# List the available transport adapters (no deps)
python3 bootloader/tools/can_flash.py --list-adapters

# Preview the exact frames without touching the bus
python3 bootloader/tools/can_flash.py --device-id 5 --bin app.bin --slot A --dry-run

# Flash over a Peak PCAN-USB dongle (channel defaults to PCAN_USBBUS1)
python3 bootloader/tools/can_flash.py --device-id 5 --bin app.bin --slot A --adapter pcan

# Flash over Linux SocketCAN can0
python3 bootloader/tools/can_flash.py --device-id 5 --bin app.bin --slot A --adapter socketcan --channel can0
```

Use `--base-addr 0x08002000` instead of `--slot` for single-slot targets
(e.g. `stm32g431cbt`).

### CAN transport adapters

How the bytes reach the bus is decoupled from the bootloader protocol. Pick a
transport with `--adapter` (see `--list-adapters`):

| adapter     | python-can interface | default channel | notes                                   |
| ----------- | -------------------- | --------------- | --------------------------------------- |
| `socketcan` | `socketcan`          | — (`--channel`) | Linux SocketCAN (default)               |
| `pcan`      | `pcan`               | `PCAN_USBBUS1`  | Peak PCAN-USB; second device `PCAN_USBBUS2` |
| `slcan`     | `slcan`              | — (`--channel`) | serial CAN dongle, e.g. `/dev/ttyACM0`  |
| `virtual`   | `virtual`            | `vcan-test`     | in-process loopback (testing, no HW)    |

`--interface`/`--channel` override an adapter's defaults for one-off setups.

**Peak PCAN driver:** `--adapter pcan` needs the Peak host driver — PCAN-Basic on
Windows/Linux, or the [PCBUSB](https://github.com/mac-can/PCBUSB-Library)
(`libPCBUSB`) library on macOS. On Linux you can alternatively use the kernel
`peak_usb` driver and flash via `--adapter socketcan --channel can0`.

**Adding a new way to get bytes onto the bus:** if python-can already supports
it, add one line to `ADAPTERS` in `can_flash.py`. If it is bespoke (a relay node,
a non-python-can dongle), subclass `CanLink` (`send_frame` / `recv_frame` /
`close`) and return it from `open_link()`. The protocol layer and `flash()` need
no changes.

## dfu_flash.py

The device must **already be in ROM-DFU** (app calls `EnterRomDfu()`
or BOOT0 strap + reset); this tool only performs the transfer.

```bash
# List attached DFU devices
python3 bootloader/tools/dfu_flash.py --list

# Preview the dfu-util command
python3 bootloader/tools/dfu_flash.py --bin app.bin --slot A --target stm32g491vet --dry-run

# Transfer
python3 bootloader/tools/dfu_flash.py --bin app.bin --address 0x08004000
```

## Makefile shortcuts

```bash
# CAN_ARGS is passed straight through, so any adapter/flag works:
make -f sharepoint.mk can-flash DEVICE_ID=5 BIN=app.bin CAN_ARGS="--slot A --adapter pcan"
make -f sharepoint.mk can-flash DEVICE_ID=5 BIN=app.bin CAN_ARGS="--slot A --adapter socketcan --channel can0"
make -f sharepoint.mk dfu-flash BIN=app.bin DFU_ARGS="--slot A --target stm32g491vet"
```
