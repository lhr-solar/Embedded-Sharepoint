#if defined(STM32F4xx)
    #include "stm32f4xx_hal_i2c.h"
    #include "stm32f4xx_hal_fmpi2c.h"
    #include "stm32f4xx_hal_rcc.h"
    #include "stm32f4xx_hal_rcc_ex.h"
    #include "stm32f4xx_hal_gpio.h"

#elif defined(STM32L4xx)
    #include "stm32l4xx_hal_i2c.h"
    #include "stm32l4xx_hal_fmpi2c.h"
    #include "stm32l4xx_hal_rcc.h"
    #include "stm32l4xx_hal_rcc_ex.h"
    #include "stm32l4xx_hal_gpio.h"
    
#endif