#include "bl_jump.h"

#include "stm32g4xx.h"

typedef void (*bl_reset_fn)(void);

void bl_jump_to_app(uint32_t vector_base)
{
    /* ponytail: duplicates Sharepoint_RelocateVectorTableTo to keep BL HAL-free */
    uint32_t msp = *(volatile uint32_t *)vector_base;
    bl_reset_fn reset = (bl_reset_fn)(*(volatile uint32_t *)(vector_base + 4U));

    __disable_irq();
    SCB->VTOR = vector_base;
    __DSB();
    __ISB();
    __set_MSP(msp);
    reset();
    for (;;) {
    }
}

void bl_enter_rom_dfu(void)
{
    /*
     * Runtime ROM-DFU entry (system memory @ 0x1FFF0000).
     * Alternative: set BOOT0 + nSWBOOT0 option bytes and reset (dfu-util -d ...).
     * ponytail: verify exact entry sequence and USB enumeration on target HW.
     */
    __disable_irq();

    RCC->AHB1ENR = 0U;
    RCC->AHB2ENR = 0U;
    RCC->AHB3ENR = 0U;
    RCC->APB1ENR1 = 0U;
    RCC->APB1ENR2 = 0U;
    RCC->APB2ENR = 0U;

    SCB->VTOR = 0U;
    __DSB();
    __ISB();

    __set_MSP(*(volatile uint32_t *)0x1FFF0000U);
    bl_reset_fn rom_reset = (bl_reset_fn)(*(volatile uint32_t *)0x1FFF0004U);
    rom_reset();

    for (;;) {
    }
}

void bl_system_reset(void)
{
    NVIC_SystemReset();
    for (;;) {
    }
}
