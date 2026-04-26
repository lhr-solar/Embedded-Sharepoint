#include <stdint.h>

#if defined(FIRMWARE_ROLE_BOOTLOADER)
#include "bootloader_board.h"
#elif defined(FIRMWARE_USES_BOOTLOADER)
#include "bootloader_command.h"
#endif

#if defined(FIRMWARE_ROLE_BOOTLOADER) || defined(FIRMWARE_USES_BOOTLOADER)
void HardFault_Handler(void) {
#if defined(FIRMWARE_ROLE_BOOTLOADER)
    GPIO_InitTypeDef init = {0};
    bootloader_board_enable_led_port_clock();

    init.Pin = BOOTLOADER_LED_PIN;
    init.Mode = GPIO_MODE_OUTPUT_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BOOTLOADER_LED_PORT, &init);

    while (1) {
        HAL_GPIO_TogglePin(BOOTLOADER_LED_PORT, BOOTLOADER_LED_PIN);
        for (volatile uint32_t i = 0U; i < 600000U; i++) {
            __NOP();
        }
    }
#elif defined(FIRMWARE_USES_BOOTLOADER)
    bootloader_command_request_reset();
    while (1) {}
#endif
}
#endif
