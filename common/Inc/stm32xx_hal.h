#ifndef STM32xx_HAL_H
#define STM32xx_HAL_H

// STM32
#if defined(STM32G4xx)
    #include "stm32g4xx.h"
    #include "stm32g4xx_hal.h"
    #include "stm32g4xx_hal_conf.h"
#else
    #error "No valid STM32 series defined. Define STM32G4xx."
#endif

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

// Standard Library
#include <stdint.h>
#include <stdbool.h>

// Init function prototypes
void Error_Handler(void);
void HAL_MspInit(void);
__weak void SystemClock_Config(void);

// Vector table relocation (used by OpenBLT/bootloader app handoff)
void Sharepoint_RelocateVectorTable(void);
void Sharepoint_RelocateVectorTableTo(uint32_t vector_base_address);

#endif /* STM32xs_HAL_H */
