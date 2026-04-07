#include "UART.h"
#include <string.h>
#include <stdint.h>
#include "portmacro.h"
#include "stm32xx_hal.h"

// Define the size of the data to be transmitted
// may need to be configured for support for packets less more than 8 bits
#ifndef UART_TX_DATA_SIZE
#define UART_TX_DATA_SIZE (64)
#endif

#ifndef UART_RX_DATA_SIZE
#define UART_RX_DATA_SIZE (32)
#endif

#ifndef UART_SINGLE_TX_SIZE
#define UART_SINGLE_TX_SIZE (64)
#endif

// Define the preemption priority for the interrupt
// Must be at least above configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY (defined in FreeRTOSConfig.h) for fromISR functions to work
#ifndef UART_NVIC_PREEMPT_PRIO
#define UART_NVIC_PREEMPT_PRIO (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#endif

typedef enum {
    STATIC_BUFFER,
    RAW_BYTES
} UART_tx_data_type_t;

typedef struct {
    UART_tx_data_type_t type;
    union {
        struct {
            uint8_t bytes[UART_TX_DATA_SIZE]; // data to be transmitted
            uint8_t len;
        } raw_bytes;
        struct {
            const uint8_t *data;
            uint16_t len;
            SemaphoreHandle_t release_sem;
        } static_buffer;
    } data;
} UART_tx_payload_t;

typedef struct {
    uint8_t data[UART_RX_DATA_SIZE]; // data received
} UART_rx_payload_t;

// UART metadata
typedef struct {
    UART_HandleTypeDef huart;

    uint16_t tx_queue_size; // number of UART_tx_payload_t in tx_queue
    QueueHandle_t tx_queue; // queue handle
    StaticQueue_t tx_queue_buffer; // static queue info (required for initialization)
    uint8_t *tx_queue_storage; // storage for tx queue
    uint8_t *tx_dir_buffer; // buffer for in-progress direct transmission
    SemaphoreHandle_t tx_queue_mutex; // used to ensure ordering in case of multiple users of same TX queue
    StaticSemaphore_t tx_queue_mutex_buffer; // static mutex info (required for initialization)
    SemaphoreHandle_t buf_mtx; // for uart_send_buf
    volatile bool tx_active;

    uint16_t rx_queue_size; // number of UART_rx_payload_t in rx_queue
    QueueHandle_t rx_queue; // queue handle
    StaticQueue_t rx_queue_buffer; // static queue info (required for initialization)
    uint8_t *rx_queue_storage; // storage for rx queue
    UART_rx_payload_t rx_buffer; // intermediate buffer of UART_RX_DATA_SIZE bytes to store received data before it is copied to the queue
} UART_periph_t;

// helper to trigger background interrupts
static void uart_transmit(UART_HandleTypeDef *huart, BaseType_t *higherPriorityTaskWoken);

#define UART_STRUCTURE(uart_name, UART_INSTANCE, TX_Q_SIZE, RX_Q_SIZE) \
static uint8_t uart_name##_tx_queue_storage[(TX_Q_SIZE) * sizeof(UART_tx_payload_t)]; \
static uint8_t uart_name##_tx_dir_buffer[UART_SINGLE_TX_SIZE]; \
static uint8_t uart_name##_rx_queue_storage[(RX_Q_SIZE) * sizeof(UART_rx_payload_t)]; \
static UART_periph_t uart_name = { \
    .huart = { \
        .Instance = (UART_INSTANCE) \
    }, \
    .tx_queue_size = (TX_Q_SIZE), \
    .tx_queue = NULL, \
    .tx_queue_storage = uart_name##_tx_queue_storage, \
    .tx_dir_buffer = uart_name##_tx_dir_buffer, \
    .tx_active = false, \
    .rx_queue_size = (RX_Q_SIZE), \
    .rx_queue = NULL, \
    .rx_queue_storage = uart_name##_rx_queue_storage \
}; \
UART_HandleTypeDef *h##uart_name = &uart_name.huart;

#ifdef UART4
// fallback UART4 TX queue size
#ifndef UART4_TX_QUEUE_SIZE
#define UART4_TX_QUEUE_SIZE (20)
#endif

// fallback UART4 RX queue size
#ifndef UART4_RX_QUEUE_SIZE
#define UART4_RX_QUEUE_SIZE (20)
#endif

// UART4 peripheral
UART_STRUCTURE(uart4, UART4, UART4_TX_QUEUE_SIZE, UART4_RX_QUEUE_SIZE)

#endif /* UART4 */

#ifdef UART5
// fallback UART5 TX queue size
#ifndef UART5_TX_QUEUE_SIZE
#define UART5_TX_QUEUE_SIZE (20)
#endif

