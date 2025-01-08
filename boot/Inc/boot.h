#ifndef BOOT_H
#define BOOT_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    // HAL Specific
    BLDR_OK, // HAL_OK
    BLDR_ERR, // HAL_ERR
    BLDR_BUSY, // HAL_BUSY
    BLDR_TMOUT, // HAL_TIMEOUT

    // Bootloader Specific
    BLDR_FAIL_INIT, // Bootloader failed to initialize
    BLDR_FAIL_STX, // Bootloader failed to receive STX
    BLDR_FAIL_HDR, // Bootloader failed during header
    BLDR_FAIL_DATA, // Bootloader failed during data
    BLDR_FAIL_FLASH, // Bootloader failed to flash

    BLDR_REGULAR_START, // Bootloader does not recognize any incoming command, start application
    BLDR_START_AFTER_UPDATE, // Bootloader starts application after update
} error_code_t;

error_code_t boot_init();
void boot_deinit();
void startapp();

#endif