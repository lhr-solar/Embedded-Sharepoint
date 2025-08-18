/**
 * @brief File that shows how to initialize and read from ADCs using static queues and FreeRTOS tasks.
 * 
 * ---------------------
 * USAGE INSTRUCTIONS:
 * ---------------------
 * 1. **ADC Initialization**:
 *    - Call `adc_init(adc_init_struct, hadcX)` to initialize a specific ADC instance.
 *    - Parameters:
 *        - `adc_init_struct` : A fully configured `ADC_InitTypeDef` structure.
 *        - `hadcX`           : The ADC handle (`hadc1`, `hadc2`, or `hadc3`).
 *    - Returns `ADC_OK` on success, or an error status on failure.
 *
 * 2. **ADC Reading**:
 *    - Call `adc_read(channel, sample_time, hadcX, &queue)` to perform a read.
 *    - Parameters:
 *        - `channel`     : ADC input channel (e.g., `ADC_CHANNEL_0`, `ADC_CHANNEL_3`).
 *        - `sample_time` : Sampling time macro.
 *        - `hadcX`       : The ADC handle corresponding to the ADC you want to use.
 *        - `&queue`      : Pointer to a FreeRTOS queue (`QueueHandle_t`) to store the reading.
 *    - Returns `ADC_OK` on success, or an error status on failure.
 *
 * 3. **Queue Retrieval**:
 *    - Use `xQueueReceive(queue, &reading, timeout)` to retrieve the ADC reading.
 *    - Set `timeout` to 0 for non-blocking behavior.
 */

#pragma once

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
