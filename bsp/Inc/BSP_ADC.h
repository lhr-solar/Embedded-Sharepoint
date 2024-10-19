#ifndef BSP_ADC_H
#define BSP_ADC_H

#include "stm32xx.h"
#include "queue.h"

/**
 * @param peripheral:   ADC_HandleTypeDef containing unique indentifiers for ADC peripheral
 * @param clock:        Clock passed in to change sampling rate 
 * @param resolution:   ADC sensitivity to changes in voltage
 */
HAL_StatusTypeDef BSP_ADC_Init(ADC_HandleTypeDef *periph);

/**
 * @param periph:   ADC_HandleTypeDef containing unique indentifiers for ADC peripheral
 * @param q         Queue structure to read ADC conversion to
 * @return          Returns HAL_StatusTypeDef of peripheral (can check states, errors, etc.)
 */
HAL_StatusTypeDef BSP_ADC_Read(ADC_HandleTypeDef *periph,  QueueHandle_t *RxQueue);

#endif