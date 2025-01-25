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


/* Function Descriptions:
 * Init initializes the UART peripheral given the configuration
 * DeInit deinitializes the UART peripheral meaning the configuration is reset
 * Start starts the UART peripheral meaning it is able to interrupt and transmit/receive
 * Stop stops the UART peripheral meaning the configuration is unchanged but inactive
 * Send queues data to be transmitted when the UART peripheral is available
 * Recv reads received data from the RX queue
 *
 * How to Use This Driver:
 * 1. Init to initialize the UART peripheral
 * 2. Start to activate the UART peripheral
 * 3. Send/Recv operations
 * 4. Stop/Start as needed
 * 5. DeInit to deinitialize
 * 6. Init to start over...
 */


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

#ifdef UART5
extern UART_HandleTypeDef* huart5;
#endif /* UART5 */

// Function prototypes with consistent parameters and return types
uart_status_t uart_init(UART_HandleTypeDef* handle, QueueHandle_t* rxQueue);
uart_status_t uart_deinit(UART_HandleTypeDef* handle);
uart_status_t uart_send(UART_HandleTypeDef* handle, const uint8_t* data, uint8_t length, bool blocking);
uart_status_t uart_recv(UART_HandleTypeDef* handle, uint8_t* data, uint8_t length, bool blocking);

#endif /* UART_UNDEFINED */
#endif /* UART_H_ */