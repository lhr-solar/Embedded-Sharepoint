#ifndef BL_TARGET_H
#define BL_TARGET_H

#include <stdint.h>

#if defined(STM32G491xx)
#define BL_FLASH_BASE       0x08000000U
#define BL_REGION_SIZE      0x4000U
#define BL_PAGE_SIZE        0x800U
#define BL_APP_VECTOR_BASE  0x08004000U
#define BL_SLOT_A_BASE      0x08004000U
#define BL_SLOT_A_SIZE      0x3C000U
#define BL_SLOT_B_BASE      0x08040000U
#define BL_SLOT_B_SIZE      0x40000U
#define BL_IMAGE_BUDGET     16384U
#define BL_RAM_BASE         0x20000000U
#define BL_RAM_SIZE         (112U * 1024U)
#elif defined(STM32G431xx)
#define BL_FLASH_BASE       0x08000000U
#define BL_REGION_SIZE      0x2000U
#define BL_PAGE_SIZE        0x800U
#define BL_APP_VECTOR_BASE  0x08002000U
#define BL_SLOT_A_BASE      0x08002000U
#define BL_SLOT_A_SIZE      0x1E000U
#define BL_SLOT_B_BASE      0U
#define BL_SLOT_B_SIZE      0U
#define BL_IMAGE_BUDGET     8192U
#define BL_RAM_BASE         0x20000000U
#define BL_RAM_SIZE         (32U * 1024U)
#else
#error "Define STM32G491xx or STM32G431xx for bootloader target"
#endif

#define BL_REGION_END (BL_FLASH_BASE + BL_REGION_SIZE)

static inline int bl_addr_in_bl_region(uint32_t addr, uint32_t len)
{
    uint32_t end = addr + len;
    return (addr < BL_REGION_END) && (end > BL_FLASH_BASE);
}

#endif /* BL_TARGET_H */
