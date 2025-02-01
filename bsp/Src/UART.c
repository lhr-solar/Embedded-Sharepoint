#include "stm32xx_hal.h" // agnostic to L4 or F4
#include "UART.h"

// Define the size of the data to be transmitted
// Currently not used, as we send uint8_t directly
// may need to be configured for support for packets less more than 8 bits
#define DATA_SIZE (1) // 1 byte of data
typedef struct {
    uint8_t data[DATA_SIZE]; // data to be transmitted, 1 byte
} tx_payload_t;

typedef struct {
    uint8_t data[DATA_SIZE]; // data received, 1 byte
} rx_payload_t;



#ifdef UART4
#ifndef UART4_TX_QUEUE_SIZE
#define UART4_TX_QUEUE_SIZE 128
#endif

//UART4 handle
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

#endif /* UART4 */


#ifdef UART5
#ifndef UART5_TX_QUEUE_SIZE
#define UART5_TX_QUEUE_SIZE 128
#endif

// UART5 handle
static UART_HandleTypeDef huart5_ = {.Instance = UART5};
UART_HandleTypeDef* huart5 = &huart5_;

// UART5 send semaphore
static SemaphoreHandle_t uart5_send_semaphore = NULL;
static StaticSemaphore_t uart5_send_semaphore_buffer;

// UART5 receive semaphore
static SemaphoreHandle_t uart5_recv_semaphore = NULL;
static StaticSemaphore_t uart5_recv_semaphore_buffer;

// UART5 TX queue
static QueueHandle_t uart5_tx_queue = NULL;
static StaticQueue_t uart5_tx_queue_buffer;
static uint8_t uart5_tx_queue_storage[UART5_TX_QUEUE_SIZE];

// User-provided RX queue pointer
static QueueHandle_t* uart5_rx_queue;
#endif /* UART5 */


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
    #ifdef UART5
    else if (huart->Instance == UART5) {
        __HAL_RCC_UART5_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();

        // UART5 GPIO Configuration    
        // PC12     ------> UART5_TX
        // PD2     ------> UART5_RX
        GPIO_InitStruct.Pin = GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_2;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(UART5_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(UART5_IRQn);
    }
    #endif /* UART5 */
}


void HAL_UART_MspDeInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART4) {
        __HAL_RCC_UART4_CLK_DISABLE();                  // disable clocks
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1);  // disable gpio
        HAL_NVIC_DisableIRQ(UART4_IRQn);                // disable interrupts
    }
    #ifdef UART5
    else if (huart->Instance == UART5) {
        __HAL_RCC_UART5_CLK_DISABLE();          // disable clocks
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);    // disable gpio C12
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);     //disable gpio D2
        HAL_NVIC_DisableIRQ(UART5_IRQn);        //disable interrupts
    }
    #endif /* UART5 */
}

/**
 * @brief Initializes the UART peripheral
 * @param handle pointer to the UART handle
 * @param rxQueue pointer to the user-provided RX queue
 * @return uart_status_t
 */
uart_status_t uart_init(UART_HandleTypeDef* handle, QueueHandle_t* rxQueue) {
    if (!rxQueue) {
        return UART_ERR;
    }

    // UART4
    if (handle->Instance == UART4) {
        uart4_rx_queue = rxQueue;

        uart4_send_semaphore = xSemaphoreCreateBinaryStatic(&uart4_send_semaphore_buffer);
        xSemaphoreGive(uart4_send_semaphore);
        uart4_recv_semaphore = xSemaphoreCreateBinaryStatic(&uart4_recv_semaphore_buffer);
        xSemaphoreGive(uart4_recv_semaphore);

        uart4_tx_queue = xQueueCreateStatic(UART4_TX_QUEUE_SIZE, 
                                          sizeof(tx_payload_t), 
                                          uart4_tx_queue_storage, 
                                          &uart4_tx_queue_buffer);
    }
    #ifdef UART5
    else if(handle->Instance == UART5) {
        uart5_rx_queue = rxQueue;

        uart5_send_semaphore = xSemaphoreCreateBinaryStatic(&uart5_send_semaphore_buffer);
        xSemaphoreGive(uart5_send_semaphore);
        uart5_recv_semaphore = xSemaphoreCreateBinaryStatic(&uart5_recv_semaphore_buffer);
        xSemaphoreGive(uart5_recv_semaphore);

        uart5_tx_queue = xQueueCreateStatic(UART5_TX_QUEUE_SIZE, 
                                          sizeof(tx_payload_t), 
                                          uart5_tx_queue_storage, 
                                          &uart5_tx_queue_buffer);
    }
    #endif /* UART5 */
    else {
        return UART_ERR;
    }
    

    // init HAL
    if (HAL_UART_Init(handle) != HAL_OK) {
        return UART_ERR;
    }

    // Start reception
    if (HAL_UART_Receive_IT(handle, (uint8_t*)handle->pRxBuffPtr, 1) != HAL_OK) {
        return UART_ERR;
    }

    initialized = true;
    return UART_OK;
}


/**
 * @brief Deinitializes the UART peripheral
 * @param handle pointer to the UART handle
 * @return uart_status_t
 */

