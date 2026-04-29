# UART/CAN Bootloader

This bootloader lets a board update its app over UART, or over classic CAN on
CAN-capable targets, after the bootloader has been installed once. Apps can
return to the bootloader through either the UART magic packet or a CAN/FDCAN boot
command frame. Normal firmware builds still work the old way by default: if you
do not specify a firmware type, the image is linked and flashed at `0x08000000`.

## Firmware Types

Use `FIRMWARE_TYPE` to select the memory map and default flash address:

- `firmware`: standalone application at `0x08000000`. This is the default.
- `bootloader`: resident bootloader at `0x08000000`, default size `64 KB`.
- `app`: app linked after the resident bootloader, default base `0x08010000`.

Legacy `FIRMWARE_ROLE=app` and `FIRMWARE_ROLE=bootloader` invocations still work,
but new commands should use `FIRMWARE_TYPE`.

Defaults can be overridden:

- `BOOTLOADER_SIZE_KB`: default `0` for `firmware`, `64` for `app` and `bootloader`.
- `BOOTLOADER_APP_BASE`: default `FLASH_BASE + BOOTLOADER_SIZE_KB * 1024`.
- `FLASH_ADDRESS`: default `0x08000000` for `firmware` and `bootloader`,
  default `BOOTLOADER_APP_BASE` for `app`.
- `FLASH_BASE`: default `0x08000000`.

## Build

Run these commands from inside `Embedded-Sharepoint` or from a parent Makefile
that forwards the required project variables into `Embedded-Sharepoint/Makefile`.

Standalone firmware, no bootloader:

```bash
make -C test clean TEST=blinky PROJECT_TARGET=stm32g473xx BEAR_ENABLE=0
make -C test TEST=blinky PROJECT_TARGET=stm32g473xx BEAR_ENABLE=0
```

Bootloader image:

```bash
make -C test clean TEST=main PROJECT_TARGET=stm32g473xx FIRMWARE_TYPE=bootloader BEAR_ENABLE=0
make -C test TEST=main PROJECT_TARGET=stm32g473xx FIRMWARE_TYPE=bootloader BEAR_ENABLE=0
```

App image for a board using the resident bootloader:

```bash
make -C test clean TEST=blinky_bootloader PROJECT_TARGET=stm32g473xx FIRMWARE_TYPE=app BEAR_ENABLE=0
make -C test TEST=blinky_bootloader PROJECT_TARGET=stm32g473xx FIRMWARE_TYPE=app BEAR_ENABLE=0
```

For parent repos, the same procedure is:

```bash
make clean FIRMWARE_TYPE=app
make FIRMWARE_TYPE=app
make flash-uart FIRMWARE_TYPE=app
```

Always run `make clean` when switching between `firmware`, `bootloader`, and
`app`; each mode uses a different flash map and stale build outputs can flash
the wrong image.

## Parent Repo Integration

Application repos usually live outside this shared embedded repo. The shared
Makefile is designed for that: the parent repo passes absolute app sources,
include paths, target, build directory, and firmware type into this repo.

Required variables:

- `PROJECT_TARGET`: MCU target, for example `stm32g473xx`.
- `PROJECT_C_SOURCES`: app source files from the parent repo.
- `PROJECT_C_INCLUDES`: app include directories from the parent repo.
- `PROJECT_BUILD_DIR`: output directory for objects, ELF, HEX, and BIN.
- `FIRMWARE_TYPE`: optional. Defaults to `firmware`; use `app` for a
  bootloader-linked image.

Bootloader-related overrides:

- `BOOTLOADER_SIZE_KB`: default `64` for `app` and `bootloader`.
- `BOOTLOADER_APP_BASE`: normally derived from `FLASH_BASE` and
  `BOOTLOADER_SIZE_KB`; only set this directly for a nonstandard memory map.
- `FLASH_ADDRESS`: normally derived from `FIRMWARE_TYPE`; only set this for a
  custom flash operation.
- `BOOTLOADER_CAN_ENABLE`: defaults to enabled when a classic `CAN1` peripheral
  exists. Set to `0` to build a UART-only resident bootloader.
