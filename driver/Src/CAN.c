#include "stm32xx_hal.h"
#include "CAN.h"

#ifndef CAN1_SEND_QUEUE_SIZE
#define CAN1_SEND_QUEUE_SIZE (10)
#endif

#ifdef CAN1
static CAN_HandleTypeDef can1 = {.Instance = CAN1};
CAN_HandleTypeDef* can1_handle = &can1;
static SemaphoreHandle_t can1_send_semaphore = NULL;
static StaticSemaphore_t can1_send_semaphore_buffer;
static QueueHandle_t can1_send_queue = NULL;
static StaticQueue_t can1_send_queue_buffer;
static uint8_t can1_send_queue_storage[CAN1_SEND_QUEUE_SIZE * sizeof(payload_tx_t)];

static SemaphoreHandle_t can1_recv_semaphore = NULL;
static StaticSemaphore_t can1_recv_semaphore_buffer;

#define CAN_RECV_ENTRY(_ID, _SIZE) \
    static uint8_t queueStorage_##_ID[_SIZE * sizeof(payload_rx_t)]; \

#include "can_entries.h"

#undef CAN_RECV_ENTRY
#define CAN_RECV_ENTRY(_ID, _SIZE) \
    { \
        .ID = (_ID), \
        .size = (_SIZE), \
        .queue = NULL, \
        .queueStorage = queueStorage_##_ID, \
        .queueControlBlock = {{0}} \
    },

can_recv_entry_t can1_recv_entries[] = {
    #include "can_entries.h"
};
#undef CAN_RECV_ENTRY
const uint32_t can1_recv_entry_count = sizeof(can1_recv_entries) / sizeof(can1_recv_entries[0]);
#endif

static bool initialized = false;

