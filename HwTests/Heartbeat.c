#include <stdio.h>
#include "stm32l4xx_hal.h"

GPIO_InitTypeDef gpio;

/**
 * PeripheralSOM GPIO pins (Spring 2024)
 * PB3, PB4, PB5
 * PA15 
*/

int main(void){
    HAL_Init();
   
    // GPIO clock enables
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Defines Pin B3 on the PeripheralSOM as an output
    gpio.Pin = GPIO_PIN_3;
    gpio.Mode = GPIO_MODE_OUTPUT_PP; 
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    

    HAL_GPIO_Init(GPIOB, &gpio);

    while (1) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);  // Toggle the LED
        HAL_Delay(500);  // Delay for 500 milliseconds
  }

}