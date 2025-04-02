#ifndef _ADC_H
#define _ADC_H

#include "stm32xx_hal.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

typedef enum {
    ADC_OK,
    ADC_INIT_FAIL,
    ADC_DEINIT_FAIL,
    ADC_CHANNEL_CONFIG_FAIL,
    ADC_INTERRUPT_BUSY,
    ADC_INTERRUPT_TIMEOUT,
    ADC_INTERRUPT_ERROR,
    ADC_QUEUE_FULL,

} adc_status_t;

#ifdef ADC1
extern ADC_HandleTypeDef* hadc1;
#endif /* ADC1 */
#ifdef ADC2
extern ADC_HandleTypeDef* hadc2;
#endif /* ADC2 */
#ifdef ADC3
extern ADC_HandleTypeDef* hadc3;
#endif /* ADC3 */

adc_status_t adc_init(ADC_InitTypeDef init, ADC_HandleTypeDef* hadc);
/** 
 * init             ADC_InitTypeDef 
 * bitNum           uint8_t         number of bits in ADC
 * Vcc              double
 * rxQueue          QueueHandle_t   pointer to user-provided Queue handle
*/


adc_status_t adc_read(uint32_t channel, uint32_t samplingTime, ADC_HandleTypeDef *h, QueueHandle_t *q); 
/**
 *   channel        uint32_t        channel to read
 *   samplingTime   uint32_t        ADC sampling time
 *   blocking       bool            whether the process is blocking
*/

adc_status_t adc_deinit(ADC_HandleTypeDef *h);

#endif