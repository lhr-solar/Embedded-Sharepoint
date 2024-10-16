#ifndef BOOT_H
#define BOOT_H

#include <stdint.h>

// Main segment offsets + sizes (defined in linker script)
extern uint32_t* __ram_start__;
extern uint32_t* __ram_size__;
extern uint32_t* __bootflash_start__;
extern uint32_t* __bootflash_size__;
extern uint32_t* __appflash_start__;
extern uint32_t* __appflash_size__;

// Data segment offsets (defined in linker script)
extern uint32_t* _sdata;
extern uint32_t* _edata;
extern uint32_t* _sidata;

// BSS segment offsets (defined in linker script)
extern uint32_t* _sbss;
extern uint32_t* _ebss;

void Reset_Handler(void) __attribute__((section(".bootloader")));
void deinit(void) __attribute__((section(".bootloader")));
void boot(void) __attribute__((section(".bootloader")));

#endif