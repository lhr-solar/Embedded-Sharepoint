#include "BSP_CAN.h"

/* handlers */
static CAN_HandleTypeDef _CarCAN = (CAN_HandleTypeDef){0};
static CAN_HandleTypeDef _LocalCAN = (CAN_HandleTypeDef){0};
const CAN_HandleTypeDef *CarCAN = &_CarCAN;
const CAN_HandleTypeDef *LocalCAN = &_LocalCAN;

/* communicators keep track of queues to use for communication */
typedef struct CAN_Listener
{
    uint32_t id;
    QueueHandle_t *queue;
} CAN_Listener_t;

#define NUM_LISTENERS 16

/* array of listeners */
static CAN_Listener_t CarListeners[NUM_LISTENERS] = {0};
static CAN_Listener_t LocalListeners[NUM_LISTENERS] = {0};

/* transmit queues */
QueueHandle_t CarCANTxQueue = (QueueHandle_t){0};
QueueHandle_t LocalCANTxQueue = (QueueHandle_t){0};

#define TX_QUEUE_LEN 32
#define TX_QUEUE_ITEM_SIZE sizeof(CAN_TxPayload_t)

/* queue storage */
uint8_t CarCANPayloads[TX_QUEUE_LEN * TX_QUEUE_ITEM_SIZE] = {0};
uint8_t LocalCANPayloads[TX_QUEUE_LEN * TX_QUEUE_ITEM_SIZE] = {0};

/* transmit queue buffers */
StaticQueue_t CarCANQueueBuffer = (StaticQueue_t){0};
StaticQueue_t LocalCANQueueBuffer = (StaticQueue_t){0};

void CAN_CarCANInit(void)
{
    /* init transmit queue */
    CarCANTxQueue = xQueueCreateStatic( 
                        TX_QUEUE_LEN,
                        TX_QUEUE_ITEM_SIZE,
                        CarCANPayloads,
                        &CarCANQueueBuffer);

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
    /* init transmit queue */
    LocalCANTxQueue = xQueueCreateStatic( 
                            TX_QUEUE_LEN,
                            TX_QUEUE_ITEM_SIZE,
                            LocalCANPayloads,
                            &LocalCANQueueBuffer);

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

bool CAN_Send(const CAN_HandleTypeDef *can, CAN_TxPayload_t payload, bool blocking)
{
    /* return status */
    int ret = true;

    /* TODO: error check payload */

    if (blocking) {
        /* add payload to queue and wait till a success */
        while (!xQueueSend(LocalCANTxQueue, &payload, 0)) {}
        ret = false;
    }
    else {
        /* add payload to queue and return status */
        if (!xQueueSend(LocalCANTxQueue, &payload, 0))
        {
            ret = false;
        }
    }

    /* TODO: if can tx is inactive, put payload into mailbox */

    return ret;
}

bool CAN_SetListener(const CAN_HandleTypeDef *can, CANID_t id, QueueHandle_t *queue)
{
    /* set listeners depending on bus */
    CAN_Listener_t *listeners = (can->Instance == CAN1) ? CarListeners : LocalListeners;

    /* find if listener is already set (linear search) */
    for (int i = 0; i < NUM_LISTENERS; i++)
    {
        if (listeners[i].id == id)
        {
            /* replace if found (don't want duplicate ids) */
            listeners[i].queue = queue;
            return false;
        }
    }

    /* find empty listener spot (linear search) */
    for (int i = 0; i < NUM_LISTENERS; i++)
    {
        if (listeners[i].queue == NULL)
        {
            /* set if found */
            listeners[i] = (CAN_Listener_t){.id = id, .queue = queue};
            return false;
        }
    }

    return true;
}

void CAN_ClearListener(const CAN_HandleTypeDef *can, CANID_t id)
{
    /* set listeners depending on bus */
    CAN_Listener_t *listeners = (can->Instance == CAN1) ? CarListeners : LocalListeners;

    /* clear listener if present */
    for (int i = 0; i < NUM_LISTENERS; i++)
    {
        if (listeners[i].id == id)
        {
            listeners[i] = (CAN_Listener_t){0};
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
        /* define data payload */
        CAN_RxPayload_t payload = {0};

        /* grab data from hardware rx fifo */
        HAL_CAN_GetRxMessage(&_CarCAN, CAN_RX_FIFO0, &payload.header, payload.data);

        /* define queue to put data into */
        QueueHandle_t *queue = NULL;

        /* find listener (linear search) */
        for (int i = 0; i < NUM_LISTENERS; i++)
        {
            if (CarListeners[i].id == payload.header.StdId)
            {
                /* if found, then set queue to put data into */
                queue = CarListeners[i].queue;
            }
        }

        /* if listener found, put data into queue */
        if (queue)
        {
            /* define variables to send to queue */
            BaseType_t pxHigherPriorityTaskWoken = 0;

            /* send data to queue */
            xQueueSendFromISR(*queue, &payload, &pxHigherPriorityTaskWoken);
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
        /* define data payload */
        CAN_RxPayload_t payload = {0};

        /* grab data from hardware rx fifo */
        HAL_CAN_GetRxMessage(&_LocalCAN, CAN_RX_FIFO0, &payload.header, payload.data);

        /* define queue to put data into */
        QueueHandle_t *queue = NULL;

        /* find listener (linear search) */
        for (int i = 0; i < NUM_LISTENERS; i++)
        {
            if (LocalListeners[i].id == payload.header.StdId)
            {
                /* if found, then set queue to put data into */
                queue = LocalListeners[i].queue;
            }
        }

        /* if listener found, put data into queue */
        if (queue)
        {
            /* define variables to send to queue */
            BaseType_t pxHigherPriorityTaskWoken = 0;

            /* send data to queue */
            xQueueSendFromISR(*queue, &payload, &pxHigherPriorityTaskWoken);
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

    /* create payload */
    CAN_TxPayload_t payload = {0};
    BaseType_t higherPriorityTaskWoken = 0;

    /* recieve data from queue */
    if (!xQueueReceiveFromISR(CarCANTxQueue, &payload, &higherPriorityTaskWoken))
    {
        /* if payload to send, add payload to mailbox */
        HAL_CAN_AddTxMessage(&_CarCAN, &payload.header, payload.data, &payload.mailbox);
    }

    /* enable interrupts */
    portEXIT_CRITICAL();
}

void CAN3_TX_IRQHandler(void)
{
    /* disable interrupts */
    portENTER_CRITICAL();

    /* TODO: acknowledge flag? */

    /* create payload */
    CAN_TxPayload_t payload = {0};
    BaseType_t higherPriorityTaskWoken = 0;

    /* recieve data from queue */
    if (!xQueueReceiveFromISR(LocalCANTxQueue, &payload, &higherPriorityTaskWoken))
    {
        /* if payload to send, add payload to mailbox */
        HAL_CAN_AddTxMessage(&_LocalCAN, &payload.header, payload.data, &payload.mailbox);
    }

    /* enable interrupts */
    portEXIT_CRITICAL();
}
