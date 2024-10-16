#include "stm32xx_hal.h"
#include <stdint.h>

#include "boot.h"

void Reset_Handler(void){
    __set_MSP(*((uint32_t*)__ram_start__));

    SystemInit();

    // Copy data segment initializers from flash to SRAM
    uint32_t* pSrc = _sidata;
    uint32_t* pDest = _sdata;
    while (pDest < _edata) {
        *pDest++ = *pSrc++;
    }

    // Zero fill the bss segment
    pDest = _sbss;
    while (pDest < _ebss) {
        *pDest++ = 0;
    }

    // Call the bootloader and reset ram
    __set_MSP(*((uint32_t*)__ram_start__));
    boot();
}

void deinit(){
    __disable_irq();
    // TODO: Deinit all hardware peripherals
    HAL_DeInit();
}

void jumpto(uint32_t sp, uint32_t pc){
    // Set the main stack pointer
    __set_MSP(sp);
    // Jump to the application's entry point
    __asm("bx %0"::"r"(pc));
}

void boot(){
    // Deinitialize HAL
    deinit();

    // Parse stuff

    // Jump to main
    uint32_t app_sp = __appflash_start__[0];
    uint32_t app_pc = __appflash_start__[1];

    jumpto(app_sp, app_pc);
}