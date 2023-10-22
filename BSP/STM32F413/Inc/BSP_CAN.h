#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__

#include "common.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "CANMetaData.h"

/* communicators keep track of queues to use for communication*/
typedef struct Communicator
{
    uint32_t id;
    QueueHandle_t *queue;
} Communicator_t;

/* handlers */
extern const CAN_HandleTypeDef *CarCAN;
extern const CAN_HandleTypeDef *LocalCAN;

void CAN_CarCANInit(void);
void CAN_LocalCANInit(void);
void CAN_SetSpeaker(const CAN_HandleTypeDef *can, CANID_t id, QueueHandle_t *queue);
void CAN_ClearSpeaker(const CAN_HandleTypeDef *can, CANID_t id);
void CAN_SetListener(const CAN_HandleTypeDef *can, CANID_t id, QueueHandle_t *queue);
void CAN_ClearListener(const CAN_HandleTypeDef *can, CANID_t id);

#endif /* __BSP_CAN_H__ */
