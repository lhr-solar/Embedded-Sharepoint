

# File CAN\_FD.h

[**File List**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Inc**](dir_f9758a777e5683291000913d5e959461.md) **>** [**CAN\_FD.h**](CAN__FD_8h.md)

[Go to the documentation of this file](CAN__FD_8h.md)


```C++
#pragma once

#include "stm32xx_hal.h"
#include "CAN_Common.h"
#include "queue_ex.h"

#if !defined(FDCAN1)
  #error "[CONFIG] CAN FD not enabled on this chip."
#endif /* FDCAN1 */


// fdcan handlers
#ifdef FDCAN1
extern FDCAN_HandleTypeDef* hfdcan1;
#endif /* FDCAN1 */

#ifdef FDCAN2
extern FDCAN_HandleTypeDef* hfdcan2;
#endif /* FDCAN2 */

#ifdef FDCAN3
extern FDCAN_HandleTypeDef* hfdcan3;
#endif /* FDCAN3 */


can_status_t can_fd_init(FDCAN_HandleTypeDef* handle, FDCAN_FilterTypeDef* filter);

can_status_t can_fd_deinit(FDCAN_HandleTypeDef* handle);

can_status_t can_fd_start(FDCAN_HandleTypeDef* handle);

can_status_t can_fd_send(FDCAN_HandleTypeDef* handle, FDCAN_TxHeaderTypeDef* header, uint8_t data[], TickType_t delay_ticks);

can_status_t can_fd_recv(FDCAN_HandleTypeDef* handle, uint16_t id, FDCAN_RxHeaderTypeDef* header, uint8_t data[], TickType_t delay_ticks);


#if ( configUSE_QUEUE_SETS == 1 )
can_status_t can_fd_register_id_set(FDCAN_HandleTypeDef* handle, can_id_set_t* set);

can_status_t can_fd_recv_set(FDCAN_HandleTypeDef* handle, can_id_set_t* set, uint16_t *id, TickType_t delay_ticks);

#endif /* ( configUSE_QUEUE_SETS == 1 ) */

void can_fd_tx_callback_hook(FDCAN_HandleTypeDef* hfdcan, const can_tx_payload_t* payload);

void can_fd_rx_callback_hook(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs, can_rx_payload_t recv_payload);
```


