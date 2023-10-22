#include "BSP_CAN.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "CANMetaData.h"

#define NUM_LISTENERS 16
#define NUM_SPEAKERS 16

/* used for initialization */
static CAN_HandleTypeDef CarCAN;
static CAN_HandleTypeDef LocalCAN;

/* array of communicators */
static Communicator_t CarListeners[NUM_LISTENERS] = {0};
static Communicator_t LocalListeners[NUM_LISTENERS] = {0};
static Communicator_t CarSpeakers[NUM_SPEAKERS] = {0};
static Communicator_t LocalSpeakers[NUM_SPEAKERS] = {0};

// void CAN_Init(CAN_TypeDef *can)
// {
//     // init init struct
//     // init gpio
//     // HAL_CAN_Start():
// }

void CAN_SetSpeaker(CAN_TypeDef *can, CANID_t id, QueueHandle_t *queue)
{
    /* set speakers depending on bus */
    Communicator_t speakers[] = can == CAN1 ? CarSpeakers : LocalSpeakers;

    /* find if speaker is already set (linear search) */
    for (int i = 0; i < NUM_SPEAKERS; i++)
    {
        if (speakers[i].id == id)
        {
            /* replace if found (don't want duplicate ids) */
            speakers[i].queue = queue;
            return;
        }
    }

    /* find empty speaker spot (linear search) */
    for (int i = 0; i < NUM_SPEAKERS; i++)
    {
        if (speakers[i].queue == NULL)
        {
            /* set if found */
            speakers[i] = (Communicator_t){.id = id, .queue = queue};
        }
    }
}

void CAN_ClearSpeaker(CAN_TypeDef *can, CANID_t id)
{
    /* set speakers depending on bus */
    Communicator_t speakers[] = can == CAN1 ? CarSpeakers : LocalSpeakers;

    /* clear speaker if present */
    for (int i = 0; i < NUM_SPEAKERS; i++)
    {
        if (speakers[i].id == id)
        {
            speakers[i] = (Communicator_t){0};
        }
    }
}

void CAN_SetListener(CAN_TypeDef *can, CANID_t id, QueueHandle_t *queue)
{
    /* set listeners depending on bus */
    Communicator_t listeners[] = can == CAN1 ? CarListeners : LocalListeners;

    /* find if listener is already set (linear search) */
    for (int i = 0; i < NUM_LISTENERS; i++)
    {
        if (listeners[i].id == id)
        {
            /* replace if found (don't want duplicate ids) */
            listeners[i].queue = queue;
            return;
        }
    }

    /* find empty listener spot (linear search) */
    for (int i = 0; i < NUM_LISTENERS; i++)
    {
        if (listeners[i].queue == NULL)
        {
            /* set if found */
            listeners[i] = (Communicator_t){.id = id, .queue = queue};
        }
    }
}

void CAN_ClearListener(CAN_TypeDef *can, CANID_t id)
{
    /* set listeners depending on bus */
    Communicator_t listeners[] = can == CAN1 ? CarListeners : LocalListeners;

    /* clear listener if present */
    for (int i = 0; i < NUM_LISTENERS; i++)
    {
        if (listeners[i].id == id)
        {
            listeners[i] = (Communicator_t){0};
        }
    }
}

void CAN1_RX0_IRQHandler(void)
{
    /* disable interrupts */
    portENTER_CRITICAL();

    /* TODO: acknowledge flag? */

    /* check for any pending messages */
    while (HAL_CAN_GetRxFifoFillLevel(CAN1, CAN_RX_FIFO0))
    {
        /* define data capture */
        CAN_RxHeaderTypeDef pHeader = (CAN_RxHeaderTypeDef){0};
        uint8_t aData[8] = {0};

        /* grab data from hardware rx fifo */
        HAL_CAN_GetRxMessage(CAN1, CAN_RX_FIFO0, &pHeader, aData);

        /* define queue to put data into */
        QueueHandle_t *queue = NULL;

        /* find listener (linear search) */
        for (int i = 0; i < NUM_LISTENERS; i++)
        {
            if (LocalListeners[i].id == pHeader.StdId)
            {
                /* If found, then set queue to put data into */
                queue = LocalListeners[i].queue;
            }
        }

        /* if listener found, put data into queue */
        if (queue)
        {
            /* define variables to send to queue */
            BaseType_t pxHigherPriorityTaskWoken = 0;

            /* send data to queue */
            xQueueSendFromISR(*queue, &aData, &pxHigherPriorityTaskWoken);
        }
    }

    /* enable interrupts */
    portEXIT_CRITICAL();
}

