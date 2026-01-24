#pragma once

#define SEND_HIGH 66
#define SEND_LOW 33
#define RST_NUM_PERIODS 40

#include "stm32g4xx_hal.h"
#include "stm32xx_hal.h"
#include "FreeRTOS.h"

typedef enum {
    STATUS_SUCCESS,
} status_t;

status_t WS2812B_Init(GPIO_TypeDef* port, uint16_t pin);

status_t WS2812B_Send(uint8_t colors[], uint8_t num_leds);

status_t WS2812B_Reset(GPIO_TypeDef* port, uint16_t pin);