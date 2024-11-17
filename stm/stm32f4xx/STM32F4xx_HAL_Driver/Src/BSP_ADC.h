#ifndef BSP_ADC_H
#define BSP_ADC_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h" 
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_adc.h"
#include "stm32f4xx_hal_adc_ex.h"
#include "sys/queue.h"

/**
 * @param peripheral:   ADC_HandleTypeDef containing unique indentifiers for ADC peripheral
 * @param clock:        Clock passed in to change sampling rate 
 * @param resolution:   ADC sensitivity to changes in voltage
 */
HAL_StatusTypeDef BSP_ADC_Init(ADC_HandleTypeDef *peripheral, uint32_t *clock, uint32_t *resolution);

/**
 * @param periph:   ADC_HandleTypeDef containing unique indentifiers for ADC peripheral
 * @param q         Queue structure to read ADC conversion to
 * @return          Returns HAL_StatusTypeDef of peripheral (can check states, errors, etc.)
 */
HAL_StatusTypeDef BSP_ADC_Read(ADC_HandleTypeDef *peripheral, struct tailhead *q);

#endif