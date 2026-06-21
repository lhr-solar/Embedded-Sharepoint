#include <stdint.h>

#include "bootloader_hooks.h"
#include "stm32g4xx.h"

// This is load bearing
void _init() {}

/*
 * Default fault policy: capture the fault status registers (inspect them at the
 * breakpoint below), then break to an attached debugger, or — in the field, with
 * no debugger — recover into the resident bootloader so a host can reflash. A
 * project can override this by defining its own HardFault_Handler, or keep it and
 * customise recovery via BootloaderRebootFromFault() (see bootloader_hooks.h).
 *
 * MemManage/BusFault/UsageFault are disabled by default on Cortex-M4 and escalate
 * to HardFault, so this one handler catches them too.
 */
void HardFault_Handler(void) {
    // Configurable Fault Status Register (MMFSR + BFSR + UFSR)
    volatile uint32_t _CFSR = SCB->CFSR;
    // Hard Fault Status Register
    volatile uint32_t _HFSR = SCB->HFSR;
    // Debug Fault Status Register
    volatile uint32_t _DFSR = SCB->DFSR;
    // Auxiliary Fault Status Register
    volatile uint32_t _AFSR = SCB->AFSR;
    // Fault address registers — valid only if CFSR MMARVALID/BFARVALID is set
    volatile uint32_t _MMFAR = SCB->MMFAR;
    volatile uint32_t _BFAR = SCB->BFAR;
    (void)_CFSR;
    (void)_HFSR;
    (void)_DFSR;
    (void)_AFSR;
    (void)_MMFAR;
    (void)_BFAR;

    if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0U) {
        __BKPT(0);  // debugger attached: break here and inspect the captured regs
    }

    BootloaderRebootFromFault();  // field recovery: warm-reset into the bootloader
    for (;;) {
    }
}