// fallback UART5 RX queue size
#ifndef UART5_RX_QUEUE_SIZE
#define UART5_RX_QUEUE_SIZE (20)
#endif

// UART5 peripheral
UART_STRUCTURE(uart5, UART5, UART5_TX_QUEUE_SIZE, UART5_RX_QUEUE_SIZE)

#endif /* UART5 */

#ifdef USART1
// fallback USART1 TX queue size
#ifndef USART1_TX_QUEUE_SIZE
#define USART1_TX_QUEUE_SIZE (20)
#endif

// fallback USART1 RX queue size
#ifndef USART1_RX_QUEUE_SIZE
#define USART1_RX_QUEUE_SIZE (20)
#endif

// USART1 peripheral
UART_STRUCTURE(usart1, USART1, USART1_TX_QUEUE_SIZE, USART1_RX_QUEUE_SIZE)

#endif /* USART1 */

#ifdef USART2
// fallback USART2 TX queue size
#ifndef USART2_TX_QUEUE_SIZE
#define USART2_TX_QUEUE_SIZE (20)
#endif

// fallback USART2 RX queue size
#ifndef USART2_RX_QUEUE_SIZE
#define USART2_RX_QUEUE_SIZE (20)
#endif

// USART2 peripheral
UART_STRUCTURE(usart2, USART2, USART2_TX_QUEUE_SIZE, USART2_RX_QUEUE_SIZE)

#endif /* USART2 */

#ifdef USART3
// fallback USART3 TX queue size
#ifndef USART3_TX_QUEUE_SIZE
#define USART3_TX_QUEUE_SIZE (20)
#endif

// fallback USART3 RX queue size
#ifndef USART3_RX_QUEUE_SIZE
#define USART3_RX_QUEUE_SIZE (20)
#endif

// USART3 peripheral
UART_STRUCTURE(usart3, USART3, USART3_TX_QUEUE_SIZE, USART3_RX_QUEUE_SIZE)

#endif /* USART3 */

#ifdef LPUART1
#ifndef LPUART1_TX_QUEUE_SIZE
#define LPUART1_TX_QUEUE_SIZE (20)
#endif  

#ifndef LPUART1_RX_QUEUE_SIZE
#define LPUART1_RX_QUEUE_SIZE (20)
#endif 

// LPUART1 peripheral
UART_STRUCTURE(lpuart1, LPUART1, LPUART1_TX_QUEUE_SIZE, LPUART1_RX_QUEUE_SIZE)

#endif /* LPUART1 */

static bool is_uart_initialized(UART_HandleTypeDef* handle) {
    // Check if the UART is in a valid state
    // HAL_UART_STATE_RESET indicates the UART is not initialized
    return (HAL_UART_GetState(handle) != HAL_UART_STATE_RESET);
}

static UART_periph_t *get_valid_uart_periph(UART_HandleTypeDef *handle){
    UART_periph_t *uart_periph = NULL;
    bool is_valid_uart_instance = false;

    #ifdef LPUART1
    if(IS_LPUART_INSTANCE(handle->Instance)){
        is_valid_uart_instance = true;
    }
    #endif /* LPUART1 */
    if(IS_UART_INSTANCE(handle->Instance) || IS_USART_INSTANCE(handle->Instance)){
        is_valid_uart_instance = true;
    }

    if(!is_valid_uart_instance){
        return NULL;
    }

    #ifdef UART4
    if (handle->Instance == UART4) {
        uart_periph = &uart4;
    }
    #endif /* UART4 */

    #ifdef UART5
    if(handle->Instance == UART5) {
        uart_periph = &uart5;
    }
    #endif /* UART5 */
    
    #ifdef USART1
    if(handle->Instance == USART1) {
        uart_periph = &usart1;
    }
    #endif /* USART1 */

    #ifdef USART2
    if(handle->Instance == USART2) {
        uart_periph = &usart2;
    }
    #endif /* USART2 */

    #ifdef USART3
    if(handle->Instance == USART3) {
        uart_periph = &usart3;
    }
    #endif /* USART3 */

    #ifdef LPUART1
    if(handle->Instance == LPUART1) {
        uart_periph = &lpuart1;
    }
    #endif /* LPUART1 */

    return uart_periph;
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
    
    #ifdef LPUART1
    if(huart->Instance == LPUART1) {

        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* PA2 -> LPUART1_TX
        PA3 -> LPUART1_RX */
        init.Pin = GPIO_PIN_2 | GPIO_PIN_3;
        init.Mode = GPIO_MODE_AF_PP;
        init.Pull = GPIO_NOPULL;
        init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        init.Alternate = GPIO_AF8_LPUART1;

        HAL_GPIO_Init(GPIOA, &init);
    }
    #endif
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

    #ifdef LPUART1
    if (huart->Instance == LPUART1) {
        __HAL_RCC_LPUART1_CLK_ENABLE();
        uart_IRQ = LPUART1_IRQn;
    }
    #endif /* LPUART1 */

    // configure GPIO pins for UART
    HAL_UART_MspGPIOInit(huart); 

    // enable uart interrupts
    HAL_NVIC_SetPriority(uart_IRQ, UART_NVIC_PREEMPT_PRIO, 0);
    HAL_NVIC_EnableIRQ(uart_IRQ); 
}

