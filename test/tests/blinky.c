#include "stm32xx_hal.h"

int main(){
    HAL_Init();

    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_3
    };
    
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable clock for GPIOA
    HAL_GPIO_Init(GPIOC, &led_config); // initialize GPIOA with led_config

    while(1){
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
        HAL_Delay(50);
    }

    return 0;
}
