#ifndef BOOTLOADER_RUNTIME_H_
#define BOOTLOADER_RUNTIME_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void bootloader_runtime_init(void);
bool bootloader_runtime_wait_for_handshake(uint32_t timeout_ms);
bool bootloader_runtime_poll_sync(uint32_t timeout_ms);
bool bootloader_runtime_send_bytes(const uint8_t *data, uint16_t len);
bool bootloader_runtime_read_bytes(uint8_t *data, uint16_t len, uint32_t timeout_ms);
bool bootloader_runtime_erase_app(void);
bool bootloader_runtime_write_app(uint32_t app_offset, const uint8_t *data, size_t len);
bool bootloader_runtime_is_app_valid(void);
void bootloader_runtime_jump_to_app(void);

#endif
