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

typedef struct {
    uint8_t data[DATA_SIZE]; // data received, 1 byte
} rx_payload_t;

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

// UART4 RX buffer
// An intermediate buffer of DATA_SIZE bytes to store received data before it is copied to the queue
static rx_payload_t uart4_rx_buffer;

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

// UART5 RX buffer
// An intermediate buffer of DATA_SIZE bytes to store received data before it is copied to the queue
static rx_payload_t uart5_rx_buffer;

#endif /* UART5 */

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
static UART_HandleTypeDef husart1_ = {.Instance = USART1};
UART_HandleTypeDef* husart1 = &husart1_;

// USART1 TX queue
static QueueHandle_t usart1_tx_queue = NULL;
static StaticQueue_t usart1_tx_queue_buffer;
static uint8_t usart1_tx_queue_storage[USART1_TX_QUEUE_SIZE * sizeof(tx_payload_t)]; 


// USART1 RX queue
static QueueHandle_t usart1_rx_queue = NULL;
static StaticQueue_t usart1_rx_queue_buffer;
static uint8_t usart1_rx_queue_storage[USART1_RX_QUEUE_SIZE * sizeof(rx_payload_t)];  // Will be allocated based on queue_size in uart_init

// USART1 RX buffer
// An intermediate buffer of DATA_SIZE bytes to store received data before it is copied to the queue
static rx_payload_t usart1_rx_buffer;

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

//USART2 handle
static UART_HandleTypeDef husart2_ = {.Instance = USART2};
UART_HandleTypeDef* husart2 = &husart2_;

// USART2 TX queue
static QueueHandle_t usart2_tx_queue = NULL;
static StaticQueue_t usart2_tx_queue_buffer;
static uint8_t usart2_tx_queue_storage[USART2_TX_QUEUE_SIZE * sizeof(tx_payload_t)]; 


// USART2 RX queue
static QueueHandle_t usart2_rx_queue = NULL;
static StaticQueue_t usart2_rx_queue_buffer;
static uint8_t usart2_rx_queue_storage[USART2_RX_QUEUE_SIZE * sizeof(rx_payload_t)];  // Will be allocated based on queue_size in uart_init

// USART2 RX buffer
// An intermediate buffer of DATA_SIZE bytes to store received data before it is copied to the queue
static rx_payload_t usart2_rx_buffer;

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

//USART3 handle
static UART_HandleTypeDef husart3_ = {.Instance = USART3};
UART_HandleTypeDef* husart3 = &husart3_;

// USART3 TX queue
static QueueHandle_t usart3_tx_queue = NULL;
static StaticQueue_t usart3_tx_queue_buffer;
static uint8_t usart3_tx_queue_storage[USART3_TX_QUEUE_SIZE * sizeof(tx_payload_t)]; 


// USART3 RX queue
static QueueHandle_t usart3_rx_queue = NULL;
static StaticQueue_t usart3_rx_queue_buffer;
static uint8_t usart3_rx_queue_storage[USART3_RX_QUEUE_SIZE * sizeof(rx_payload_t)];  // Will be allocated based on queue_size in uart_init

// USART3 RX buffer
// An intermediate buffer of DATA_SIZE bytes to store received data before it is copied to the queue
static rx_payload_t usart3_rx_buffer;

#endif /* USART3 */

static bool is_uart_initialized(UART_HandleTypeDef* handle) {
    // Check if the UART is in a valid state
    // HAL_UART_STATE_RESET indicates the UART is not initialized
    return (handle->gState != HAL_UART_STATE_RESET);
}

