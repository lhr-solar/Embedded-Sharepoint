#include "bootloader_command.h"

#if defined(FIRMWARE_ROLE_BOOTLOADER)
#include "bootloader_hal.h"
#else
#include "stm32xx_hal.h"
#endif

#if defined(FIRMWARE_TYPE_APP) && !defined(FIRMWARE_USES_BOOTLOADER)
#error "FIRMWARE_TYPE=app requires FIRMWARE_USES_BOOTLOADER. Check BOOTLOADER_SIZE_KB."
#endif

#if defined(FIRMWARE_USES_BOOTLOADER) && !defined(BOOTLOADER_APP_BASE)
#error "FIRMWARE_USES_BOOTLOADER requires BOOTLOADER_APP_BASE."
#endif

static size_t s_uart_command_match_len = 0U;
static volatile bool s_entry_allowed = true;

static volatile uint32_t *bootloader_command_magic_register(void) {
#if defined(TAMP) && defined(TAMP_BKP0R)
    return &TAMP->BKP0R;
#elif defined(RTC) && defined(RTC_BKP0R)
    return &RTC->BKP0R;
#else
    return NULL;
#endif
}

static volatile uint32_t *bootloader_command_can_handshake_register(void) {
#if defined(TAMP) && defined(TAMP_BKP1R)
    return &TAMP->BKP1R;
#elif defined(RTC) && defined(RTC_BKP1R)
    return &RTC->BKP1R;
#else
    return NULL;
#endif
}

static void bootloader_command_enable_backup_access(void) {
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

static void bootloader_command_set_can_handshake_value(uint8_t value) {
    bootloader_command_enable_backup_access();

    volatile uint32_t *handshake = bootloader_command_can_handshake_register();
    if (handshake != NULL) {
        *handshake = value;
    }
}

static void bootloader_command_set_magic_and_reset(uint32_t magic_word) {
    bootloader_command_enable_backup_access();

    volatile uint32_t *magic = bootloader_command_magic_register();
    if (magic != NULL) {
        *magic = magic_word;
    }

    __DSB();
    NVIC_SystemReset();
}

bool bootloader_command_feed_uart_byte(uint8_t byte) {
    const char command[] = BOOTLOADER_UART_COMMAND;
    const size_t command_len = sizeof(command) - 1U;

    if (byte == (uint8_t)command[s_uart_command_match_len]) {
        s_uart_command_match_len++;
        if (s_uart_command_match_len == command_len) {
            s_uart_command_match_len = 0U;
            return true;
        }
        return false;
    }

    s_uart_command_match_len = (byte == (uint8_t)command[0]) ? 1U : 0U;
    return false;
}

void bootloader_command_reset_uart_parser(void) {
    s_uart_command_match_len = 0U;
}

bool bootloader_command_is_can_reset_message(uint32_t id, const uint8_t *data, size_t len) {
    return (id == BOOTLOADER_CAN_COMMAND_ID) && (data != NULL) && (len > 0U) && (data[0] == 0U);
}

bool bootloader_command_handle_can_message(uint32_t id, const uint8_t *data, size_t len) {
    if ((id != BOOTLOADER_CAN_COMMAND_ID) || (data == NULL) || (len == 0U)) {
        return false;
    }

    if (bootloader_command_is_can_reset_message(id, data, len)) {
        bootloader_command_clear_request();
        __DSB();
        NVIC_SystemReset();
        return true;
    }

    if (!bootloader_command_is_entry_allowed()) {
        return false;
    }

    bootloader_command_set_can_handshake_value(data[0]);

    if (data[0] == (uint8_t)BOOTLOADER_CAN_NODE_ID) {
        bootloader_command_request_reset();
    } else {
        bootloader_command_request_hold_reset();
    }

    return true;
}

void bootloader_command_set_entry_allowed(bool allowed) {
    s_entry_allowed = allowed;
}

bool bootloader_command_is_entry_allowed(void) {
    return s_entry_allowed;
}

void bootloader_command_init_app_vector_table(void) {
#if defined(FIRMWARE_USES_BOOTLOADER) && defined(BOOTLOADER_APP_BASE)
    SCB->VTOR = BOOTLOADER_APP_BASE;
    __DSB();
    __ISB();
#endif
}

void bootloader_command_request_reset(void) {
    bootloader_command_set_magic_and_reset(BOOTLOADER_MAGIC_WORD);
}

void bootloader_command_request_hold_reset(void) {
    bootloader_command_set_magic_and_reset(BOOTLOADER_HOLD_MAGIC_WORD);
}

void bootloader_command_clear_request(void) {
    bootloader_command_enable_backup_access();

    volatile uint32_t *magic = bootloader_command_magic_register();
    if (magic != NULL) {
        *magic = 0U;
    }

    volatile uint32_t *handshake = bootloader_command_can_handshake_register();
    if (handshake != NULL) {
        *handshake = 0U;
    }
}

bootloader_command_request_t bootloader_command_consume_request(void) {
    bootloader_command_enable_backup_access();

    volatile uint32_t *magic = bootloader_command_magic_register();
    if (magic == NULL) {
        return BOOTLOADER_COMMAND_REQUEST_NONE;
    }

    uint32_t magic_value = *magic;
    *magic = 0U;

    if (magic_value == BOOTLOADER_MAGIC_WORD) {
        return BOOTLOADER_COMMAND_REQUEST_FLASH;
    }
    if (magic_value == BOOTLOADER_HOLD_MAGIC_WORD) {
        return BOOTLOADER_COMMAND_REQUEST_HOLD;
    }

    return BOOTLOADER_COMMAND_REQUEST_NONE;
}

uint8_t bootloader_command_consume_can_handshake_value(void) {
    bootloader_command_enable_backup_access();

    volatile uint32_t *handshake = bootloader_command_can_handshake_register();
    if (handshake == NULL) {
        return 0U;
    }

    uint8_t value = (uint8_t)(*handshake & 0xFFU);
    *handshake = 0U;
    return value;
}
