/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "BSP_I2C.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_rcc_ex.h"
#include "stm32f4xx_hal_gpio.h"

#define I2C_CLK_SPD 100000 // must be <= 400kHz
#define TIMEOUT_THRESHOLD 10

void BSP_I2C_Init() {
    GPIO_InitTypeDef gpio_struct;
    I2C_InitTypeDef i2c_struct;

    // Instead of RCC_AHB... HAL provides macros for these
    __HAL_RCC_I2C3_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // i2c config
    i2c_struct.ClockSpeed = I2C_CLK_SPD;
    i2c_struct.DutyCycle = I2C_DUTYCYCLE_2;
    // 7 bit addr mode
    i2c_struct.OwnAddress1 = 0x50 << 1; // pulled from BPS's I2C.c file
    i2c_struct.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2c_struct.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2c_struct.GeneralCallMode = (1 << 6); // I2C_CR1_ENGC == 1<<6
    i2c_struct.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    // gpio config
    gpio_struct.Pin = GPIO_PIN_8; // will also use pin 9, do later
    gpio_struct.Mode = MODE_AF;
    gpio_struct.Pull = GPIO_PULLUP;
    gpio_struct.Speed = GPIO_SPEED_MEDIUM; // ~ 50MHz. Supposedly.
    gpio_struct.Alternate = GPIO_AF4_I2C3;
    // PA8 is SCL
    HAL_GPIO_Init(GPIOA, &gpio_struct);
    // PC9 is SDA
    gpio_struct.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOC, &gpio_struct);

    // Now we can init I2C on these pins
    HAL_I2C_Init(&i2c_struct);
}

/**
 * @brief   Transmits data onto the I2C bus.
 * @param   deviceAddr : the device address to write to
 * @param   regAddr : the register address to write to in the IC's memory.
 * @param   txData : the data array to be sent onto the bus.
 * @param   txLen : the length of the data array.
 * @return  error status, 0 if fail, 1 if success
 */
I2C_STATUS BSP_I2C_Write(uint8_t deviceAddr, uint16_t regAddr, uint8_t *txData, uint32_t txLen) {
    volatile uint8_t timeout_ctr = 0;
    // spin on busy
    while (HAL_I2C_GetState(I2C3) == I2C_FLAG_BUSY) {
        timeout_ctr++;
        if (timeout_ctr > TIMEOUT_THRESHOLD) {
            return !ERROR;
        }
    }
    // DMA? Interrupt? Which kind
    return (I2C_STATUS)HAL_I2C_Mem_Write(I2C3, deviceAddr, regAddr, sizeof(uint16_t), txData, txLen, 1);
}

/**
 * @brief   Gets the data from a device through the I2C bus.
 * @param   deviceAddr : the device address to read from
 * @param   regAddr : the register address to read from the IC's memory.
 * @param   rxData : the data array to store the data that is received.
 * @param   rxLen : the length of the data array.
 * @return  error status, 0 if fail, 1 if success
 */
I2C_STATUS BSP_I2C_Read(uint8_t deviceAddr, uint16_t regAddr, uint8_t *rxData, uint32_t rxLen) {
    volatile uint8_t timeout_ctr = 0;
    // spin on busy
    while (HAL_I2C_GetState(I2C3) == I2C_FLAG_BUSY) {
        timeout_ctr++;
        if (timeout_ctr > TIMEOUT_THRESHOLD) {
            return !ERROR;
        }
    }
    // DMA? Interrupt? Which kind
    return (I2C_STATUS)HAL_I2C_Mem_Read(I2C3, deviceAddr, regAddr, sizeof(uint16_t), rxData, rxLen, 1);
}