#ifndef BOOTLOADER_BOARD_H_
#define BOOTLOADER_BOARD_H_

#include "bootloader_hal.h"

#if defined(STM32L432xx)
#define BOOTLOADER_LED_PIN GPIO_PIN_3
#define BOOTLOADER_LED_PORT GPIOB
#elif defined(STM32L431xx)
#define BOOTLOADER_LED_PIN GPIO_PIN_11
#define BOOTLOADER_LED_PORT GPIOB
#elif defined(STM32G473xx)
#define BOOTLOADER_LED_PIN GPIO_PIN_3
#define BOOTLOADER_LED_PORT GPIOC
#else
#define BOOTLOADER_LED_PIN GPIO_PIN_5
#define BOOTLOADER_LED_PORT GPIOA
#endif

static inline void bootloader_board_enable_led_port_clock(void) {
    if (BOOTLOADER_LED_PORT == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (BOOTLOADER_LED_PORT == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (BOOTLOADER_LED_PORT == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (BOOTLOADER_LED_PORT == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
}

#endif /* BOOTLOADER_BOARD_H_ */