- `BOOTLOADER_CAN_STARTUP_PRESCALER`, `BOOTLOADER_CAN_TIME_SEG1`,
  `BOOTLOADER_CAN_TIME_SEG2`, and `BOOTLOADER_CAN_SJW`: override the resident
  classic CAN startup timing. Defaults target the AN3154 startup behavior.

Parent Makefiles should not export empty bootloader override variables. For
example, do not export `BOOTLOADER_APP_BASE` unless you actually assign it,
because an empty exported value prevents the shared Makefile default from being
computed.

Example parent repo layout:

```text
my-app/
  Makefile
  Src/main.c
  Inc/app_config.h
  vendor/ES_BLT/
```

Example parent `Makefile`:

```make
ES_BLT_DIR := vendor/ES_BLT
PROJECT_TARGET ?= stm32g473xx
FIRMWARE_TYPE ?= firmware

PROJECT_C_SOURCES := $(abspath Src/main.c)
PROJECT_C_INCLUDES := $(abspath Inc)

ifeq ($(FIRMWARE_TYPE),app)
PROJECT_BUILD_DIR := $(abspath build/app)
else
PROJECT_BUILD_DIR := $(abspath build)
endif

export PROJECT_TARGET
export PROJECT_C_SOURCES
export PROJECT_C_INCLUDES
export PROJECT_BUILD_DIR
export FIRMWARE_TYPE

.PHONY: all clean flash flash-uart
all:
	$(MAKE) -C $(ES_BLT_DIR) all

clean:
	$(MAKE) -C $(ES_BLT_DIR) clean

flash:
	$(MAKE) -C $(ES_BLT_DIR) flash

flash-uart:
	$(MAKE) -C $(ES_BLT_DIR) flash-uart
```

With that wrapper:

```bash
# Old behavior: standalone app linked and flashed at 0x08000000.
make
make flash

# Bootloader app: linked at 0x08010000 by default.
make clean FIRMWARE_TYPE=app
make FIRMWARE_TYPE=app
make flash-uart FIRMWARE_TYPE=app
```

For a custom bootloader size, override only `BOOTLOADER_SIZE_KB`; the app base
and flash address follow automatically:

```bash
make FIRMWARE_TYPE=app BOOTLOADER_SIZE_KB=96
```

If a parent repo needs a fixed nonstandard app base, override
`BOOTLOADER_APP_BASE` directly.

## App Requirements

Apps that run behind the resident bootloader need these pieces:

- Build with `FIRMWARE_TYPE=app` so the linker script starts flash at
  `BOOTLOADER_APP_BASE`.
- Use the standard Embedded-Sharepoint startup files, which set `SCB->VTOR` to
  `BOOTLOADER_APP_BASE` during `SystemInit()` for bootloader app builds.
- Optionally call `bootloader_command_init_app_vector_table()` at the start of
  `main()` as an explicit second set.
- Initialize whichever command interfaces the app supports.

For UART entry, service the UART bootloader command parser on the same UART used
by the bootloader:

```c
while (1) {
    (void)uart_boot_command_service(husart3, portMAX_DELAY);
}
```

`uart_boot_command_service()` waits for one byte, then drains any bytes already
queued by the UART ISR. This keeps parent apps tolerant of host tools that send
the `ESBLT_BOOT\n` command as a short burst.

For CAN or FDCAN entry, initialize and start the normal BSP driver. The boot
command is handled in the CAN/FDCAN RX ISR path before normal queue delivery, so
the app does not need to register a receive queue for the command ID:

```c
// G4 FDCAN apps
can_fd_init(hfdcan1, &filter);
can_fd_start(hfdcan1);

// L4 bxCAN apps
can_init(hcan1, &filter);
can_start(hcan1);
```

The app's CAN filter must allow `BOOTLOADER_CAN_COMMAND_ID`, which defaults to
standard ID `0x6F0`. This keeps the app-side boot-control frame out of the
AN3154 command/data ID range.

If the app cannot safely reset at some point, gate command entry:

