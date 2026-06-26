#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * @file bootloader_lite.h
 * @brief Tiny, opt-in "escape to the ROM bootloader" helper.
 *
 * A lightweight, interim way to recover/reflash a board over UART without a
 * resident bootloader. On request (UART magic word "BOOT") or on a hard fault,
 * the chip reboots into the STM32 built-in system-memory (ROM) bootloader, after
 * which it is reflashable with STM32CubeProgrammer / stm32flash on the same UART.
 *
 * How it works (and why): you can't reliably jump to the ROM bootloader from a
 * running app -- especially not from a fault handler, where the active exception
 * keeps the maskable interrupts the ROM bootloader needs permanently blocked. So
 * instead of jumping, we stash a magic value in a backup register and trigger a
 * system reset (NVIC_SystemReset). That guarantees a pristine, thread-mode state.
 * The actual jump happens at the very start of the next boot, in
 * bootloader_lite_check(), before any peripherals are touched.
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
 *   int main(void) {
 *       BOOTLOADER_LITE_CHECK();      // FIRST thing in main(), before HAL_Init()
 *       HAL_Init(); SystemClock_Config();
 *       ... BOOTLOADER_LITE_SETUP(huartX);
 *   }
 *
 * Or call directly: bootloader_lite_check() (first in main), bootloader_lite_init()
 * (faults), BootloaderTask_Init(huartX) (UART).
 * Custom UART handling (no RTOS/BSP): if (bootloader_lite_feed_byte(rx)) bootloader_lite_reboot_to_rom();
 *
 * Opt out: don't enable it. Full guide: docs/BootloaderLite.md.
 */

/** UART magic word the host sends to request the bootloader. */
#define BOOTLOADER_LITE_COMMAND "BOOT"
/** Reply a project may send back before entering the bootloader (host handshake). */
#define BOOTLOADER_LITE_ACK "BOOTACK\r\n"

/**
 * @brief Enter the ROM bootloader now if a previous boot requested it.
 *
 * Reads the backup-register magic value set by bootloader_lite_reboot_to_rom()
 * (UART "BOOT") or the fault handler. If present, clears it and jumps into the
 * ROM bootloader; otherwise returns immediately and the app boots normally.
 *
 * @note MUST be the first call in main(), before HAL_Init()/SystemClock_Config()
 *       and the RTOS, so the jump happens with peripherals still in reset.
 */
void bootloader_lite_check(void);

/**
 * @brief Request the ROM bootloader: stash a magic value and system-reset.
 *
 * Never returns. Safe from any context (thread, ISR, or fault handler) because
 * it doesn't jump -- it reboots, and bootloader_lite_check() finishes the jump
 * on the next boot from a clean state. Send any UART ack/handshake and wait for
 * it to drain *before* calling this (the reset kills the UART mid-byte).
 */
void bootloader_lite_reboot_to_rom(void);

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