// Redefine me!
__weak void HAL_UART_MspGPIODeInit(UART_HandleTypeDef *huart){ 
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

    #ifdef LPUART1
    if (huart->Instance == LPUART1) {

        __HAL_RCC_LPUART1_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(LPUART1_IRQn);      //disable interrupts

    }
    #endif

    HAL_UART_MspGPIODeInit(huart);
}


/**
 * @brief Initializes the UART peripheral
 * @param handle pointer to the UART handle
 * @return uart_status_t
 */
uart_status_t uart_init(UART_HandleTypeDef* handle) {
    UART_periph_t *uart_periph = get_valid_uart_periph(handle);
    if(uart_periph == NULL) return UART_ERR; 

    // Create TX queue
    uart_periph->tx_queue = xQueueCreateStatic(uart_periph->tx_queue_size, 
                                        sizeof(UART_tx_payload_t), 
                                        uart_periph->tx_queue_storage, 
                                        &uart_periph->tx_queue_buffer);
    
    
    // Create RX queue
    uart_periph->rx_queue = xQueueCreateStatic(uart_periph->rx_queue_size,
                                        sizeof(UART_rx_payload_t),
                                        uart_periph->rx_queue_storage,
                                        &uart_periph->rx_queue_buffer);

    uart_periph->tx_queue_mutex = xSemaphoreCreateMutexStatic(&uart_periph->tx_queue_mutex_buffer);
    uart_periph->buf_mtx = NULL;

    // init HAL
    if(HAL_UART_Init(handle) != HAL_OK){
        return UART_ERR;
    }

    // Start reception
    if (HAL_UART_Receive_IT(handle, uart_periph->rx_buffer.data, UART_RX_DATA_SIZE) != HAL_OK) {
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
    UART_periph_t *uart_periph = get_valid_uart_periph(handle);
    if(uart_periph == NULL) return UART_ERR;

    // Stop any ongoing transfers first
    HAL_UART_Abort(handle);

    // Deinitialize HAL
    if (HAL_UART_DeInit(handle) != HAL_OK) {
        return UART_ERR; 
    }

    // Deinitialize Handler
    uart_periph->tx_queue = NULL;
    uart_periph->rx_queue = NULL;

    return UART_OK;
}

/**
 * @brief Transmits data over UART. If transmission is in progress, data will be queued in internal TX queue. 
 * @param data pointer to the data buffer that will be written/transmitted
 * @param length of the buffer that will be written/transmitted
 * @param delay_ticks number of ticks to wait for data to be transmitted
 * @return uart_status_t
 */
uart_status_t uart_send(UART_HandleTypeDef* handle, const uint8_t* data, uint16_t length, TickType_t delay_ticks) {
    if (!data || length == 0 || !is_uart_initialized(handle)) { // check if UART is initialized and data length is not 0
        return UART_ERR;
    }

    UART_periph_t *uart_periph = get_valid_uart_periph(handle);
    if(uart_periph == NULL) return UART_ERR;

    // Try direct transmission if possible
    portENTER_CRITICAL();
    if (!uart_periph->tx_active &&
        uxQueueMessagesWaiting (uart_periph->tx_queue) == 0 &&
        length <= UART_SINGLE_TX_SIZE) { // check if UART is ready and queue is empty and length is enough to fit in single tx buffer
        // Copy all input data to static buffer
        memcpy(uart_periph->tx_dir_buffer, data, length);
        uart_periph->tx_active = true;
        portEXIT_CRITICAL();

        if (HAL_UART_Transmit_IT(handle, uart_periph->tx_dir_buffer, length) != HAL_OK) {
            uart_periph->tx_active = false;
            return UART_ERR;
        }

        return UART_OK;
    }
    portEXIT_CRITICAL();

    // Send data in chunks based on UART_TX_DATA_SIZE
    if(xSemaphoreTake(uart_periph->tx_queue_mutex, delay_ticks) != pdTRUE) return UART_ERR; // if we timeout, err out
    for (uint16_t i = 0; i < length; i+=UART_TX_DATA_SIZE) {
	UART_tx_payload_t payload;
        payload.type = RAW_BYTES;

	// Ensure we only copy UART_TX_DATA_SIZE bytes at a time
	payload.data.raw_bytes.len = (length - i < UART_TX_DATA_SIZE) ? (length - i) : UART_TX_DATA_SIZE;
	// EX: i=4, length=6, DataSize=4, then chunk_size = 2, instead of usual 4 since we've reached end of length

        // Copy the appropriate number of bytes to the payload data
	memcpy(payload.data.raw_bytes.bytes, &data[i], payload.data.raw_bytes.len); // Usually chunk_size = UART_TX_DATA_SIZE until end of data length

        // If the queue is completely full, and uart tx is NOT active, we deadlock waiting for the queue to open up.
        // Kickstart here if that's the case so we can keep adding our messages
        if(!uart_periph->tx_active && uxQueueSpacesAvailable(uart_periph->tx_queue) == 0 ) {
            BaseType_t higherPriorityTaskWoken;

            portENTER_CRITICAL();
            if(!uart_periph->tx_active){
                uart_transmit(handle, &higherPriorityTaskWoken);
            }
            portEXIT_CRITICAL();
        }

	// Enqueue the payload to be transmitted
	if (xQueueSend(uart_periph->tx_queue, &payload, delay_ticks) != pdTRUE) {
            xSemaphoreGive(uart_periph->tx_queue_mutex);
	    return UART_ERR;
	} // delay_ticks: 0 = no wait, portMAX_DELAY = wait until space is available
    }
    xSemaphoreGive(uart_periph->tx_queue_mutex);

    // If the background interrupts are not active we need to kickstart them
    portENTER_CRITICAL();
    if(!uart_periph->tx_active) {
        BaseType_t higherPriorityTaskWoken = pdFALSE;
        uart_transmit(handle, &higherPriorityTaskWoken);
    }
    portEXIT_CRITICAL();

    return UART_OK;
}

/**
 * @brief Transmits a mutexed buffer over UART. The mutex must already be acquired before this call.
 */
uart_status_t uart_send_buf(UART_HandleTypeDef* handle, const uint8_t* data, uint16_t length, SemaphoreHandle_t release_sem, TickType_t delay_ticks) {
    if (!data || length == 0 || !is_uart_initialized(handle)) {
        return UART_ERR;
    }

    UART_periph_t *uart_periph = get_valid_uart_periph(handle);
    if(uart_periph == NULL) return UART_ERR;

    UART_tx_payload_t payload = {
        .type = STATIC_BUFFER,
        .data.static_buffer = {
            .data = data,
            .len = length,
            .release_sem = release_sem,
        }
    };

    if (xQueueSend(uart_periph->tx_queue, &payload, delay_ticks) != pdTRUE) {
        return UART_ERR;
    }

    portENTER_CRITICAL();
    if (!uart_periph->tx_active) {
        BaseType_t higherPriorityTaskWoken = pdFALSE;
        uart_transmit(handle, &higherPriorityTaskWoken);
    }
    portEXIT_CRITICAL();

    return UART_OK;
}

/**
 * @brief Reads data from the user-provided RX queue
 * @param huart pointer to the UART handle
 * @param data pointer to the data buffer that will be read into
 * @param length of the buffer that will be read
 * @param delay_ticks number of ticks to wait for data to be received
 * @return uart_status_t
 */
uart_status_t uart_recv(UART_HandleTypeDef* handle, uint8_t* data, uint16_t length, TickType_t delay_ticks) {
    if (!data || length == 0 || !is_uart_initialized(handle)) { // check if data is not null, length is not 0 and UART is initialized
        return UART_ERR;
    }

    UART_periph_t *uart_periph = get_valid_uart_periph(handle);
    if(uart_periph == NULL) return UART_ERR;

    uart_status_t status = UART_OK;
    UART_rx_payload_t receivedPayload;
    uint16_t bytes_received = 0;

    // Receive all requested bytes
    while (bytes_received < length) {
        if (xQueueReceive(uart_periph->rx_queue, &receivedPayload, delay_ticks) == errQUEUE_EMPTY) {
            return UART_EMPTY;  // Queue empty, no more data to receive
        }

        // Calculate how many bytes to copy from the payload based on UART_RX_DATA_SIZE
        uint8_t copy_length = (length - bytes_received) >= UART_RX_DATA_SIZE ? UART_RX_DATA_SIZE : (length - bytes_received);

        // Copy the data from the payload to the user's data buffer
        memcpy(&data[bytes_received], receivedPayload.data, copy_length);

        // Update the number of bytes received
        bytes_received += copy_length;
    }

    return status;
}

// MUST BE CALLED FROM ISR OR CRIT SECTION
static void uart_transmit(UART_HandleTypeDef *huart, BaseType_t *higherPriorityTaskWoken){
    uint16_t count = 0;

    UART_periph_t *uart_periph = get_valid_uart_periph(huart);
    if(uart_periph == NULL) return;

    // Pull as many bytes as we can fit in the buffer
    UART_tx_payload_t payload;
    if (xQueuePeekFromISR(uart_periph->tx_queue, &payload) != pdTRUE) {
        uart_periph->tx_active = false;
        return;
    }
    
    switch(payload.type){
        case STATIC_BUFFER:
            xQueueReceiveFromISR(uart_periph->tx_queue, &payload, higherPriorityTaskWoken);
            uart_periph->buf_mtx = payload.data.static_buffer.release_sem;
            if(payload.data.static_buffer.len > 0){
                uart_periph->tx_active = true;
            }

            if(HAL_UART_Transmit_IT(&uart_periph->huart, payload.data.static_buffer.data, payload.data.static_buffer.len) != HAL_OK){
                uart_periph->tx_active = false;
                uart_periph->buf_mtx = NULL;
                xSemaphoreGiveFromISR(payload.data.static_buffer.release_sem, higherPriorityTaskWoken);
            }

            break;
        case RAW_BYTES:
            while(xQueuePeekFromISR(uart_periph->tx_queue, &payload) == pdTRUE) { // there's still something in queue?
                if(payload.type != RAW_BYTES) break;
                if(count + payload.data.raw_bytes.len > UART_SINGLE_TX_SIZE) break;
                xQueueReceiveFromISR(uart_periph->tx_queue, &payload, higherPriorityTaskWoken); // pop from queue

                // Safely copy the data from the payload into the tx_buffer
                memcpy(&(uart_periph->tx_dir_buffer[count]), payload.data.raw_bytes.bytes, payload.data.raw_bytes.len);
                count += payload.data.raw_bytes.len;
            }

            // If we got any bytes, transmit them
            if(count > 0) {
                uart_periph->tx_active = true;
                if(HAL_UART_Transmit_IT(huart, uart_periph->tx_dir_buffer, count) != HAL_OK){
                    uart_periph->tx_active = false;
                }
            } else {
                uart_periph->tx_active = false;
            }
            break;
    }
}

// Transmit Callback occurs after a transmission if complete (depending on how huart is configure)
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    UART_periph_t *uart_periph = get_valid_uart_periph(huart);

    SemaphoreHandle_t sem = uart_periph->buf_mtx;
    uart_periph->buf_mtx = NULL;
    uart_transmit(huart, &higherPriorityTaskWoken);
    if(sem) xSemaphoreGiveFromISR(sem, &higherPriorityTaskWoken);

    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

// Receive Callback occurs after a receive is complete
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    UART_periph_t *uart_periph = get_valid_uart_periph(huart);
    if(uart_periph == NULL) return;

    UART_rx_payload_t receivedData;
    for (int i = 0; i < UART_RX_DATA_SIZE; i++) {
        receivedData.data[i] = uart_periph->rx_buffer.data[i];
    }

    BaseType_t higherPriorityTaskWoken = pdFALSE;

    xQueueSendFromISR(uart_periph->rx_queue, &receivedData, &higherPriorityTaskWoken); // Send data from &receivedData(pRxBuffPtr) to rx_queue

    // Trigger the next interrupt
    HAL_UART_Receive_IT(huart, uart_periph->rx_buffer.data, UART_RX_DATA_SIZE);// pRxBufferPtr is a pointer to the buffer that will store the received data

    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

#ifdef UART4
void UART4_IRQHandler(void) {
    HAL_UART_IRQHandler(&uart4.huart);
}
#endif /* UART4 */

#ifdef UART5
void UART5_IRQHandler(void) {
    HAL_UART_IRQHandler(&uart5.huart);
}
#endif /* UART5 */

#ifdef USART1
void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&usart1.huart);
}
#endif /* USART1 */

#ifdef USART2
void USART2_IRQHandler(void) {
    HAL_UART_IRQHandler(&usart2.huart);
}
#endif /* USART2 */

#ifdef USART3
void USART3_IRQHandler(void) {
    HAL_UART_IRQHandler(&usart3.huart);
}
#endif /* USART3 */

#ifdef LPUART1
void LPUART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&lpuart1.huart);
}
#endif /* LPUART1 */
