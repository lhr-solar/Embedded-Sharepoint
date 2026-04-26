#include "stm32xx_hal.h"

#if defined(STM32L432xx)
#define LED_PIN GPIO_PIN_3
#define LED_PORT GPIOB
#elif defined(STM32L431xx)
#define LED_PIN GPIO_PIN_11
#define LED_PORT GPIOB
#elif defined(STM32G473xx)
#define LED_PIN GPIO_PIN_3
#define LED_PORT GPIOC
#else
#define LED_PIN GPIO_PIN_5
#define LED_PORT GPIOA
#endif

#ifndef APP_VECTOR_TABLE_BASE
#define APP_VECTOR_TABLE_BASE (0x08010000UL)
#endif

static void heartbeat_clock_init(void) {
    if (LED_PORT == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (LED_PORT == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (LED_PORT == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (LED_PORT == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
}

static void vectortable_init(void) {
    /* App is linked after bootloader, so move VTOR accordingly. */
    SCB->VTOR = APP_VECTOR_TABLE_BASE;
    __DSB();
    __ISB();
}

int main(void) {
    vectortable_init();
    HAL_Init();
    SystemClock_Config();

    GPIO_InitTypeDef led_config = {0};
    led_config.Mode = GPIO_MODE_OUTPUT_PP;
    led_config.Pull = GPIO_NOPULL;
    led_config.Pin = LED_PIN;

    heartbeat_clock_init();
    HAL_GPIO_Init(LED_PORT, &led_config);

    while (1) {
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        HAL_Delay(1000);
    }
}
