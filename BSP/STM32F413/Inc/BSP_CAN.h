#include <stdint.h> 
#include "stm32f4xx_hal.h"

/* rename for readability */
#define CarCAN1 (CAN1)
#define LocalCAN3 (CAN3)

/* communicators keep track of queues to use for communication*/
typedef struct Communicator {
    uint32_t id;
    QueueHandle_t *queue;
} Communicator_t;

void CAN_CarCANInit(void);
void CAN_LocalCANInit(void);
void CAN_SetSpeaker(CAN_TypeDef *can, CANID_t id, QueueHandle_t *queue);
void CAN_ClearSpeaker(CAN_TypeDef *can, CANID_t id);
void CAN_SetListener(CAN_TypeDef *can, CANID_t id, QueueHandle_t *queue);
void CAN_ClearListener(CAN_TypeDef *can, CANID_t id);