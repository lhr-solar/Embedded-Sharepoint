/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
// BSP UART header file

#ifndef BSP_UART_H
#define BSP_UART_H

#include <stdint.h>
#include "stm32f4xx_hal.h" // this ALWAYS needs to be included
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_usart.h"
#include "BSP.h"

#define TX_SIZE 2048
#define RX_SIZE 512 // keep 64?

typedef enum {UART_2, UART_3, NUM_UART} UART_t;

/**
 * @brief Initialize the UART pins as necessary
 * @note Uses PB10, PC5; UART2/3;
 * @note Configuration pulled from BPS repository's BSP_UART.c
 */
BSP_Status BSP_UART_Init();

/**
 * @brief Reads a single line of input 
 * 
 * @return Pointer to the populated rx_buffer; returns NULL if error occurred
 */
char* BSP_UART_ReadLine(UART_t usart);

/**
 * @brief Writes a single line of input to UART
 * 
 * @param input The input string
 * @param len Length of input string
 */
BSP_Status BSP_UART_WriteLine(UART_t usart, char* input, uint32_t len);

#endif