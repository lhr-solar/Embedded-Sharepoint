#include <stdint.h>

// This is load bearing
void _init(){}

#if !defined(FIRMWARE_ROLE_BOOTLOADER) && !defined(FIRMWARE_USES_BOOTLOADER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
void HardFault_Handler(){
    // Configurable Fault Status Register
    // Consists of MMSR, BFSR and UFSR
    volatile uint32_t _CFSR = (*((volatile unsigned long *)(0xE000ED28))) ;

    // Hard Fault Status Register
    volatile uint32_t _HFSR = (*((volatile unsigned long *)(0xE000ED2C))) ;

    // Debug Fault Status Register
    volatile uint32_t _DFSR = (*((volatile unsigned long *)(0xE000ED30))) ;

    // Auxiliary Fault Status Register
    volatile uint32_t _AFSR = (*((volatile unsigned long *)(0xE000ED3C))) ;

    // Read the Fault Address Registers. These may not contain valid values.
    // Check BFARVALID/MMARVALID to see if they are valid values
    // MemManage Fault Address Register
    volatile uint32_t _MMAR = (*((volatile unsigned long *)(0xE000ED34))) ;
    // Bus Fault Address Register
    volatile uint32_t _BFAR = (*((volatile unsigned long *)(0xE000ED38))) ;

    __asm("BKPT #0\n") ; // Break into the debugger
}
#pragma GCC diagnostic pop
#endif