```c
bootloader_command_set_entry_allowed(false);
```

Hard faults still force bootloader entry for apps built with
`FIRMWARE_TYPE=app`; standalone apps keep the normal debug hardfault behavior.

## Generic Bring-Up Procedure

Before enabling a full vehicle or board application behind the bootloader, make
a minimal app in the parent repo and build it with `FIRMWARE_TYPE=app`.

The bring-up app should:

- Use the standard Embedded-Sharepoint startup files so `SystemInit()` sets the
  app vector table before `main()`.
- Optionally call `bootloader_command_init_app_vector_table()` before `HAL_Init()`
  as an explicit second set.
- Initialize the same UART pins and instance used by the resident bootloader if
  using UART app-to-bootloader entry.
- Run `uart_boot_command_service()` in a task or main-loop path if using UART, so
  `bootloader/scripts/uart_bootloader_flash.py --enter --boot` can return the
  board to update mode.
- Initialize/start CAN or FDCAN and accept `BOOTLOADER_CAN_COMMAND_ID` if using
  CAN app-to-bootloader entry.
- Blink an LED or use another board-local indicator so you can confirm the app
  jumped successfully.
- Keep only peripherals needed for bench validation. Gate or stub hardware
  dependencies that can fault when sensors, contactors, transceivers, or other
  board connections are absent.
- Add one low-risk peripheral heartbeat, such as a UART print or CAN frame, if
  that peripheral is part of the bootloader update path or early board bring-up.

After the minimal app can be flashed, reset, and re-enter the bootloader, move
the same `FIRMWARE_TYPE=app`, vector-table init, and command-interface changes
into the normal application startup path.

## Flash

Standalone firmware uses the normal ST-Link flow and flashes to `0x08000000`:

```bash
make -C test flash TEST=blinky PROJECT_TARGET=stm32g473xx BEAR_ENABLE=0
```

Install the resident bootloader once using STM32 ROM boot mode:

```bash
./bootloader/scripts/flash_bootloader.py
```

If autodetection picks the wrong UART, pass the port explicitly:

```bash
./bootloader/scripts/flash_bootloader.py --port /dev/cu.usbserial-310
```

Flash an app through the resident bootloader:

```bash
./bootloader/scripts/uart_bootloader_flash.py --boot
```

If the app is already running and supports the ES_BLT magic packet, ask it to
reset into the bootloader first:

```bash
./bootloader/scripts/uart_bootloader_flash.py --enter --boot
```

Only enter bootloader without flashing:

```bash
./bootloader/scripts/uart_bootloader_enter.py
```

All UART scripts accept `--port` and `--baud`; the flash script also accepts
path/address overrides. The entry scripts send `ESBLT_BOOT\n` byte-by-byte by
default to avoid overrunning simple app-side UART command paths. Use
`--byte-delay <seconds>` if a board needs a different spacing.

Classic CAN flashing is handled by the resident bootloader using the STM32
AN3154 protocol. Use STM32CubeProgrammer's CAN bootloader flow for CAN-capable
targets such as L4:

```text
Protocol: STM32 CAN bootloader / AN3154
Startup:  standard ID 0x079
Default bitrate: 125 kbit/s target timing, configurable in bootloader_config.h
```

The CAN implementation uses the HAL directly inside the resident bootloader. It
does not depend on the FreeRTOS CAN BSP queues used by normal applications.

For PEAK PCAN or other `python-can` backends, use the custom CAN flashing
script:

```bash
./bootloader/scripts/can_bootloader_flash.py --bin build/app/stm32l431cbt.bin --boot
```

The default backend is `pcan` on `PCAN_USBBUS1` at `125000` bit/s. Override
those when needed:

```bash
./bootloader/scripts/can_bootloader_flash.py --interface socketcan --channel can0 --bitrate 125000 --boot
```

If the app is still running, send the app-side CAN boot-control value first:

```bash
./bootloader/scripts/can_bootloader_flash.py --enter-value 0x12 --bin build/app/stm32l431cbt.bin --boot
```

