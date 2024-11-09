#include "stm32xx_hal.h"
#include "CAN.h"

// 8 for now unless extended payload is supported
#define DATA_SIZE (8)

// entries in queues
typedef struct {
    CAN_TxHeaderTypeDef header;
    uint8_t data[DATA_SIZE];
} tx_payload_t;

typedef struct {
    CAN_RxHeaderTypeDef header;
    uint8_t data[DATA_SIZE];
} rx_payload_t;

// metadata for recieve queues
typedef struct {
    uint16_t id;
    uint16_t size;
    QueueHandle_t queue;
    uint8_t* storage;
    StaticQueue_t buffer;
} recv_entry_t;

#ifdef CAN1
// fallback send queue size
#ifndef CAN1_SEND_QUEUE_SIZE
#define CAN1_SEND_QUEUE_SIZE (10)
#endif

// can1 handle
static CAN_HandleTypeDef hcan1_ = {.Instance = CAN1};
CAN_HandleTypeDef* hcan1 = &hcan1_;

// can1 send semaphore
static SemaphoreHandle_t can1_send_semaphore = NULL;
static StaticSemaphore_t can1_send_semaphore_buffer;

// can1 send queue
static QueueHandle_t can1_send_queue = NULL;
static StaticQueue_t can1_send_queue_buffer;
static uint8_t can1_send_queue_storage[CAN1_SEND_QUEUE_SIZE * sizeof(tx_payload_t)];

// can1 recv semaphore
static SemaphoreHandle_t can1_recv_semaphore = NULL;
static StaticSemaphore_t can1_recv_semaphore_buffer;

// create recv queue storage
#define CAN_RECV_ENTRY(ID_, SIZE_) \
    static uint8_t recv_queue_storage_##ID_[SIZE_ * sizeof(rx_payload_t)]; \

#include "can_entries.h"

#undef CAN_RECV_ENTRY

// create recv queue array
#define CAN_RECV_ENTRY(ID_, SIZE_) \
    { \
        .id = (ID_), \
        .size = (SIZE_), \
        .queue = NULL, \
        .storage = recv_queue_storage_##ID_, \
        .buffer = {{0}} \
    },

static recv_entry_t can1_recv_entries[] = {
    #include "can_entries.h"
};
#undef CAN_RECV_ENTRY

// calculate amount of can1 recv entries
static const uint32_t can1_recv_entry_count = sizeof(can1_recv_entries) / sizeof(can1_recv_entries[0]);
#endif

// safety check send/recv
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

