#include "stm32xx_hal.h"

int main(){
    __disable_irq();

    SystemClock_Config();
    HAL_Init();

    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = LED_PIN
    };
    
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable clock for GPIOA
    HAL_GPIO_Init(LED_PORT, &led_config); // initialize GPIOA with led_config
    
    // Systick
    HAL_NVIC_EnableIRQ(SysTick_IRQn);
    __enable_irq();

    while(1){
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        HAL_Delay(1000);
    }

    return 0;
}