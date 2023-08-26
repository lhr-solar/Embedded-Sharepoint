/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
// BSP I2C header file

#ifndef BSP_I2C_H
#define BSP_I2C_H

#include <stdint.h>

#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_rcc_ex.h"
#include "stm32f4xx_hal_gpio.h"

typedef enum {
    OK = 0x0,
    ERROR = 0x1,
    BUSY = 0x2,
    TIMEOUT = 0x3
} I2C_STATUS;

/**
 * @brief Initialize I2C port for EEPROM interaction
 * 
 */
void BSP_I2C_Init();
/**
 * @brief   Transmits data onto the I2C bus.
 * @param   deviceAddr : the device address to write to
 * @param   regAddr : the register address to write to in the IC's memory.
 * @param   txData : the data array to be sent onto the bus.
 * @param   txLen : the length of the data array.
 * @return  error status
 */
I2C_STATUS BSP_I2C_Write(uint8_t deviceAddr, uint16_t regAddr, uint8_t *txData, uint32_t txLen);

/**
 * @brief   Gets the data from a device through the I2C bus.
 * @param   deviceAddr : the device address to read from
 * @param   regAddr : the register address to read from the IC's memory.
 * @param   rxData : the data array to store the data that is received.
 * @param   rxLen : the length of the data array.
 * @return  error status
 */
I2C_STATUS BSP_I2C_Read(uint8_t deviceAddr, uint16_t regAddr, uint8_t *rxData, uint32_t rxLen);


#endif