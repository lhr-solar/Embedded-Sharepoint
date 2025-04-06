/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
// BSP I2C header file

#ifndef BSP_I2C_H
#define BSP_I2C_H

#define I2C_HAL_OFFSET  0x10
typedef enum {
    I2C_OK          = 0x00U,
    I2C_SENT        = 0x01U,
    I2C_QFAIL       = 0x02U,    // Queue failure
    I2C_QFULL       = 0x03U,    // Queue full
    I2C_IFAIL       = 0x04U,    // Init failure
    I2C_BADSIZE     = 0x05U,    // Incorrect payload size
    I2C_BADINST     = 0x06U,    // Invalid I2C instance

    /* Conversions for defalt HAL transmit */
    I2C_HAL_OK      = I2C_HAL_OFFSET + 0x00U,
    I2C_HAL_ERROR   = I2C_HAL_OFFSET + 0x01U,
    I2C_HAL_BUSY    = I2C_HAL_OFFSET + 0x02U,
    I2C_HAL_TIMEOUT = I2C_HAL_OFFSET + 0x03U
} I2C_StatusTypeDef;


#if defined(I2C1) || defined(I2C2) || defined(I2C3)
#include "stm32xx_hal.h"

/**
 * @brief Initialize I2C port for EEPROM interaction
 *
 * @param hi2c Config for the i2c peripheral setup
 * @return I2C_StatusTypeDef
 */
I2C_StatusTypeDef i2c_init(I2C_HandleTypeDef *hi2c);

/**
* @brief    Transmits data onto the I2C bus.
* @param    hi2c           to structure that has config data
* @param    deviceAddr     target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data
* @return   I2C_StatusTypeDef
*/
I2C_StatusTypeDef i2c_send(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAddr,
              uint8_t* pDataBuff,
              uint16_t len);

/**
* @brief    Receives data from the I2C bus.
* @param    hi2c           to structure that has config data
* @param    deviceAddr      target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data
* @return   I2C_StatusTypeDef
*/
I2C_StatusTypeDef i2c_recv(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAddr,
              uint8_t* pDataBuff,
              uint16_t len);

#endif
#endif