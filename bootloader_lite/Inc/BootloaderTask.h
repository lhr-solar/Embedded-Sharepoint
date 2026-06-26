#pragma once

#include "UART.h"
#include "bootloader_lite.h"

/**
 * @file BootloaderTask.h
 * @brief Drop-in FreeRTOS task + macro opt-in for bootloader_lite.
 *
 * Standard, ready-made UART listener so projects don't have to write their own:
 * it waits for "BOOT" on a UART, replies "BOOTACK", then reboots into the STM32
 * ROM bootloader (bootloader_lite_reboot_to_rom()).
 *
 * Macro opt-in (define in your main BEFORE including this header, then call
 * BOOTLOADER_LITE_CHECK() first and BOOTLOADER_LITE_SETUP(uart) during startup):
 *
 *     #define USE_BOOTLOADER             // enable the UART "BOOT" -> ROM bootloader
 *     #define BOOTLOADER_ON_HARDFAULT    // also enter the bootloader on a hard fault
 *     #include "BootloaderTask.h"
 *     ...
 *     int main(void) {
 *         BOOTLOADER_LITE_CHECK();          // FIRST: jump to ROM if a prior boot asked
 *         HAL_Init();
 *         SystemClock_Config();
 *         BOOTLOADER_LITE_SETUP(husart3);   // no-op unless USE_BOOTLOADER is defined
 *     }
 *
 * BOOTLOADER_LITE_CHECK() must run before HAL_Init()/SystemClock_Config(): the
 * "BOOT"/hard-fault paths reboot the chip and this is where the actual jump
 * happens, with peripherals still in their reset state.
 *
 * BOOTLOADER_ON_HARDFAULT requires USE_BOOTLOADER (it can't route faults to a
 * bootloader that isn't enabled). Works with static or dynamic FreeRTOS
 * allocation; override BOOTLOADER_TASK_STACK_SIZE / BOOTLOADER_TASK_PRIORITY if
 * needed. You can also call the functions directly instead of using the macro.
 */

#if defined(BOOTLOADER_ON_HARDFAULT) && !defined(USE_BOOTLOADER)
#error "BOOTLOADER_ON_HARDFAULT requires USE_BOOTLOADER"
#endif

/**
 * @brief Create the bootloader UART listener task.
 * @param uart Console UART handle to listen on (e.g. husart3).
 */
void BootloaderTask_Init(UART_HandleTypeDef *uart);

/**
 * @brief Finish a pending ROM-bootloader request left by a prior boot.
 *
 * Expands to bootloader_lite_check() when USE_BOOTLOADER is defined, else to
 * nothing. Call it as the FIRST statement in main(), before HAL_Init().
 */
#if defined(USE_BOOTLOADER)
#  define BOOTLOADER_LITE_CHECK() bootloader_lite_check()
#else
#  define BOOTLOADER_LITE_CHECK() ((void)0)
#endif

/**
 * @brief One-call opt-in, driven by the USE_BOOTLOADER / BOOTLOADER_ON_HARDFAULT
 *        macros defined before this header is included.
 *
 *  - USE_BOOTLOADER            -> start the UART "BOOT" listener on @p uart
 *  - + BOOTLOADER_ON_HARDFAULT -> also route HardFault/&c. to the ROM bootloader
 *  - neither                   -> expands to nothing (and is --gc-sections'd away)
 *
 * Pair with BOOTLOADER_LITE_CHECK() at the very top of main().
 */
#if defined(USE_BOOTLOADER)
#  if defined(BOOTLOADER_ON_HARDFAULT)
#    define BOOTLOADER_LITE_SETUP(uart) \
        do { bootloader_lite_init(); BootloaderTask_Init(uart); } while (0)
#  else
#    define BOOTLOADER_LITE_SETUP(uart) \
        do { BootloaderTask_Init(uart); } while (0)
#  endif
#else
#  define BOOTLOADER_LITE_SETUP(uart) ((void)0)
#endif
