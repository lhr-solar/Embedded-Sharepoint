#ifndef BSP_CONF_H
#define BSP_CONF_H

/**
 * Common features / necessities used on the BSP layer of
 * Embedded Sharepoint
 * 
 * Has a struct used for 
 */
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_uart.h"

// Global handles to the possible peripherals
extern UART_HandleTypeDef uart4_handle;
extern UART_HandleTypeDef uart5_handle;
extern TIM_HandleTypeDef tim1_handle, tim2_handle, tim3_handle, tim9_handle;
extern I2C_HandleTypeDef i2c1_handle;
extern I2C_HandleTypeDef i2c3_handle;
extern SPI_HandleTypeDef spi2_handle;
extern SPI_HandleTypeDef spi3_handle;
extern CAN_HandleTypeDef can2_handle;
extern CAN_HandleTypeDef can3_handle;

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

// BSP-level error information and handling
typedef enum {
    BSP_OK = 0x0,
    BSP_ERROR = 0x1,
    BSP_BUSY = 0x2,
    BSP_TIMEOUT = 0x4
} BSP_Status;

// Macros for turning HAL_StatusTypeDef to our BSP error
#define CONVERT_RETURN(x) (((x) == 0x3U) ? (BSP_TIMEOUT) : (x))
// Macros for interpreting BSP returns
#define HAS_TIMEOUT(x) ((x) & 0x4)
#define HAS_BUSY(x) ((x) & 0x2)
#define HAS_ERROR(x) ((x) & 0x1)
#define IS_OK(x) (!(HAS_TIMEOUT(x) || HAS_BUSY(x) || HAS_ERROR(x)))

#endif