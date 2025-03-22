#include "USART.h"
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

#ifdef USART1
// fallback USART1 TX queue size
#ifndef USART1_TX_QUEUE_SIZE
#define USART1_TX_QUEUE_SIZE (10)
#endif

// fallback USART1 RX queue size
#ifndef USART1_RX_QUEUE_SIZE
#define USART1_RX_QUEUE_SIZE (10)
#endif

//USART1 handle
static USART_HandleTypeDef husart1_ = {.Instance = USART1};
USART_HandleTypeDef* husart1 = &husart1_;

// USART1 TX queue
static QueueHandle_t usart1_tx_queue = NULL;
static StaticQueue_t usart1_tx_queue_buffer;
static uint8_t usart1_tx_queue_storage[USART1_TX_QUEUE_SIZE * sizeof(tx_payload_t)]; 

// USART1 RX queue
static QueueHandle_t usart1_rx_queue = NULL;
static StaticQueue_t usart1_rx_queue_buffer;
static uint8_t usart1_rx_queue_storage[USART1_RX_QUEUE_SIZE * sizeof(rx_payload_t)];  // Will be allocated based on queue_size in usart_init
#endif /* USART1 */

#ifdef USART2
// fallback USART2 TX queue size
#ifndef USART2_TX_QUEUE_SIZE
#define USART2_TX_QUEUE_SIZE (10)
#endif

// fallback USART2 RX queue size
#ifndef USART2_RX_QUEUE_SIZE
#define USART2_RX_QUEUE_SIZE (10)
#endif

// USART2 handle
static USART_HandleTypeDef husart2_ = {.Instance = USART2};
USART_HandleTypeDef* husart2 = &husart2_;

// USART2 TX queue
static QueueHandle_t usart2_tx_queue = NULL;
static StaticQueue_t usart2_tx_queue_buffer;
static uint8_t usart2_tx_queue_storage[USART2_TX_QUEUE_SIZE * sizeof(tx_payload_t)];

// USART2 RX queue
static QueueHandle_t usart2_rx_queue = NULL;
static StaticQueue_t usart2_rx_queue_buffer;
static uint8_t usart2_rx_queue_storage[USART2_RX_QUEUE_SIZE * sizeof(rx_payload_t)];  // Will be allocated based on queue_size
#endif /* USART2 */

#ifdef USART3
// fallback USART3 TX queue size
#ifndef USART3_TX_QUEUE_SIZE
#define USART3_TX_QUEUE_SIZE (10)
#endif

// fallback USART3 RX queue size
#ifndef USART3_RX_QUEUE_SIZE
#define USART3_RX_QUEUE_SIZE (10)
#endif

// USART3 handle
static USART_HandleTypeDef husart3_ = {.Instance = USART3};
USART_HandleTypeDef* husart3 = &husart3_;

// USART3 TX queue
static QueueHandle_t usart3_tx_queue = NULL;
static StaticQueue_t usart3_tx_queue_buffer;
static uint8_t usart3_tx_queue_storage[USART3_TX_QUEUE_SIZE * sizeof(tx_payload_t)];

// USART3 RX queue
static QueueHandle_t usart3_rx_queue = NULL;
static StaticQueue_t usart3_rx_queue_buffer;
static uint8_t usart3_rx_queue_storage[USART3_RX_QUEUE_SIZE * sizeof(rx_payload_t)];  // Will be allocated based on queue_size
#endif /* USART3 */



bool is_usart_initialized(USART_HandleTypeDef* handle) {
    // Check if the USART is in a valid state
    // HAL_USART_STATE_RESET indicates the USART is not initialized
    return (handle->State != HAL_USART_STATE_RESET); 
}

