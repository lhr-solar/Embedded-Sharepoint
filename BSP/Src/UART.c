#include "stm32xx_hal.h"
#include "UART.h"


#define QUEUE_LENGTH 128 // Allow it to be overwritten
#define QUEUE_ITEM_SIZE sizeof(uint8_t)

static UART_HandleTypeDef huart4_ = {.Instance = UART4};
UART_HandleTypeDef* huart4 = &huart4_;

// Single TX queue
static StaticQueue_t tx_queue_buffer;
static uint8_t tx_queue_storage[QUEUE_LENGTH * QUEUE_ITEM_SIZE];
static QueueHandle_t tx_queue;

// RX queue (provided by user)
static QueueHandle_t* rx_queue;


static bool initialized = false;

uart_status_t uart_init(UART_HandleTypeDef* handle, QueueHandle_t* rxQueue) {
    if (handle->Instance != UART4) {
        return UART_ERR;
    }

    // Create TX queue
    tx_queue = xQueueCreateStatic(QUEUE_LENGTH, 
                                 QUEUE_ITEM_SIZE,
                                 tx_queue_storage, 
                                 &tx_queue_buffer);
    
    rx_queue = rxQueue;

    // Init UART hardware
    if (HAL_UART_Init(handle) != HAL_OK) {
        return UART_ERR;
    }

    // actually these should be done the msp
    // Setup interrupts 
    // HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
    // HAL_NVIC_EnableIRQ(UART4_IRQn);

    // Start first reception
    if (HAL_UART_Receive_IT(handle, (uint8_t*)handle->pRxBuffPtr, 1) != HAL_OK) {
        return UART_ERR;
    }

    initialized = true;
    return UART_OK;
}

uart_status_t uart_send(UART_HandleTypeDef* handle, const uint8_t* data, uint8_t length, bool blocking) {
    if (!initialized || !data || length == 0) {
        return UART_ERR;
    }

    TickType_t timeout = blocking ? portMAX_DELAY : 0;

    // Try direct transmission if possible
    if (HAL_UART_GetState(handle) == HAL_UART_STATE_READY && 
        uxQueueMessagesWaiting(tx_queue) == 0) {
        if (HAL_UART_Transmit_IT(handle, (uint8_t*)data, length) == HAL_OK) {
            return UART_SENT;
        }
        return UART_ERR;
    }

    // Otherwise queue the data
    for (uint8_t i = 0; i < length; i++) {
        if (xQueueSend(tx_queue, &data[i], timeout) != pdTRUE) {
            return UART_ERR;
        }
    }

    // Start transmission if needed
    if (HAL_UART_GetState(handle) != HAL_UART_STATE_BUSY_TX) {
        uint8_t byte;
        if (xQueueReceive(tx_queue, &byte, 0) == pdTRUE) {
            if (HAL_UART_Transmit_IT(handle, &byte, 1) != HAL_OK) {
                return UART_ERR;
            }
        }
    }

    return UART_SENT;
}

uart_status_t uart_recv(UART_HandleTypeDef* handle, uint8_t* data, uint8_t length, bool blocking) {
    if (!initialized || !data || length == 0 || !rx_queue) {
        return UART_ERR;
    }

    TickType_t timeout = blocking ? portMAX_DELAY : 0;

    // Read from queue
    for (uint8_t i = 0; i < length; i++) {
        if (xQueueReceive(*rx_queue, &data[i], timeout) != pdTRUE) {
            return blocking ? UART_ERR : UART_EMPTY;
        }
    }

    return UART_RECV;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    uint8_t nextByte;
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    if (xQueueReceiveFromISR(tx_queue, &nextByte, &higherPriorityTaskWoken) == pdTRUE) {
        HAL_UART_Transmit_IT(huart, &nextByte, 1);
    }
    
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    uint8_t receivedByte = huart->pRxBuffPtr[0];
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    xQueueSendFromISR(*rx_queue, &receivedByte, &higherPriorityTaskWoken);
    HAL_UART_Receive_IT(huart, (uint8_t*)huart->pRxBuffPtr, 1);
    
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void UART4_IRQHandler(void) {
    HAL_UART_IRQHandler(huart4);
}