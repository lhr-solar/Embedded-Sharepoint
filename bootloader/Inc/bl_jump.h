#ifndef BL_JUMP_H
#define BL_JUMP_H

#include <stdint.h>

void bl_jump_to_app(uint32_t vector_base);
void bl_enter_rom_dfu(void);
void bl_system_reset(void);

#endif /* BL_JUMP_H */
