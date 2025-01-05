#ifndef CAN_H
#define CAN_H
#ifndef CAN_UNDEFINED

#include <stdbool.h>
#include "stm32xx_hal.h"

// return code for can driver
typedef enum {
    CAN_ERR,
    CAN_OK,
    CAN_SENT,
    CAN_RECV,
    CAN_EMPTY,
} can_status_t;

// can handlers
#ifdef CAN1
extern CAN_HandleTypeDef* hcan1;
#endif /* CAN1 */

can_status_t can_init(CAN_HandleTypeDef* handle, CAN_FilterTypeDef* filter);
can_status_t can_send(CAN_HandleTypeDef* handle, const CAN_TxHeaderTypeDef* header, const uint8_t data[], bool blocking);
can_status_t can_recv(CAN_HandleTypeDef* handle, uint16_t id, CAN_RxHeaderTypeDef* header, uint8_t data[], bool blocking);

#endif /* CAN_UNDEFINED */
#endif /* CAN_H */
