#include "BSP_CAN.h"

#define NUM_LISTENERS 16
#define NUM_SPEAKERS 16

/* handlers */
static CAN_HandleTypeDef _CarCAN = (CAN_HandleTypeDef){0};
static CAN_HandleTypeDef _LocalCAN = (CAN_HandleTypeDef){0};
const CAN_HandleTypeDef *CarCAN = &_CarCAN;
const CAN_HandleTypeDef *LocalCAN = &_LocalCAN;

/* array of communicators */
static Communicator_t CarListeners[NUM_LISTENERS] = {0};
static Communicator_t LocalListeners[NUM_LISTENERS] = {0};
static Communicator_t CarSpeakers[NUM_SPEAKERS] = {0};
static Communicator_t LocalSpeakers[NUM_SPEAKERS] = {0};

void CAN_CarCANInit(void)
{
    /* init struct */
    _CarCAN.Instance = CAN1;
    _CarCAN.Init.Prescaler = 16;
    _CarCAN.Init.Mode = CAN_MODE_NORMAL;
    _CarCAN.Init.SyncJumpWidth = CAN_SJW_1TQ;
    _CarCAN.Init.TimeSeg1 = CAN_BS1_1TQ;
    _CarCAN.Init.TimeSeg2 = CAN_BS2_1TQ;
    _CarCAN.Init.TimeTriggeredMode = DISABLE;
    _CarCAN.Init.AutoBusOff = DISABLE;
    _CarCAN.Init.AutoWakeUp = DISABLE;
    _CarCAN.Init.AutoRetransmission = DISABLE;
    _CarCAN.Init.ReceiveFifoLocked = DISABLE;
    _CarCAN.Init.TransmitFifoPriority = DISABLE;

    /* initialize can perph */
    if (HAL_CAN_Init(&_CarCAN) != HAL_OK)
    {
        Error_Handler();
    }

    /* start can perph */
    HAL_CAN_Start(&_CarCAN);
}

void CAN_LocalCANInit(void)
{
    /* init struct */
    _LocalCAN.Instance = CAN3;
    _LocalCAN.Init.Prescaler = 16;
    _LocalCAN.Init.Mode = CAN_MODE_NORMAL;
    _LocalCAN.Init.SyncJumpWidth = CAN_SJW_1TQ;
    _LocalCAN.Init.TimeSeg1 = CAN_BS1_1TQ;
    _LocalCAN.Init.TimeSeg2 = CAN_BS2_1TQ;
    _LocalCAN.Init.TimeTriggeredMode = DISABLE;
    _LocalCAN.Init.AutoBusOff = DISABLE;
    _LocalCAN.Init.AutoWakeUp = DISABLE;
    _LocalCAN.Init.AutoRetransmission = DISABLE;
    _LocalCAN.Init.ReceiveFifoLocked = DISABLE;
    _LocalCAN.Init.TransmitFifoPriority = DISABLE;

    /* initialize can perph */
    if (HAL_CAN_Init(&_LocalCAN) != HAL_OK)
    {
        Error_Handler();
    }

    /* start can perph */
    HAL_CAN_Start(&_LocalCAN);
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *canHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = (GPIO_InitTypeDef){0};

    if (canHandle->Instance == CAN1)
    {

        /* CAN1 clock enable */
        __HAL_RCC_CAN1_CLK_ENABLE();

        /* GPIOA clock enable */
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /**CAN1 GPIO Configuration
        PA11     ------> CAN1_RX
        PA12     ------> CAN1_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
    else if (canHandle->Instance == CAN3)
    {
        /* CAN3 clock enable */
        __HAL_RCC_CAN3_CLK_ENABLE();

        /* GPIOA clock enable */
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /**CAN3 GPIO Configuration
        PA8     ------> CAN3_RX
        PA15     ------> CAN3_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF11_CAN3;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef *canHandle)
{
    if (canHandle->Instance == CAN1)
    {
        /* CAN1 clock disable */
        __HAL_RCC_CAN1_CLK_DISABLE();

        /**CAN1 GPIO Configuration
        PA11     ------> CAN1_RX
        PA12     ------> CAN1_TX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);
    }
    else if (canHandle->Instance == CAN3)
    {
        /* CAN3 clock disable */
        __HAL_RCC_CAN3_CLK_DISABLE();

        /**CAN3 GPIO Configuration
        PA8     ------> CAN3_RX
        PA15     ------> CAN3_TX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8 | GPIO_PIN_15);
    }
}

void CAN_SetSpeaker(const CAN_HandleTypeDef *canHandle, CANID_t id, QueueHandle_t *queue)
{
    /* set speakers depending on bus */
    Communicator_t *speakers = (canHandle->Instance == CAN1) ? CarSpeakers : LocalSpeakers;

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

void CAN_ClearSpeaker(const CAN_HandleTypeDef *canHandle, CANID_t id)
{
    /* set speakers depending on bus */
    Communicator_t *speakers = (canHandle->Instance == CAN1) ? CarSpeakers : LocalSpeakers;

    /* clear speaker if present */
    for (int i = 0; i < NUM_SPEAKERS; i++)
    {
        if (speakers[i].id == id)
        {
            speakers[i] = (Communicator_t){0};
        }
    }
}

void CAN_SetListener(const CAN_HandleTypeDef *canHandle, CANID_t id, QueueHandle_t *queue)
{
    /* set listeners depending on bus */
    Communicator_t *listeners = (canHandle->Instance == CAN1) ? CarListeners : LocalListeners;

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

void CAN_ClearListener(const CAN_HandleTypeDef *canHandle, CANID_t id)
{
    /* set listeners depending on bus */
    Communicator_t *listeners = (canHandle->Instance == CAN1) ? CarListeners : LocalListeners;

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
    while (HAL_CAN_GetRxFifoFillLevel(&_CarCAN, CAN_RX_FIFO0))
    {
        /* define data capture */
        CAN_RxHeaderTypeDef pHeader = (CAN_RxHeaderTypeDef){0};
        uint8_t aData[8] = {0};

        /* grab data from hardware rx fifo */
        HAL_CAN_GetRxMessage(&_CarCAN, CAN_RX_FIFO0, &pHeader, aData);

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
    while (HAL_CAN_GetRxFifoFillLevel(&_LocalCAN, CAN_RX_FIFO0))
    {
        /* define data capture */
        CAN_RxHeaderTypeDef pHeader = (CAN_RxHeaderTypeDef){0};
        uint8_t aData[8] = {0};

        /* grab data from hardware rx fifo */
        HAL_CAN_GetRxMessage(&_LocalCAN, CAN_RX_FIFO0, &pHeader, aData);

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
        HAL_CAN_AddTxMessage(&_CarCAN, &pHeader, aData, &pTxMailbox);
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
        HAL_CAN_AddTxMessage(&_LocalCAN, &pHeader, aData, &pTxMailbox);
    }

    /* enable interrupts */
    portEXIT_CRITICAL();
}
