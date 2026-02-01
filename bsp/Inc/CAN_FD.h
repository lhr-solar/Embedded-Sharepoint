#pragma once

#include "stm32xx_hal.h"
#include "CAN_Common.h"

#if !defined(FDCAN1)
  #error "[CONFIG] CAN FD not enabled on this chip."
#endif /* FDCAN1 */


// can handlers
#ifdef FDCAN1
extern FDCAN_HandleTypeDef* hfdcan1;
#endif /* CAN1 */

#ifdef FDCAN2
extern FDCAN_HandleTypeDef* hfdcan2;
#endif /* CAN2 */

#ifdef FDCAN3
extern FDCAN_HandleTypeDef* hfdcan3;
#endif /* CAN3 */

can_status_t can_fd_init(FDCAN_HandleTypeDef* handle, FDCAN_FilterTypeDef* filter);
can_status_t can_fd_deinit(FDCAN_HandleTypeDef* handle);
can_status_t can_fd_start(FDCAN_HandleTypeDef* handle);
can_status_t can_fd_send(FDCAN_HandleTypeDef* handle, FDCAN_TxHeaderTypeDef* header, uint8_t data[], TickType_t delay_ticks);

void can_fd_tx_complete_hook();