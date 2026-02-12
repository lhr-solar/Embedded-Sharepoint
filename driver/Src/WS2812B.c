#include "WS2812B.h"

// ordering of data in my PWM array
typedef enum{
    WS2812B_LEDNUMBER = 0,
    WS2812B_GREEN = 1,
    WS2812B_RED = 2,
    WS2812B_BLUE = 3
}pwm_data_ordering_t;


ws2812b_status_t ws2812b_init(ws2812b_handle_t *ledHandler, uint8_t ledData[][NUMBER_PWM_DATA_ELEMENTS], uint16_t *pwmData, TIM_HandleTypeDef *timerHandle, uint32_t channel, uint8_t numberLeds){
    
    if(ledHandler == NULL){
        return WS2812B_NULL_ERROR;
    }
    if(pwmData == NULL){
        return WS2812B_NULL_ERROR;
    }
    ledHandler->pwmBuffer = pwmData;
    if(timerHandle == NULL){
        return WS2812B_NULL_ERROR;
    }

    ledHandler->timerHandle = timerHandle;

    ledHandler->channel = channel;

    if(ledData == NULL){
        return WS2812B_NULL_ERROR;
    }
    ledHandler->ledData = ledData;

    ledHandler->dmaActive = 0;

    ledHandler->mutex = xSemaphoreCreateMutexStatic(&ledHandler->mutexBuf);
    ledHandler->framePendingSem = xSemaphoreCreateBinaryStatic(&ledHandler->framePendingBuf);

    ledHandler->numberLeds = numberLeds;
    return WS2812B_OK;
}

static uint32_t ws2812b_encode_pwm(ws2812b_handle_t *ledHandler){


    /* Encode entire strip */
    uint32_t idx = 0;
    uint32_t color;

    for (uint8_t i = 0; i < ledHandler->numberLeds; i++)
    {
        color =
            (ledHandler->ledData[i][WS2812B_GREEN] << 16) |
            (ledHandler->ledData[i][WS2812B_RED]   << 8)  |
            (ledHandler->ledData[i][WS2812B_BLUE]);

        for (int8_t bit = 23; bit >= 0; bit--)
        {
            ledHandler->pwmBuffer[idx++] =
                (color & (1 << bit)) ? WS2812_HIGH : WS2812_LOW;
        }
    }

    for (uint8_t i = 0; i < WS2812_RESET_SLOTS; i++){
        ledHandler->pwmBuffer[idx++] = 0;
    }
    return idx;

}

ws2812b_status_t ws2812b_set_color(ws2812b_handle_t *ledHandler, uint8_t led_num, ws2812b_color_t color, TickType_t delay_ticks){

    if (ledHandler == NULL){
        return WS2812B_NULL_ERROR;
    }

    if (led_num >= ledHandler->numberLeds){
        return WS2812B_ERROR;
    } 

    // take the mutex so other threads can't modify the thread
    if(xSemaphoreTake(ledHandler->mutex, delay_ticks) != pdTRUE){
        return WS2812B_BUSY;
    }


    ledHandler->ledData[led_num][WS2812B_LEDNUMBER] = led_num;
    ledHandler->ledData[led_num][WS2812B_GREEN] = color.green;
    ledHandler->ledData[led_num][WS2812B_RED] = color.red;
    ledHandler->ledData[led_num][WS2812B_BLUE] = color.blue;

    uint32_t idx = 0;
    idx = ws2812b_encode_pwm(ledHandler);

    if(xSemaphoreGive(ledHandler->mutex) != pdTRUE){
        return WS2812B_ERROR;
    }

    // indiciate that there's a new frame to send for the leds
    if(xSemaphoreGive(ledHandler->framePendingSem) != pdTRUE){
        return WS2812B_ERROR;
    }

    HAL_StatusTypeDef err = HAL_OK;
    taskENTER_CRITICAL();
    if (ledHandler->dmaActive == 0)
    {
        // addressable led dma is not active and we need to start the DMA transmission
        ledHandler->dmaActive = 1;
        err = HAL_TIM_PWM_Start_DMA(ledHandler->timerHandle, ledHandler->channel, (uint32_t *)ledHandler->pwmBuffer, idx);
        if(err == HAL_ERROR){
            ledHandler->dmaActive = 0;
        }
    }
    taskEXIT_CRITICAL();

    if(err != HAL_OK){
        return WS2812B_ERROR;
    }

    return WS2812B_OK;

}

