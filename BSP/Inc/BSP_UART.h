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
#include "BSP.h"

#define TX_SIZE 60
#define RX_SIZE 60

typedef enum {UART_1, UART_2, UART_3, NUM_UART} UART_t;

//TODO This will be replaced with general BSP status enum when it is merged in
typedef enum {WRITE_FAIL, WRITE_SUCCESS} Write_Status;

/**
 * @brief Initialize the UART pins as necessary
 */
BSP_Status BSP_UART_Init();

/**
 * @brief   Writes a message to the specified UART device
 * @param   device the proper UART device to write to defined by the UART_t enum
 * @param   data pointer to the array containing the message to sned
 * @param   len length of the message in bytes
 * @return  Status of write operation (WRITE_SUCCESS OR WRITE_FAIL)
 */
Write_Status BSP_UART_Write(UART_t device, char* data, uint32_t len);

/**
 * @brief   Reads a message from the specified UART device
 * @param   device the proper UART device to write to defined by the UART_t
 * @param   len number of bytes that will be returned
 * @return  len bytes from the recieve queue
 */
char* BSP_UART_Read(UART_t device, uint32_t len);

#endif