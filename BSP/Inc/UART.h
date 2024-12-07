#ifndef UART_H_
#define UART_H_
#ifndef UART_UNDEFINED // if UART is not defined

// FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

// Standard Library
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "stm32l4xx_hal.h" // stm32l4xx HAL library, substituing for the agnostic approach

// Return codes for UART operations
typedef enum {
    UART_ERR,
    UART_OK,
    UART_SENT,
    UART_RECV,
    UART_EMPTY,
} uart_status_t;

// UART handlers - similar to how CAN exposes its handlers
#ifdef UART4
extern UART_HandleTypeDef* huart4;
#endif /* UART4 */

// Function prototypes with consistent parameters and return types
/**
 * @brief Initializes UART peripheral and internal TX queue.
 * Hardware configuration must be performed outside of this function.
 *
 * @param handle pointer to the UART handle
 * @param rxQueue pointer to the receive queue
 * @return uart_status_t
 */
uart_status_t uart_init(UART_HandleTypeDef* handle, QueueHandle_t* rxQueue);

/**
 * @brief Transmits data over UART. If transmission is in progress,
 * data will be queued in internal TX queue.
 * @param handle pointer to the UART handle
 * @param data pointer to the data buffer that will be written/transmitted
 * @param length of the buffer that will be written/transmitted
 * @param blocking whether the operation should be blocking
 * @return uart_status_t
 */
uart_status_t uart_send(UART_HandleTypeDef* handle, const uint8_t* data, uint8_t length, bool blocking);


/**
 * @brief Reads data from the user-provided RX queue
 * @param handle pointer to the UART handle
 * @param data buffer to store the received data
 * @param length of number of bytes to read
 */
uart_status_t uart_recv(UART_HandleTypeDef* handle, uint8_t* data, uint8_t length, bool blocking);

#endif /* UART_UNDEFINED */
#endif /* UART_H_ */