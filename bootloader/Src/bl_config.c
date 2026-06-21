#include "bl_config.h"
#include "bl_target.h"
#include "bl_crc32.h"
#include "bl_protocol.h"

#include "stm32g4xx.h"

/* ponytail: production reads device_id from flash config word or node registry */
#ifndef BL_DEFAULT_DEVICE_ID
#define BL_DEFAULT_DEVICE_ID 0U
#endif

static uint8_t s_device_id = BL_DEFAULT_DEVICE_ID;

static void bl_backup_enable(void)
{
    RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_RTCAPBEN;
    PWR->CR1 |= PWR_CR1_DBP;
}

static uint32_t bl_flags_read(void)
{
    return TAMP->BKP0R;
}

static void bl_flags_write(uint32_t v)
{
    TAMP->BKP0R = v;
}

void bl_config_init(void)
{
    bl_backup_enable();
    s_device_id = BL_DEFAULT_DEVICE_ID;
}

uint8_t bl_config_device_id(void)
{
    return s_device_id;
}

void bl_config_read_uid(uint8_t uid12[12])
{
    const uint32_t *uid = (const uint32_t *)UID_BASE;
    for (int i = 0; i < 3; i++) {
        uint32_t w = uid[i];
        uid12[i * 4 + 0] = (uint8_t)(w & 0xFFU);
        uid12[i * 4 + 1] = (uint8_t)((w >> 8) & 0xFFU);
        uid12[i * 4 + 2] = (uint8_t)((w >> 16) & 0xFFU);
        uid12[i * 4 + 3] = (uint8_t)((w >> 24) & 0xFFU);
    }
}

int bl_config_update_requested(void)
{
    return (bl_flags_read() & BL_BKP_UPDATE) != 0U;
}

void bl_config_set_update_requested(int on)
{
    uint32_t f = bl_flags_read();
    if (on) {
        f |= BL_BKP_UPDATE;
    } else {
        f &= ~BL_BKP_UPDATE;
    }
    bl_flags_write(f);
}

int bl_config_dfu_requested(void)
{
    return (bl_flags_read() & BL_BKP_DFU) != 0U;
}

void bl_config_set_dfu_requested(int on)
{
    uint32_t f = bl_flags_read();
    if (on) {
        f |= BL_BKP_DFU;
    } else {
        f &= ~BL_BKP_DFU;
    }
    bl_flags_write(f);
}

int bl_config_dfu_gpio_asserted(void)
{
    /* ponytail: board-specific DFU entry GPIO; verify pin/ polarity on HW */
    (void)0;
    return 0;
}

int bl_config_fault_entry(void)
{
    return (bl_flags_read() & BL_BKP_FAULT) != 0U;
}

void bl_config_clear_fault(void)
{
    bl_flags_write(bl_flags_read() & ~BL_BKP_FAULT);
}

uint32_t bl_config_active_slot_base(void)
{
#if defined(STM32G491xx)
    if ((bl_flags_read() & BL_BKP_SLOT_B) != 0U) {
        return BL_SLOT_B_BASE;
    }
#endif
    return BL_SLOT_A_BASE;
}

void bl_config_set_active_slot(uint8_t slot)
{
    uint32_t f = bl_flags_read();
#if defined(STM32G491xx)
    if (slot != 0U) {
        f |= BL_BKP_SLOT_B;
    } else {
        f &= ~BL_BKP_SLOT_B;
    }
#else
    (void)slot;
#endif
    bl_flags_write(f);
}

uint32_t bl_config_committed_length(void)
{
    return TAMP->BKP1R;
}

uint32_t bl_config_committed_crc(void)
{
    return TAMP->BKP2R;
}

void bl_config_set_committed(uint32_t length, uint32_t crc)
{
    TAMP->BKP1R = length;
    TAMP->BKP2R = crc;
}

static int bl_vector_looks_valid(uint32_t vector_base)
{
    uint32_t msp = *(const uint32_t *)vector_base;
    uint32_t reset = *(const uint32_t *)(vector_base + 4U);

    if (msp < BL_RAM_BASE || msp > (BL_RAM_BASE + BL_RAM_SIZE)) {
        return 0;
    }
    if (reset < BL_APP_VECTOR_BASE || reset < vector_base) {
        return 0;
    }
    if ((reset & 1U) == 0U) {
        return 0;
    }
    return 1;
}

int bl_config_app_valid(void)
{
    uint32_t base = bl_config_active_slot_base();
    uint32_t len = bl_config_committed_length();
    uint32_t expect = bl_config_committed_crc();

    if (!bl_vector_looks_valid(base)) {
        return 0;
    }
    if (len == 0U) {
        return 0;
    }
    if (bl_addr_in_bl_region(base, len)) {
        return 0;
    }

    return bl_crc32_buf((const uint8_t *)base, len) == expect;
}
