#if defined(STM32F4xx)
    #include "stm32f4xx_hal.h"
#elif defined(STM32L4xx)
    #include "stm32l4xx_hal.h"
#else
    #error "No valid STM32 series defined. Please define either STM32F4xx or STM32L4xx."
#endif