// Redefine me!
__weak void HAL_UART_MspGPIOInit(UART_HandleTypeDef *huart){ 
    GPIO_InitTypeDef init = {0};
    // Same sets of pins for L4/F4
    #ifdef UART4
    if(huart->Instance == UART4) {
        //enable port A clock
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* enable port A UART4 gpio
        PA0 -> UART4_TX
        PA1 -> UART4_RX    
        */
        init.Pin = GPIO_PIN_0|GPIO_PIN_1;
        init.Mode = GPIO_MODE_AF_PP;
        init.Pull = GPIO_NOPULL;
        init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        init.Alternate = GPIO_AF8_UART4;
        HAL_GPIO_Init(GPIOA, &init);
    }
    #endif /* UART4 */
    
    #ifdef UART5
    if (huart->Instance == UART5) {
        __HAL_RCC_GPIOC_CLK_ENABLE();

	__HAL_RCC_GPIOD_CLK_ENABLE();

        /* enable UART5 gpio   
        PC12 -> UART5_TX
        PD2 -> UART5_RX
        */
        init.Pin = GPIO_PIN_12;
        init.Mode = GPIO_MODE_AF_PP;
        init.Pull = GPIO_NOPULL;
        init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        init.Alternate = GPIO_AF8_UART5;
        HAL_GPIO_Init(GPIOC, &init);

        init.Pin = GPIO_PIN_2;
	
	HAL_GPIO_Init(GPIOD, &init);
    }
    #endif /* UART5 */
    
    #ifdef USART1
    if(huart->Instance == USART1) {
        //enable port A clock
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* enable port A USART1 gpio
        PA9 -> USART1_TX
        PA10 -> USART1_RX    
        */
        init.Pin = GPIO_PIN_9|GPIO_PIN_10;
        init.Mode = GPIO_MODE_AF_PP;
        init.Pull = GPIO_NOPULL;
        init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        init.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &init);
    }
    #endif /* USART1 */
    
    #if defined(USART2) && defined(GPIOD)
    if(huart->Instance == USART2) {
        //enable port D clock
        __HAL_RCC_GPIOD_CLK_ENABLE();

        /* enable port D USART2 gpio
        PD5 -> USART2_TX
        PD6 -> USART2_RX    
        */
        init.Pin = GPIO_PIN_5|GPIO_PIN_6;
        init.Mode = GPIO_MODE_AF_PP;
        init.Pull = GPIO_NOPULL;
        init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        init.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOD, &init);
    }
    #endif /* USART2 && GPIOD*/

    #ifdef USART3
    if(huart->Instance == USART3) {
        //enable port C clock
        __HAL_RCC_GPIOC_CLK_ENABLE();

        /* enable port C USART3 gpio
        PC10 -> USART3_TX
        PC11 -> USART3_RX    
        */
        init.Pin = GPIO_PIN_10|GPIO_PIN_11;
        init.Mode = GPIO_MODE_AF_PP;
        init.Pull = GPIO_NOPULL;
        init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        init.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOC, &init);
    }
    #endif /* USART3 */    
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    IRQn_Type uart_IRQ = NonMaskableInt_IRQn; // IRQn_Type for UART interrupts

    #ifdef UART4
    if (huart->Instance == UART4) {
        __HAL_RCC_UART4_CLK_ENABLE(); // enable UART4 clock
        uart_IRQ = UART4_IRQn;
    }
    #endif /* UART4 */
    
    #ifdef UART5
    if (huart->Instance == UART5) {
        __HAL_RCC_UART5_CLK_ENABLE(); // enable UART5 clock
        uart_IRQ = UART5_IRQn;
    }
    #endif /* UART5 */

    #ifdef USART1
    if (huart->Instance == USART1) {
        __HAL_RCC_USART1_CLK_ENABLE(); // enable USART1 clock
        uart_IRQ = USART1_IRQn;
    }
    #endif /* USART1 */

    #ifdef USART2
    if (huart->Instance == USART2) {
        __HAL_RCC_USART2_CLK_ENABLE(); // enable USART2 clock
        uart_IRQ = USART2_IRQn;
    }
    #endif /* USART2 */

    #ifdef USART3
    if (huart->Instance == USART3) {
        __HAL_RCC_USART3_CLK_ENABLE(); // enable USART3 clock
        uart_IRQ = USART3_IRQn;
    }
    #endif /* USART3 */

    // configure GPIO pins for UART
    HAL_UART_MspGPIOInit(huart); 

    // enable uart interrupts
    HAL_NVIC_SetPriority(uart_IRQ, 5, 0);
    HAL_NVIC_EnableIRQ(uart_IRQ); 
}

