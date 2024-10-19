#include "boot.h"

#ifdef STM32F4xx
#include "system_stm32f4xx.h"
#else
#include "system_stm32l4xx.h"
#endif

#include "cmsis_gcc.h"

void Reset_Handler(){
    __set_MSP((uint32_t)_estack);

    SystemInit();

    // Copy the data segment initializers from flash to SRAM.
    uint8_t *pSrc = (uint8_t *)_app_start;
    uint8_t *pDest = (uint8_t *)_boot_start;
    for ( ; pDest < _boot_start + _app_len;){
        *pDest++ = *pSrc++;
    }

    // Zero fill the bss segment.
    for (pDest = _boot_start; pDest < _boot_start + _boot_len;){
        *pDest++ = 0;
    }

    boot();
}

void boot(){
    // Call the application's entry point.
    startapp(_app_start, _estack);
}

void startapp(uint8_t *addr, uint8_t *stack){
    // Set the stack pointer.
    __set_MSP((uint32_t)stack);

    // Jump to the application's entry point.
    void (*app_entry)(void) = (void (*)(void))addr;
    app_entry();
}