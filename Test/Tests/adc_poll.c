#include "stm32xx_hal.h"


int main() {
    HAL_Init(); 

    GPIO_InitTypeDef adc_in = {
        .Pin = GPIO_PIN_0,
        .Mode = GPIO_MODE_ANALOG,
        .Pull = GPIO_NOPULL

    };

    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable clock for GPIOA
    
    HAL_GPIO_Init(GPIOA, &adc_in);
    
    #if defined(STM32F4xx)  
        __HAL_RCC_ADC1_CLK_ENABLE(); 
    #elif defined(STM32L4xx)
        __HAL_RCC_ADC_CLK_ENABLE(); // enable ADC clock4
    #endif

    ADC_HandleTypeDef adc;
    HAL_ADC_Init(&adc);
    

    // Test A --- Poll for ADC conversion
    volatile uint32_t adc_val; // turn off compiler optimization to view variable
    HAL_ADC_Start(&adc);

    while(1) {
        HAL_ADC_PollForConversion(&adc, 100);
        adc_val = HAL_ADC_GetValue(&adc);
        //printf(adc_val);
        adc_val += 0; // c needs some sort of change on adc [filler line]

        HAL_ADC_Stop(&adc);
        HAL_Delay(500);
    }

    return 0;
}