ws2812b_status_t ws2812b_set_all_leds(ws2812b_handle_t *ledHandler, ws2812b_color_t color, TickType_t delay_ticks){
    if (ledHandler == NULL){
        return WS2812B_NULL_ERROR;
    }

    // take the mutex so other threads can't modify the thread
    if(xSemaphoreTake(ledHandler->mutex, delay_ticks) != pdTRUE){
        return WS2812B_BUSY;
    }

    for(uint8_t i = 0; i < ledHandler->numberLeds; i++) {
        ledHandler->ledData[i][WS2812B_LEDNUMBER] = i;
        ledHandler->ledData[i][WS2812B_GREEN] = color.green;
        ledHandler->ledData[i][WS2812B_RED] = color.red;
        ledHandler->ledData[i][WS2812B_BLUE] = color.blue;
    }

    uint32_t idx = 0;
    idx = ws2812b_encode_pwm(ledHandler);

    if(xSemaphoreGive(ledHandler->mutex) != pdTRUE){
        return WS2812B_ERROR;
    }

    // indiciate that there's a new frame to send for the leds
    if(xSemaphoreGive(ledHandler->framePendingSem) != pdTRUE){
        return WS2812B_ERROR;
    }

    HAL_StatusTypeDef err = HAL_OK;
    taskENTER_CRITICAL();
    if (ledHandler->dmaActive == 0)
    {
        // addressable led dma is not active and we need to start the DMA transmission
        ledHandler->dmaActive = 1;
        err = HAL_TIM_PWM_Start_DMA(ledHandler->timerHandle, ledHandler->channel, (uint32_t *)ledHandler->pwmBuffer, idx);
        if(err == HAL_ERROR){
            ledHandler->dmaActive = 0;
        }
    }
    taskEXIT_CRITICAL();

    if(err != HAL_OK){
        return WS2812B_ERROR;
    }

    return WS2812B_OK;
}

ws2812b_status_t ws2812b_set_led_range(ws2812b_handle_t *ledHandler, uint8_t start, uint8_t end, ws2812b_color_t color, TickType_t delay_ticks){
    
    if (ledHandler == NULL){
        return WS2812B_NULL_ERROR;
    }

    if (start >= ledHandler->numberLeds || end >= ledHandler->numberLeds){
        return WS2812B_ERROR;
    } 

    // take the mutex so other threads can't modify the thread
    if(xSemaphoreTake(ledHandler->mutex, delay_ticks) != pdTRUE){
        return WS2812B_BUSY;
    }

    for(uint8_t i = start; i <= end; i++) {
        ledHandler->ledData[i][WS2812B_LEDNUMBER] = i;
        ledHandler->ledData[i][WS2812B_GREEN] = color.green;
        ledHandler->ledData[i][WS2812B_RED] = color.red;
        ledHandler->ledData[i][WS2812B_BLUE] = color.blue;
    }

    uint32_t idx = 0;
    idx = ws2812b_encode_pwm(ledHandler);

    if(xSemaphoreGive(ledHandler->mutex) != pdTRUE){
        return WS2812B_ERROR;
    }

    // indiciate that there's a new frame to send for the leds
    if(xSemaphoreGive(ledHandler->framePendingSem) != pdTRUE){
        return WS2812B_ERROR;
    }

    HAL_StatusTypeDef err = HAL_OK;
    taskENTER_CRITICAL();
    if (ledHandler->dmaActive == 0)
    {
        // addressable led dma is not active and we need to start the DMA transmission
        ledHandler->dmaActive = 1;
        err = HAL_TIM_PWM_Start_DMA(ledHandler->timerHandle, ledHandler->channel, (uint32_t *)ledHandler->pwmBuffer, idx);
        if(err == HAL_ERROR){
            ledHandler->dmaActive = 0;
        }
    }
    taskEXIT_CRITICAL();

    if(err != HAL_OK){
        return WS2812B_ERROR;
    }

    return WS2812B_OK;
}

void ws2812b_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim, ws2812b_handle_t *ledHandler,  BaseType_t *xHigherPriorityTaskWoken){
    if(ledHandler == NULL){
        return;
    }


    if (xSemaphoreTakeFromISR(ledHandler->framePendingSem, xHigherPriorityTaskWoken) == pdTRUE){
        // start the DMA transmission again
         HAL_TIM_PWM_Start_DMA( ledHandler->timerHandle, ledHandler->channel, (uint32_t *)ledHandler->pwmBuffer, (24 * ledHandler->numberLeds) + WS2812_RESET_SLOTS);
    }
    else{
        // no need to keep running the dma timer if no updated frames
        HAL_TIM_PWM_Stop_DMA(ledHandler->timerHandle, ledHandler->channel);
        ledHandler->dmaActive = 0;
    }
}