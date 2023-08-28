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

#define TX_SIZE 2048
#define RX_SIZE 512 // keep 64?

/**
 * @brief Initialize the UART pins as necessary
 * @note Uses PB10, PC5; UART2/3;
 * @note Configuration pulled from BPS repository's BSP_UART.c
 */
void BSP_UART_Init();

/**
 * @brief Reads a single line of input 
 * 
 * @return Pointer to the populated rx_buffer
 */
char* BSP_UART_ReadLine();

/**
 * @brief Writes a single line of input to UART
 * 
 * @param input The input string
 * @param len Length of input string
 */
void BSP_UART_WriteLine(const char* input, uint32_t len);

#endif