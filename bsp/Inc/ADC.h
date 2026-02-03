/**
 * @file ADC.h
 * @brief Provides functions for initializing, reading, and managing ADC peripherals.
 *
 * @details
 * This file contains the API for working with ADC (Analog-to-Digital Converter) peripherals
 * in an embedded system, including initialization, reading channels, and queue-based
 * data handling (e.g., with FreeRTOS). The functions support flexible configuration of
 * ADC instances, sampling times, and data storage mechanisms.
 *
 * **Initialization**:
 *
 *    - Call `adc_init()` to configure and initialize a specific ADC instance.
 *
 *    - Provide the ADC configuration structure and handle for the target peripheral.
 *
 *    - Returns a status code indicating success or failure.
 *
 * **Reading ADC Values**:
 *
 *    - Call `adc_read()` to sample a channel on the ADC.
 *
 *    - Provide the channel number, sampling time, ADC handle, and storage/queue reference.
 *
 *    - Returns a status code indicating success or failure.
 *
 * **Data Retrieval**:
 *
 *    - If using queues, use standard FreeRTOS queue functions (e.g., `xQueueReceive()`)
 *      to retrieve ADC readings.
 *
 *    - Supports both blocking and non-blocking behavior depending on the chosen timeout.
 *
 * @note This driver is designed to work with multiple ADC instances and supports
 *       integration with an RTOS for asynchronous data handling.
 */

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
// #ifdef ADC2
// extern ADC_HandleTypeDef* hadc2;
// #endif /* ADC2 */
// #ifdef ADC3
// extern ADC_HandleTypeDef* hadc3;
// #endif /* ADC3 */
// #ifdef ADC4
// extern ADC_HandleTypeDef* hadc4;
// #endif /* ADC4 */
// #ifdef ADC5
// extern ADC_HandleTypeDef* hadc5;
// #endif /* ADC5 */

/**
 * @brief Initializes the ADC peripheral with the specified configuration.
 *
 * This function sets up the ADC hardware according to the parameters in the
 * initialization structure and prepares it for subsequent conversions.
 *
 * @param init     ADC initialization structure containing configuration parameters.
 * @param hadc     Pointer to the ADC handle structure.
 *
 * @return adc_status_t  Returns ADC_OK on success or an appropriate error code.
 */
adc_status_t adc_init(ADC_InitTypeDef *init, ADC_HandleTypeDef* hadc);

/**
 * @brief Reads a value from the specified ADC channel.
 *
 * This function triggers a conversion on the given channel and stores the
 * converted result in the provided queue. It supports both blocking and
 * non-blocking operation depending on configuration.
 *
 * @param channel        ADC channel to read from.
 * @param samplingTime   ADC sampling time (in ADC clock cycles).
 * @param h              Pointer to the ADC handle structure.
 * @param q              Pointer to the user-provided queue handle for result storage.
 *
 * @return adc_status_t  Returns ADC_OK on success or an appropriate error code.
 */
adc_status_t adc_read(uint32_t channel, uint32_t samplingTime, ADC_HandleTypeDef *h, QueueHandle_t q);

/**
 * @brief Deinitializes the ADC peripheral.
 *
 * This function releases resources used by the ADC and resets it to its default
 * uninitialized state.
 *
 * @param h  Pointer to the ADC handle structure.
 *
 * @return adc_status_t  Returns ADC_OK on success or an appropriate error code.
 */
adc_status_t adc_deinit(ADC_HandleTypeDef *h);

#endif