// Redefine me!
__weak void HAL_UART_MspGPIODeInit(UART_HandleTypeDef *huart){ 
    // Same sets of pins for L4/F4
    #ifdef UART4
    if(huart->Instance == UART4) {
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1);
    }
    #endif /* UART4 */
    
    #ifdef UART5
    if (huart->Instance == UART5) {
	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);
	HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);
    }
    #endif /* UART5 */
    
    #ifdef USART1
    if(huart->Instance == USART1) {
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);
    }
    #endif /* USART1 */
    
    #if defined(USART2) && defined(GPIOD)
    if(huart->Instance == USART2) {
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5);
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_6);
    }
    #endif /* USART2 */

    #ifdef USART3
    if(huart->Instance == USART3) {
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10);
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_11);
    }
    #endif /* USART3 */
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart) {
    // UART4
    #ifdef UART4
    if (huart->Instance == UART4) {
        //disable clocks
        __HAL_RCC_UART4_CLK_DISABLE();

        // disable interrupts
        HAL_NVIC_DisableIRQ(UART4_IRQn);
    }
    #endif /* UART4 */

    // UART5
    #ifdef UART5
    if (huart->Instance == UART5) {
        // disable clocks
        __HAL_RCC_UART5_CLK_DISABLE();
    
        // disable interrupts
        HAL_NVIC_DisableIRQ(UART5_IRQn);        //disable interrupts
    }
    #endif /* UART5 */

    // USART1
    #ifdef USART1
    if (huart->Instance == USART1) {
        // disable clocks
        __HAL_RCC_USART1_CLK_DISABLE();
    
        // disable interrupts
        HAL_NVIC_DisableIRQ(USART1_IRQn);        //disable interrupts
    }
    #endif /* USART1 */

    // USART2
    #ifdef USART2
    if (huart->Instance == USART2) {
        // disable clocks
        __HAL_RCC_USART2_CLK_DISABLE();
    
        // disable interrupts
        HAL_NVIC_DisableIRQ(USART2_IRQn);        //disable interrupts
    }
    #endif /* USART2 */
    
    // USART3
    #ifdef USART3
    if (huart->Instance == USART3) {
        // disable clocks
        __HAL_RCC_USART3_CLK_DISABLE();
    
        // disable interrupts
        HAL_NVIC_DisableIRQ(USART3_IRQn);        //disable interrupts
    }
    #endif /* USART3 */

    HAL_UART_MspGPIODeInit(huart);
}

/**
 * @brief Initializes the UART peripheral
 * @param handle pointer to the UART handle
 * @return uart_status_t
 */
