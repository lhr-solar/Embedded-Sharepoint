#include "stm32xx_hal.h" // agnostic to L4 or F4
#include "UART.h"


// Default queue sizes if not defined
#ifndef UART4_TX_QUEUE_SIZE
#define UART4_TX_QUEUE_SIZE 128
#endif

// I don't think a payload is need for UART to the extent that CAN does
// Only metadata needed for UART would be the busID if we use multiple UART buses
// Would likely need to account for busIDs in the future, but for now ignored

// #ifdef UART4 
// UART handle
static UART_HandleTypeDef huart4_ = {.Instance = UART4};
UART_HandleTypeDef* huart4 = &huart4_;

// UART4 send semaphore
static SemaphoreHandle_t uart4_send_semaphore = NULL; // semaphore handle initialized to NULL
static StaticSemaphore_t uart4_send_semaphore_buffer; // semaphore needs a buffer to be created

// UART4 receive semaphore
static SemaphoreHandle_t uart4_recv_semaphore = NULL;
static StaticSemaphore_t uart4_recv_semaphore_buffer;


// UART4 TX queue
static QueueHandle_t uart4_tx_queue = NULL;
static StaticQueue_t uart4_tx_queue_buffer;
static uint8_t uart4_tx_queue_storage[UART4_TX_QUEUE_SIZE];


// User-provided RX queue 
static QueueHandle_t* uart4_rx_queue;
// #endif /* UART4 */

static bool initialized = false;

// HAL UART MSP init 
void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if(huart->Instance == UART4) {
        __HAL_RCC_UART4_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        // UART4 GPIO Configuration    
        // PA0     ------> UART4_TX
        // PA1     ------> UART4_RX
        GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(UART4_IRQn, 5, 0); // set at priority 5
        HAL_NVIC_EnableIRQ(UART4_IRQn);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART4) {
        // disable clocks
        __HAL_RCC_UART4_CLK_DISABLE();

        // disable gpio
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1);

        // disable interrupts
        HAL_NVIC_DisableIRQ(UART4_IRQn);
    }
}


uart_status_t uart_init(UART_HandleTypeDef* handle, QueueHandle_t* rxQueue) {
    if (handle->Instance != UART4 || !rxQueue) { // if not UART4 or rxQueue is NULL
        return UART_ERR;
    }

    // Set the user-provided RX queue
    uart4_rx_queue = rxQueue;

    // Create semaphores
    uart4_send_semaphore = xSemaphoreCreateBinaryStatic(&uart4_send_semaphore_buffer); // Create the send semaphore
    xSemaphoreGive(uart4_send_semaphore); // Give the semaphore to start with 1
    uart4_recv_semaphore = xSemaphoreCreateBinaryStatic(&uart4_recv_semaphore_buffer); // Create the receive semaphore
    xSemaphoreGive(uart4_recv_semaphore); // Give the semaphore to start with 1

    // Create TX queue
    uart4_tx_queue = xQueueCreateStatic(UART4_TX_QUEUE_SIZE, 
                                        sizeof(uint8_t), 
                                        uart4_tx_queue_storage, 
                                        &uart4_tx_queue_buffer);
    
    // Init UART hardware
    if (HAL_UART_Init(handle) != HAL_OK) {
        return UART_ERR;
    }
    // Start first reception
    if (HAL_UART_Receive_IT(handle, (uint8_t*)handle->pRxBuffPtr, 1) != HAL_OK) {
        return UART_ERR;
    }

    initialized = true;
    return UART_OK;
}

/**
 * @brief Transmits data over UART. If transmission is in progress, data will be queued in internal TX queue. 
 * @param data pointer to the data buffer that will be written/transmitted
 * @param length of the buffer that will be written/transmitted
 * @param blocking if true, function will block until data is sent
 * @return uart_status_t
 */
uart_status_t uart_send(UART_HandleTypeDef* handle, const uint8_t* data, uint8_t length, bool blocking) {
    if (!initialized || !data || length == 0) {
        return UART_ERR;
    }

    // determine timeout
    TickType_t timeout = blocking ? portMAX_DELAY : 0;  // Set timeout to max delay if blocking, 0 if non-blocking

    // get semamphore for sending
    if (handle->Instance == UART4) {
        if (xSemaphoreTake(uart4_send_semaphore, timeout) != pdTRUE) { // if semaphore cannot be taken, return error
            return UART_ERR;
        }
    } else {
        return UART_ERR;
    }

    // disable inerrupts as we are in a critical section
    portENTER_CRITICAL(); // Enter critical section

    uart_status_t status = UART_SENT;

    // Try direct transmission if possible 
    if (HAL_UART_GetState(handle) == HAL_UART_STATE_READY && 
        uxQueueMessagesWaiting(uart4_tx_queue) == 0) {
        if (HAL_UART_Transmit_IT(handle, (uint8_t*)data, length) != HAL_OK) {
            status = UART_ERR;
        }
        goto exit;
    }

    // Queue the data
    for (uint8_t i = 0; i < length; i++) {
        if (xQueueSend(uart4_tx_queue, &data[i], timeout) != pdTRUE) { // if data cannot be sent to the queue, return error and exit
            status = UART_ERR;
            goto exit;
        }
    }

    // Start transmission if needed
    if (HAL_UART_GetState(handle) != HAL_UART_STATE_BUSY_TX) {
        uint8_t byte;
        if (xQueueReceive(uart4_tx_queue, &byte, 0) == pdTRUE) {
            if (HAL_UART_Transmit_IT(handle, &byte, 1) != HAL_OK) { // if transmission cannot be started, return error
                status = UART_ERR;
                goto exit; 
            }
        }
    }

exit:
    // give semaphore 
    if (handle->Instance == UART4) {
        xSemaphoreGive(uart4_send_semaphore); // Give the semaphore back
    }

    // enable interrupts
    portEXIT_CRITICAL(); // Exit critical section

    return status;
}

/**
 * @brief Reads data from the user-provided RX queue
 * @param huart pointer to the UART handle
 * @param rxQueue pointer to the receive queue
 * @return uart_status_t
 */
uart_status_t uart_recv(UART_HandleTypeDef* handle, uint8_t* data, uint8_t length, bool blocking) {
    if (!initialized || !data || length == 0 || handle->Instance != UART4 || !uart4_rx_queue) { 
        return UART_ERR;
    }

    TickType_t timeout = blocking ? portMAX_DELAY : 0;

    uart_status_t status = UART_RECV;

    // Read from queue
    for (uint8_t i = 0; i < length; i++) {
        if (xQueueReceive(*uart4_rx_queue, &data[i], timeout) != pdTRUE) { // if data cannot be received from the queue, then return status
            status = blocking ? UART_ERR : UART_EMPTY; // if blocking, return error, else return empty
            return status;
        }
    }

    return status;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if(huart->Instance != UART4) {
        return;
    }

    uint8_t nextByte;
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    if (xQueueReceiveFromISR(uart4_tx_queue, &nextByte, &higherPriorityTaskWoken) == pdTRUE) {
        HAL_UART_Transmit_IT(huart, &nextByte, 1);
    }
    
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    // basic check
    if(huart->Instance != UART4 || !uart4_rx_queue) { // if not UART4 or rxQueue is NULL
        return;
    }

    uint8_t receivedByte = huart->pRxBuffPtr[0];
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    xQueueSendFromISR(*uart4_rx_queue, &receivedByte, &higherPriorityTaskWoken);
    HAL_UART_Receive_IT(huart, (uint8_t*)huart->pRxBuffPtr, 1);
    
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void UART4_IRQHandler(void) {
    HAL_UART_IRQHandler(huart4);
}