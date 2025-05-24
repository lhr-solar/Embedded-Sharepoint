#ifndef BOOT_SHARED_H
#define BOOT_SHARED_H

#include <stdint.h>

// Meant to be included in both the bootloader and the application
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

typedef struct {
    uint64_t magic_num;
    error_code_t err_code;
} shared_bootmem_t;

#define BOOT_MAGIC_NUM 0xDEADBEEFBABECAFE

#define SHARED_MEM_LEN (1020)
extern uint8_t _estack;
#define SHARED_MEM_START ((uint8_t*)(&_estack) + 4) // Start of shared memory (+4 to avoid stack collision)

// UART configuration
#if defined(STM32F446xx)
#elif defined(STM32F429xx)

#endif

#endif
