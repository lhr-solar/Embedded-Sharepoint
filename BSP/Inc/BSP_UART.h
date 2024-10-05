#ifndef BSP_UART_H
#define BSP_UART_H


#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h" 
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_usart.h"
#include "FreeRTOS.h"
#include "queue.h"


#define TX_SIZE 60
#define RX_SIZE 60
#define QUEUE_SIZE 300

/**
 * @brief Initilizes UART peripheral. Struct config must be performed 
 *  outside of this function
 * 
 * @param config 
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef BSP_UART_Initi(UART_HandleTypeDef *device);

/**
 * @brief Perform a UART write to the specified UART device
 * @param device that the proper UART device to write to
 * @param data pointer to the array containg the message to send
 * @param length of the message in bytes
 * @return Status of write operation 
 */
HAL_StatusTypeDef BSP_UART_Write(UART_HandleTypeDef *device, char* data, uint32_t length);


/**
 * @brief  Reads message from the specified UART device
 * @param device the proper UART device to read fro
 * @param len of the number of bytes that will be return
 * @return len of bytes from the receive queue
 */
HAL_StatusTypeDef BSP_UART_Read(UART_HandleTypeDef *device, uint32_t len);


#endif
