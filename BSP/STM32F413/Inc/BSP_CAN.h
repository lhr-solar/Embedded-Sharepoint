#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__

#include <stdbool.h>
#include "common.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "CANMetaData.h"

typedef struct CAN_TxPayload
{
    CAN_TxHeaderTypeDef header;
    uint32_t mailbox;
    uint8_t data[8];
} CAN_TxPayload_t;

typedef struct CAN_RxPayload
{
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
} CAN_RxPayload_t;

/* handlers */
extern const CAN_HandleTypeDef *CarCAN;
extern const CAN_HandleTypeDef *LocalCAN;

void CAN_CarCANInit(void);
void CAN_LocalCANInit(void);
bool CAN_Send(const CAN_HandleTypeDef *can, CAN_TxPayload_t payload, bool blocking);
bool CAN_SetListener(const CAN_HandleTypeDef *can, CANID_t id, QueueHandle_t *queue);
void CAN_ClearListener(const CAN_HandleTypeDef *can, CANID_t id);

#endif /* __BSP_CAN_H__ */
