#if defined(STM32F4xx)
    #include "stm32f4xx.h"
    #include "stm32f4xx_hal.h"

    // Supported ports
    #if defined(STM32F401xE)
        #include "stm32f401xe.h"
    #elif defined(STM32F413xx)
        #include "stm32f413xx.h"
    #elif defined(STM32F446xx)
        #include "stm32f446xx.h"
    #endif

#elif defined(STM32L4xx)
    #include "stm32l4xx.h"
    #include "stm32l4xx_hal.h"
#else
    #error "No valid STM32 series defined. Please define either STM32F4xx or STM32L4xx."
#endif