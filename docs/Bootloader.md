# Bootloader: Fault Recovery & Status LED

Two integration points for boards that run the resident CAN/DFU bootloader. The
protocol and flash contract live in `bootloader/contract/CONTRACT.md`; this guide
covers the app-side fault hook and the bootloader's status-LED patterns.

## Return to the bootloader on a fault

If the application hits an unrecoverable fault, it warm-resets back into the
bootloader and **stays there** instead of relaunching the faulty image. The
bootloader is resident at the flash base, so a host can immediately reflash over
CAN/DFU — no crash → reboot → crash loop, no debugger required to recover a unit
in the field.

### Default behavior (no wiring needed)

`common/Src/stubs.c` provides the default `HardFault_Handler`. It captures the
fault status registers (`CFSR`, `HFSR`, `DFSR`, `MMFAR`, `BFAR`) and then:

- **debugger attached** → executes `BKPT` so you can inspect the captured registers;
- **no debugger** → calls `BootloaderRebootFromFault()` to recover into the bootloader.

On Cortex-M4 the MemManage/BusFault/UsageFault exceptions are disabled by default
and escalate to HardFault, so this one handler covers them too. Because it links
in through `common/Src`, every app gets this behavior automatically.

### The utility

```c
#include "bootloader_hooks.h"

void BootloaderRebootFromFault(void);  // sets BKP0R UPDATE+FAULT, then resets; never returns
```

`UPDATE` keeps the bootloader resident; `FAULT` records that the entry was
crash-driven (a normal `ENTER` / `BootloaderRequestUpdate` leaves it clear). The
bootloader clears `FAULT` on `COMMIT`. Call it from anywhere you decide the app
cannot safely continue:

```c
#include "bootloader_hooks.h"

void on_assert_failed(void) {
    BootloaderRebootFromFault();  // never returns
}
```

To customize the fault policy, define your own `HardFault_Handler` (it overrides
the default) and call `BootloaderRebootFromFault()` where appropriate.

> Boards without a resident bootloader at `0x08000000` (e.g. a standalone test
> image) simply warm-reset on fault — there is nothing to "stay in", so it acts as
> a reset-on-fault. With a debugger attached you still break.

## Status LED

While resident, the bootloader drives an optional on-board status LED so the
pattern tells you *why* it is sitting in the bootloader:

| Pattern | Meaning |
|---------|---------|
| **Breathe** (smooth pulse) | Resident on request (host `ENTER` / `BootloaderRequestUpdate`) or after fault recovery (`BootloaderRebootFromFault`). |
| **Hard blink** (~5 Hz) | Resident only because there is no valid app: invalid-image fallback, or a fresh / blank MCU. |

The pattern engine (`bootloader/Src/bl_indicator.c`) is board-agnostic; the LED
pin is **user-defined**. The default hooks are weak no-ops, so a board with no
status LED builds and runs with the feature inert.

### Wiring an LED

Give `bl_indicator_led_set()` (and usually `bl_indicator_led_init()`) a strong
definition in your resident-bootloader build (add a board source under
`bootloader/Src`). The engine software-PWMs the binary LED to render the breathe
ramp, so a plain GPIO LED breathes with no timer:

```c
#include "bl_indicator.h"
#include "stm32g4xx.h"

/* Status LED on PA5, active-high — ponytail: set to your board's LED pin. */
#define LED_PORT GPIOA
#define LED_PIN  5U

void bl_indicator_led_init(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    LED_PORT->MODER &= ~(3U << (LED_PIN * 2U));
    LED_PORT->MODER |= (1U << (LED_PIN * 2U));  // general-purpose output
}

void bl_indicator_led_set(uint8_t on) {
    LED_PORT->BSRR = on ? (1U << LED_PIN) : (1U << (LED_PIN + 16U));
}
```

That is all most boards need — the bootloader already selects the pattern and
calls these hooks from its main loop.

### Hardware PWM (advanced)

For a PWM-capable LED, override `bl_indicator_render(uint8_t brightness)` instead
and write `brightness` (0..255) to your timer's compare register; `bl_indicator_led_set`
is then unused. The default time base is the DWT cycle counter (always present on
these Cortex-M4 parts).
