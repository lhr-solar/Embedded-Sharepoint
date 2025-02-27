#ifndef _ADC_H
#define _ADC_H

#include "stm32xx_hal.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

ADC_HandleTypeDef* hadc;

typedef enum {
    ADC_OK,
    ADC_INIT_FAIL,
    ADC_CHANNEL_CONFIG_FAIL,
    ADC_QUEUE_FULL,

} adc_status_t;


adc_status_t ADC_Init(ADC_InitTypeDef init, QueueHandle_t *rxQueue);
/** 
 * init             ADC_InitTypeDef 
 * bitNum           uint8_t         number of bits in ADC
 * Vcc              double
 * rxQueue          QueueHandle_t   pointer to user-provided Queue handle
*/


adc_status_t ADC_OneShotRead(uint32_t channel, uint32_t samplingTime, bool blocking); 
/**
 *   channel        uint32_t        channel to read
 *   samplingTime   uint32_t        ADC sampling time
 *   blocking       bool            whether the process is blocking
*/

#endif