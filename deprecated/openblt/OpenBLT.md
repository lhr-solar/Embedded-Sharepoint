# OpenBLT Bootloader Bring-up (STM32 G4/L4)

This page sets up a low-risk, low-coupling OpenBLT workflow so we can validate bootloading first, before integrating deeply with Embedded-Sharepoint.

Current goal:
- Build app firmware as usual with Embedded-Sharepoint.
- Convert app image to a bootloader-friendly format (`.srec`).
- Flash app through OpenBLT over:
  - USB CDC (virtual COM port over USB-C)
  - CAN using PEAK PCAN adapter

## Scope and Integration Level

This setup is intentionally **lightly integrated**:
- No changes to existing build/flash behavior (`make`, `make flash` still work the same).
- OpenBLT-specific flow is isolated in `scripts/openblt/`.
- You can iterate on hardware and transport reliability before locking in architecture decisions.

## Prerequisites

Inside `nix develop`, these host-side utilities are now available:
- `srec_cat` (from `srecord`)
- Python packages: `pyserial`, `can` (PyPI name `python-can`), `intelhex`

You still need OpenBLT host tools and target bootloader implementation:
- OpenBLT BootCommander executable (installed separately)
- OpenBLT target project for your specific STM32G4 and STM32L4 boards

## Build bootloader binaries from upstream OpenBLT

This repository includes a helper:

```sh
./scripts/openblt/build_openblt_bootloader.sh --target all
```

What it does:
- Clones `https://github.com/feaser/openblt` (if needed).
- Builds GCC bootloader demos for:
  - `g473` (auto-generated local demo from upstream `g474`)
  - `g474` (`openblt_stm32g474`)
  - `g431` (`openblt_stm32g431`)
  - `l476` (`openblt_stm32l476`)
  - `l496` (`openblt_stm32l496`)
- Converts generated `.elf` to `.bin`.

Useful options:

```sh
# Use an existing OpenBLT checkout
./scripts/openblt/build_openblt_bootloader.sh --openblt-dir ~/src/openblt --target g473

# Clone to a custom location
./scripts/openblt/build_openblt_bootloader.sh --clone-dir ~/work/openblt --target l476 --clean
```

Important:
- Upstream does not provide GCC demo folders for `STM32G473` or `STM32L431/L432`.
- This helper auto-creates a local `g473` demo by cloning/modifying the upstream `g474` demo.
- The generated `g473` build currently uses the `STM32G474xx` device define because
  the upstream demo CMSIS bundle does not include `stm32g473xx.h`.
- L431/L432 still require manual porting from `l476`/`l496` (pinout, clocks, linker memory, transport settings).

## OpenBLT port-specific memory map notes (important)

Per the official OpenBLT STM32 ports:
- STM32G4 user vector table is typically relocated to `0x08003000`.
- STM32L4 user vector table is typically relocated to `0x08002800`.

This repository now uses those values in per-target `*.openblt.cfg` placeholders.

Also keep in mind:
- Bootloader reserved size should be sector aligned (2KB steps on these ports).
- If you change bootloader size/options, update your app linker flash origin accordingly.

## 1) Build your application as usual

Example:

```sh
cd test
make TEST=blinky PROJECT_TARGET=stm32g473xx
```

This produces `.elf`, `.bin`, and `.hex` artifacts in your build directory.

## 1.5) Relocate vector table in application startup

OpenBLT changes VTOR before starting your app, but `SystemInit()` can overwrite it.

Embedded-Sharepoint now provides:
- `Sharepoint_RelocateVectorTable();`
- `Sharepoint_RelocateVectorTableTo(uint32_t vector_base_address);`

Call this at the very start of `main()` before `HAL_Init()`:

```c
int main(void) {
    Sharepoint_RelocateVectorTable();
    HAL_Init();
    // ...
}
```

This helper is in `common/Src/vector_table.c` and declared in `common/Inc/stm32xx_hal.h`.

## 2) Convert firmware to `.srec` for OpenBLT

Run:

```sh
./scripts/openblt/make_boot_image.sh \
  --elf test/build/stm32g473xx.elf \
  --out test/build/stm32g473xx_app.srec
```

Notes:
- OpenBLT workflows commonly use S-record for host flashing.
- Use your real application `.elf` path.

If your project uses `template/template-Makefile`, you can also run:

```sh
make openblt-image PROJECT_TARGET=stm32g473xx
```

This generates:
- `<build-dir>/<target>_app.srec`

## 3) USB bootloading (USB-C as virtual COM)

High-level flow:
1. Put target into OpenBLT bootloader mode.
2. Identify device node:
   - macOS: `ls /dev/cu.*`
   - Linux: `ls /dev/ttyACM*` or `ls /dev/ttyUSB*`
3. Run BootCommander with the COM/tty port and generated `.srec`.

Example placeholder (adapt flags to your BootCommander build/version):

```sh
BootCommander <usb-serial-options> test/build/stm32g473xx_app.srec
```

## 4) CAN bootloading (PEAK PCAN)

High-level flow:
1. Connect PEAK PCAN interface and confirm host sees adapter.
2. Put target into OpenBLT bootloader mode on CAN.
3. Use BootCommander CAN transport with:
   - PCAN interface/channel
   - matching CAN bitrate
   - generated `.srec`

Example placeholder:

```sh
BootCommander <pcan-options> test/build/stm32g473xx_app.srec
```

## Recommended Next Steps

Once bring-up is stable, we can add:
- Per-target application linker offsets for bootloader/application separation.
- Dedicated `make openblt-image` and `make openblt-flash-{usb,can}` targets.
- Board-specific OpenBLT config templates for G4 and L4.
- CI checks that validate image conversion and size constraints.

## Optional per-target OpenBLT config pattern

OpenBLT target config files are optional and follow this naming pattern:

- `stm/<series>/<line>/<project_target>.openblt.cfg`

Current placeholders are included for existing G4/L4 ports and expose:

- `OPENBLT_APP_START_ADDRESS`

Values are currently:
- G4 targets: `0x08003000`
- L4 targets: `0x08002800`

Right now this is a bring-up hint for memory-map planning and logging; linker scripts must still match.

## First-time bootloader flashing (non-OpenBLT path)

You cannot use OpenBLT to flash OpenBLT itself the first time. Do this once with ST-Link/OpenOCD:

1. Build the OpenBLT bootloader target for your board (USB + CAN enabled).
2. Flash bootloader binary at flash base:
   - `st-flash write <bootloader.bin> 0x08000000`
3. Update your application linker script `FLASH ORIGIN` to the app start address for your port.
4. Build app and flash with BootCommander thereafter (USB or CAN).

If your G4 target is a dual-bank category 3 device and bank layout introduces a hole, switch to single-bank mode in option bytes before bring-up.

