#include <stdint.h>
#include <stm32xx_hal.h>

int main() {
    HAL_Init();

    // Configure PB4 as an output pin
    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_4  // Configure PB4
    };

    GPIO_InitTypeDef led_config1 = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_5  // Configure PB4
    };
    
    __HAL_RCC_GPIOB_CLK_ENABLE(); // Enable clock for GPIOB
    HAL_GPIO_Init(GPIOB, &led_config); // Initialize GPIOB with led_config
    HAL_GPIO_Init(GPIOB, &led_config1);

    // Main loop
    while (1) {
        // Turn PB4 ON
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
        HAL_Delay(500);
       
    }

    return 0;
}