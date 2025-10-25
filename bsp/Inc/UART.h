#ifndef UART_H_
#define UART_H_

#include "stm32xx_hal.h"
#include "bsp_config.h"

#if !defined(UART4) && !defined(USART1)
  #error "[CONFIG] UART not enabled on this chip."
#endif /* UART4 */

/* Function Descriptions:
 * Init initializes the UART peripheral given the configuration
 * DeInit deinitializes the UART peripheral meaning the configuration is reset
 * Send queues data to be transmitted when the UART peripheral is available
 * Recv reads length bytes of received data from the RX queue
 *
 * How to Use This Driver:
 * 1. uart_init to initialize the UART peripheral
 * 2. uart_send/uart_recv operations
 * 3. uart_deinit to deinitialize
 * 
 * - The function HAL_UART_MspGPIOInit and HAL_UART_MspGPIODeInit must be overriden with the user's implementation of initializing/deinitializing the GPIO pins for the selected peripheral. This includes initializing the GPIO Clk and claling HAL_GPIO_Init on the correct GPIO_InitTypeDef structure.
 * - For now, a UART peripheral needs to be contained to one task as only one receive queue exists for each.
 *
 */

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
bsp_status_t uart_init(UART_HandleTypeDef* handle); // Create queue at compile time, User passes in the size of the queue they want
bsp_status_t uart_deinit(UART_HandleTypeDef* handle);
bsp_status_t uart_send(UART_HandleTypeDef* handle, const uint8_t* data, uint8_t length, TickType_t delay_ticks);
bsp_status_t uart_recv(UART_HandleTypeDef* handle, uint8_t* data, uint8_t length, TickType_t delay_ticks);

#endif /* UART_H_ */