uart_status_t uart_deinit(UART_HandleTypeDef* handle) {
    if (!initialized) {
        return UART_ERR;
    }

    // Stop any ongoing transfers first
    HAL_UART_Abort(handle);

    // Deinitialize HAL UART
    if (handle->Instance == UART4) {
        if (uart4_send_semaphore != NULL) {
            vSemaphoreDelete(uart4_send_semaphore);
            uart4_send_semaphore = NULL;
        }
        if (uart4_recv_semaphore != NULL) {
            vSemaphoreDelete(uart4_recv_semaphore);
            uart4_recv_semaphore = NULL;
        }
        if (uart4_tx_queue != NULL) {
            vQueueDelete(uart4_tx_queue);
            uart4_tx_queue = NULL;
        }
        uart4_rx_queue = NULL;
    }
    #ifdef UART5
    else if (handle->Instance == UART5) {
        if (uart5_send_semaphore != NULL) {
            vSemaphoreDelete(uart5_send_semaphore);
            uart5_send_semaphore = NULL;
        }
        if (uart5_recv_semaphore != NULL) {
            vSemaphoreDelete(uart5_recv_semaphore);
            uart5_recv_semaphore = NULL;
        }
        if (uart5_tx_queue != NULL) {
            vQueueDelete(uart5_tx_queue);
            uart5_tx_queue = NULL;
        }
        uart5_rx_queue = NULL;
    }
    #endif /* UART5 */
    else {
        return UART_ERR;
    }

    initialized = false;
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

    TickType_t timeout = blocking ? portMAX_DELAY : 0;
    SemaphoreHandle_t* send_semaphore = NULL;
    QueueHandle_t* tx_queue = NULL;

    if(handle->Instance == UART4) {
        send_semaphore = &uart4_send_semaphore;
        tx_queue = &uart4_tx_queue;
    }
    #ifdef UART5
    else if(handle->Instance == UART5) {
        send_semaphore = &uart5_send_semaphore;
        tx_queue = &uart5_tx_queue;
    }
    #endif /* UART5 */
    else {
        return UART_ERR;
    }

    if (xSemaphoreTake(*send_semaphore, timeout) != pdTRUE) {
        return UART_ERR;
    }

    uart_status_t status = UART_SENT;

    // Try direct transmission if possible
    if (HAL_UART_GetState(handle) == HAL_UART_STATE_READY && 
        uxQueueMessagesWaiting(*tx_queue) == 0) {
        if (HAL_UART_Transmit_IT(handle, (uint8_t*)data, length) != HAL_OK) {
            status = UART_ERR;
        }
        goto exit;
    }

    // Queue the data
    for (uint8_t i = 0; i < length; i++) {
        if (xQueueSend(*tx_queue, &data[i], timeout) != pdTRUE) {
            status = UART_ERR;
            goto exit;
        }
    }

    // Start transmission if needed
    if (HAL_UART_GetState(handle) != HAL_UART_STATE_BUSY_TX) {
        uint8_t byte;
        if (xQueueReceive(*tx_queue, &byte, 0) == pdTRUE) {
            if (HAL_UART_Transmit_IT(handle, &byte, 1) != HAL_OK) {
                status = UART_ERR;
                goto exit;
            }
        }
    }

exit:
    xSemaphoreGive(*send_semaphore);
    return status;
}

/**
 * @brief Reads data from the user-provided RX queue
 * @param huart pointer to the UART handle
 * @param data pointer to the data buffer that will be read into
 * @param length of the buffer that will be read
 * @param blocking if true, function will block until data is received
 * @return uart_status_t
 */
uart_status_t uart_recv(UART_HandleTypeDef* handle, uint8_t* data, uint8_t length, bool blocking) {
    if (!initialized || !data || length == 0) {
        return UART_ERR;
    }

    QueueHandle_t* rx_queue = NULL;
    if(handle->Instance == UART4) {
        rx_queue = uart4_rx_queue;
    }
    #ifdef UART5
    else if(handle->Instance == UART5) {
        rx_queue = uart5_rx_queue;
    }
    #endif /* UART5 */
    else {
        return UART_ERR;
    }

    if (!rx_queue) {
        return UART_ERR;
    }

    TickType_t timeout = blocking ? portMAX_DELAY : 0;
    uart_status_t status = UART_RECV;

    for (uint8_t i = 0; i < length; i++) {
        if (xQueueReceive(*rx_queue, &data[i], timeout) != pdTRUE) { // extract data from the rx queue and store it in data
            status = blocking ? UART_ERR : UART_EMPTY;
            return status;
        }
    }

    return status;
}

// Transmit Callback occurs after a transmission if complete (depending on how huart is configure)
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    uint8_t nextByte;
    QueueHandle_t* tx_queue = NULL;

    if(huart->Instance == UART4) {
        tx_queue = &uart4_tx_queue;
    }
    #ifdef UART5
    else if(huart->Instance == UART5) {
        tx_queue = &uart5_tx_queue;
    }
    #endif /* UART5 */
    else {
        return;
    }

    if (xQueueReceiveFromISR(*tx_queue, &nextByte, &higherPriorityTaskWoken) == pdTRUE) {
        HAL_UART_Transmit_IT(huart, &nextByte, 1);
    }
    
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

// Receive Callback occurs after a receive is complete
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    QueueHandle_t* rx_queue = NULL;
    if(huart->Instance == UART4) {
        rx_queue = uart4_rx_queue;
    }
    #ifdef UART5
    else if(huart->Instance == UART5) {
        rx_queue = uart5_rx_queue;
    }
    #endif /* UART5 */
    else {
        return;
    }

    if (!rx_queue) {
        return;
    }

    uint8_t receivedByte = huart->pRxBuffPtr[0];
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    xQueueSendFromISR(*rx_queue, &receivedByte, &higherPriorityTaskWoken);
    HAL_UART_Receive_IT(huart, (uint8_t*)huart->pRxBuffPtr, 1); // pRxBufferPtr is a pointer to the buffer that will store the received data
    
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void UART4_IRQHandler(void) {
    HAL_UART_IRQHandler(huart4);
}

#ifdef UART5
void UART5_IRQHandler(void) {
    HAL_UART_IRQHandler(huart5);
}
#endif /* UART5 */