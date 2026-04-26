#ifndef BOOTLOADER_INDICATOR_H_
#define BOOTLOADER_INDICATOR_H_

#include <stdint.h>

typedef enum {
    BOOTLOADER_INDICATOR_NO_APP = 0,
    BOOTLOADER_INDICATOR_APP_PRESENT,
    BOOTLOADER_INDICATOR_CONNECTED,
    BOOTLOADER_INDICATOR_FLASHING,
    BOOTLOADER_INDICATOR_ERROR,
} bootloader_indicator_mode_t;

void bootloader_indicator_init(void);
void bootloader_indicator_set_mode(bootloader_indicator_mode_t mode);
void bootloader_indicator_update(uint32_t tick_ms);

#endif
