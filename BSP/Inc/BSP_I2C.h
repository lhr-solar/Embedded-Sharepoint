/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
// BSP I2C header file

#ifndef BSP_I2C_H
#define BSP_I2C_H

#include <stdint.h>
#include "stm32xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_fmpi2c.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_rcc_ex.h"
#include "stm32f4xx_hal_gpio.h"
#include "stdbool.h"

/**
 * @brief Initialize I2C port for EEPROM interaction
 * 
 */
HAL_StatusTypeDef BSP_I2C_Init(I2C_HandleTypeDef *hi2c);

/**
* @brief    Transmits data onto the I2C bus.
* @param    hi2c           to structure that has config data
* @param    deviceAdd      target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data   
* @return   HAL_StatusTypeDef   
*/
HAL_StatusTypeDef BSP_I2C_TX(I2C_HandleTypeDef *hi2c, 
              uint8_t deviceAdd, 
              uint8_t* pDataBuff, 
              uint16_t len);

/**
* @brief    Receives data from the I2C bus.
* @param    hi2c           to structure that has config data
* @param    deviceAdd      target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data   
* @return   HAL_StatusTypeDef   
*/
HAL_StatusTypeDef BSP_I2C_RX(I2C_HandleTypeDef *hi2c, 
              uint8_t deviceAdd, 
              uint8_t* pDataBuff, 
              uint16_t len);


#endif
