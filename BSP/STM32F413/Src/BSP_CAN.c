#include "BSP_CAN.h"
#include "queue.h"
#include "FreeRTOS.h"
#include "CANMetaData.h"

#define NUM_LISTENERS 16


static CAN_HandleTypeDef CarCAN;
static CAN_HandleTypeDef LocalCAN;

static Listener_t CarListeners[NUM_LISTENERS] = {0};
static Listener_t LocalListeners[NUM_LISTENERS] = {0};
static CarListenersCount = 0;
static LocalListenersCount = 0;

void CAN_Init(CAN_TypeDef *can)
{
    // init init struct
    // init gpio
    // HAL_CAN_Start():
}

void CAN_Send(CAN_TypeDef *can, CANID_t id, QueueHandle_t *xQueue)
{
    
}

void CAN_SetListener(CAN_TypeDef *can, CANID_t id, QueueHandle_t *xQueue)
{
    /* set listeners depending on bus */
    Listener_t *listeners = can == CAN1 ? CarListeners : LocalListeners;

    /* find if listener is already set (linear set) */
    for (int i = 0; i < NUM_LISTENERS; i++) {
        if (listeners[i].id == id) {
            /* replace if found (don't want duplicate ids) */
            listeners[i].queue = xQueue;
            return;
        }
    }

    /* find empty listener spot */
    for (int i = 0; i < NUM_LISTENERS; i++) {
        if (listeners[i].queue == NULL) {
            /* set if found */
            listeners[i].id = id;
            listeners[i].queue = xQueue;
        }
    }
}

void CAN_ClearListener(CAN_TypeDef *can, CANID_t id)
{
    /* set listeners depending on bus */
    Listener_t *listeners = can == CAN1 ? CarListeners : LocalListeners;

    /* clear listener if present */
    for (int i = 0; i < NUM_LISTENERS; i++) {
        if (listeners[i].id == id) {
            listeners[i].id = 0;
            listeners[i].queue = NULL;
        }
    }
}

void CAN1_RX0_IRQHandler()
{
    /* disable interrupts */
    portENTER_CRITICAL();

    /* check for any pending messages */
    while(HAL_CAN_GetRxFifoFillLevel(CAN1, CAN_RX_FIFO0))
    {
        /* define data capture */
        CAN_RxHeaderTypeDef pHeader;
        uint8_t aData[8] = {0};

        /* grab data from hardware rx fifo */
        HAL_CAN_GetRxMessage(CAN1, CAN_RX_FIFO0, &pHeader, aData);

        /* define queue to put data into */
        QueueHandle_t *queue = NULL;

        /* find listener (linear search) */
        for (int i = 0; i < CarListeners; i++) {
            if (CarListeners[i].id == pHeader.StdId) {

                /* If found, then set queue to put data into */
                queue = CarListeners[i].queue;
            }
        }

        /* if listener found, put data into queue */
        if (queue) {
            xQueueSend(*queue, &aData, 0);
        }
    }

    /* enable interrupts */
    portEXIT_CRITICAL();
}

void CAN3_RX0_IRQHandler()
{
    /* disable interrupts */
    portENTER_CRITICAL();

    /* check for any pending messages */
    while(HAL_CAN_GetRxFifoFillLevel(CAN3, CAN_RX_FIFO0))
    {
        /* define data capture */
        CAN_RxHeaderTypeDef pHeader;
        uint8_t aData[8] = {0};

        /* grab data from hardware rx fifo */
        HAL_CAN_GetRxMessage(CAN3, CAN_RX_FIFO0, &pHeader, aData);

        /* define queue to put data into */
        QueueHandle_t *queue = NULL;

        /* find listener (linear search) */
        for (int i = 0; i < LocalListeners; i++) {
            if (LocalListeners[i].id == pHeader.StdId) {

                /* If found, then set queue to put data into */
                queue = LocalListeners[i].queue;
            }
        }

        /* if listener found, put data into queue */
        if (queue) {
            xQueueSend(*queue, &aData, 0);
        }
    }

    /* enable interrupts */
    portEXIT_CRITICAL();
}