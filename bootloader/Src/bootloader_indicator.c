#include "bootloader_indicator.h"

#include "bootloader_hal.h"

#include <stdbool.h>

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

static bootloader_indicator_mode_t s_mode = BOOTLOADER_INDICATOR_NO_APP;

static void bootloader_indicator_enable_port_clock(void) {
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

void bootloader_indicator_init(void) {
    GPIO_InitTypeDef init = {0};
    bootloader_indicator_enable_port_clock();

    init.Pin = LED_PIN;
    init.Mode = GPIO_MODE_OUTPUT_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &init);
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}

void bootloader_indicator_set_mode(bootloader_indicator_mode_t mode) {
    s_mode = mode;
    if (s_mode == BOOTLOADER_INDICATOR_CONNECTED) {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
    }
}

void bootloader_indicator_update(uint32_t tick_ms) {
    bool led_on = false;

    if (s_mode == BOOTLOADER_INDICATOR_CONNECTED) {
        led_on = true;
    } else if (s_mode == BOOTLOADER_INDICATOR_FLASHING) {
        led_on = (tick_ms % 600U) < 300U;
    } else if (s_mode == BOOTLOADER_INDICATOR_APP_PRESENT) {
        uint32_t phase = tick_ms % 1000U;
        led_on = (phase < 100U) || ((phase >= 200U) && (phase < 300U));
    } else if (s_mode == BOOTLOADER_INDICATOR_NO_APP) {
        /*
         * Software PWM fade. The update loop runs often while polling UART, so
         * a 20 ms PWM frame is enough for a visible breathe without a timer.
         */
        uint32_t breath = tick_ms % 2000U;
        uint32_t level = (breath < 1000U) ? breath : (1999U - breath);
        uint32_t pwm = tick_ms % 20U;
        led_on = pwm < (level / 50U);
    } else if (s_mode == BOOTLOADER_INDICATOR_ERROR) {
        led_on = (tick_ms % 500U) < 250U;
    }

    HAL_GPIO_WritePin(LED_PORT, LED_PIN, led_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
