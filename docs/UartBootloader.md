# UART Bootloader

This bootloader is designed for the shared Embedded Sharepoint repo so each firmware project can reuse the same UART flashing flow.

## Behavior

- Runs from flash start (`0x08000000` by default).
- Listens on the configured bootloader UART at `115200 8E1`.
- LED behavior in bootloader:
  - Waiting for host connection: fade in/out.
  - Flashing: rapid blink.
- If no handshake arrives before timeout and a valid app exists, it jumps to the app.
- Speaks the STM32 AN3155 UART bootloader protocol subset used by STM32CubeProgrammer.

## Memory Map

- Bootloader region: `0x08000000` .. `0x0800FFFF` (default 64 KB).
- App region: `0x08010000` .. end of flash.

Override by build variables:

- `BOOTLOADER_SIZE_KB`
- `BOOTLOADER_APP_BASE` (compile-time macro)
- `BOOTLOADER_APP_MAX_SIZE` (compile-time macro)

## Build

Bootloader image:

```bash
make -C test clean TEST=main PROJECT_TARGET=stm32g473xx FIRMWARE_ROLE=bootloader BEAR_ENABLE=0
make -C test TEST=main PROJECT_TARGET=stm32g473xx FIRMWARE_ROLE=bootloader BEAR_ENABLE=0
```

App image (linked after bootloader):

```bash
make -C test clean TEST=blinky_bootloader PROJECT_TARGET=stm32g473xx FIRMWARE_ROLE=app BOOTLOADER_SIZE_KB=64 BEAR_ENABLE=0
make -C test TEST=blinky_bootloader PROJECT_TARGET=stm32g473xx FIRMWARE_ROLE=app BOOTLOADER_SIZE_KB=64 BEAR_ENABLE=0
```

## Flash Flow

### 1) Initial bootloader flash

Use STM32 ROM boot mode (BOOT pin/switch) and flash bootloader at `0x08000000`.

You can use existing helper:

```bash
./flash-uart.sh build/stm32g473xx.bin 0x08000000
```

### 2) Subsequent app updates through bootloader

With board running bootloader (or reset into bootloader), run:

```bash
python3 scripts/uart_bootloader_flash.py --port /dev/tty.usbserial-310 --bin build/app/stm32g473xx.bin --boot
```

## Wire Protocol (host <-> bootloader)

- Handshake:
  - Host sends `0x7F`
  - Bootloader replies `ACK(0x79)`
- UART format:
  - `115200 8E1`
- Command packets follow AN3155:
  - `CMD`, then bitwise-complement command byte.
  - Multi-byte addresses are sent MSB first.
  - Checksums are XOR bytes, matching AN3155.

Commands:

- `0x00`: Get
- `0x01`: Get Version
- `0x02`: Get ID
- `0x11`: Read Memory
- `0x21`: Go
- `0x31`: Write Memory
- `0x43`: Erase
- `0x44`: Extended Erase

The implementation constrains erase and write operations to the app region so the resident
bootloader is not erased by CubeProgrammer special erase requests.
