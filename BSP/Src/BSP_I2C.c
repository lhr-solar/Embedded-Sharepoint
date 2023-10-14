
#include "BSP_I2C.h"
#include "BSP.h"
#include "FreeRTOS.h"

#define I2C_QUEUE_SIZE   32
#define I2C_ITEM_LENGTH    sizeof( unint 64 )

static Static_Queue_t staticQueueDataStructure;

unit8_t QueueStorageArea[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];

static Queue_Handle_t I2C_Queue;

BSP_Status BSP_I2C_Init() {
    //QUEUE
    I2C_Queue = xQueueCreateStatic(I2C_QUEUE_SIZE, I2C_ITEM_LENGTH, staticQueueDataStructure, &QueueStorageArea);

    GPIO_InitTypeDef gpio_struct;
    I2C_HandleTypeDef i2c_struct;

    if (__HAL_RCC_I2C3_IS_CLK_DISABLED()) {
        __HAL_RCC_I2C3_CLK_ENABLE();
    }
    if (__HAL_RCC_GPIOA_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    if (__HAL_RCC_GPIOC_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }

    // i2c config
    i2c_struct.ClockSpeed = I2C_CLK_SPD;
    i2c_struct.DutyCycle = I2C_DUTYCYCLE_2;
    // 7 bit addr mode
    i2c_struct.OwnAddress1 = 0x50 << 1;
    i2c_struct.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2c_struct.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2c_struct.GeneralCallMode = (1 << 6); 
    i2c_struct.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    // gpio config
    gpio_struct.Pin = GPIO_PIN_8; 
    gpio_struct.Mode = MODE_AF;
    gpio_struct.Pull = GPIO_PULLUP;
    gpio_struct.Speed = GPIO_SPEED_MEDIUM; 
    gpio_struct.Alternate = GPIO_AF4_I2C3;

    // PA8 is SCL
    HAL_GPIO_Init(GPIOA, &gpio_struct);

    // PC9 is SDA
    gpio_struct.Pin = GPIO_PIN_9;
    HAL_GPIO_Init(GPIOC, &gpio_struct);

    //config
    //if (HAL_I2C_Init(hi2c) != HAL_OK) return BSP_ERROR;
}

void I2CWrite() {
    HAL_I2C_MEM_Write_IT()

    for (uint8_t i = 0; i < )

}


/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "BSP_I2C.h"

#define I2C_CLK_SPD 100000 // must be <= 400kHz
#define TIMEOUT_THRESHOLD 10

BSP_Status BSP_I2C_Init() {
    GPIO_InitTypeDef gpio_struct;
    // I2C_HandleTypeDef i2c_struct;
    I2C_HandleTypeDef *hi2c; 

    __HAL_RCC_I2C1_CLK_ENABLE();
    //do I need to config clock speed and duty cycle? 

    if (__HAL_RCC_I2C3_IS_CLK_DISABLED()) {
        __HAL_RCC_I2C3_CLK_ENABLE();
    }
    if (__HAL_RCC_GPIOA_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    if (__HAL_RCC_GPIOC_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }

    hi2c->Instance = I2C1;
    hi2c->Init.OwnAddress1 = 0x50 << 1;
    hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c->Init.GeneralCallMode = (1 << 6); // I2C_CR1_ENGC == 1<<6; this is just enable)
    hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    // Interrupt Activation
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

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

    if (HAL_I2C_Init(hi2c) != HAL_OK) {
        return ;
    }

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