#pragma once

#include "stm32l4xx_hal.h"
#include <stdint.h>


 // Choose board and configuration
 #define STM32L432_PA8  // STM32L432KCU
 // stm32l431 is lighting psom

 //#define WS2812B // These lights are on the TPS27SA08 test board but not the lighting board
                // If not defined, WS2814 is assumed default

void test_set_leds_high();
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void Error_Handler(void);
void Gamer_LEDs_Init(void);