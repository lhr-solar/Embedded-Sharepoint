/**
 * @file bootloader_lite.c
 * @brief Tiny, opt-in "escape to the ROM bootloader" helper (see header).
 * @copyright Copyright (c) 2026 UT Longhorn Racing Solar
 */

#include "bootloader_lite.h"
#include "stm32xx_hal.h"

#include <stddef.h>
#include <stdint.h>

/* STM32 system-memory (ROM bootloader) base. Same for F4/G4/L4. Override with
 * -DBOOTLOADER_LITE_SYSMEM_BASE=0x... for other families (see RM "system memory"). */
#ifndef BOOTLOADER_LITE_SYSMEM_BASE
#define BOOTLOADER_LITE_SYSMEM_BASE 0x1FFF0000UL
#endif

/* 16 core exceptions + IRQs fit in 128 entries (512 B). VTOR must be aligned to
 * the table size rounded up to a power of two => 512-byte alignment. */
#define BOOTLOADER_LITE_VECTOR_COUNT 128U
#define EXC_HARDFAULT  3U
#define EXC_MEMMANAGE  4U
#define EXC_BUSFAULT   5U
#define EXC_USAGEFAULT 6U

static const char BL_CMD[] = BOOTLOADER_LITE_COMMAND;
static size_t s_match_len;
static uint32_t s_ram_vectors[BOOTLOADER_LITE_VECTOR_COUNT] __attribute__((aligned(512)));

void bootloader_lite_enter_rom(void) {
    /* Read the bootloader's initial SP and entry point before tearing down. */
    const uint32_t boot_sp = *(volatile uint32_t *)(BOOTLOADER_LITE_SYSMEM_BASE);
    void (*const boot_entry)(void) =
        (void (*)(void))(*(volatile uint32_t *)(BOOTLOADER_LITE_SYSMEM_BASE + 4U));

    /* Reset clocks/peripherals so the ROM bootloader starts from a known state. */
    HAL_RCC_DeInit();
    HAL_DeInit();

    __disable_irq();

    SysTick->CTRL = 0U;
    SysTick->LOAD = 0U;
    SysTick->VAL = 0U;

    for (uint32_t i = 0U; i < (sizeof(NVIC->ICER) / sizeof(NVIC->ICER[0])); i++) {
        NVIC->ICER[i] = 0xFFFFFFFFU;
        NVIC->ICPR[i] = 0xFFFFFFFFU;
    }

    SCB->VTOR = BOOTLOADER_LITE_SYSMEM_BASE;
    __DSB();
    __ISB();

    __set_MSP(boot_sp);
    __set_CONTROL(0U); /* privileged, use MSP */
    __ISB();

    __enable_irq();

    boot_entry();

    for (;;) {
        /* Never reached. */
    }
}

/* Installed in the RAM vector table for the fault exceptions. */
static void bootloader_lite_fault_handler(void) {
    bootloader_lite_enter_rom();
}

void bootloader_lite_init(void) {
    const uint32_t *src = (const uint32_t *)SCB->VTOR;
    for (uint32_t i = 0U; i < BOOTLOADER_LITE_VECTOR_COUNT; i++) {
        s_ram_vectors[i] = src[i];
    }

    const uint32_t handler = (uint32_t)&bootloader_lite_fault_handler;
    s_ram_vectors[EXC_HARDFAULT] = handler;
    s_ram_vectors[EXC_MEMMANAGE] = handler;
    s_ram_vectors[EXC_BUSFAULT] = handler;
    s_ram_vectors[EXC_USAGEFAULT] = handler;

    __DSB();
    SCB->VTOR = (uint32_t)s_ram_vectors;
    __DSB();
    __ISB();

    s_match_len = 0U;
}

bool bootloader_lite_feed_byte(uint8_t byte) {
    const size_t command_len = sizeof(BL_CMD) - 1U;

    if (byte == (uint8_t)BL_CMD[s_match_len]) {
        s_match_len++;
        if (s_match_len == command_len) {
            s_match_len = 0U;
            return true;
        }
        return false;
    }

    /* Mismatch: restart, but let this byte start a fresh match (e.g. "BBOOT"). */
    s_match_len = (byte == (uint8_t)BL_CMD[0]) ? 1U : 0U;
    return false;
}
