#include <stdio.h>
#include "stm32f4xx_hal.h" // to do: change to L4

GPIO_InitTypeDef gpio;

int main(void){
    printf("Hi! helllo there!!");
    HAL_Init();

    // Defines Pin A15 on the PeripheralSOM as an output
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_15;
    gpio.Mode = GPIO_MODE_OUTPUT_PP; 
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOA, &gpio);

    while (1) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);  // Toggle the LED
        HAL_Delay(500);  // Delay for 500 milliseconds
  }

}