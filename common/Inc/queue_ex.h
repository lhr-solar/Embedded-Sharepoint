#ifndef QUEUE_EX_H
#define QUEUE_EX_H

#define xQueueSendCircularBuffer( xQueue, pvItemToQueue, xTicksToWait, QUEUE_ITEM_SIZE ) \
    do { \
        taskENTER_CRITICAL(); \
        if (xQueueSend(xQueue, pvItemToQueue, 0) == errQUEUE_FULL) \
        { \
            uint8_t tempBuffer[QUEUE_ITEM_SIZE]; \
            xQueueReceive(xQueue, tempBuffer, 0); \
            xQueueSend(xQueue, pvItemToQueue, xTicksToWait); \
        } \
        taskEXIT_CRITICAL(); \
    } while(0)

#define xQueueSendCircularBufferFromISR( xQueue, pvItemToQueue, pxHigherPriorityTaskWoken, QUEUE_ITEM_SIZE ) \
    do { \
        UBaseType_t uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR(); \
        if (xQueueSendFromISR(xQueue, pvItemToQueue, pxHigherPriorityTaskWoken) == errQUEUE_FULL) { \
            uint8_t tempBuffer[QUEUE_ITEM_SIZE];    \
            xQueueReceiveFromISR(xQueue, tempBuffer, NULL); \
            xQueueSendFromISR(xQueue, pvItemToQueue, pxHigherPriorityTaskWoken);    \
        }   \
        taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus); \
    } while(0)

#endif