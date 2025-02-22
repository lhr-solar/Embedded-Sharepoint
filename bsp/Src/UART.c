#include "UART.h"
#include <string.h>
// Define the size of the data to be transmitted
// Currently not used, as we send uint8_t directly
// may need to be configured for support for packets less more than 8 bits
#ifndef DATA_SIZE
#define DATA_SIZE (1) // fallback to 1 byte
#endif

typedef struct {
    uint8_t data[DATA_SIZE]; // data to be transmitted, 1 byte
} tx_payload_t;

uint16_t sizeof_tx_payload_t = sizeof(tx_payload_t); // Precompute size of payload

typedef struct {
    uint8_t data[DATA_SIZE]; // data received, 1 byte
} rx_payload_t;

uint16_t sizeof_rx_payload_t = sizeof(rx_payload_t);

#ifdef UART4
// fallback UART4 TX queue size
#ifndef UART4_TX_QUEUE_SIZE
#define UART4_TX_QUEUE_SIZE (10)
#endif

// fallback UART4 RX queue size
#ifndef UART4_RX_QUEUE_SIZE
#define UART4_RX_QUEUE_SIZE (10)
#endif
//UART4 handle
static UART_HandleTypeDef huart4_ = {.Instance = UART4};
UART_HandleTypeDef* huart4 = &huart4_;

// UART4 TX queue
static QueueHandle_t uart4_tx_queue = NULL;
static StaticQueue_t uart4_tx_queue_buffer;
static uint8_t uart4_tx_queue_storage[UART4_TX_QUEUE_SIZE * sizeof(tx_payload_t)]; 


// UART4 RX queue
static QueueHandle_t uart4_rx_queue = NULL;
static StaticQueue_t uart4_rx_queue_buffer;
static uint8_t uart4_rx_queue_storage[UART4_RX_QUEUE_SIZE * sizeof(rx_payload_t)];  // Will be allocated based on queue_size in uart_init


#endif /* UART4 */


#ifdef UART5
// fallback UART5 TX queue size
#ifndef UART5_TX_QUEUE_SIZE
#define UART5_TX_QUEUE_SIZE (10)
#endif

// fallback UART5 RX queue size
#ifndef UART5_RX_QUEUE_SIZE
#define UART5_RX_QUEUE_SIZE (10)
#endif

// UART5 handle
static UART_HandleTypeDef huart5_ = {.Instance = UART5};
UART_HandleTypeDef* huart5 = &huart5_;

// UART5 TX queue
static QueueHandle_t uart5_tx_queue = NULL;
static StaticQueue_t uart5_tx_queue_buffer;
static uint8_t uart5_tx_queue_storage[UART5_TX_QUEUE_SIZE * sizeof(tx_payload_t)];

// UART5 RX queue
static QueueHandle_t uart5_rx_queue = NULL;
static StaticQueue_t uart5_rx_queue_buffer;
static uint8_t uart5_rx_queue_storage[UART5_RX_QUEUE_SIZE * sizeof(rx_payload_t)];  // Will be allocated based on queue_size

#endif /* UART5 */


static bool initialized = false;

