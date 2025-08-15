

# File queue\_ex.h

[**File List**](files.md) **>** [**common**](dir_bdd9a5d540de89e9fe90efdfc6973a4f.md) **>** [**Inc**](dir_4152f073bf32bf79124b676c9fe2adc1.md) **>** [**queue\_ex.h**](queue__ex_8h.md)

[Go to the documentation of this file](queue__ex_8h.md)


```C++
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
```


