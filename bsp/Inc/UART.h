/**
 * @file UART.h
 * @brief Provides functions for initializing, sending, receiving, and managing UART peripherals.
 *
 * @details
 * This file contains the API for working with UART (Universal Asynchronous Receiver/Transmitter)
 * peripherals in an embedded system, including initialization, data transmission, reception,
 * and queue-based handling (e.g., with FreeRTOS). The functions support flexible configuration
 * of UART instances and integration with user-provided GPIO initialization.
 *
 * **Initialization**:
 *
 *    - Call `uart_init()` to configure and initialize a specific UART peripheral.
 *
 *    - Provide the UART configuration structure and handle for the target peripheral.
 *
 *    - Returns a status code indicating success or failure.
 *
 * **Data Transmission**:
 *
 *    - Call `uart_send()` to queue data for transmission over the UART.
 *
 *    - The function will transmit when the peripheral is available.
 *
 * **Data Reception**:
 *
 *    - Call `uart_recv()` to read received data from the RX queue.
 *
 *    - Specify the number of bytes to read.
 *
 * **Deinitialization**:
 *
 *    - Call `uart_deinit()` to reset the UART peripheral and release resources.
 *
 * @note
 *
 *    - The functions `HAL_UART_MspGPIOInit()` and `HAL_UART_MspGPIODeInit()` must be
 *      overridden by the user to provide GPIO initialization/deinitialization for the selected UART,
 *      including enabling clocks and calling `HAL_GPIO_Init()` with the correct `GPIO_InitTypeDef`.
 *
 *    - Each UART peripheral is currently intended for use by a single task since only one receive queue exists.
 */
#ifndef UART_H_
#define UART_H_

#include "stm32xx_hal.h"

#if !defined(UART4) && !defined(USART1)
  #error "[CONFIG] UART not enabled on this chip."
#endif /* UART4 */

// Return codes for UART operations
typedef enum {
    UART_ERR,
    UART_OK,
    UART_SENT,
    UART_RECV,
    UART_EMPTY,
} uart_status_t;

// uart handlers
#ifdef UART4
extern UART_HandleTypeDef* huart4;
#endif /* UART4 */

#ifdef UART5
extern UART_HandleTypeDef* huart5;
#endif /* UART5 */

#ifdef USART1
extern UART_HandleTypeDef* husart1;
#endif /* USART1 */

#ifdef USART2
extern UART_HandleTypeDef* husart2;
#endif /* USART2 */

#ifdef USART3
extern UART_HandleTypeDef* husart3;
#endif /* USART3 */

/**
 * @brief Initializes the specified UART peripheral.
 * 
 * @param handle Pointer to the UART handle.
 * @return uart_status_t Returns UART_OK on success, UART_ERR on failure.
 */
uart_status_t uart_init(UART_HandleTypeDef* handle);

/**
 * @brief Deinitializes the specified UART peripheral.
 * 
 * @param handle Pointer to the UART handle.
 * @return uart_status_t Returns UART_OK on success, UART_ERR on failure.
 */
uart_status_t uart_deinit(UART_HandleTypeDef* handle);

/**
 * @brief Transmits data over UART. If a transmission is in progress, data will be queued.
 * 
 * @param handle Pointer to the UART handle.
 * @param data Pointer to the buffer containing data to send.
 * @param length Number of bytes to transmit.
 * @param delay_ticks Ticks to wait if TX queue is full (0 = no wait, portMAX_DELAY = wait indefinitely).
 * @return uart_status_t Returns UART_SENT on queued/sent, UART_ERR on failure.
 */
uart_status_t uart_send(UART_HandleTypeDef* handle, const uint8_t* data, uint8_t length, TickType_t delay_ticks);

/**
 * @brief Receives data from UART RX queue.
 * 
 * @param handle Pointer to the UART handle.
 * @param data Pointer to buffer where received data will be stored.
 * @param length Number of bytes to read.
 * @param delay_ticks Ticks to wait for data (0 = non-blocking, portMAX_DELAY = block until available).
 * @return uart_status_t Returns UART_RECV on success, UART_EMPTY if RX queue empty, UART_ERR on failure.
 */
uart_status_t uart_recv(UART_HandleTypeDef* handle, uint8_t* data, uint8_t length, TickType_t delay_ticks);

#endif /* UART_H_ */
