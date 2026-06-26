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

/* Parked in a backup register to ask the *next* boot to enter the ROM bootloader.
 * Backup registers survive a software reset and need no linker changes (unlike a
 * .noinit SRAM variable), so the same code works across F4/G4/L4. */
#ifndef BOOTLOADER_LITE_MAGIC
#define BOOTLOADER_LITE_MAGIC 0xB00710ADUL
#endif

/* Backup registers moved from RTC to the TAMP block on newer families (G4).
 * Override -DBOOTLOADER_LITE_BKP_REG=... if your app already uses backup reg 0. */
#ifndef BOOTLOADER_LITE_BKP_REG
#  if defined(TAMP)
#    define BOOTLOADER_LITE_BKP_REG (TAMP->BKP0R) /* G4 and newer */
#  else
#    define BOOTLOADER_LITE_BKP_REG (RTC->BKP0R)  /* F4 / L4 */
#  endif
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

/* Unlock + clock the backup domain so BOOTLOADER_LITE_BKP_REG is read/writable.
 * Cheap and idempotent, so we just (re)do it before every access instead of
 * tracking state. Works before HAL_Init() (only RCC/PWR registers are touched). */
static void bl_backup_access(void) {
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();          /* clears the backup-domain write protection (DBP) */
#ifdef __HAL_RCC_RTCAPB_CLK_ENABLE
    __HAL_RCC_RTCAPB_CLK_ENABLE();       /* G4/L4 gate RTC/TAMP registers behind this clock; F4 does not */
#endif
}

/* Jump straight into the ROM bootloader. Only safe from a pristine, post-reset
 * state (thread mode, peripherals in reset) -- i.e. only from bootloader_lite_check().
 * Jumping from an exception (e.g. a fault handler) deadlocks the ROM bootloader,
 * which needs maskable interrupts that stay blocked while a fault is active. */
static void bl_jump_to_rom(void) {
    /* Read the bootloader's initial SP and entry point before switching stacks. */
    const uint32_t boot_sp = *(volatile uint32_t *)(BOOTLOADER_LITE_SYSMEM_BASE);
    void (*const boot_entry)(void) =
        (void (*)(void))(*(volatile uint32_t *)(BOOTLOADER_LITE_SYSMEM_BASE + 4U));

    __disable_irq();

    SCB->VTOR = BOOTLOADER_LITE_SYSMEM_BASE;
    __DSB();
    __ISB();

    __set_MSP(boot_sp);
    __set_CONTROL(0U); /* privileged, use MSP */
    __ISB();

    __enable_irq(); /* the ROM bootloader runs on maskable interrupts */

    boot_entry();

    for (;;) {
        /* Never reached. */
    }
}

void bootloader_lite_reboot_to_rom(void) {
    bl_backup_access();
    BOOTLOADER_LITE_BKP_REG = BOOTLOADER_LITE_MAGIC;
    __DSB();

    NVIC_SystemReset(); /* clean hardware state; bootloader_lite_check() finishes the jump */

    for (;;) {
        /* NVIC_SystemReset() never returns. */
    }
}

void bootloader_lite_check(void) {
    bl_backup_access();
    if (BOOTLOADER_LITE_BKP_REG == BOOTLOADER_LITE_MAGIC) {
        BOOTLOADER_LITE_BKP_REG = 0U; /* one-shot: clear first so the next reset boots the app */
        __DSB();
        bl_jump_to_rom();
    }
}

/* Installed in the RAM vector table for the fault exceptions. */
static void bootloader_lite_fault_handler(void) {
    bootloader_lite_reboot_to_rom();
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
