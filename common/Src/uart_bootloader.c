#include "uart_bootloader.h"

#if defined(FIRMWARE_ROLE_BOOTLOADER)
#include "bootloader_hal.h"
#else
#include "stm32xx_hal.h"
#endif

#include <stddef.h>

static size_t s_command_match_len = 0U;

static volatile uint32_t *uart_bootloader_magic_register(void) {
#if defined(TAMP) && defined(TAMP_BKP0R)
    return &TAMP->BKP0R;
#elif defined(RTC) && defined(RTC_BKP0R)
    return &RTC->BKP0R;
#else
    return NULL;
#endif
}

static void uart_bootloader_enable_backup_access(void) {
#if defined(__HAL_RCC_PWR_CLK_ENABLE)
    __HAL_RCC_PWR_CLK_ENABLE();
#endif
    HAL_PWR_EnableBkUpAccess();
#if defined(__HAL_RCC_RTCAPB_CLK_ENABLE)
    __HAL_RCC_RTCAPB_CLK_ENABLE();
#elif defined(__HAL_RCC_RTC_CLK_ENABLE)
    __HAL_RCC_RTC_CLK_ENABLE();
#endif
}

bool uart_bootloader_feed_command_byte(uint8_t byte) {
    const char command[] = UART_BOOTLOADER_COMMAND;
    const size_t command_len = sizeof(command) - 1U;

    if (byte == (uint8_t)command[s_command_match_len]) {
        s_command_match_len++;
        if (s_command_match_len == command_len) {
            s_command_match_len = 0U;
            return true;
        }
        return false;
    }

    s_command_match_len = (byte == (uint8_t)command[0]) ? 1U : 0U;
    return false;
}

void uart_bootloader_command_reset(void) {
    s_command_match_len = 0U;
}

void uart_bootloader_request_reset(void) {
    uart_bootloader_enable_backup_access();

    volatile uint32_t *magic = uart_bootloader_magic_register();
    if (magic != NULL) {
        *magic = UART_BOOTLOADER_MAGIC_WORD;
    }

    __DSB();
    NVIC_SystemReset();
}

bool uart_bootloader_consume_request(void) {
    uart_bootloader_enable_backup_access();

    volatile uint32_t *magic = uart_bootloader_magic_register();
    if ((magic == NULL) || (*magic != UART_BOOTLOADER_MAGIC_WORD)) {
        return false;
    }

    *magic = 0U;
    return true;
}