void CAN3_RX0_IRQHandler(void)
{
    /* disable interrupts */
    portENTER_CRITICAL();

    /* TODO: acknowledge flag? */

    /* check for any pending messages */
    while (HAL_CAN_GetRxFifoFillLevel(CAN3, CAN_RX_FIFO0))
    {
        /* define data capture */
        CAN_RxHeaderTypeDef pHeader = (CAN_RxHeaderTypeDef){0};
        uint8_t aData[8] = {0};

        /* grab data from hardware rx fifo */
        HAL_CAN_GetRxMessage(CAN3, CAN_RX_FIFO0, &pHeader, aData);

        /* define queue to put data into */
        QueueHandle_t *queue = NULL;

        /* find listener (linear search) */
        for (int i = 0; i < NUM_LISTENERS; i++)
        {
            if (LocalListeners[i].id == pHeader.StdId)
            {
                /* If found, then set queue to put data into */
                queue = LocalListeners[i].queue;
            }
        }

        /* if listener found, put data into queue */
        if (queue)
        {
            /* define variables to send to queue */
            BaseType_t pxHigherPriorityTaskWoken = 0;

            /* send data to queue */
            xQueueSendFromISR(*queue, &aData, &pxHigherPriorityTaskWoken);
        }
    }

    /* enable interrupts */
    portEXIT_CRITICAL();
}

void CAN1_TX_IRQHandler(void)
{
    /* disable interrupts */
    portENTER_CRITICAL();

    /* TODO: acknowledge flag? */

    /* define speaker to send */
    Communicator_t speaker = {0};

    /* look for a speaker to put into mailbox (linear search) */
    for (int i = 0; i < NUM_SPEAKERS; i++)
    {
        if (CarSpeakers[i].id > speaker.id && !xQueueIsQueueEmptyFromISR(*speaker.queue)) /* greater ids have priority, only consider if queue is non-empty */
        {
            speaker = CarSpeakers[i];
        }
    }

    /* if viable speaker found, send the message to mailbox */
    if (speaker.queue)
    {
        /* define variables to recieve from queue */
        uint8_t aData[8] = {0};
        BaseType_t pxHigherPriorityTaskWoken = 0;

        /* recieve data from queue */
        xQueueReceiveFromISR(*speaker.queue, &aData, &pxHigherPriorityTaskWoken);

        /* define can message structs */
        CAN_TxHeaderTypeDef pHeader = (CAN_TxHeaderTypeDef){.StdId = speaker.id};
        uint32_t pTxMailbox = 0;

        /* send message to mailbox */
        HAL_CAN_AddTxMessage(CAN1, &pHeader, aData, &pTxMailbox);
    }

    /* enable interrupts */
    portEXIT_CRITICAL();
}

void CAN3_TX_IRQHandler(void)
{
    /* disable interrupts */
    portENTER_CRITICAL();

    /* TODO: acknowledge flag? */

    /* define speaker to send */
    Communicator_t speaker = {0};

    /* look for a speaker to put into mailbox (linear search) */
    for (int i = 0; i < NUM_SPEAKERS; i++)
    {
        if (CarSpeakers[i].id > speaker.id && !xQueueIsQueueEmptyFromISR(*speaker.queue)) /* greater ids have priority, only consider if queue is non-empty */
        {
            speaker = CarSpeakers[i];
        }
    }

    /* if viable speaker found, send the message to mailbox */
    if (speaker.queue)
    {
        /* define variables to recieve from queue */
        uint8_t aData[8] = {0};
        BaseType_t pxHigherPriorityTaskWoken = 0;

        /* recieve data from queue */
        xQueueReceiveFromISR(*speaker.queue, &aData, &pxHigherPriorityTaskWoken);

        /* define can message structs */
        CAN_TxHeaderTypeDef pHeader = (CAN_TxHeaderTypeDef){.StdId = speaker.id};
        uint32_t pTxMailbox = 0;

        /* send message to mailbox */
        HAL_CAN_AddTxMessage(CAN3, &pHeader, aData, &pTxMailbox);
    }

    /* enable interrupts */
    portEXIT_CRITICAL();
}
