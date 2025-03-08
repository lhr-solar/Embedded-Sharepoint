#ifndef USART_H_
#define USART_H_
#ifndef USART_UNDEFINED 

#include "stm32xx_hal.h" 

/* Function Descriptions:
 * Init initializes the USART peripheral given the configuration
 * DeInit deinitializes the USART peripheral meaning the configuration is reset
 * Send queues data to be transmitted when the USART peripheral is available
 * Recv reads received data from the RX queue
 *
 * How to Use This Driver:
 * 1. Init to initialize the USART peripheral
 * 2. Send/Recv operations
 * 3. DeInit to deinitialize
 * 4. Init to start over...
 */

// Return codes for USART operations
typedef enum {
    USART_ERR,
    USART_OK,
    USART_SENT,
    USART_RECV,
    USART_EMPTY,
} usart_status_t;

// USART handlers
#ifdef USART1
extern USART_HandleTypeDef* husart1;
#endif /* USART4 */

#ifdef USART2
extern USART_HandleTypeDef* husart2;
#endif /* USART5 */

#ifdef USART3
extern USART_HandleTypeDef* husart3;
#endif /* USART5 */


// Function prototypes with consistent parameters and return types
usart_status_t USART_init(USART_HandleTypeDef* handle); // Create queue at compile time, User passes in the size of the queue they want
usart_status_t USART_deinit(USART_HandleTypeDef* handle);
usart_status_t USART_send(USART_HandleTypeDef* handle, const uint8_t* data, uint8_t length, TickType_t delay_ticks);
usart_status_t USART_recv(USART_HandleTypeDef* handle, uint8_t* data, uint8_t length, TickType_t delay_ticks);

#endif /* USART_UNDEFINED */
#endif /* USART_H_ */