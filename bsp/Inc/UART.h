#ifndef UART_H_
#define UART_H_

#include "stm32xx_hal.h"

#if !defined(UART4) && !defined(USART1)
  #error "[CONFIG] UART not enabled on this chip."
#endif /* UART4 */

/* Function Descriptions:
 * Init initializes the UART peripheral given the configuration
 * DeInit deinitializes the UART peripheral meaning the configuration is reset
 * Send queues data to be transmitted when the UART peripheral is available
 * Recv reads received data from the RX queue
 *
 * How to Use This Driver:
 * 1. Init to initialize the UART peripheral
 * 2. Send/Recv operations
 * 3. DeInit to deinitialize
 * 4. Init to start over...
 */

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

// Function prototypes with consistent parameters and return types
uart_status_t uart_init(UART_HandleTypeDef* handle); // Create queue at compile time, User passes in the size of the queue they want
uart_status_t uart_deinit(UART_HandleTypeDef* handle);
uart_status_t uart_send(UART_HandleTypeDef* handle, const uint8_t* data, uint8_t length, TickType_t delay_ticks);
uart_status_t uart_recv(UART_HandleTypeDef* handle, uint8_t* data, uint8_t length, TickType_t delay_ticks);

#endif /* UART_H_ */