// HAL USART MSP init 
void HAL_USART_MspInit(USART_HandleTypeDef *husart) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    #ifdef USART1
    if(husart->Instance == USART1) {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        // USART1 GPIO Configuration    
        // PA9     ------> USART1_TX
        // PA10    ------> USART1_RX
        GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(USART1_IRQn, 5, 0); 
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
    #endif /* USART1 */

    #ifdef USART2
    if (husart->Instance == USART2) {
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        // USART1 GPIO Configuration    
        // PA2     ------> USART2_TX
        // PA3     ------> USART2_RX
        GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(USART2_IRQn, 5, 0); 
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
    #endif /* USART2 */

    #ifdef USART3
    if (husart->Instance == USART3) {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        // USART1 GPIO Configuration    
        // PC4     ------> USART3_TX
        // PC5     ------> USART3_RX
        GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(USART3_IRQn, 5, 0); 
        HAL_NVIC_EnableIRQ(USART3_IRQn);
    }
    #endif /* USART3 */
}


void HAL_USART_MspDeInit(USART_HandleTypeDef *husart) {
    #ifdef USART1
    if (husart->Instance == USART1) {
        __HAL_RCC_USART1_CLK_DISABLE();                  // disable clocks
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1);  // disable gpio
        HAL_NVIC_DisableIRQ(USART1_IRQn);                // disable interrupts
    }
    #endif /* USART1 */

    #ifdef USART2
    if (husart->Instance == USART2) {
        __HAL_RCC_USART2_CLK_DISABLE();          // disable clocks
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);    // disable gpio C12
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);     //disable gpio D2
        HAL_NVIC_DisableIRQ(USART2_IRQn);        //disable interrupts
    }
    #endif /* USART2 */

    #ifdef USART3
    if (husart->Instance == USART3) {
        __HAL_RCC_USART3_CLK_DISABLE();          // disable clocks
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);    // disable gpio C12
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);     //disable gpio D2
        HAL_NVIC_DisableIRQ(USART2_IRQn);        //disable interrupts
    }
    #endif /* USART3 */
}

/**
 * @brief Initializes the USART peripheral
 * @param handle pointer to the USART handle
 * @return usart_status_t
 */
usart_status_t usart_init(USART_HandleTypeDef* handle) {

    #ifdef USART1
    if (handle->Instance == USART1) {

        // Create TX queue
        usart1_tx_queue = xQueueCreateStatic(USART1_TX_QUEUE_SIZE, 
                                            sizeof(sizeof_tx_payload_t), 
                                            usart1_tx_queue_storage, 
                                            &usart1_tx_queue_buffer);
        
        
        // Create RX queue
        usart1_rx_queue = xQueueCreateStatic(USART1_RX_QUEUE_SIZE,
                                            sizeof(sizeof_rx_payload_t),
                                            usart1_rx_queue_storage,
                                            &usart1_rx_queue_buffer);
    }
    #endif /* USART1 */

    #ifdef USART2
    if(handle->Instance == USART2) {

        // Allocate static storage for TX queue
        usart2_tx_queue = xQueueCreateStatic(USART2_TX_QUEUE_SIZE, 
                                          sizeof(sizeof_tx_payload_t), 
                                          usart2_tx_queue_storage, 
                                          &usart2_tx_queue_buffer);

        // Create RX queue
        usart2_rx_queue = xQueueCreateStatic(USART2_RX_QUEUE_SIZE,
                                          sizeof(sizeof_rx_payload_t),
                                          usart2_rx_queue_storage,
                                          &usart2_rx_queue_buffer);
    }
    #endif /* USART2 */

    #ifdef USART3
    if(handle->Instance == USART3) {

        // Allocate static storage for TX queue
        usart3_tx_queue = xQueueCreateStatic(USART3_TX_QUEUE_SIZE, 
                                          sizeof(sizeof_tx_payload_t), 
                                          usart3_tx_queue_storage, 
                                          &usart3_tx_queue_buffer);

        // Create RX queue
        usart3_rx_queue = xQueueCreateStatic(USART3_RX_QUEUE_SIZE,
                                          sizeof(sizeof_rx_payload_t),
                                          usart3_rx_queue_storage,
                                          &usart3_rx_queue_buffer);
    }
    #endif /* USART3 */
    

    // init HAL
    if (HAL_USART_Init(handle) != HAL_OK) {
        return USART_ERR;
    }

    // Start reception
    if (HAL_USART_Receive_IT(handle, (uint8_t*)handle->pRxBuffPtr, DATA_SIZE) != HAL_OK) {
        return USART_ERR;
    }

    return USART_OK;
}


