#ifndef FLASH_H
#define FLASH_H

#include <stdbool.h>
#include <stdint.h>

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
    NUM_FLASH_IDS
} command_id_t;

typedef struct {
    // Three byte header
    command_id_t id : 8; // command id
    uint16_t data_size : 16; // data size (bytes)
    uint32_t address : 32;
} flash_cmd_t;

#define MAX_BUFFER_SIZE (0xFFFF)

bool exec_flash_command(void* buf, flash_cmd_t *cmd);
bool flash_write(void* buf, uint32_t address, uint16_t data_size);
bool flash_read_single(void* buf, uint32_t address, uint16_t data_size);
bool flash_read_buf(void* buf, uint32_t address, uint16_t data_size);
bool flash_erase(uint32_t address);
bool flash_mass_erase(void);

#endif
