#include "boot.h"
#include "stm32xx_hal.h"
#include "cmsis_gcc.h"
#include <stdint.h>

static GPIO_InitTypeDef GPIO_InitStruct = {
    .Pin = LED_PIN,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_LOW
};

void boot_init(){
    // Disable interrupts
    __disable_irq();

    HAL_Init();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // Turn on LED with HAL
    HAL_GPIO_WritePin(LED_PORT, GPIO_PIN_5, GPIO_PIN_SET);
}

void boot_deinit(){
    // Turn off LED with HAL
    HAL_GPIO_WritePin(LED_PORT, GPIO_PIN_5, GPIO_PIN_RESET);

    HAL_GPIO_DeInit(LED_PORT, LED_PIN);
    __HAL_RCC_GPIOA_CLK_DISABLE();

    HAL_DeInit();
}

void boot(){
    // Initialize
    boot_init();

    // Deinitialize
    boot_deinit();

    // Call the application's entry point.
    startapp();
}