/**
 * @brief Deinitializes the USART peripheral
 * @param handle pointer to the USART handle
 * @return usart_status_t
 */

usart_status_t usart_deinit(USART_HandleTypeDef* handle) {

    // Stop any ongoing transfers first
    HAL_USART_Abort(handle);

    // Deinitialize HAL
    if (HAL_USART_DeInit(handle) != HAL_OK) {
        return USART_ERR; 
    }

    // Deinitialize Handler
    #ifdef USART1
    if (handle->Instance == USART1) {
        if (usart1_tx_queue != NULL) {
            vQueueDelete(usart1_tx_queue);
            usart1_tx_queue = NULL;
        }
        if (usart1_rx_queue != NULL) {
            vQueueDelete(usart1_rx_queue);
            usart1_rx_queue = NULL;
        }
    }
    #endif /* USART1 */
    #ifdef USART2
    if (handle->Instance == USART2) {
        if (usart2_tx_queue != NULL) {
            usart2_tx_queue = NULL;
        }
        if (usart2_rx_queue != NULL) {
            usart2_rx_queue = NULL;
        }
    }
    #endif /* USART3 */
    #ifdef USART3
    if (handle->Instance == USART3) {
        if (usart3_tx_queue != NULL) {
            usart3_tx_queue = NULL;
        }
        if (usart3_rx_queue != NULL) {
            usart3_rx_queue = NULL;
        }
    }
    #endif /* USART3 */


    return USART_OK;
}


/**
 * @brief Transmits data over USART. If transmission is in progress, data will be queued in internal TX queue. 
 * @param data pointer to the data buffer that will be written/transmitted
 * @param length of the buffer that will be written/transmitted
 * @param delay_ticks number of ticks to wait for data to be transmitted
 * @return usart_status_t
 */
#ifdef USART1
// static buffer for USART1 TX
static uint8_t usart1_tx_buffer[USART1_TX_QUEUE_SIZE];
#endif

#ifdef USART2
// static buffer for USART2 TX
static uint8_t usart2_tx_buffer[USART2_TX_QUEUE_SIZE]; // make buffer size same as queue size 
#endif

#ifdef USART3
// static buffer for USART3 TX
static uint8_t usart3_tx_buffer[USART3_TX_QUEUE_SIZE]; // make buffer size same as queue size 
#endif

usart_status_t usart_send(USART_HandleTypeDef* handle, const uint8_t* data, uint8_t length, TickType_t delay_ticks) {
    if (length == 0 || !is_usart_initialized(handle)) { // check if USART is initialized and data length is not 0
        return USART_ERR;
    }

    QueueHandle_t* tx_queue = NULL;
    uint8_t* tx_buffer = NULL;

    #ifdef USART1
    if(handle->Instance == USART1) {
        tx_buffer = usart1_tx_buffer; // for direct transmission
        tx_queue = &usart1_tx_queue;  // for queuing 
    }
    #endif /* USART 4*/
    #ifdef USART2
    if(handle->Instance == USART2) {
        tx_buffer = usart2_tx_buffer;
        tx_queue = &usart2_tx_queue;
    }
    #endif /* USART2 */
    #ifdef USART3
    if(handle->Instance == USART3) {
        tx_buffer = usart3_tx_buffer;
        tx_queue = &usart3_tx_queue;
    }
    #endif /* USART3 */

    usart_status_t status = USART_SENT;

    // Try direct transmission if possible
    portENTER_CRITICAL();
    if (HAL_USART_GetState(handle) == HAL_USART_STATE_READY && 
        tx_queue != NULL && uxQueueMessagesWaiting (*tx_queue) == 0 ) { // check if USART is ready and queue is empty
        // Copy data to static buffer
        memcpy(tx_buffer, data, length);
        if (HAL_USART_Transmit_IT(handle, tx_buffer, length) != HAL_OK) {
            status = USART_ERR;
        }
        portEXIT_CRITICAL();
        goto exit;
    }
    portEXIT_CRITICAL();


    // put into send queues
    for (uint8_t i = 0; i < length; i++) {
        if (xQueueSend(*tx_queue, &data[i], delay_ticks) != pdTRUE){
            return USART_ERR;
        }  //delay_ticks: 0 = no wait, portMAX_DELAY = wait until space is available
    }
    


exit:
    return status;
}

