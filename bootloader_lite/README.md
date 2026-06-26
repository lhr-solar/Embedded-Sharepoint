# bootloader_lite

Opt-in escape into the STM32 system-memory (ROM) bootloader on the UART magic
word `"BOOT"` or a hard fault — an interim way to reflash/recover over UART
without a resident bootloader.

Inert unless you call it (pure CMSIS/HAL, no symbol overrides, removed by
`--gc-sections` when unused), so it's safe to adopt incrementally.

Quick opt-in:

```c
#include "bootloader_lite.h"

bootloader_lite_init();                 /* in main(): faults -> ROM bootloader */
if (bootloader_lite_feed_byte(rx)) {    /* feed your UART RX bytes            */
    /* optional: uart_send(..., BOOTLOADER_LITE_ACK, ...) */
    bootloader_lite_enter_rom();
}
```

Flashing from the host: `make flash-lite` (or `make bl-send`), or
`scripts/bootloader_lite_flash.py`.

Full guide: [docs/BootloaderLite.md](../docs/BootloaderLite.md).
