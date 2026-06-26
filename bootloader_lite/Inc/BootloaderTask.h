#pragma once

#include "UART.h"

/**
 * @file BootloaderTask.h
 * @brief Drop-in FreeRTOS task that listens for the bootloader_lite magic word.
 *
 * Standard, ready-made UART listener so projects don't have to write their own:
 * it waits for "BOOT" on @p uart, replies "BOOTACK", then jumps into the STM32
 * ROM bootloader (bootloader_lite_enter_rom()).
 *
 * For hard-fault -> bootloader routing as well, also call bootloader_lite_init()
 * early in main(). Works with either static or dynamic FreeRTOS allocation.
 *
 * Optional compile-time overrides (e.g. in your project macros):
 *   BOOTLOADER_TASK_STACK_SIZE, BOOTLOADER_TASK_PRIORITY
 */

/**
 * @brief Create the bootloader UART listener task.
 * @param uart Console UART handle to listen on (e.g. husart3).
 */
void BootloaderTask_Init(UART_HandleTypeDef *uart);
