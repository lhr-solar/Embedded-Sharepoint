#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * @file bootloader_lite.h
 * @brief Tiny, opt-in "escape to the ROM bootloader" helper.
 *
 * A lightweight, interim way to recover/reflash a board over UART without a
 * resident bootloader. On request (UART magic word) or on a hard fault, the
 * app jumps to the STM32 built-in system-memory (ROM) bootloader, after which
 * the chip is reflashable with STM32CubeProgrammer / stm32flash on the same UART.
 *
 * It is pure CMSIS/HAL (no RTOS, no BSP dependency) and does nothing unless you
 * call into it, so bumping a project's Embedded-Sharepoint to this branch is a
 * no-op until you opt in. See bootloader_lite/README.md.
 *
 * Opt in (3 lines):
 *   1) bootloader_lite_init();                      // early in main()
 *   2) if (bootloader_lite_feed_byte(rx)) {         // feed your UART RX bytes
 *          // (optional) send BOOTLOADER_LITE_ACK back, then:
 *          bootloader_lite_enter_rom();
 *      }
 *
 * Opt out: don't call anything (the code is --gc-sections'd away).
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
 * @note Safe to call from a normal (thread-mode) context, e.g. your UART task.
 */
void bootloader_lite_enter_rom(void);
