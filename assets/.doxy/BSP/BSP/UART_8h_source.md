

# File UART.h

[**File List**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Inc**](dir_f9758a777e5683291000913d5e959461.md) **>** [**UART.h**](UART_8h.md)

[Go to the documentation of this file](UART_8h.md)


```C++

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

uart_status_t uart_init(UART_HandleTypeDef* handle);

uart_status_t uart_deinit(UART_HandleTypeDef* handle);

uart_status_t uart_send(UART_HandleTypeDef* handle, const uint8_t* data, uint8_t length, TickType_t delay_ticks);

uart_status_t uart_recv(UART_HandleTypeDef* handle, uint8_t* data, uint8_t length, TickType_t delay_ticks);

#endif /* UART_H_ */
```