// HAL UART MSP init 
void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    #ifdef UART4
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

        HAL_NVIC_SetPriority(UART4_IRQn, 5, 0); 
        HAL_NVIC_EnableIRQ(UART4_IRQn);
    }
    #endif /* UART4 */

    #ifdef UART5
    if (huart->Instance == UART5) {
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
    #ifdef UART4
    if (huart->Instance == UART4) {
        __HAL_RCC_UART4_CLK_DISABLE();                  // disable clocks
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1);  // disable gpio
        HAL_NVIC_DisableIRQ(UART4_IRQn);                // disable interrupts
    }
    #endif /* UART4 */

    #ifdef UART5
    if (huart->Instance == UART5) {
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
uart_status_t uart_init(UART_HandleTypeDef* handle) {

    #ifdef UART4
    if (handle->Instance == UART4) {

        // Create TX queue
        uart4_tx_queue = xQueueCreateStatic(UART4_TX_QUEUE_SIZE, 
                                            sizeof(sizeof_tx_payload_t), 
                                            uart4_tx_queue_storage, 
                                            &uart4_tx_queue_buffer);
        
        
        // Create RX queue
        uart4_rx_queue = xQueueCreateStatic(UART4_RX_QUEUE_SIZE,
                                            sizeof(sizeof_rx_payload_t),
                                            uart4_rx_queue_storage,
                                            &uart4_rx_queue_buffer);
    }
    #endif /* UART4 */

    #ifdef UART5
    if(handle->Instance == UART5) {

        // Allocate static storage for TX queue
        uart5_tx_queue = xQueueCreateStatic(UART5_TX_QUEUE_SIZE, 
                                          sizeof(sizeof_tx_payload_t), 
                                          uart5_tx_queue_storage, 
                                          &uart5_tx_queue_buffer);

        // Create RX queue
        uart5_rx_queue = xQueueCreateStatic(UART5_RX_QUEUE_SIZE,
                                          sizeof(sizeof_rx_payload_t),
                                          uart5_rx_queue_storage,
                                          &uart5_rx_queue_buffer);
    }
    #endif /* UART5 */
    

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

    // Stop any ongoing transfers first
    HAL_UART_Abort(handle);

    // Deinitialize HAL
    if (HAL_UART_DeInit(handle) != HAL_OK) {
        return UART_ERR; 
    }

    // Deinitialize Handler
    #ifdef UART4
    if (handle->Instance == UART4) {
        if (uart4_tx_queue != NULL) {
            vQueueDelete(uart4_tx_queue);
            uart4_tx_queue = NULL;
        }
        if (uart4_rx_queue != NULL) {
            vQueueDelete(uart4_rx_queue);
            uart4_rx_queue = NULL;
        }
        uart4_rx_queue = NULL;
    }
    #endif /* UART4 */
    #ifdef UART5
    if (handle->Instance == UART5) {
        if (uart5_tx_queue != NULL) {
            vQueueDelete(uart5_tx_queue);
            uart5_tx_queue = NULL;
        }
        if (uart5_rx_queue != NULL) {
            vQueueDelete(uart5_rx_queue);
            uart5_rx_queue = NULL;
        }
        uart5_rx_queue = NULL;
    }
    #endif /* UART5 */

    initialized = false;
    return UART_OK;
}


/**
 * @brief Transmits data over UART. If transmission is in progress, data will be queued in internal TX queue. 
 * @param data pointer to the data buffer that will be written/transmitted
 * @param length of the buffer that will be written/transmitted
 * @param delay_ticks number of ticks to wait for data to be transmitted
 * @return uart_status_t
 */
#ifdef UART4
// static buffer for UART4 TX
static uint8_t uart4_tx_buffer[UART4_TX_QUEUE_SIZE];
#endif

#ifdef UART5
// static buffer for UART5 TX
static uint8_t uart5_tx_buffer[UART5_TX_QUEUE_SIZE]; // make buffer size same as queue size 
#endif
uart_status_t uart_send(UART_HandleTypeDef* handle, const uint8_t* data, uint8_t length, TickType_t delay_ticks) {
    if (length == 0) {
        return UART_OK;
    }

    QueueHandle_t* tx_queue = NULL;
    uint8_t* tx_buffer = NULL;

    #ifdef UART4
    if(handle->Instance == UART4) {
        tx_buffer = uart4_tx_buffer; // for direct transmission
        tx_queue = &uart4_tx_queue;  // for queuing 
    }
    #endif /* UART 4*/
    #ifdef UART5
    if(handle->Instance == UART5) {
        tx_buffer = uart5_tx_buffer;
        tx_queue = &uart5_tx_queue;
    }
    #endif /* UART5 */

    uart_status_t status = UART_SENT;

    // Try direct transmission if possible
    portENTER_CRITICAL();
    if (HAL_UART_GetState(handle) == HAL_UART_STATE_READY && 
        tx_queue != NULL && uxQueueMessagesWaiting (*tx_queue) == 0 ) {
        // Copy data to static buffer
        memcpy(tx_buffer, data, length);
        if (HAL_UART_Transmit_IT(handle, tx_buffer, length) != HAL_OK) {
            status = UART_ERR;
        }
        portEXIT_CRITICAL();
        goto exit;
    }
    portEXIT_CRITICAL();


    // put into send queues
    for (uint8_t i = 0; i < length; i++) {
        if (xQueueSend(*tx_queue, &data[i], delay_ticks) != pdTRUE){
            return UART_ERR;
        }  //delay_ticks: 0 = no wait, portMAX_DELAY = wait until space is available
    }
    

    // Start transmission if needed
    portENTER_CRITICAL();
    if (HAL_UART_GetState(handle) != HAL_UART_STATE_BUSY_TX) {
        uint8_t byte;
        if (xQueueReceive(*tx_queue, &byte, 0) == pdTRUE) {
            if (HAL_UART_Transmit_IT(handle, &byte, 1) != HAL_OK) {
                status = UART_ERR;
                portEXIT_CRITICAL();
                goto exit;
            }
        }
    }
    portEXIT_CRITICAL();

exit:
    return status;
}

/**
 * @brief Reads data from the user-provided RX queue
 * @param huart pointer to the UART handle
 * @param data pointer to the data buffer that will be read into
 * @param length of the buffer that will be read
 * @param delay_ticks number of ticks to wait for data to be received
 * @return uart_status_t
 */
uart_status_t uart_recv(UART_HandleTypeDef* handle, uint8_t* data, uint8_t length, TickType_t delay_ticks) {
    if (!data || length == 0) {
        return UART_ERR;
    }

    QueueHandle_t rx_queue = NULL;
    #ifdef UART4
    if(handle->Instance == UART4) {
        rx_queue = uart4_rx_queue;
    }
    #endif /* UART4 */
    #ifdef UART5
    if(handle->Instance == UART5) {
        rx_queue = uart5_rx_queue;
    }
    #endif /* UART5 */

    uart_status_t status = UART_RECV;

    // Receive all requested bytes
    for (uint8_t i = 0; i < length; i++) {
        if (xQueueReceive(rx_queue, &data[i], delay_ticks) == errQUEUE_EMPTY) {
            return UART_EMPTY;
        } 
    }
    

    return status;
}

// Transmit Callback occurs after a transmission if complete (depending on how huart is configure)
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    uint8_t tx_buffer[32];  // Buffer for collecting bytes to send
    uint8_t count = 0;

    QueueHandle_t* tx_queue = NULL;

    #ifdef UART4
    if(huart->Instance == UART4) {
        tx_queue = &uart4_tx_queue;
    }
    #endif /* UART4 */
    #ifdef UART5
    if(huart->Instance == UART5) {
        tx_queue = &uart5_tx_queue;
    }
    #endif /* UART5 */
    else {
        return;
    }


    // Get as many bytes as we can from queue (up to buffer size)
    while(count < sizeof(tx_buffer) && 
        xQueueReceiveFromISR(*tx_queue, &tx_buffer[count], &higherPriorityTaskWoken) == pdTRUE) {
        count++;
    }

    // If we got any bytes, transmit them
    portENTER_CRITICAL();
    if(count > 0) {
      HAL_UART_Transmit_IT(huart, tx_buffer, count);
    }
    portEXIT_CRITICAL();
    
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

// Receive Callback occurs after a receive is complete
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    QueueHandle_t rx_queue = NULL;
    #ifdef UART4
    if(huart->Instance == UART4) {
        rx_queue = uart4_rx_queue;
    }
    #endif /* UART4 */

    #ifdef UART5
    if(huart->Instance == UART5) {
        rx_queue = uart5_rx_queue;
    }
    #endif /* UART5 */

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