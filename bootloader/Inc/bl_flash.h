#ifndef BL_FLASH_H
#define BL_FLASH_H

#include <stdint.h>

int bl_flash_unlock(void);
void bl_flash_lock(void);
int bl_flash_page_erase(uint32_t page_addr);
int bl_flash_program_block(uint32_t addr, const uint8_t *data, uint32_t len);
uint32_t bl_flash_crc32(uint32_t base, uint32_t len);
uint32_t bl_flash_addr_to_page(uint32_t addr);

#endif /* BL_FLASH_H */
