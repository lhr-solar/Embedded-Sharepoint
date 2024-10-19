#include <stdint.h>
#include <stm32xx_hal.h>
//working blinky for stm32f401re


// int main() {
//     HAL_Init();

//     // Configure PB4 as an output pin
//     GPIO_InitTypeDef led_config = {
//         .Mode = GPIO_MODE_OUTPUT_PP,
//         .Pull = GPIO_NOPULL,
//         .Pin = GPIO_PIN_4  // Configure PB4
//     };

//     GPIO_InitTypeDef led_config1 = {
//         .Mode = GPIO_MODE_OUTPUT_PP,
//         .Pull = GPIO_NOPULL,
//         .Pin = GPIO_PIN_5  // Configure PB4
//     };
    
//     __HAL_RCC_GPIOB_CLK_ENABLE(); // Enable clock for GPIOB
//     HAL_GPIO_Init(GPIOA, &led_config); // Initialize GPIOB with led_config
//     HAL_GPIO_Init(GPIOA, &led_config1);

//     // Main loop
//     while (1) {
//         // Turn PB4 ON
//         HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//         HAL_Delay(500);
       
//     }

//     return 0;
// }

// #include "stm32xx_hal.h"

// Define LED pin and port
#define LED_PIN GPIO_PIN_5
#define LED_PORT GPIOA

// Function prototypes
// GPIO initialization function
static void MX_GPIO_Init(void)
{
    // Enable GPIOA clock
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure the LED pin as output
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

int main(void)
{
    // Initialize HAL Library
    HAL_Init();

    // Configure the system clock
    //SystemClock_Config();

    // Initialize GPIO for LED
    MX_GPIO_Init();

    // Main loop
    while (1)
    {
        // Toggle the LED
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);

        // Wait for 500 milliseconds
        HAL_Delay(500);
    }
}
