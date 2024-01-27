/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#ifndef BSP_SPI_H
#define BSP_SPI_H

#include "common.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_i2s.h"
#include "BSP.h"

// Queue pointers - should be passed into init call ?
static QueueHandle_t* _transmitQueuePtr;
static QueueHandle_t* _receiveQueuePtr;

typedef enum {
    SLOW,
    FAST,
    NUM_SPEEDS
} BSP_SPI_Speed;
static const uint32_t _speedLut[NUM_SPEEDS] = {SPI_BAUDRATEPRESCALER_256, SPI_BAUDRATEPRESCALER_128};

/**
 * @brief Initialize the provided SPI port
 * 
 * @param spiHandle The SPI handle to configure and init
 * @return BSP_Status 
 */
HAL_StatusTypeDef BSP_SPI_Init(SPI_HandleTypeDef* spiHandle, QueueHandle_t* txQptr, QueueHandle_t* rxQptr);

/**
 * @brief Set SPI clock speed
 * 
 * @param spiHandle The SPI handle whose clock you're configuring
 * @return BSP_Status 
 */
HAL_StatusTypeDef BSP_SPI_SetClock(SPI_HandleTypeDef* spiHandle, BSP_SPI_Speed speed);

/**
 * @brief Perform a SPI write
 * 
 * @param spiHandle The SPI handle to write from
 * @return BSP_Status 
 */
HAL_StatusTypeDef BSP_SPI_Write(SPI_HandleTypeDef* spiHandle, uint8_t* buffer, uint16_t len);

/**
 * @brief Perform a SPI read
 * 
 * @param spiHandle The SPI handle to read from
 * @return BSP_Status 
 */
HAL_StatusTypeDef BSP_SPI_Read(SPI_HandleTypeDef* spiHandle, uint8_t* data, uint16_t len);

/**
 * @brief Set's SPI chip select
 * 
 * @param spiHandle SPI handle to modify - WIP
 * @return BSP_Status 
 */
void BSP_SPI_SetStateCS(SPI_HandleTypeDef* spiHandle);

#endif