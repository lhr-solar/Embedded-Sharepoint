/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */
#define ADC_RANGE_MILLIVOLTS 3300
#define ADC_PRECISION_BITS 12

typedef enum 
{
    Accelerator_ADC, 
    Brake_ADC,
    NUMBER_OF_CHANNELS
} ADC_t;
/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */
/**
 * @brief   Initializes the ADC module. This is to measure the hall effect sensors
 *          on the Current Monitor Board.
 * @param   None
 * @return  None
 */
void BSP_ADC_Init(void);

/**
 * @brief   Provides the ADC value of the channel at the specified index
 * @param   hardwareDevice pedal enum that represents the specific device
 * @return  Raw ADC value without conversion
 */ 
int16_t BSP_ADC_Get_Value(ADC_t hardwareDevice);

/**
 * @brief   Provides the ADC value in millivolts of the channel at the specified index
 * @param   hardwareDevice pedal enum that represents the specific device
 * @return  ADC value in millivolts
 */ 
int16_t BSP_ADC_Get_Millivoltage(ADC_t hardwareDevice);

/**
 * @brief   Gets converted ADC value in units of mV.
 * @param   None
 * @return  millivoltage value ADC measurement
 */
uint16_t BSP_ADC_High_GetMilliVoltage(void);

/**
 * @brief   Gets converted ADC value in units of mV.
 * @param   None
 * @return  millivoltage value ADC measurement
 */
uint16_t BSP_ADC_Low_GetMilliVoltage(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

