

# File CAN\_FD.h

[**File List**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Inc**](dir_f9758a777e5683291000913d5e959461.md) **>** [**CAN\_FD.h**](CAN__FD_8h.md)

[Go to the documentation of this file](CAN__FD_8h.md)


```C++
#pragma once

#include "CAN_Common.h"
#include "queue_ex.h"
#include "stm32xx_hal.h"

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

#define FDCAN_DLC_BYTES(len) \
((len) == 0  ? FDCAN_DLC_BYTES_0  : \
 (len) == 1  ? FDCAN_DLC_BYTES_1  : \
 (len) == 2  ? FDCAN_DLC_BYTES_2  : \
 (len) == 3  ? FDCAN_DLC_BYTES_3  : \
 (len) == 4  ? FDCAN_DLC_BYTES_4  : \
 (len) == 5  ? FDCAN_DLC_BYTES_5  : \
 (len) == 6  ? FDCAN_DLC_BYTES_6  : \
 (len) == 7  ? FDCAN_DLC_BYTES_7  : \
 (len) == 8  ? FDCAN_DLC_BYTES_8  : \
 (len) == 12 ? FDCAN_DLC_BYTES_12 : \
 (len) == 16 ? FDCAN_DLC_BYTES_16 : \
 (len) == 20 ? FDCAN_DLC_BYTES_20 : \
 (len) == 24 ? FDCAN_DLC_BYTES_24 : \
 (len) == 32 ? FDCAN_DLC_BYTES_32 : \
 (len) == 48 ? FDCAN_DLC_BYTES_48 : \
 (len) == 64 ? FDCAN_DLC_BYTES_64 : \
 FDCAN_DLC_BYTES_0)

#define FDCAN_BYTES_FROM_DLC(dlc) \
((dlc) == FDCAN_DLC_BYTES_0  ? 0  : \
 (dlc) == FDCAN_DLC_BYTES_1  ? 1  : \
 (dlc) == FDCAN_DLC_BYTES_2  ? 2  : \
 (dlc) == FDCAN_DLC_BYTES_3  ? 3  : \
 (dlc) == FDCAN_DLC_BYTES_4  ? 4  : \
 (dlc) == FDCAN_DLC_BYTES_5  ? 5  : \
 (dlc) == FDCAN_DLC_BYTES_6  ? 6  : \
 (dlc) == FDCAN_DLC_BYTES_7  ? 7  : \
 (dlc) == FDCAN_DLC_BYTES_8  ? 8  : \
 (dlc) == FDCAN_DLC_BYTES_12 ? 12 : \
 (dlc) == FDCAN_DLC_BYTES_16 ? 16 : \
 (dlc) == FDCAN_DLC_BYTES_20 ? 20 : \
 (dlc) == FDCAN_DLC_BYTES_24 ? 24 : \
 (dlc) == FDCAN_DLC_BYTES_32 ? 32 : \
 (dlc) == FDCAN_DLC_BYTES_48 ? 48 : \
 (dlc) == FDCAN_DLC_BYTES_64 ? 64 : \
 0)

can_status_t can_fd_init(FDCAN_HandleTypeDef* handle, FDCAN_FilterTypeDef* filter);

can_status_t can_fd_deinit(FDCAN_HandleTypeDef* handle);

can_status_t can_fd_start(FDCAN_HandleTypeDef* handle);

can_status_t can_fd_send(FDCAN_HandleTypeDef* handle, FDCAN_TxHeaderTypeDef* header, uint8_t data[],
                         TickType_t delay_ticks);
can_status_t can_fd_send_isr(FDCAN_HandleTypeDef* handle, FDCAN_TxHeaderTypeDef* header, 
                             uint8_t data[], BaseType_t* higherPriorityTaskWoken);

can_status_t can_fd_recv(FDCAN_HandleTypeDef* handle, uint32_t id, FDCAN_RxHeaderTypeDef* header,
                         uint8_t data[], TickType_t delay_ticks);


#if (configUSE_QUEUE_SETS == 1)
can_status_t can_fd_register_id_set(FDCAN_HandleTypeDef* handle, can_id_set_t* set);

can_status_t can_fd_recv_set(FDCAN_HandleTypeDef* handle, can_id_set_t* set, uint32_t* id,
                             TickType_t delay_ticks);

#endif /* ( configUSE_QUEUE_SETS == 1 ) */

void can_fd_tx_callback_hook(FDCAN_HandleTypeDef* hfdcan, const can_tx_payload_t* payload);

void can_fd_rx_callback_hook(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs,
                             can_rx_payload_t recv_payload);
```


