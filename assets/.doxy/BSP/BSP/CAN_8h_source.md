

# File CAN.h

[**File List**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Inc**](dir_f9758a777e5683291000913d5e959461.md) **>** [**CAN.h**](CAN_8h.md)

[Go to the documentation of this file](CAN_8h.md)


```C++

#ifndef CAN_H
#define CAN_H

#include "stm32xx_hal.h"

#if !defined(CAN1)
  #error "[CONFIG] CAN not enabled on this chip."
#endif /* CAN1 */

// return code for can driver
typedef enum {
    CAN_ERR,   // unsuccessful operation
    CAN_OK,
    CAN_SENT,  // successful send
    CAN_RECV,  // successful recieve
    CAN_EMPTY, // recieved nothing with no errors
} can_status_t;

// can handlers
#ifdef CAN1
extern CAN_HandleTypeDef* hcan1;
#endif /* CAN1 */

#ifdef CAN2
extern CAN_HandleTypeDef* hcan2;
#endif /* CAN2 */

#ifdef CAN3
extern CAN_HandleTypeDef* hcan3;
#endif /* CAN3 */

can_status_t can_init(CAN_HandleTypeDef* handle, CAN_FilterTypeDef* filter);

can_status_t can_deinit(CAN_HandleTypeDef* handle);

can_status_t can_start(CAN_HandleTypeDef* handle);

can_status_t can_stop(CAN_HandleTypeDef* handle);

can_status_t can_send(CAN_HandleTypeDef* handle,
                      const CAN_TxHeaderTypeDef* header, const uint8_t data[],
                      TickType_t delay_ticks);

can_status_t can_recv(CAN_HandleTypeDef* handle, uint16_t id,
                      CAN_RxHeaderTypeDef* header, uint8_t data[],
                      TickType_t delay_ticks);

#endif /* CAN_H */
```