// CAN MSP init
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan) {
    GPIO_InitTypeDef init = {0};

    if (hcan->Instance == CAN1) {
        // enable clocks
        __HAL_RCC_CAN1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* enable gpio
        PA11 -> CAN1_RX
        PA12 -> CAN1_TX
        */
        init.Pin = GPIO_PIN_11;
        init.Mode = GPIO_MODE_AF_PP;
        init.Pull = GPIO_PULLUP;
        init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        init.Alternate = GPIO_AF9_CAN1;
        HAL_GPIO_Init(GPIOA, &init);

        init.Pin = GPIO_PIN_12;
        init.Mode = GPIO_MODE_AF_PP;
        init.Pull = GPIO_NOPULL;
        init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        init.Alternate = GPIO_AF9_CAN1;
        HAL_GPIO_Init(GPIOA, &init);

        // enable interrupts
        HAL_NVIC_SetPriority(CAN1_TX_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
        HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    }
}

// CAN MSP deinit
void HAL_CAN_MspDeInit(CAN_HandleTypeDef *hcan) {
    if (hcan->Instance == CAN1) {
        // disable clocks
        __HAL_RCC_CAN1_CLK_DISABLE();

        /* disable gpio
        PA11 -> CAN1_RX
        PA12 -> CAN1_TX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11);
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);

        // disable interrupts
        HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
        HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    }
}

CAN_Status_e can_init(CAN_HandleTypeDef *handle, CAN_FilterTypeDef *filter) {
    // init semaphore and queues
    if (handle->Instance == CAN1) {
        can1_send_semaphore = xSemaphoreCreateBinaryStatic(
            &can1_send_semaphore_buffer);
        xSemaphoreGive(can1_send_semaphore);
        can1_recv_semaphore = xSemaphoreCreateBinaryStatic(
            &can1_recv_semaphore_buffer);
        xSemaphoreGive(can1_recv_semaphore);
        can1_send_queue = xQueueCreateStatic(
            CAN1_SEND_QUEUE_SIZE,
            sizeof(payload_tx_t),
            can1_send_queue_storage,
            &can1_send_queue_buffer);
        for (uint32_t i = 0; i < can1_recv_entry_count; i++) {
            can1_recv_entries[i].queue = xQueueCreateStatic(
                can1_recv_entries[i].size,
                sizeof(payload_rx_t),
                can1_recv_entries[i].queueStorage,
                &can1_recv_entries[i].queueControlBlock
            );
        }
    } else {
        return INVALID_INSTANCE;
    }


    // init HAL
    if (HAL_CAN_Init(handle) != HAL_OK) {
        return INIT;
    }

    // init filter
    if (HAL_CAN_ConfigFilter(handle, filter) != HAL_OK) {
        return CONFIG;
    }

    // enable interrupts
    if (HAL_CAN_ActivateNotification(handle, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
        return ACTIVATE_NOTIFICATION;
    }
    if (HAL_CAN_ActivateNotification(handle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        return ACTIVATE_NOTIFICATION;
    }

    // start CAN
    if (HAL_CAN_Start(handle) != HAL_OK) {
        return START;
    }

    initialized = true;
    return OK;
}

CAN_Status_e can_recv(CAN_HandleTypeDef *handle, uint32_t ID, payload_rx_t* payload, bool blocking) {
    if (!initialized) {
        return NOT_INITIALIZED;
    }

    int ret = OK;

    // determine timeout
    TickType_t timeout = (blocking) ? portMAX_DELAY : 0;

    // get semaphore
    if (handle->Instance == CAN1) {
        if (xSemaphoreTake(can1_recv_semaphore, timeout) != pdTRUE) {
            return SEMAPHORE_TAKE;
        }
    } else {
        return INVALID_INSTANCE;
    }
    
    // disable interrupts
    portENTER_CRITICAL();

    // something here
    int i;
    for (i = 0; i < can1_recv_entry_count; i++) {
        if (can1_recv_entries[i].ID == ID) {
            if (xQueueReceive(
                can1_recv_entries[i].queue,
                payload,
                timeout
            ) != pdTRUE) {
                ret = BAD;
            }
        }
    }
    
    // give semaphore
    if (handle->Instance == CAN1) {
        xSemaphoreGive(can1_recv_semaphore);
    } else {
        ret = INVALID_INSTANCE;
    }
    
    // enable interrupts
    portEXIT_CRITICAL();

    return ret;
}

CAN_Status_e can_send(CAN_HandleTypeDef *handle, const CAN_TxHeaderTypeDef *header, const uint8_t data[], bool blocking) {
    if (!initialized) {
        return NOT_INITIALIZED;
    }

    int ret = OK;

    // determine timeout
    TickType_t timeout = (blocking) ? portMAX_DELAY : 0;

    // get semaphore
    if (handle->Instance == CAN1) {
        if (xSemaphoreTake(can1_send_semaphore, timeout) != pdTRUE) {
            return SEMAPHORE_TAKE;
        }
    } else {
        return INVALID_INSTANCE;
    }
    
    // disable interrupts
    portENTER_CRITICAL();

    // if transmit is inactive, put payload into mailbox
    if (HAL_CAN_GetTxMailboxesFreeLevel(handle) >= 1) {
        uint32_t mailbox;
        if (HAL_CAN_AddTxMessage(handle, header, data, &mailbox) != HAL_OK) {
            ret = ADD_MESSAGE;
            goto failed;
        } else {
            ret = YESSS;
        }
    }
    // otherwise, put into queue
    else {
        payload_tx_t payload = {0};
        payload.header = *header;
        for (int i = 0; i < CAN_DATA_SIZE; i++) {
            payload.data[i] = data[i];
        }
        if (handle->Instance == CAN1) {
            if (xQueueSend(can1_send_queue, &payload, timeout) != pdTRUE) {
                ret = QUEUE_SEND;
                goto failed;
            }
        } else {
            ret = INVALID_INSTANCE;
            goto failed;
        }   
    }

failed:
    // give semaphore
    if (handle->Instance == CAN1) {
        xSemaphoreGive(can1_send_semaphore);
    } else {
        ret = INVALID_INSTANCE;
    }
    
    // enable interrupts
    portEXIT_CRITICAL();

    return ret;
}

void transmit_function(CAN_HandleTypeDef *hcan) {
    // create payload
    payload_tx_t payload = {0};

    // receive data from queue
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    if (hcan->Instance == CAN1) {
        if (xQueueReceiveFromISR(can1_send_queue, &payload, &higherPriorityTaskWoken) == pdTRUE) {
            // add payload to mailbox
            uint32_t mailbox;
            if (HAL_CAN_AddTxMessage(hcan, &payload.header, payload.data, &mailbox) != HAL_OK) {
                // Handle transmission error (optional: log or retry mechanism)
            }
        }
    }
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}


void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan) {
    if (hcan->Instance == CAN1) {
        transmit_function(hcan);
    }
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan) {
    if (hcan->Instance == CAN1) {
        transmit_function(hcan);
    }
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan) {
    if (hcan->Instance == CAN1) {
        transmit_function(hcan);
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    payload_rx_t payload = {0};

    while (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &payload.header, payload.data) == HAL_OK) {
        for (int i = 0; i < can1_recv_entry_count; i++) {
            if (can1_recv_entries[i].ID == payload.header.StdId) {
                xQueueSendFromISR(can1_recv_entries[i].queue, &payload, &higherPriorityTaskWoken);
            }
        }
    }
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void CAN1_TX_IRQHandler(void) {
    HAL_CAN_IRQHandler(&can1);
}

void CAN1_RX0_IRQHandler(void) {
    HAL_CAN_IRQHandler(&can1);
}