can_status_t can_init(CAN_HandleTypeDef* handle, CAN_FilterTypeDef* filter) {
    if (handle->Instance == CAN1) {
        // init semaphores
        can1_send_semaphore = xSemaphoreCreateBinaryStatic(
            &can1_send_semaphore_buffer);
        xSemaphoreGive(can1_send_semaphore);
        can1_recv_semaphore = xSemaphoreCreateBinaryStatic(
            &can1_recv_semaphore_buffer);
        xSemaphoreGive(can1_recv_semaphore);

        // init queues
        can1_send_queue = xQueueCreateStatic(
            CAN1_SEND_QUEUE_SIZE,
            sizeof(tx_payload_t),
            can1_send_queue_storage,
            &can1_send_queue_buffer);
        for (int i = 0; i < can1_recv_entry_count; i++) {
            can1_recv_entries[i].queue = xQueueCreateStatic(
                can1_recv_entries[i].size,
                sizeof(rx_payload_t),
                can1_recv_entries[i].storage,
                &can1_recv_entries[i].buffer
            );
        }
    } else {
        return CAN_ERR;
    }

    // init HAL
    if (HAL_CAN_Init(handle) != HAL_OK) {
        return CAN_ERR;
    }

    // init filter
    if (HAL_CAN_ConfigFilter(handle, filter) != HAL_OK) {
        return CAN_ERR;
    }

    // enable interrupts
    if (HAL_CAN_ActivateNotification(handle, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
        return CAN_ERR;
    }
    if (HAL_CAN_ActivateNotification(handle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        return CAN_ERR;
    }

    // start CAN
    if (HAL_CAN_Start(handle) != HAL_OK) {
        return CAN_ERR;
    }

    initialized = true;
    return CAN_OK;
}

can_status_t can_recv(CAN_HandleTypeDef* handle, uint16_t id, CAN_RxHeaderTypeDef* header, uint8_t data[], bool blocking) {
    if (!initialized) {
        return CAN_ERR;
    }

    // determine timeout
    TickType_t timeout = (blocking) ? portMAX_DELAY : 0;

    // get semaphore
    if (handle->Instance == CAN1) {
        if (xSemaphoreTake(can1_recv_semaphore, timeout) != pdTRUE) {
            return CAN_ERR;
        }
    } else {
        return CAN_ERR;
    }

    // disable interrupts (don't want to CAN_RX to interrupt here)
    portENTER_CRITICAL();

    can_status_t status = CAN_RECV;

    // recieve from queue matching id
    rx_payload_t payload = {0};
    bool valid_id = false;
    for (int i = 0; i < can1_recv_entry_count; i++) {
        if (can1_recv_entries[i].id == id) {
            valid_id = true;
            // if blocking, retry on empty
            if (blocking) {
                while (xQueueReceive(can1_recv_entries[i].queue, &payload, 0) == errQUEUE_EMPTY) {
                    // quickly enable interrupts so maybe next time we can hit
                    portEXIT_CRITICAL();
                    portENTER_CRITICAL();
                }
            
            // otherwise, fail on empty
            } else {
                if (xQueueReceive(
                    can1_recv_entries[i].queue, &payload, 0) == errQUEUE_EMPTY) {
                    status = CAN_EMPTY;
                    goto failed;
                }
            }
        }
    }

    // decode payload if if is valid and message recieved
    if (valid_id) {
        *header = payload.header;
        for (int i = 0; i < DATA_SIZE; i++) {
            data[i] = payload.data[i];
        }
    } else { 
        status = CAN_ERR;
        goto failed;
    }

failed:
    // give semaphore
    if (handle->Instance == CAN1) {
        xSemaphoreGive(can1_recv_semaphore);
    }
    
    // enable interrupts
    portEXIT_CRITICAL();

    return status;
}

can_status_t can_send(CAN_HandleTypeDef* handle, const CAN_TxHeaderTypeDef* header, const uint8_t data[], bool blocking) {
    if (!initialized) {
        return CAN_ERR;
    }

    // determine timeout
    TickType_t timeout = (blocking) ? portMAX_DELAY : 0;

    // get semaphore
    if (handle->Instance == CAN1) {
        if (xSemaphoreTake(can1_send_semaphore, timeout) != pdTRUE) {
            return CAN_ERR;
        }
    } else {
        return CAN_ERR;
    }
    
    // disable interrupts (don't want CAN_TX to interrupt here)
    portENTER_CRITICAL();

    can_status_t status = CAN_SENT;

    // if transmit is inactive, put payload into mailbox
    if (HAL_CAN_GetTxMailboxesFreeLevel(handle) >= 1) {
        uint32_t mailbox;
        if (HAL_CAN_AddTxMessage(handle, header, data, &mailbox) != HAL_OK) {
            status = CAN_ERR;
            goto failed;
        }
    }

    // otherwise, put into send queue
    else {
        tx_payload_t payload = {0};
        payload.header = *header;
        for (int i = 0; i < DATA_SIZE; i++) {
            payload.data[i] = data[i];
        }
        if (handle->Instance == CAN1) {
            if (xQueueSend(can1_send_queue, &payload, timeout) != pdTRUE) {
                status = CAN_ERR;
                goto failed;
            }
        }
    }

failed:
    // give semaphore
    if (handle->Instance == CAN1) {
        xSemaphoreGive(can1_send_semaphore);
    }
    
    // enable interrupts
    portEXIT_CRITICAL();

    return status;
}

static void transmit(CAN_HandleTypeDef* handle) {
    tx_payload_t payload = {0};
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    // receive data from send queue
    if (handle->Instance == CAN1) {
        if (xQueueReceiveFromISR(can1_send_queue, &payload, &higherPriorityTaskWoken) == pdTRUE) {
            // add payload to mailbox
            uint32_t mailbox;
            if (HAL_CAN_AddTxMessage(handle, &payload.header, payload.data, &mailbox) != HAL_OK) {
                // Handle transmission error (optional: log or retry mechanism)
                // treated as lost packet for now
            }
        }
    }
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}


void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan) {
    if (hcan->Instance == CAN1) {
        transmit(hcan);
    }
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan) {
    if (hcan->Instance == CAN1) {
        transmit(hcan);
    }
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan) {
    if (hcan->Instance == CAN1) {
        transmit(hcan);
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    rx_payload_t payload = {0};
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    // recieve messages from queue till empty and put into recieve queues
    while (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &payload.header, payload.data) == HAL_OK) {
        for (int i = 0; i < can1_recv_entry_count; i++) {
            if (can1_recv_entries[i].id == payload.header.StdId) {
                xQueueSendFromISR(can1_recv_entries[i].queue, &payload, &higherPriorityTaskWoken);
            }
        }
    }
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void CAN1_TX_IRQHandler(void) {
    HAL_CAN_IRQHandler(hcan1);
}

void CAN1_RX0_IRQHandler(void) {
    HAL_CAN_IRQHandler(hcan1);
}

