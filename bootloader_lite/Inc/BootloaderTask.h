#pragma once

#include "UART.h"
#include "bootloader_lite.h"

/**
 * @file BootloaderTask.h
 * @brief Drop-in FreeRTOS task + macro opt-in for bootloader_lite.
 *
 * Standard, ready-made UART listener so projects don't have to write their own:
 * it waits for "BOOT" on a UART, replies "BOOTACK", then jumps into the STM32
 * ROM bootloader (bootloader_lite_enter_rom()).
 *
 * Macro opt-in (define in your main BEFORE including this header, then call
 * BOOTLOADER_LITE_SETUP(uart) once during startup):
 *
 *     #define USE_BOOTLOADER             // enable the UART "BOOT" -> ROM bootloader
 *     #define BOOTLOADER_ON_HARDFAULT    // also enter the bootloader on a hard fault
 *     #include "BootloaderTask.h"
 *     ...
 *     int main(void) {
 *         ...
 *         BOOTLOADER_LITE_SETUP(husart3);   // no-op unless USE_BOOTLOADER is defined
 *     }
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
 * @brief One-call opt-in, driven by the USE_BOOTLOADER / BOOTLOADER_ON_HARDFAULT
 *        macros defined before this header is included.
 *
 *  - USE_BOOTLOADER            -> start the UART "BOOT" listener on @p uart
 *  - + BOOTLOADER_ON_HARDFAULT -> also route HardFault/&c. to the ROM bootloader
 *  - neither                   -> expands to nothing (and is --gc-sections'd away)
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
