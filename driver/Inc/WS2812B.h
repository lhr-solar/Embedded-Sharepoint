#pragma once

#include "stm32xx_hal.h"
#include "FreeRTOS.h"
#include "semphr.h"

#define MAX_RGB_VAL 255

#define NUMBER_PWM_DATA_ELEMENTS 4

#define WS2812B_DUTY_CYCLE 90

#define WS2812_HIGH (2*WS2812B_DUTY_CYCLE)/3
#define WS2812_LOW WS2812B_DUTY_CYCLE/3

#define WS2812_RESET_SLOTS 50



typedef enum{
    WS2812B_OK,
    WS2812B_NULL_ERROR,
    WS2812B_ERROR,
    WS2812B_BUSY
}ws2812b_status_t;

typedef enum{
    WS2812B_SOLID_GREEN,
    WS2812B_SOLID_RED,
    WS2812B_SOLID_BLUE,
    WS2812B_SOLID_YELLOW,
    WS2812B_SOLID_BURNT_ORANGE,
    WS2812B_SOLID_PURPLE,
    WS2812B_SOLID_OFF,
    WS2812B_SOLID_NUM_COLORS
}ws2812b_solid_color_t;

// Represents a string of leds
typedef struct{
    uint8_t (*ledData)[NUMBER_PWM_DATA_ELEMENTS];        // [LED][LEDNUM, G, R, B]
    uint16_t *pwmBuffer;          // PWM bitstream
    TIM_HandleTypeDef *timerHandle;
    uint32_t channel; 
    uint8_t numberLeds; // the number of leds in the string
    SemaphoreHandle_t mutex; // protects multiple threads from writting to the handle
    StaticSemaphore_t mutexBuf;

    volatile uint8_t dmaActive; // indicates when a dma transmission is active

    SemaphoreHandle_t framePendingSem; // indiciates that there's a new rgb frame to send
    StaticSemaphore_t framePendingBuf;
}ws2812b_handle_t;


ws2812b_status_t ws2812b_init(ws2812b_handle_t *ledHandler, uint8_t ledData[][NUMBER_PWM_DATA_ELEMENTS], uint16_t *pwmData, TIM_HandleTypeDef *timerHandle, uint32_t channel, uint8_t numberLeds);

ws2812b_status_t ws2812b_set_color(ws2812b_handle_t *ledHandler, uint8_t led_num, uint8_t red, uint8_t green, uint8_t blue, TickType_t delay_ticks);

void ws2812b_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim, ws2812b_handle_t *ledHandler,  BaseType_t *xHigherPriorityTaskWoken);

ws2812b_status_t ws2812b_set_solid_color(ws2812b_handle_t *ledHandler, uint8_t led_num, ws2812b_solid_color_t color, TickType_t delay_ticks);