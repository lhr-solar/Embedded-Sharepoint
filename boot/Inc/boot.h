#ifndef BOOT_H
#define BOOT_H

#include <stdint.h>
#include <stdbool.h>

#include "boot_shared.h"

// These map directly to the command_id_t enum in flash.h
typedef enum {
    CMD_FLASH_WRITE, // FLASH_WRITE
    CMD_FLASH_READ_SINGLE, // FLASH_READ_SINGLE
    CMD_FLASH_READ_BUF, // FLASH_READ_BUF
    CMD_FLASH_ERASE, // FLASH_ERASE
    CMD_FLASH_MASS_ERASE, // FLASH_MASS_ERASE
    CMD_BLDR_START_AFTER_UPDATE, // BLDR_START_APP_AFTER_UPDATE
    NUM_BLDR_IDS
} bldr_command_id_t;

error_code_t boot_init();
void boot_deinit();
void startapp();

#endif
