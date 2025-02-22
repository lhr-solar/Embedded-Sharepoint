#include "stm32xx_hal.h"

#include <stdio.h>
#include <string.h>

int main(void) {
    HAL_Init();
    
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();

    HAL_ADC_DMA();
}