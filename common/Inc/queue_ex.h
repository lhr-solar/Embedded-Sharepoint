#ifndef QUEUE_EX_H
#define QUEUE_EX_H

#define xQueueSendCircularBuffer( xQueue, pvItemToQueue, xTicksToWait ) \
    do { \
        portENTER_CRITICAL(); \
        if( uxQueueSpacesAvailable( xQueue ) == 0 ) \
        { \
            xQueueReceive( xQueue, NULL, 0 ); \
        } \
        xQueueSend( xQueue, pvItemToQueue, xTicksToWait ); \
        portEXIT_CRITICAL(); \
    } while(0);

#define xQueueSendCircularBufferFromISR( xQueue, pvItemToQueue, pxHigherPriorityTaskWoken ) \
    do { \
        portENTER_CRITICAL(); \
        if( xQueueIsQueueFullFromISR( xQueue ) ) \
        { \
            xQueueReceiveFromISR( xQueue, NULL, 0 ); \
        } \
        xQueueSendFromISR( xQueue, pvItemToQueue, pxHigherPriorityTaskWoken ); \
        portEXIT_CRITICAL(); \
    } while(0);

#endif