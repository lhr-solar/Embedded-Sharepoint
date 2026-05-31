#ifndef BOOTLOADER_COMMAND_H_
#define BOOTLOADER_COMMAND_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BOOTLOADER_UART_COMMAND "ESBLT_BOOT\n"
#define BOOTLOADER_MAGIC_WORD (0x4553424CUL)
#define BOOTLOADER_HOLD_MAGIC_WORD (0x45534248UL)

#ifndef BOOTLOADER_CAN_COMMAND_ID
#define BOOTLOADER_CAN_COMMAND_ID (0x6F0U)
#endif

#ifndef BOOTLOADER_CAN_NODE_ID
#define BOOTLOADER_CAN_NODE_ID (0xFFU)
#endif

#ifndef BOOTLOADER_CAN_HANDSHAKE_ID
#define BOOTLOADER_CAN_HANDSHAKE_ID (BOOTLOADER_CAN_COMMAND_ID + 1U)
#endif

#ifndef BOOTLOADER_CAN_HANDSHAKE_COUNT
#define BOOTLOADER_CAN_HANDSHAKE_COUNT (50U)
#endif

#ifndef BOOTLOADER_CAN_HANDSHAKE_INTERVAL_MS
#define BOOTLOADER_CAN_HANDSHAKE_INTERVAL_MS (10U)
#endif

typedef enum {
    BOOTLOADER_COMMAND_REQUEST_NONE = 0,
    BOOTLOADER_COMMAND_REQUEST_HOLD,
    BOOTLOADER_COMMAND_REQUEST_FLASH,
} bootloader_command_request_t;

typedef bool (*bootloader_command_can_send_t)(uint32_t id, const uint8_t *data, size_t len, void *context);

bool bootloader_command_feed_uart_byte(uint8_t byte);
void bootloader_command_reset_uart_parser(void);
bool bootloader_command_is_can_reset_message(uint32_t id, const uint8_t *data, size_t len);
bool bootloader_command_handle_can_message(uint32_t id, const uint8_t *data, size_t len);
bool bootloader_command_can_handshake_pending(void);
bool bootloader_command_can_handshake_service(bootloader_command_can_send_t send, void *context);
void bootloader_command_set_entry_allowed(bool allowed);
bool bootloader_command_is_entry_allowed(void);
void bootloader_command_init_app_vector_table(void);
void bootloader_command_request_reset(void);
void bootloader_command_request_hold_reset(void);
void bootloader_command_clear_request(void);
bootloader_command_request_t bootloader_command_consume_request(void);

#endif /* BOOTLOADER_COMMAND_H_ */
