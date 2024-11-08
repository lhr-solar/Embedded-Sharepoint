#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>
#include <stdbool.h>

typedef enum{
    UART,
    CAN
} protocol_t;

typedef enum {
    FLASH_WRITE,
    FLASH_READ_SINGLE,
    FLASH_READ_BUF,
    FLASH_ERASE,
    FLASH_MASS_ERASE,
    NUM_IDS
} command_id_t;

typedef struct {
    // Three byte header
    command_id_t id : 8; // command id
    uint8_t data_size : 8; // data size (bytes)
    uint32_t address : 32;
} flash_cmd_t;

#define MAX_BUFFER_SIZE

bool exec_flash_command(flash_cmd_t *cmd);
bool init_boot();
bool flash_write();
bool flash_read_single();
bool flash_erase();
bool flash_read_buf();

#endif