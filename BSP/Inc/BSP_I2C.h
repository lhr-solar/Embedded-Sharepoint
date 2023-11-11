/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
// BSP I2C header file

#ifndef BSP_I2C_H
#define BSP_I2C_H

#include <stdint.h>
#include "BSP.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_rcc_ex.h"
#include "stm32f4xx_hal_gpio.h"
#include "stdbool.h"

#define I2C_QUEUE_SIZE   128

typedef struct {
    uint8_t deviceAddr;
    uint8_t memoryAddr;
    uint8_t memoryAddrSize;
    uint8_t pDataBuffer;
    uint16_t length;
    uint8_t temp;
} MetaInfo_t;

/**
 * @brief Initialize I2C port for EEPROM interaction
 * 
 */
HAL_StatusTypeDef BSP_I2C_Init();

/**
* @brief    Transmits data onto the I2C bus.
* @param    hi2c:            to structure that has config data
* @param    deviceAdd :      target device address
* @param    pDataBuff :      data buffer
* @param    len :            amount of data   
* @return   HAL_StatusTypeDef   
*/
HAL_StatusTypeDef BSP_I2C_Write(I2C_HandleTypeDef *hi2c, 
              uint8_t deviceAdd, 
              uint8_t* pDataBuff, 
              uint16_t len);

/**
* @param    hi2c:            to structure that has config data
* @param    deviceAdd :      target device address
* @param    memoryAdd :      the register address to write to in the IC's memory.
* @param    memoryAddSize :  the register address size
* @param    pDataBuff :      data buffer
* @param    len :            amount of data   
* @return   HAL_StatusTypeDef   
*/
HAL_StatusTypeDef BSP_I2C_RegisterWrite(I2C_HandleTypeDef *hi2c, 
              uint8_t deviceAdd, 
              uint8_t memoryAdd, //register
              uint8_t memoryAddSize,
              uint8_t* pDataBuff, 
              uint16_t len);

/**
* @brief    Gets the data from a device through the I2C bus.
* @param    hi2c:            to structure that has config data
* @param    deviceAdd :      target device address
* @param    memoryAdd :      the register address to write to in the IC's memory.
* @param    memoryAddSize :  the register address size
* @return   HAL_StatusTypeDef 
 */
HAL_StatusTypeDef BSP_I2C_Read(I2C_HandleTypeDef* hi2c,
              uint8_t deviceAdd,
              uint8_t memoryAdd,
              uint8_t memoryAddSize);


#endif