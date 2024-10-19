#ifndef BSP_UART_H
#define BSP_UART_H


#include <stdint.h>
#include <stdbool.h>

#include <stm32xx_hal.h> //  header for system agnostic support for L4 and F4

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h" 
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_usart.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

// RX size will exist outside of the BSP
// TX size will be defined by the user since the TX queue will exist outside of the BSP

/**
 * @brief Initilizes UART peripheral. Struct config must be performed 
 *  outside of this function
 * 
 * @param config 
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef BSP_UART_Init(UART_HandleTypeDef *huart, QueueHandle_t *rxQueue);

/**
 * @brief Perform a UART write to the specified UART device
 * @param huart pointer to the UART handle
 * @param buffer pointer to the buffer that will be written
 * @param length of the buffer that will be written
 * @param busID the bus to which message will be sent over
 * @return Status of write operation 
 */
HAL_StatusTypeDef BSP_UART_Write(char *data, uint8_t length, uint8_t busId);


/**
 * @brief  Reads message from the specified UART device 
 * @param huart pointer to the UART handle
 * @param length of bytes to read from the receive queue
 * @param busId the bus to which message will be received from
 * @return len of bytes from the receive queue
 */
HAL_StatusTypeDef BSP_UART_Read(int* data, uint8_t length, uint8_t busId);


#endif
