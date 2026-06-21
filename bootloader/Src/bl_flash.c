#include "bl_flash.h"
#include "bl_crc32.h"
#include "bl_protocol.h"
#include "bl_target.h"

#include "stm32g4xx.h"

#define FLASH_KEY1 0x45670123U
#define FLASH_KEY2 0xCDEF89ABU

static int bl_flash_wait(void)
{
    while ((FLASH->SR & FLASH_SR_BSY) != 0U) {
    }

    if ((FLASH->SR & (FLASH_SR_OPERR | FLASH_SR_PROGERR | FLASH_SR_WRPERR |
                      FLASH_SR_PGAERR | FLASH_SR_SIZERR | FLASH_SR_PGSERR |
                      FLASH_SR_MISERR | FLASH_SR_FASTERR | FLASH_SR_RDERR)) != 0U) {
        FLASH->SR = FLASH_SR_OPERR | FLASH_SR_PROGERR | FLASH_SR_WRPERR |
                    FLASH_SR_PGAERR | FLASH_SR_SIZERR | FLASH_SR_PGSERR |
                    FLASH_SR_MISERR | FLASH_SR_FASTERR | FLASH_SR_RDERR |
                    FLASH_SR_EOP;
        return -1;
    }

    if ((FLASH->SR & FLASH_SR_EOP) != 0U) {
        FLASH->SR = FLASH_SR_EOP;
    }

    return 0;
}

int bl_flash_unlock(void)
{
    if ((FLASH->CR & FLASH_CR_LOCK) != 0U) {
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
        if ((FLASH->CR & FLASH_CR_LOCK) != 0U) {
            return -1;
        }
    }
    return 0;
}

void bl_flash_lock(void)
{
    FLASH->CR |= FLASH_CR_LOCK;
}

uint32_t bl_flash_addr_to_page(uint32_t addr)
{
    return (addr - BL_FLASH_BASE) / BL_PAGE_SIZE;
}

int bl_flash_page_erase(uint32_t page_addr)
{
    if (bl_addr_in_bl_region(page_addr, BL_PAGE_SIZE)) {
        return -1;
    }

    if (bl_flash_unlock() != 0) {
        return -1;
    }

    uint32_t page = bl_flash_addr_to_page(page_addr);
    FLASH->CR = (FLASH->CR & ~(FLASH_CR_PNB | FLASH_CR_PER)) |
                ((page << FLASH_CR_PNB_Pos) & FLASH_CR_PNB);
    FLASH->CR |= FLASH_CR_PER;
    FLASH->CR |= FLASH_CR_STRT;

    if (bl_flash_wait() != 0) {
        FLASH->CR &= ~(FLASH_CR_PER | FLASH_CR_PNB);
        bl_flash_lock();
        return -1;
    }

    FLASH->CR &= ~(FLASH_CR_PER | FLASH_CR_PNB);
    bl_flash_lock();
    return 0;
}

int bl_flash_program_block(uint32_t addr, const uint8_t *data, uint32_t len)
{
    if ((len != BL_DATA_BLOCK_BYTES) || ((addr & 7U) != 0U) ||
        bl_addr_in_bl_region(addr, len)) {
        return -1;
    }

    if (bl_flash_unlock() != 0) {
        return -1;
    }

    for (uint32_t off = 0; off < len; off += 8U) {
        uint64_t dw = 0;
        for (uint32_t i = 0; i < 8U; i++) {
            dw |= ((uint64_t)data[off + i] << (8U * i));
        }

        FLASH->CR |= FLASH_CR_PG;
        *(volatile uint32_t *)addr = (uint32_t)dw;
        __ISB();
        *(volatile uint32_t *)(addr + 4U) = (uint32_t)(dw >> 32);

        if (bl_flash_wait() != 0) {
            FLASH->CR &= ~FLASH_CR_PG;
            bl_flash_lock();
            return -1;
        }

        FLASH->CR &= ~FLASH_CR_PG;
        addr += 8U;
    }

    bl_flash_lock();
    return 0;
}

uint32_t bl_flash_crc32(uint32_t base, uint32_t len)
{
    return bl_crc32_buf((const uint8_t *)base, len);
}
