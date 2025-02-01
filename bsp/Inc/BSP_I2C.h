/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
// BSP I2C header file

#ifndef BSP_I2C_H
#define BSP_I2C_H
#if defined(I2C1) || defined(I2C2) ||defined(I2C3)
#include "stm32xx_hal.h"

/**
 * @brief Initialize I2C port for EEPROM interaction
 *
 * @param hi2c Config for the i2c peripheral setup
 */
HAL_StatusTypeDef i2c_init(I2C_HandleTypeDef *hi2c);

/**
* @brief    Transmits data onto the I2C bus.
* @param    hi2c           to structure that has config data
* @param    deviceAddr     target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data
* @return   HAL_StatusTypeDef
*/
HAL_StatusTypeDef i2c_send(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAddr,
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
HAL_StatusTypeDef i2c_recv(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAdd,
              uint8_t* pDataBuff,
              uint16_t len);

#endif
#endif