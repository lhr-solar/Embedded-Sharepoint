

# File stm32xx\_hal.h

[**File List**](files.md) **>** [**common**](dir_bdd9a5d540de89e9fe90efdfc6973a4f.md) **>** [**Inc**](dir_4152f073bf32bf79124b676c9fe2adc1.md) **>** [**stm32xx\_hal.h**](stm32xx__hal_8h.md)

[Go to the documentation of this file](stm32xx__hal_8h.md)


```C++
#ifndef STM32xx_HAL_H
#define STM32xx_HAL_H

// STM32
#if defined(STM32F4xx)
    #include "stm32f4xx.h"
    #include "stm32f4xx_hal.h"
    #include "stm32f4xx_hal_conf.h"
#elif defined(STM32L4xx)
    #include "stm32l4xx.h"
    #include "stm32l4xx_hal.h"
    #include "stm32l4xx_hal_conf.h"
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
```


