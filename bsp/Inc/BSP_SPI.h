#ifndef BSP_SPI_H
#define BSP_SPI_H

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_rcc.h"
#include "FreeRTOS.h"
#include "semphr.h"

// SPI prescalers
typedef enum {
    SLOW,
    FAST,
    NUM_SPEEDS
} BSP_SPI_Speed;
static const uint32_t _speedLut[NUM_SPEEDS] = {SPI_BAUDRATEPRESCALER_256, SPI_BAUDRATEPRESCALER_128};

/**
 * @brief Initialize a given SPI peripheral. Struct config must be performed
 *        outside of this function.
 * 
 * @param config 
 * @return HAL_StatusTypeDef 
 */
HAL_StatusTypeDef BSP_SPI_Init(SPI_HandleTypeDef* spiHandle, QueueHandle_t* txPtr, QueueHandle_t* rxPtr);
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
HAL_StatusTypeDef BSP_SPI_Read(SPI_HandleTypeDef* spiHandle, uint16_t len);

/**
 * @brief Set's SPI chip select
 * 
 * @param spiHandle SPI handle to modify - WIP
 * @return BSP_Status 
 */
void BSP_SPI_SetStateCS(SPI_HandleTypeDef* spiHandle, uint8_t val);

#endif