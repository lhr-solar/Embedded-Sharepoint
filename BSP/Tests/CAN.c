#include <stdio.h>
#include "BSP_CAN.h"

StaticQueue_t xQueueBuffer = (StaticQueue_t){0};
uint8_t ucQueueStorage[8 * sizeof(char)];

int main(void)
{
    /* setup queue */
    QueueHandle_t xQueue = xQueueCreateStatic(
        8,
        sizeof(char),
        ucQueueStorage,
        &xQueueBuffer);

    /* init can */
    CAN_CarCANInit();

    /* set speaker */
    CAN_SetSpeaker(CarCAN, 0x01, &xQueue);
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}