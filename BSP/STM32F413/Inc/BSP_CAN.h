#include <stdint.h> 
#include "stm32f4xx_hal.h"

typedef struct Communicator {
    uint32_t id;
    QueueHandle_t *queue;
} Communicator_t;

// void CAN_Init(CAN_TypeDef *can);
void CAN_SetSpeaker(CAN_TypeDef *can, CANID_t id, QueueHandle_t *queue);
void CAN_ClearSpeaker(CAN_TypeDef *can, CANID_t id);
void CAN_SetListener(CAN_TypeDef *can, CANID_t id, QueueHandle_t *queue);
void CAN_ClearListener(CAN_TypeDef *can, CANID_t id);