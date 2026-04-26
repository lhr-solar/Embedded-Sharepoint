#include <stdint.h>

#if defined(FIRMWARE_ROLE_BOOTLOADER)
#if defined(STM32F4xx)
#include "stm32f4xx_hal.h"
#elif defined(STM32L4xx)
#include "stm32l4xx_hal.h"
#elif defined(STM32G4xx)
#include "stm32g4xx_hal.h"
#endif
#endif

// This is load bearing
void _init(){}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
void HardFault_Handler(){
#if defined(FIRMWARE_ROLE_BOOTLOADER)
#if defined(STM32L432xx)
    #define HF_LED_PIN GPIO_PIN_3
    #define HF_LED_PORT GPIOB
#elif defined(STM32L431xx)
    #define HF_LED_PIN GPIO_PIN_11
    #define HF_LED_PORT GPIOB
#elif defined(STM32G473xx)
    #define HF_LED_PIN GPIO_PIN_3
    #define HF_LED_PORT GPIOC
#else
    #define HF_LED_PIN GPIO_PIN_5
    #define HF_LED_PORT GPIOA
#endif

    if (HF_LED_PORT == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (HF_LED_PORT == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (HF_LED_PORT == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (HF_LED_PORT == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }

    GPIO_InitTypeDef init = {0};
    init.Pin = HF_LED_PIN;
    init.Mode = GPIO_MODE_OUTPUT_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(HF_LED_PORT, &init);

    while (1) {
        HAL_GPIO_TogglePin(HF_LED_PORT, HF_LED_PIN);
        for (volatile uint32_t i = 0U; i < 600000U; i++) {
            __NOP();
        }
    }
#else
    // Configurable Fault Status Register
    // Consists of MMSR, BFSR and UFSR
    volatile uint32_t _CFSR = (*((volatile unsigned long *)(0xE000ED28))) ;

    // Hard Fault Status Register
    volatile uint32_t _HFSR = (*((volatile unsigned long *)(0xE000ED2C))) ;

    // Debug Fault Status Register
    volatile uint32_t _DFSR = (*((volatile unsigned long *)(0xE000ED30))) ;

    // Auxiliary Fault Status Register
    volatile uint32_t _AFSR = (*((volatile unsigned long *)(0xE000ED3C))) ;

    // Read the Fault Address Registers. These may not contain valid values.
    // Check BFARVALID/MMARVALID to see if they are valid values
    // MemManage Fault Address Register
    volatile uint32_t _MMAR = (*((volatile unsigned long *)(0xE000ED34))) ;
    // Bus Fault Address Register
    volatile uint32_t _BFAR = (*((volatile unsigned long *)(0xE000ED38))) ;

    __asm("BKPT #0\n") ; // Break into the debugger
#endif
}
#pragma GCC diagnostic pop

