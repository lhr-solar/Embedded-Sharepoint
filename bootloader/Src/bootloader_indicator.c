#include "bootloader_indicator.h"

#include "bootloader_board.h"
#include "bootloader_hal.h"

#include <stdbool.h>

static bootloader_indicator_mode_t s_mode = BOOTLOADER_INDICATOR_NO_APP;

void bootloader_indicator_init(void) {
    GPIO_InitTypeDef init = {0};
    bootloader_board_enable_led_port_clock();

    init.Pin = BOOTLOADER_LED_PIN;
    init.Mode = GPIO_MODE_OUTPUT_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BOOTLOADER_LED_PORT, &init);
    HAL_GPIO_WritePin(BOOTLOADER_LED_PORT, BOOTLOADER_LED_PIN, GPIO_PIN_RESET);
}

void bootloader_indicator_set_mode(bootloader_indicator_mode_t mode) {
    s_mode = mode;
    if (s_mode == BOOTLOADER_INDICATOR_CONNECTED) {
        HAL_GPIO_WritePin(BOOTLOADER_LED_PORT, BOOTLOADER_LED_PIN, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(BOOTLOADER_LED_PORT, BOOTLOADER_LED_PIN, GPIO_PIN_RESET);
    }
}

void bootloader_indicator_update(uint32_t tick_ms) {
    bool led_on = false;

    if (s_mode == BOOTLOADER_INDICATOR_CONNECTED) {
        led_on = true;
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

    HAL_GPIO_WritePin(BOOTLOADER_LED_PORT, BOOTLOADER_LED_PIN, led_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
