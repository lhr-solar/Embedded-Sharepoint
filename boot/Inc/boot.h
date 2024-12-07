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
    BLDR_FAIL_UART_INIT, // Bootloader failed to recv UART init msgs
    BLDR_FAIL_STX, // Bootloader failed to receive STX
} error_code_t;

error_code_t boot_init();
void boot_deinit();
void startapp();

#endif