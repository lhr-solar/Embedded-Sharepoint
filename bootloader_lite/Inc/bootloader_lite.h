#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * @file bootloader_lite.h
 * @brief Tiny, opt-in "escape to the ROM bootloader" helper.
 *
 * A lightweight, interim way to recover/reflash a board over UART without a
 * resident bootloader. On request (UART magic word "BOOT") or on a hard fault,
 * the app jumps to the STM32 built-in system-memory (ROM) bootloader, after
 * which the chip is reflashable with STM32CubeProgrammer / stm32flash on the
 * same UART.
 *
 * This core is pure CMSIS/HAL (no RTOS, no BSP dependency) and does nothing
 * unless you call into it, so adopting it is purely additive: projects that
 * don't use it build and behave exactly as before (unused code is
 * --gc-sections'd away).
 *
 * Typical opt-in (no listener code to write) via BootloaderTask.h macros:
 *   #define USE_BOOTLOADER
 *   #define BOOTLOADER_ON_HARDFAULT   // optional; requires USE_BOOTLOADER
 *   #include "BootloaderTask.h"
 *   ... BOOTLOADER_LITE_SETUP(huartX);
 *
 * Or call directly: bootloader_lite_init() (faults) + BootloaderTask_Init(huartX) (UART).
 * Custom UART handling (no RTOS/BSP): if (bootloader_lite_feed_byte(rx)) bootloader_lite_enter_rom();
 *
 * Opt out: don't enable it. Full guide: docs/BootloaderLite.md.
 */

/** UART magic word the host sends to request the bootloader. */
#define BOOTLOADER_LITE_COMMAND "BOOT"
/** Reply a project may send back before entering the bootloader (host handshake). */
#define BOOTLOADER_LITE_ACK "BOOTACK\r\n"

/**
 * @brief Opt in: route HardFault/MemManage/BusFault/UsageFault to the ROM
 *        bootloader (via a RAM vector table). Call once, early in main().
 */
void bootloader_lite_init(void);

/**
 * @brief Feed one received UART byte to the magic-word matcher.
 * @return true exactly when the full command ("BOOT") has just been matched.
 */
bool bootloader_lite_feed_byte(uint8_t byte);

/**
 * @brief Jump straight into the STM32 system-memory ROM bootloader. Never returns.
 * @note Intended to be called from a task/thread-mode context (e.g. your UART
 *       handler). The fault handler installed by bootloader_lite_init() also uses it.
 */
void bootloader_lite_enter_rom(void);
