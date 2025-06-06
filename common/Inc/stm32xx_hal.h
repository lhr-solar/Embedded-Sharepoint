#ifndef STM32xx_HAL_H
#define STM32xx_HAL_H

// STM32
#if defined(STM32F4xx)
    #include "stm32f4xx.h"
    #include "stm32f4xx_hal.h"
#elif defined(STM32L4xx)
    #include "stm32l4xx.h"
    #include "stm32l4xx_hal.h"
#else
    #error "No valid STM32 series defined. Please define either STM32F4xx or STM32L4xx."
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
void SystemClock_Config(void);

#endif /* STM32xs_HAL_H */