uart_status_t uart_init(UART_HandleTypeDef* handle) {
    uint8_t *rx_buffer = NULL;

    #ifdef UART4
    if (handle->Instance == UART4) {
        // Create TX queue
        uart4_tx_queue = xQueueCreateStatic(UART4_TX_QUEUE_SIZE, 
                                            sizeof(tx_payload_t), 
                                            uart4_tx_queue_storage, 
                                            &uart4_tx_queue_buffer);
        
        
        // Create RX queue
        uart4_rx_queue = xQueueCreateStatic(UART4_RX_QUEUE_SIZE,
                                            sizeof(rx_payload_t),
                                            uart4_rx_queue_storage,
                                            &uart4_rx_queue_buffer);

        rx_buffer = uart4_rx_buffer.data;
    }
    #endif /* UART4 */

    #ifdef UART5
    if(handle->Instance == UART5) {

        // Allocate static storage for TX queue
        uart5_tx_queue = xQueueCreateStatic(UART5_TX_QUEUE_SIZE, 
                                          sizeof(tx_payload_t), 
                                          uart5_tx_queue_storage, 
                                          &uart5_tx_queue_buffer);

        // Create RX queue
        uart5_rx_queue = xQueueCreateStatic(UART5_RX_QUEUE_SIZE,
                                          sizeof(rx_payload_t),
                                          uart5_rx_queue_storage,
                                          &uart5_rx_queue_buffer);

        rx_buffer = uart5_rx_buffer.data;
    }
    #endif /* UART5 */
    
    #ifdef USART1
    if(handle->Instance == USART1) {

        // Allocate static storage for TX queue
        usart1_tx_queue = xQueueCreateStatic(USART1_TX_QUEUE_SIZE, 
                                          sizeof(tx_payload_t), 
                                          usart1_tx_queue_storage, 
                                          &usart1_tx_queue_buffer);

        // Create RX queue
        usart1_rx_queue = xQueueCreateStatic(USART1_RX_QUEUE_SIZE,
                                          sizeof(rx_payload_t),
                                          usart1_rx_queue_storage,
                                          &usart1_rx_queue_buffer);

        rx_buffer = usart1_rx_buffer.data;
    }
    #endif /* USART1 */

    #ifdef USART2
    if(handle->Instance == USART2) {

        // Allocate static storage for TX queue
        usart2_tx_queue = xQueueCreateStatic(USART2_TX_QUEUE_SIZE, 
                                          sizeof(tx_payload_t), 
                                          usart2_tx_queue_storage, 
                                          &usart2_tx_queue_buffer);

        // Create RX queue
        usart2_rx_queue = xQueueCreateStatic(USART2_RX_QUEUE_SIZE,
                                          sizeof(rx_payload_t),
                                          usart2_rx_queue_storage,
                                          &usart2_rx_queue_buffer);

        rx_buffer = usart2_rx_buffer.data;
    }
    #endif /* USART2 */

    #ifdef USART3
    if(handle->Instance == USART3) {

        // Allocate static storage for TX queue
        usart3_tx_queue = xQueueCreateStatic(USART3_TX_QUEUE_SIZE, 
                                          sizeof(tx_payload_t), 
                                          usart3_tx_queue_storage, 
                                          &usart3_tx_queue_buffer);

        // Create RX queue
        usart3_rx_queue = xQueueCreateStatic(USART3_RX_QUEUE_SIZE,
                                          sizeof(rx_payload_t),
                                          usart3_rx_queue_storage,
                                          &usart3_rx_queue_buffer);

        rx_buffer = usart3_rx_buffer.data;
    }
    #endif /* USART3 */

    // init HAL
    if (HAL_UART_Init(handle) != HAL_OK ||
	!IS_UART_INSTANCE(handle->Instance) ||
	!IS_USART_INSTANCE(handle->Instance)){
	return UART_ERR;
    }

    // Start reception
    if (HAL_UART_Receive_IT(handle, rx_buffer, DATA_SIZE) != HAL_OK) {
        return UART_ERR;
    }

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
	uart4_tx_queue = NULL;
	uart4_rx_queue = NULL;
    }
    #endif /* UART4 */
    
    #ifdef UART5
    if (handle->Instance == UART5) {
	uart5_tx_queue = NULL;
	uart5_rx_queue = NULL;
    }
    #endif /* UART5 */
    
    #ifdef USART1
    if (handle->Instance == USART1) {
	usart1_tx_queue = NULL;
	usart1_rx_queue = NULL;
    }
    #endif /* USART1 */

    #ifdef USART2
    if (handle->Instance == USART2) {
	usart2_tx_queue = NULL;
	usart2_rx_queue = NULL;
    }
    #endif /* USART2 */

    #ifdef USART3
    if (handle->Instance == USART3) {
	usart3_tx_queue = NULL;
	usart3_rx_queue = NULL;
    }
    #endif /* USART3 */

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
#endif /* UART4 */

#ifdef UART5
// static buffer for UART5 TX
static uint8_t uart5_tx_buffer[UART5_TX_QUEUE_SIZE]; // make buffer size same as queue size 
#endif /* UART5 */

#ifdef USART1
// static buffer for USART1 TX
static uint8_t usart1_tx_buffer[USART1_TX_QUEUE_SIZE];
#endif /* USART1 */

#ifdef USART2
// static buffer for USART2 TX
static uint8_t usart2_tx_buffer[USART2_TX_QUEUE_SIZE]; // make buffer size same as queue size 
#endif /* USART2 */

#ifdef USART3
// static buffer for USART3 TX
static uint8_t usart3_tx_buffer[USART3_TX_QUEUE_SIZE]; // make buffer size same as queue size 
#endif /* USART3 */

