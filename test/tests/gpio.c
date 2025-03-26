#include "stm32xx_hal.h"

int main() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_Init();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    // SystemClock_Config();
    // MX_GPIO_Init();

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    volatile uint32_t val;

    for (;;) {
        val = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
        val += 0;
    } 

}