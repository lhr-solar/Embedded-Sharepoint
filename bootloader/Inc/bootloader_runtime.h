#ifndef BOOTLOADER_RUNTIME_H_
#define BOOTLOADER_RUNTIME_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    BOOTLOADER_TRANSPORT_UART = 0,
    BOOTLOADER_TRANSPORT_CAN,
} bootloader_transport_t;

typedef struct {
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
} bootloader_can_frame_t;

void bootloader_runtime_init(void);
bootloader_transport_t bootloader_runtime_active_transport(void);
bool bootloader_runtime_wait_for_handshake(uint32_t timeout_ms);
bool bootloader_runtime_poll_sync(uint32_t timeout_ms);
bool bootloader_runtime_send_bytes(const uint8_t *data, uint16_t len);
bool bootloader_runtime_read_bytes(uint8_t *data, uint16_t len, uint32_t timeout_ms);
bool bootloader_runtime_can_send(uint32_t id, const uint8_t *data, uint8_t len);
bool bootloader_runtime_can_recv(bootloader_can_frame_t *frame, uint32_t timeout_ms);
bool bootloader_runtime_erase_app(void);
bool bootloader_runtime_write_app(uint32_t app_offset, const uint8_t *data, size_t len);
bool bootloader_runtime_is_app_valid(void);
void bootloader_runtime_jump_to_app(void);

#endif
