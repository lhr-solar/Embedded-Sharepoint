#ifndef BOOT_H
#define BOOT_H

#include <stdint.h>

extern uint8_t* _estack;
extern uint8_t* _app_start;
extern uint32_t _app_len;
extern uint8_t* _boot_start;
extern uint32_t _boot_len;

void Reset_Handler();

void boot();

void startapp(uint8_t *addr, uint8_t *stack);

#endif