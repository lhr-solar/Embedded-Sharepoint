# Bootloader Lite

`bootloader_lite` is a tiny, **opt-in** way to escape into the STM32 built-in
**system-memory (ROM) bootloader** so a board can be reflashed over UART without a
resident bootloader. It triggers on a UART magic word (`BOOT`) or on a hard fault.

It is an **interim backup** flashing path — deliberately simple — while a more
robust bootloader is developed.

## How it works (context-aware: direct jump vs. reset)

`bootloader_lite_reboot_to_rom()` picks how to reach the ROM bootloader based on
the **caller's CPU context** (`IPSR`):

- **Thread mode** (the normal case — e.g. the UART `"BOOT"` listener task): it
  tears the app's clocks/peripherals/interrupts back to a reset-like state
  (`__disable_irq()`, stop SysTick, disable+clear every NVIC IRQ, `HAL_RCC_DeInit()`,
  `HAL_DeInit()`) and **jumps straight** into the ROM bootloader. This is
  immediate and doesn't depend on a backup register surviving a reset. The
  teardown matters: a still-running SysTick/DMA/timer or a pending IRQ can wedge
  the ROM bootloader the instant interrupts are re-enabled.
- **Handler/exception mode** (e.g. the **hard-fault** path): you *can't* jump —
  the fault is an active exception (priority `-1`) that masks every normal
  interrupt (SysTick/UART/USB), so the ROM bootloader would wait forever on
  interrupts that can't fire. Instead it:
  1. Writes a **magic value** to a **backup register** (survives a reset, no
     linker changes — works the same on F4/G4/L4).
  2. Calls **`NVIC_SystemReset()`** for a completely clean, power-on-like state.
  3. At the **very start of the next boot**, `bootloader_lite_check()` sees the
     magic, clears it, and jumps to the ROM bootloader — now in thread mode with
     peripherals in reset, exactly what the ROM bootloader expects.

For the reset path you must add the `bootloader_lite_check()` call (via
`BOOTLOADER_LITE_CHECK()`) as the **first** line of `main()`.

## Safe by default (zero cost unless you opt in)

The code is pure CMSIS/HAL (no RTOS or BSP dependency) and defines **no** new
strong symbols (in particular it does **not** override `HardFault_Handler`). It
does nothing unless you call into it, and anything you don't call is removed by
`--gc-sections`. So a project that doesn't use it builds and behaves exactly as
before — adopting it is purely additive.

## Opt in (macros)

Define the opt-in macros in your `main` **before** including `BootloaderTask.h`,
call `BOOTLOADER_LITE_CHECK()` **first**, then the setup once during startup:

```c
#define USE_BOOTLOADER             // enable the UART "BOOT" -> ROM bootloader
#define BOOTLOADER_ON_HARDFAULT    // also enter the bootloader on a hard fault
#include "BootloaderTask.h"

int main(void) {
    BOOTLOADER_LITE_CHECK();          // FIRST: jump to ROM if a prior boot asked
    HAL_Init();
    SystemClock_Config();
    BOOTLOADER_LITE_SETUP(husart3);   // pass your console UART handle
    ...
}
```

> `BOOTLOADER_LITE_CHECK()` **must** be the first thing in `main()`, before
> `HAL_Init()` / `SystemClock_Config()` and the RTOS. That's where the actual
> jump to the ROM bootloader happens (after the "BOOT"/hard-fault reset), with
> peripherals still in their reset state. It's a no-op on a normal boot.

- `USE_BOOTLOADER` alone → just the UART "BOOT" listener.
- `+ BOOTLOADER_ON_HARDFAULT` → also routes HardFault/MemManage/BusFault/UsageFault
  to the bootloader. It **requires** `USE_BOOTLOADER` (defining it alone is a
  compile error — you can't route faults to a bootloader that isn't enabled).
- Neither → `BOOTLOADER_LITE_SETUP` expands to nothing and the whole module is
  `--gc-sections`'d away.

`BootloaderTask` works with static or dynamic FreeRTOS allocation; override
`BOOTLOADER_TASK_STACK_SIZE` / `BOOTLOADER_TASK_PRIORITY` if needed.

### Without the macro (direct calls / custom UART)

You can call the functions directly instead of the macro:

```c
int main(void) {
    bootloader_lite_check();     // FIRST, before HAL_Init(): finish a pending request
    HAL_Init();
    SystemClock_Config();
    bootloader_lite_init();      // faults -> ROM bootloader
    BootloaderTask_Init(husart3);// UART "BOOT" listener
    ...
}
```

Or, if you don't use the bsp UART (or want your own RX loop), skip
`BootloaderTask` and feed bytes yourself — `bootloader_lite_feed_byte()` /
`bootloader_lite_reboot_to_rom()` have no RTOS/BSP dependency:

```c
if (bootloader_lite_feed_byte(rx)) {
    /* optionally send BOOTLOADER_LITE_ACK first, and wait for it to drain */
    bootloader_lite_reboot_to_rom();   // sets magic flag + resets; never returns
}
```

(You still need `bootloader_lite_check()` first in `main()` for the jump to happen.)

## Opt out

Don't call any `bootloader_lite_*` function; the module is garbage collected at
link time (zero flash/RAM cost).

## Flashing from the host

Serial-port and `STM32_Programmer_CLI` detection match the other flash tooling
(`scripts/bootloader_script_utils.py`).

```bash
make bl-send        # send "BOOT": a running app jumps into the ROM bootloader
make flash-bl       # send "BOOT", then flash the built image @ 0x08000000
# override the port if autodetect picks the wrong one:
make flash-bl BL_LITE_PORT=/dev/cu.usbserial-XXXX
```

Or call the script directly:

```bash
python3 scripts/bootloader_lite_flash.py --bin build/stm32g473xx.bin
python3 scripts/bootloader_lite_flash.py            # just enter the bootloader
```

The script sends `BOOT`, waits up to 3 s for the board's `BOOTACK`, errors if it
never arrives, then (with `--bin`) flashes over the ROM UART bootloader.

## API

| Symbol | Purpose |
| --- | --- |
| `USE_BOOTLOADER` / `BOOTLOADER_ON_HARDFAULT` | Compile-time opt-in macros; define before `#include "BootloaderTask.h"`. |
| `BOOTLOADER_LITE_CHECK()` | Finish a pending request; call **first** in `main()` (no-op when not enabled). |
| `BOOTLOADER_LITE_SETUP(uart)` | One-call setup driven by those macros (no-op when not enabled). |
| `bootloader_lite_check()` | First in `main()` (before `HAL_Init`): jump to ROM if a prior boot requested it. |
| `bootloader_lite_init()` | Opt in: route faults to the ROM bootloader (call once, early in `main`). |
| `BootloaderTask_Init(uart)` | Start the ready-made UART listener task (no task code to write). |
| `bootloader_lite_feed_byte(b)` | Feed UART RX bytes; returns `true` when `"BOOT"` is matched (for custom loops). |
| `bootloader_lite_reboot_to_rom()` | Set the magic flag + system-reset to request the ROM bootloader (never returns). |
| `BOOTLOADER_LITE_COMMAND` / `BOOTLOADER_LITE_ACK` | Magic word `"BOOT"` / reply `"BOOTACK\r\n"`. |

## Notes & limitations (it's a lite/interim tool)

- **System-memory address** defaults to `0x1FFF0000` (STM32F4/G4/L4). For other
  families override `-DBOOTLOADER_LITE_SYSMEM_BASE=0x...` (see the RM "system memory").
- **Backup register:** the magic flag lives in backup register 0 (`TAMP->BKP0R`
  on G4, `RTC->BKP0R` on F4/L4). If your app already uses that register, override
  `-DBOOTLOADER_LITE_BKP_REG=...` (and/or `-DBOOTLOADER_LITE_MAGIC=0x...`). The
  jump won't happen unless `bootloader_lite_check()` runs first in `main()`.
- **`bootloader_lite_check()` must run first** in `main()`, before `HAL_Init()`.
  The "BOOT"/hard-fault paths reset the chip; the jump happens here on the next
  boot. It is not a substitute for proper fault diagnostics.
- **Independent watchdog (IWDG):** a software reset does **not** stop a running
  IWDG, so if your app started it, it keeps counting into the ROM bootloader and
  will reset the chip after its timeout. Flash promptly, or don't use the IWDG
  when relying on this.
- **VTOR:** `bootloader_lite_init()` relocates the vector table to RAM. If your
  app also sets `SCB->VTOR`, call `bootloader_lite_init()` afterwards.
