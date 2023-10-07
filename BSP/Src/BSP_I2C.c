/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "BSP_I2C.h"

#define I2C_CLK_SPD 100000 // must be <= 400kHz
#define TIMEOUT_THRESHOLD 10

BSP_Status BSP_I2C_Init() {
    GPIO_InitTypeDef gpio_struct;
    I2C_InitTypeDef i2c_struct;
}

/**
 * @brief   Transmits data onto the I2C bus.
 * @param   deviceAddr : the device address to write to
 * @param   regAddr : the register address to write to in the IC's memory.
 * @param   txData : the data array to be sent onto the bus.
 * @param   txLen : the length of the data array.
 * @return  error status, 0 if fail, 1 if success
 */
BSP_Status BSP_I2C_Write(uint8_t deviceAddr, uint16_t regAddr, uint8_t *txData, uint32_t txLen) {
}

/**
 * @brief   Gets the data from a device through the I2C bus.
 * @param   deviceAddr : the device address to read from
 * @param   regAddr : the register address to read from the IC's memory.
 * @param   rxData : the data array to store the data that is received.
 * @param   rxLen : the length of the data array.
 * @return  error status, 0 if fail, 1 if success
 */
BSP_Status BSP_I2C_Read(uint8_t deviceAddr, uint16_t regAddr, uint8_t *rxData, uint32_t rxLen) {

}