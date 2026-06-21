# Deprecated: OpenBLT bootloader scaffolding

These OpenBLT-based scripts and notes are **superseded** by the resident
CAN/DFU bootloader in [`bootloader/`](../../bootloader/). They are kept here for
historical reference only and are no longer built or maintained.

## Use instead

- **Protocol & memory map:** `bootloader/contract/CONTRACT.md`,
  `bootloader/contract/bl_protocol.schema.json`, `bootloader/memory_map/*.json`
- **Firmware:** `bootloader/Src/`, built with
  `make -f bootloader/bootloader.mk TARGET=stm32g491vet` (or `stm32g431cbt`)
- **App entry hooks:** `common/Inc/bootloader_hooks.h`
- **Host flashing:** `bootloader/tools/can_flash.py` (CAN),
  `bootloader/tools/dfu_flash.py` (ROM DFU)

## Contents (archived)

- `OpenBLT.md` — original OpenBLT integration notes
- `build_openblt_bootloader.sh`, `make_boot_image.sh` — original build scripts
