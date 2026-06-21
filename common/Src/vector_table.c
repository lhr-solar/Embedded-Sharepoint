#include "stm32xx_hal.h"

void Sharepoint_RelocateVectorTable(void) {
    extern const unsigned long g_pfnVectors[];
    Sharepoint_RelocateVectorTableTo((uint32_t)&g_pfnVectors[0]);
}

void Sharepoint_RelocateVectorTableTo(uint32_t vector_base_address) {
    SCB->VTOR = vector_base_address;
    __DSB();
    __ISB();
}
