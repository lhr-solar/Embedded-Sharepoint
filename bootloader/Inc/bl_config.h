#ifndef BL_CONFIG_H
#define BL_CONFIG_H

#include <stdint.h>

void bl_config_init(void);
uint8_t bl_config_device_id(void);
void bl_config_read_uid(uint8_t uid12[12]);

int bl_config_update_requested(void);
void bl_config_set_update_requested(int on);
int bl_config_dfu_requested(void);
void bl_config_set_dfu_requested(int on);
int bl_config_dfu_gpio_asserted(void);

int bl_config_fault_entry(void);
void bl_config_clear_fault(void);

uint32_t bl_config_active_slot_base(void);
void bl_config_set_active_slot(uint8_t slot);

uint32_t bl_config_committed_length(void);
uint32_t bl_config_committed_crc(void);
void bl_config_set_committed(uint32_t length, uint32_t crc);

int bl_config_app_valid(void);

#endif /* BL_CONFIG_H */
