#include <stdio.h>
#include "stm32l4xx_hal.h"

GPIO_InitTypeDef gpio;
GPIO_InitTypeDef gpio2;


int main(void){
    HAL_Init();
   
    // GPIO clock enables
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();


    // Defines Pin B3, B4, and B5 as an output
    gpio.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    gpio.Mode = GPIO_MODE_OUTPUT_PP; 
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &gpio);

    // Defines Pin A15 as an output
    gpio2.Pin = GPIO_PIN_15;
    gpio2.Mode = GPIO_MODE_OUTPUT_PP; 
    gpio2.Pull = GPIO_NOPULL;
    gpio2.Speed = GPIO_SPEED_FREQ_LOW;  
    HAL_GPIO_Init(GPIOA, &gpio2);

    while (1) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
        HAL_Delay(500);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
        HAL_Delay(500);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
        HAL_Delay(500);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15); 
        HAL_Delay(500);
  }

}