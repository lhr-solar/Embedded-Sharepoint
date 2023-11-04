/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
// BSP UART header file

#ifndef BSP_UART_H
#define BSP_UART_H

#include <stdint.h>
#include "stm32f4xx_hal.h" 
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_usart.h"
#include "queue.h"

#define TX_SIZE 60
#define RX_SIZE 60
#define QUEUE_SIZE 300

typedef enum {UART_OK, UART_ERROR, UART_BUSY, UART_TIMEOUT} UART_Init_Status;
typedef enum {WRITE_SUCCESS, WRITE_FAIL} UART_Write_Status;

/**
 * @brief   Initialize the UART pins as necessary
 * @param   device the proper UART device to write to defined by the UART_t enum\
 * @return  Status of init operation
 */
UART_Init_Status BSP_UART_Init(USART_HandleTypeDef device);

/**
 * @brief   Writes a message to the specified UART device
 * @param   device the proper UART device to write to defined by the UART_t enum
 * @param   data pointer to the array containing the message to sned
 * @param   len length of the message in bytes
 * @return  Status of write operation (WRITE_SUCCESS OR WRITE_FAIL)
 */
UART_Write_Status BSP_UART_Write(USART_HandleTypeDef device, char* data, uint32_t len);

/**
 * @brief   Reads a message from the specified UART device
 * @param   device the proper UART device to write to defined by the UART_t
 * @param   len number of bytes that will be returned
 * @return  len bytes from the recieve queue
 */
char* BSP_UART_Read(USART_HandleTypeDef device, uint32_t len);

#endif