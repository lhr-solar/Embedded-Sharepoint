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

/**
 * @brief Initialize I2C port for EEPROM interaction
 * 
 */
BSP_Status BSP_I2C_Init();

/**
* @brief   Transmits data onto the I2C bus.
* @param    hi2c:            to structure that has config data
* @param    deviceAdd :      target device address
* @param    pDataBuff :      data buffer
* @param    len :           amount of data   
* @return   HAL Status    
*/
void BSP_I2C_Write(I2C_HandleTypeDef *hi2c, 
              uint32_t deviceAdd, 
              uint8_t* pDataBuff, 
              uint16_t len);

/**
* @param    hi2c:            to structure that has config data
* @param    deviceAdd :      target device address
* @param    memoryAdd :      the register address to write to in the IC's memory.
* @param    memoryAddSize :  the register address size
* @param    pDataBuff :      data buffer
* @param    len :            amount of data   
* @return   HAL Status    
*/
void BSP_I2C_RegisterWrite(I2C_HandleTypeDef *hi2c, 
              uint32_t deviceAdd, 
              uint32_t memoryAdd, //register
              uint32_t memoryAddSize,
              uint8_t* pDataBuff, 
              uint16_t len);

/**
* @brief   Gets the data from a device through the I2C bus.
* @param    hi2c:            to structure that has config data
* @param    deviceAdd :      target device address
* @param    memoryAdd :      the register address to write to in the IC's memory.
* @param    memoryAddSize :  the register address size
 */
void BSP_I2C_Read(I2C_HandleTypeDef* hi2c,
              uint16_t deviceAdd,
              uint16_t memoryAdd,
              uint16_t memoryAddSize);


#endif