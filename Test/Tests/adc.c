#include <stm32xx_hal.h>

int main() {
    HAL_Init(); 

    GPIO_InitTypeDef adc_in = {
        .Pin = GPIO_PIN_0,
        .Mode = GPIO_MODE_INPUT

    };

    HAL_GPIO_Init(GPIOA, &adc_in);
    
    ADC_HandleTypeDef adc;
    HAL_ADC_Init(&adc);

    // Test A --- Poll for ADC conversion
    //uint32_t adc_val;
    HAL_ADC_Start(&adc);

    HAL_ADC_PollForConversion(&adc, 100);
    //adc_val = HAL_ADC_GetValue(&adc);

    HAL_ADC_Stop(&adc);
    HAL_Delay(500);


}
