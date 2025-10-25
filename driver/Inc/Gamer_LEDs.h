#pragma once

#include "stm32l4xx_hal.h"
#include <stdint.h>


 // Choose board and configuration
 //#define STM32L432_PA8  // STM32L432KCU - MCU for the TPS Test board
                        // STM32L431CBT - MCY for the lighting PSOM


 //#define WS2812B  // These lights are on the TPS27SA08 test board but not the lighting board
                    // If not defined, WS2814 is assumed by default

// Will probably need to add more function prototyes
void test_set_leds_high();
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void Error_Handler(void);
void Gamer_LEDs_Init(void);