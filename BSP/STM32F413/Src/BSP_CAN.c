#include "BSP_CAN.h"

/* handlers */
static CAN_HandleTypeDef CarCAN_ = (CAN_HandleTypeDef){0};
static CAN_HandleTypeDef LocalCAN_ = (CAN_HandleTypeDef){0};
const CAN_HandleTypeDef *CarCAN = &CarCAN_;
const CAN_HandleTypeDef *LocalCAN = &LocalCAN_;

/* communicators keep track of queues to use for communication */
typedef struct CAN_Listener
{
    uint32_t id;            /* can id */
    QueueHandle_t *queue;   /* queue to wait on */
} CAN_Listener_t;

#define NUM_LISTENERS 16

/* array of listeners */
static CAN_Listener_t CarCAN_Listeners[NUM_LISTENERS] = {0};
static CAN_Listener_t LocalCAN_Listeners[NUM_LISTENERS] = {0};

/* transmit queues */
QueueHandle_t CarCAN_TxQueue = (QueueHandle_t){0};
QueueHandle_t LocalCAN_TxQueue = (QueueHandle_t){0};

#define TX_QUEUE_LEN 32
#define TX_QUEUE_ITEM_SIZE sizeof(CAN_TxPayload_t)

/* queue storage */
uint8_t CarCAN_Payloads[TX_QUEUE_LEN * TX_QUEUE_ITEM_SIZE] = {0};
uint8_t LocalCAN_Payloads[TX_QUEUE_LEN * TX_QUEUE_ITEM_SIZE] = {0};

/* transmit queue buffers */
StaticQueue_t CarCAN_QueueBuffer = (StaticQueue_t){0};
StaticQueue_t LocalCAN_QueueBuffer = (StaticQueue_t){0};

void CAN_CarCANInit(void)
{
    /* init transmit queue */
    CarCAN_TxQueue = xQueueCreateStatic( 
                        TX_QUEUE_LEN,
                        TX_QUEUE_ITEM_SIZE,
                        CarCAN_Payloads,
                        &CarCAN_QueueBuffer);

    /* init struct */
    CarCAN_.Instance = CAN1;
    CarCAN_.Init.Prescaler = 16;
    CarCAN_.Init.Mode = CAN_MODE_NORMAL;
    CarCAN_.Init.SyncJumpWidth = CAN_SJW_1TQ;
    CarCAN_.Init.TimeSeg1 = CAN_BS1_1TQ;
    CarCAN_.Init.TimeSeg2 = CAN_BS2_1TQ;
    CarCAN_.Init.TimeTriggeredMode = DISABLE;
    CarCAN_.Init.AutoBusOff = DISABLE;
    CarCAN_.Init.AutoWakeUp = DISABLE;
    CarCAN_.Init.AutoRetransmission = DISABLE;
    CarCAN_.Init.ReceiveFifoLocked = DISABLE;
    CarCAN_.Init.TransmitFifoPriority = DISABLE;

    /* initialize can perph */
    if (HAL_CAN_Init(&CarCAN_) != HAL_OK)
    {
        Error_Handler();
    }

    /* start can perph */
    HAL_CAN_Start(&CarCAN_);
}

void CAN_LocalCANInit(void)
{
    /* init transmit queue */
    LocalCAN_TxQueue = xQueueCreateStatic( 
                            TX_QUEUE_LEN,
                            TX_QUEUE_ITEM_SIZE,
                            LocalCAN_Payloads,
                            &LocalCAN_QueueBuffer);

    /* init struct */
    LocalCAN_.Instance = CAN3;
    LocalCAN_.Init.Prescaler = 16;
    LocalCAN_.Init.Mode = CAN_MODE_NORMAL;
    LocalCAN_.Init.SyncJumpWidth = CAN_SJW_1TQ;
    LocalCAN_.Init.TimeSeg1 = CAN_BS1_1TQ;
    LocalCAN_.Init.TimeSeg2 = CAN_BS2_1TQ;
    LocalCAN_.Init.TimeTriggeredMode = DISABLE;
    LocalCAN_.Init.AutoBusOff = DISABLE;
    LocalCAN_.Init.AutoWakeUp = DISABLE;
    LocalCAN_.Init.AutoRetransmission = DISABLE;
    LocalCAN_.Init.ReceiveFifoLocked = DISABLE;
    LocalCAN_.Init.TransmitFifoPriority = DISABLE;

    /* initialize can perph */
    if (HAL_CAN_Init(&LocalCAN_) != HAL_OK)
    {
        Error_Handler();
    }

    /* start can perph */
    HAL_CAN_Start(&LocalCAN_);
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
        while (!xQueueSend(LocalCAN_TxQueue, &payload, 0)) {}
        ret = false;
    }
    else {
        /* add payload to queue and return status */
        if (!xQueueSend(LocalCAN_TxQueue, &payload, 0))
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
    CAN_Listener_t *listeners = (can->Instance == CAN1) ? CarCAN_Listeners : LocalCAN_Listeners;

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
    CAN_Listener_t *listeners = (can->Instance == CAN1) ? CarCAN_Listeners : LocalCAN_Listeners;

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
    while (HAL_CAN_GetRxFifoFillLevel(&CarCAN_, CAN_RX_FIFO0))
    {
        /* define data payload */
        CAN_RxPayload_t payload = {0};

        /* grab data from hardware rx fifo */
        HAL_CAN_GetRxMessage(&CarCAN_, CAN_RX_FIFO0, &payload.header, payload.data);

        /* define queue to put data into */
        QueueHandle_t *queue = NULL;

        /* find listener (linear search) */
        for (int i = 0; i < NUM_LISTENERS; i++)
        {
            if (CarCAN_Listeners[i].id == payload.header.StdId)
            {
                /* if found, then set queue to put data into */
                queue = CarCAN_Listeners[i].queue;
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
    while (HAL_CAN_GetRxFifoFillLevel(&LocalCAN_, CAN_RX_FIFO0))
    {
        /* define data payload */
        CAN_RxPayload_t payload = {0};

        /* grab data from hardware rx fifo */
        HAL_CAN_GetRxMessage(&LocalCAN_, CAN_RX_FIFO0, &payload.header, payload.data);

        /* define queue to put data into */
        QueueHandle_t *queue = NULL;

        /* find listener (linear search) */
        for (int i = 0; i < NUM_LISTENERS; i++)
        {
            if (LocalCAN_Listeners[i].id == payload.header.StdId)
            {
                /* if found, then set queue to put data into */
                queue = LocalCAN_Listeners[i].queue;
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
    if (!xQueueReceiveFromISR(CarCAN_TxQueue, &payload, &higherPriorityTaskWoken))
    {
        /* if payload to send, add payload to mailbox */
        HAL_CAN_AddTxMessage(&CarCAN_, &payload.header, payload.data, &payload.mailbox);
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
    if (!xQueueReceiveFromISR(LocalCAN_TxQueue, &payload, &higherPriorityTaskWoken))
    {
        /* if payload to send, add payload to mailbox */
        HAL_CAN_AddTxMessage(&LocalCAN_, &payload.header, payload.data, &payload.mailbox);
    }

    /* enable interrupts */
    portEXIT_CRITICAL();
}