After `--enter-value`, the script waits for a resident bootloader health
handshake before starting AN3154 sync. The bootloader sends 50 frames on
`BOOTLOADER_CAN_HANDSHAKE_ID` (`0x6F1` by default), spaced 10 ms apart, with
byte 0 set to the requested board ID. The script reports time to first
handshake frame, frames received out of 50, average interval, and average jitter.
If the board is already in bootloader because no valid app exists, the handshake
value is `0x00` and flashing can still proceed.

If the board is already in bootloader and you want the health check without
sending a boot-control frame:

```bash
./bootloader/scripts/can_bootloader_flash.py --wait-handshake --bin build/app/stm32l431cbt.bin --boot
```

Send reset-all without flashing:

```bash
./bootloader/scripts/can_bootloader_flash.py --reset-all
```

## Runtime Entry Policy

Apps can deny command-based bootloader entry while they are in an unsafe state:

```c
bootloader_command_set_entry_allowed(false);
```

Set it back to true when the app is safe to reset:

```c
bootloader_command_set_entry_allowed(true);
```

Hard faults still force bootloader entry for apps built with `FIRMWARE_TYPE=app`.
Standalone `FIRMWARE_TYPE=firmware` apps keep the normal debug hardfault handler.

## Wire Protocol

The resident bootloader speaks the STM32 AN3155 UART protocol subset used by
STM32CubeProgrammer:

- UART format: `115200 8E1`.
- Handshake: host sends `0x7F`, bootloader replies `ACK(0x79)`.
- Supported commands: Get, Get Version, Get ID, Read Memory, Go, Write Memory,
  Erase, and Extended Erase.

Erase and write operations are constrained to the app region so the resident
bootloader is not erased by CubeProgrammer special erase requests.

On targets with classic CAN enabled, the resident bootloader also speaks the
STM32 AN3154 CAN protocol subset:

- Startup sync: host sends a standard CAN frame with ID `0x079`.
- ACK/NACK: one-byte data frames with `0x79` or `0x1F`.
- Commands: Get, Get Version, Get ID, Read Memory, Go, Write Memory, and Erase.
- Data chunks: up to 8 bytes per classic CAN data frame.

Erase and write operations are still constrained to the app region.

Apps enter the bootloader through the common UART command packet:

```text
ESBLT_BOOT\n
```

Apps can also react to a boot-control CAN data frame, received by either L4
bxCAN or G4 FDCAN. The node-ID decision is made only in the application; the
resident bootloader only consumes the persisted magic value written by the app.

```text
Standard ID: BOOTLOADER_CAN_COMMAND_ID, default 0x6F0
Byte 0:      0x00                      app clears magic and resets normally
Byte 0:      BOOTLOADER_CAN_NODE_ID    app writes flash magic and resets
Byte 0:      any other nonzero value   app writes hold-only magic and resets
```

`BOOTLOADER_CAN_NODE_ID` is intended to be overridden by the parent application
or build system from the board ID value table in the DBC. The default `0xFF`
keeps bench tests simple because it is the maximum 8-bit value.

The hold-only mode is useful when flashing one node on a shared CAN bus. All
boards can be silenced into their resident bootloader so normal application
traffic stops, but only the board whose `BOOTLOADER_CAN_NODE_ID` matches the
command value writes the flashable bootloader magic. The bootloader itself is
ID-agnostic and only distinguishes normal boot, hold-only bootloader entry, and
flashable bootloader entry by the magic value stored before reset.

While the resident bootloader is idle waiting for a host connection, it also
honors the reset-all form (`0x6F0` with byte `0x00`) and resets back to normal
boot. It does not process that control frame during an active flashing session.

On bootloader entry, the resident bootloader emits a CAN health handshake before
AN3154 sync:

```text
Standard ID: BOOTLOADER_CAN_HANDSHAKE_ID, default 0x6F1
Count:       BOOTLOADER_CAN_HANDSHAKE_COUNT, default 50
Spacing:     BOOTLOADER_CAN_HANDSHAKE_INTERVAL_MS, default 10 ms
Byte 0:      requested board ID, or 0x00 when no app/default bootloader entry
```
