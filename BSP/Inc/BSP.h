#ifndef BSP_CONF_H
#define BSP_CONF_H

/**
 * @file BSP.h
 * @brief This file includes necessities for the BSP layer of Embedded Sharepoint,
 * including global peripheral handles, pin map definitions, etc. This standard
 * config is spread across four files: BSP.h (this file), bsp_config.h (in /Util/),
 * BSP.c (in BSP/Src/), and bsp_config.c (in BSP/Src/).
 * 
 * @copyright Copyright Longhorn Solar Racing (c) 2023
 * 
 */

#include <stdbool.h>
#include "stm32f4xx_hal.h"

// Global handles to the possible peripherals
extern UART_HandleTypeDef uart4_handle;
extern UART_HandleTypeDef uart5_handle;
extern TIM_HandleTypeDef tim1_handle, tim2_handle, tim3_handle, tim9_handle;
extern I2C_HandleTypeDef i2c1_handle;
extern I2C_HandleTypeDef i2c3_handle;
extern SPI_HandleTypeDef spi2_handle;
extern SPI_HandleTypeDef spi3_handle;
extern CAN_HandleTypeDef carcan_handle;
extern CAN_HandleTypeDef localcan_handle;

// Possible pins to refer to.
typedef enum {
    BSP_GPIO_PA0, BSP_GPIO_PA1, BSP_GPIO_PA2, BSP_GPIO_PA3, BSP_GPIO_PA4,
    BSP_GPIO_PA5, BSP_GPIO_PA6, BSP_GPIO_PA7, BSP_GPIO_PA8, BSP_GPIO_PA9,
    BSP_GPIO_PA10, BSP_GPIO_PA11, BSP_GPIO_PA12, BSP_GPIO_PA13, BSP_GPIO_PA14,
    BSP_GPIO_PA15,

    BSP_GPIO_PB0, BSP_GPIO_PB1, BSP_GPIO_PB2, BSP_GPIO_PB3, BSP_GPIO_PB4,
    BSP_GPIO_PB5, BSP_GPIO_PB6, BSP_GPIO_PB7, BSP_GPIO_PB8, BSP_GPIO_PB9,
    BSP_GPIO_PB10, BSP_GPIO_PB11, BSP_GPIO_PB12, BSP_GPIO_PB13, BSP_GPIO_PB14, BSP_GPIO_PB15,

    BSP_GPIO_PC0, BSP_GPIO_PC1, BSP_GPIO_PC2, BSP_GPIO_PC3, BSP_GPIO_PC4,
    BSP_GPIO_PC5, BSP_GPIO_PC6, BSP_GPIO_PC7, BSP_GPIO_PC8, BSP_GPIO_PC9, 
    BSP_GPIO_PC10, BSP_GPIO_PC11, BSP_GPIO_PC12, BSP_GPIO_PC13, // PC14/15 reserved for xtal

    BSP_GPIO_PD2,

    NUM_BSP_PINS
} BSP_PINS;

// Macro to get HAL GPIO port out from enum value
#define GET_GPIO(x) ( (( (x) >= BSP_GPIO_PA0 && (x) <= BSP_GPIO_PA15 )) ? GPIOA : \
                    (  (( (x) >= BSP_GPIO_PB0 && (x) <= BSP_GPIO_PB15 )) ? GPIOB : \
                    (  (( (x) >= BSP_GPIO_PC0 && (x) <= BSP_GPIO_PC13 ))) ? GPIOC : GPIOD )) \

/**
 * @brief Gets GPIO_PIN_XX number from a BSP_PIN enum value
 * 
 * @param pin 
 * @return uint8_t 
 */
uint8_t PinFromEnum(BSP_PINS pin);
/**
 * @brief Determines if pin AF mapping is correct
 * 
 * @param pin 
 * @param mapped_function 
 * @return true 
 * @return false 
 */
bool isValidPinMapping(BSP_PINS pin, uint8_t mapped_function);

/**
 * @brief Configures the BSP layer according to the configuration specified in bsp_config.h
 * 
 */
void BSP_Init();

// Only print BSP configuration once
// #includes are ignored in false preprocessor statements
#ifndef CONFIGURATION_PRINTED
#define CONFIGURATION_PRINTED
#include "bsp_config.h"
#endif

#endif