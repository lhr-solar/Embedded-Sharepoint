# bootloader_lite

Opt-in escape into the STM32 system-memory (ROM) bootloader on the UART magic
word `"BOOT"` or a hard fault — an interim way to reflash/recover over UART
without a resident bootloader.

Inert unless you call it (pure CMSIS/HAL, no symbol overrides, removed by
`--gc-sections` when unused), so it's safe to adopt incrementally.

Quick opt-in — define two macros in your `main` (before including the header),
then one call:

```c
#define USE_BOOTLOADER             /* enable UART "BOOT" -> ROM bootloader  */
#define BOOTLOADER_ON_HARDFAULT    /* also enter bootloader on a hard fault */
#include "BootloaderTask.h"

BOOTLOADER_LITE_SETUP(husart3);    /* in main(); no-op unless USE_BOOTLOADER set */
```

`BOOTLOADER_ON_HARDFAULT` requires `USE_BOOTLOADER`. You can also call
`bootloader_lite_init()` / `BootloaderTask_Init(uart)` directly instead.

Flashing from the host: `make flash-lite` (or `make bl-send`), or
`scripts/bootloader_lite_flash.py`.

Full guide: [docs/BootloaderLite.md](../docs/BootloaderLite.md).
