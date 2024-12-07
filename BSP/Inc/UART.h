#ifndef UART_H_
#define UART_H_

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

#include "stm32l4xx_hal.h"

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
uart_status_t uart_init(UART_HandleTypeDef* handle, QueueHandle_t* rxQueue);
uart_status_t uart_send(UART_HandleTypeDef* handle, const uint8_t* data, uint8_t length, bool blocking);
uart_status_t uart_recv(UART_HandleTypeDef* handle, uint8_t* data, uint8_t length, bool blocking);

#endif /* INC_UART_H_ */