/**
 * @brief Reads data from the user-provided RX queue
 * @param husart pointer to the USART handle
 * @param data pointer to the data buffer that will be read into
 * @param length of the buffer that will be read
 * @param delay_ticks number of ticks to wait for data to be received
 * @return usart_status_t
 */
usart_status_t usart_recv(USART_HandleTypeDef* handle, uint8_t* data, uint8_t length, TickType_t delay_ticks) {
    if (!data || length == 0 || !is_usart_initialized(handle)) { // check if data is not null, length is not 0 and USART is initialized
        return USART_ERR;
    }

    QueueHandle_t rx_queue = NULL;
    #ifdef USART1
    if(handle->Instance == USART1) {
        rx_queue = usart1_rx_queue;
    }
    #endif /* USART1 */
    #ifdef USART2
    if(handle->Instance == USART2) {
        rx_queue = usart2_rx_queue;
    }
    #endif /* USART2 */
    #ifdef USART3
    if(handle->Instance == USART3) {
        rx_queue = usart3_rx_queue;
    }
    #endif /* USART3 */

    usart_status_t status = USART_RECV;

    // Receive all requested bytes
    for (uint8_t i = 0; i < length; i++) {
        if (xQueueReceive(rx_queue, &data[i], delay_ticks) == errQUEUE_EMPTY) {
            return USART_EMPTY;
        } 
    }
    

    return status;
}

// Transmit Callback occurs after a transmission if complete (depending on how husart is configure)
void HAL_USART_TxCpltCallback(USART_HandleTypeDef *husart) {
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    uint8_t tx_buffer[32];  // Buffer for collecting bytes to send
    uint8_t count = 0;

    QueueHandle_t *tx_queue = NULL;

    #ifdef USART1
    if(husart->Instance == USART1) {
        tx_queue = &usart1_tx_queue;
    }
    #endif /* USART1 */
    #ifdef USART2
    if(husart->Instance == USART2) {
        tx_queue = &usart2_tx_queue;
    }
    #endif /* USART2 */
    #ifdef USART3
    if(husart->Instance == USART3) {
        tx_queue = &usart3_tx_queue;
    }
    #endif /* USART3 */
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
      HAL_USART_Transmit_IT(husart, tx_buffer, count);
    }
    portEXIT_CRITICAL();
    
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

// Receive Callback occurs after a receive is complete
void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart) {
    QueueHandle_t *rx_queue = NULL;
    #ifdef USART1
    if(husart->Instance == USART1) {
        rx_queue = &usart1_rx_queue;
    }
    #endif /* USART1 */

    #ifdef USART2
    if(husart->Instance == USART2) {
        rx_queue = &usart2_rx_queue;
    }
    #endif /* USART2 */

    #ifdef USART3
    if(husart->Instance == USART3) {
        rx_queue = &usart3_rx_queue;
    }
    #endif /* USART3 */


    rx_payload_t receivedData;

    for (int i = 0; i < DATA_SIZE; i++) {
        receivedData.data[i] = husart->pRxBuffPtr[i];
    }

    BaseType_t higherPriorityTaskWoken = pdFALSE;

    xQueueSendFromISR(*rx_queue, &receivedData, &higherPriorityTaskWoken); // Send data from &receivedData(pRxBuffPtr) to rx_queue
    
    portENTER_CRITICAL();
    HAL_USART_Receive_IT(husart, (uint8_t*)husart->pRxBuffPtr, DATA_SIZE);// pRxBufferPtr is a pointer to the buffer that will store the received data
    portEXIT_CRITICAL();
    
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

#ifdef USART1
void USART1_IRQHandler(void) {
    HAL_USART_IRQHandler(husart1);
}
#endif /* USART1 */

#ifdef USART2
void USART2_IRQHandler(void) {
    HAL_USART_IRQHandler(husart2);
}
#endif /* USART2 */

#ifdef USART3
void USART3_IRQHandler(void) {
    HAL_USART_IRQHandler(husart3);
}
#endif /* USART3 */