uart_status_t uart_send(UART_HandleTypeDef* handle, const uint8_t* data, uint8_t length, TickType_t delay_ticks) {
    if (length == 0 || !is_uart_initialized(handle)) { // check if UART is initialized and data length is not 0
        return UART_ERR;
    }

    QueueHandle_t* tx_queue = NULL;
    uint8_t* tx_buffer = NULL;

    #ifdef UART4
    if(handle->Instance == UART4) {
        tx_buffer = uart4_tx_buffer; // for direct transmission
        tx_queue = &uart4_tx_queue;  // for queuing
    }
    #endif /* UART4*/

    #ifdef UART5
    if(handle->Instance == UART5) {
        tx_buffer = uart5_tx_buffer;
        tx_queue = &uart5_tx_queue;
    }
    #endif /* UART5 */

    #ifdef USART1
    if(handle->Instance == USART1) {
        tx_buffer = usart1_tx_buffer; // for direct transmission
        tx_queue = &usart1_tx_queue;  // for queuing
    }
    #endif /* USART1*/

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
  

    uart_status_t status = UART_SENT;

    // Try direct transmission if possible
    portENTER_CRITICAL();
    if ((HAL_UART_GetState(handle) & HAL_UART_STATE_BUSY_TX) != HAL_UART_STATE_BUSY_TX &&
        tx_queue != NULL && uxQueueMessagesWaiting (*tx_queue) == 0 ) { // check if UART is ready and queue is empty
        // Copy data to static buffer
        memcpy(tx_buffer, data, length);
        if (HAL_UART_Transmit_IT(handle, tx_buffer, length) != HAL_OK) {
            status = UART_ERR;
        }
        portEXIT_CRITICAL();
        goto exit;
    }
    portEXIT_CRITICAL();

    // Send data in chunks based on DATA_SIZE
    for (uint8_t i = 0; i < length; i+=DATA_SIZE) {
	tx_payload_t payload;

	// Ensure we only copy DATA_SIZE bytes at a time
	uint8_t chunk_size = (length - i < DATA_SIZE) ? (length - i) : DATA_SIZE;
	// EX: i=4, length=6, DataSize=4, then chunk_size = 2, instead of usual 4 since we've reached end of length

	// Copy the appropriate number of bytes to the payload data
	 memcpy(payload.data, &data[i], chunk_size); // Usually chunk_size = DATA_SIZE until end of data length

	 // If data size is smaller than DATA_SIZE, fill the rest of the payload
	 if (chunk_size < DATA_SIZE) {
	     memset(&payload.data[chunk_size], 0, DATA_SIZE - chunk_size); // Fill the rest with 0 (or other padding if needed)
	 }

	// Enqueue the payload to be transmitted
	if (xQueueSend(*tx_queue, &payload, delay_ticks) != pdTRUE) {
	    return UART_ERR;
	} //delay_ticks: 0 = no wait, portMAX_DELAY = wait until space is available
    }

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
    if (!data || length == 0 || !is_uart_initialized(handle)) { // check if data is not null, length is not 0 and UART is initialized
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

    uart_status_t status = UART_RECV;
    rx_payload_t receivedPayload;
    uint8_t bytes_received = 0;

    // Receive all requested bytes
    while (bytes_received < length) {
        if (xQueueReceive(rx_queue, &receivedPayload, delay_ticks) == errQUEUE_EMPTY) {
            return UART_EMPTY;  // Queue empty, no more data to receive
        }

        // Calculate how many bytes to copy from the payload based on DATA_SIZE
        uint8_t copy_length = (length - bytes_received) >= DATA_SIZE ? DATA_SIZE : (length - bytes_received);

        // Copy the data from the payload to the user's data buffer
        for (uint8_t i = 0; i < copy_length; i++) {
            data[bytes_received + i] = receivedPayload.data[i];
        }

        // Update the number of bytes received
        bytes_received += copy_length;
    }

    return status;
}

// Transmit Callback occurs after a transmission if complete (depending on how huart is configure)
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    uint8_t tx_buffer[32];  // Buffer for collecting bytes to send
    uint8_t count = 0;

    QueueHandle_t *tx_queue = NULL;

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

    #ifdef USART1
    if(huart->Instance == USART1) {
        tx_queue = &usart1_tx_queue;
    }
    #endif /* USART1 */

    #ifdef USART2
    if(huart->Instance == USART2) {
        tx_queue = &usart2_tx_queue;
    }
    #endif /* USART2 */

    #ifdef USART3
    if(huart->Instance == USART3) {
        tx_queue = &usart3_tx_queue;
    }
    #endif /* USART3 */

    // Pull as many bytes as we can fit in the buffer
    tx_payload_t payload;
    while(count + DATA_SIZE <= sizeof(tx_buffer) && xQueueReceiveFromISR(*tx_queue, &payload, &higherPriorityTaskWoken) == pdTRUE) {
        // Safely copy the data from the payload into the tx_buffer
    	memcpy(&tx_buffer[count], payload.data, DATA_SIZE);
        count += DATA_SIZE;
    }

    if(tx_queue == NULL) {
        return; // No valid uart tx queue found
    }

    // Get as many bytes as we can from queue (up to buffer size)
    while(count < sizeof(tx_buffer) && 
        xQueueReceiveFromISR(*tx_queue, &tx_buffer[count], &higherPriorityTaskWoken) == pdTRUE) {
        count++;
    }

    // 	If we got any bytes, transmit them
    if(count > 0) {
      HAL_UART_Transmit_IT(huart, tx_buffer, count);
    }

    // Yield to a higher priority task if needed
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

// Receive Callback occurs after a receive is complete
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    QueueHandle_t *rx_queue = NULL;
    uint8_t *rx_buffer = NULL;
    
    #ifdef UART4
    if(huart->Instance == UART4) {
        rx_queue = &uart4_rx_queue;
        rx_buffer = uart4_rx_buffer.data;
    }
    #endif /* UART4 */

    #ifdef UART5
    if(huart->Instance == UART5) {
        rx_queue = &uart5_rx_queue;
        rx_buffer = uart5_rx_buffer.data;
    }
    #endif /* UART5 */

    #ifdef USART1
    if(huart->Instance == USART1) {
        rx_queue = &usart1_rx_queue;
        rx_buffer = usart1_rx_buffer.data;
    }
    #endif /* USART1 */

    #ifdef USART2
    if(huart->Instance == USART2) {
        rx_queue = &usart2_rx_queue;
        rx_buffer = usart2_rx_buffer.data;
    }
    #endif /* USART2 */

    #ifdef USART3
    if(huart->Instance == USART3) {
        rx_queue = &usart3_rx_queue;
        rx_buffer = usart3_rx_buffer.data;
    }
    #endif /* USART3 */

    rx_payload_t receivedData;
    for (int i = 0; i < DATA_SIZE; i++) {
        receivedData.data[i] = rx_buffer[i]; //uartN_rx_buffer.data
	rx_buffer[i] = 0x00; // clear rx buffer element
    }

    BaseType_t higherPriorityTaskWoken = pdFALSE;

    xQueueSendFromISR(*rx_queue, &receivedData, &higherPriorityTaskWoken); // Send data from &receivedData(pRxBuffPtr) to rx_queue

    // Trigger the next interrupt
    HAL_UART_Receive_IT(huart, rx_buffer, DATA_SIZE);// pRxBufferPtr is a pointer to the buffer that will store the received data

    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

#ifdef UART4
void UART4_IRQHandler(void) {
    HAL_UART_IRQHandler(huart4);
}
#endif /* UART4 */

#ifdef UART5
void UART5_IRQHandler(void) {
    HAL_UART_IRQHandler(huart5);
}
#endif /* UART5 */

#ifdef USART1
void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(husart1);
}
#endif /* USART1 */

#ifdef USART2
void USART2_IRQHandler(void) {
    HAL_UART_IRQHandler(husart2);
}
#endif /* USART2 */

#ifdef USART3
void USART3_IRQHandler(void) {
    HAL_UART_IRQHandler(husart3);
}
#endif /* USART3 */
