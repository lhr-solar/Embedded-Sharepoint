# bootloader_lite

Opt-in escape into the STM32 system-memory (ROM) bootloader on the UART magic
word `"BOOT"` or a hard fault — an interim way to reflash/recover over UART
without a resident bootloader.

Inert unless you call it (pure CMSIS/HAL, no symbol overrides, removed by
`--gc-sections` when unused), so it's safe to adopt incrementally.

Both triggers stash a magic value in a backup register and `NVIC_SystemReset()`;
the jump to the ROM bootloader happens at the very start of the next boot, from a
clean thread-mode state. (Jumping from a fault handler can't work — the active
fault masks the interrupts the ROM bootloader needs.)

Quick opt-in — define two macros in your `main` (before including the header),
then two calls:

```c
#define USE_BOOTLOADER             /* enable UART "BOOT" -> ROM bootloader  */
#define BOOTLOADER_ON_HARDFAULT    /* also enter bootloader on a hard fault */
#include "BootloaderTask.h"

int main(void) {
    BOOTLOADER_LITE_CHECK();       /* FIRST, before HAL_Init(): finish a pending request */
    HAL_Init();
    SystemClock_Config();
    BOOTLOADER_LITE_SETUP(husart3);/* no-op unless USE_BOOTLOADER set */
}
```

`BOOTLOADER_ON_HARDFAULT` requires `USE_BOOTLOADER`. `BOOTLOADER_LITE_CHECK()`
must be the first thing in `main()`. You can also call `bootloader_lite_check()`
/ `bootloader_lite_init()` / `BootloaderTask_Init(uart)` directly instead.

Flashing from the host: `make flash-lite` (or `make bl-send`), or
`scripts/bootloader_lite_flash.py`.

Full guide: [docs/BootloaderLite.md](../docs/BootloaderLite.md).
