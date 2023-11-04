#include <stdbool.h>
#include "BSP_UART.h"

static QueueHandle_t rx_queues[2];
static QueueHandle_t tx_queues[2];

//TODO needs rewrite after standard config for pins is merged in
//TODO will need to set different pins based on which UART device is passed in
UART_Init_Status BSP_UART_Init(USART_HandleTypeDef device) {
    GPIO_InitTypeDef gpio_struct;
    UART_InitTypeDef uart_struct;

    UART_Init_Status returnInfo;
    
    // Init clocks to port b, c, and usart3

    // check if they're disabled
    // if so, enable
    if (__HAL_RCC_GPIOB_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    if (__HAL_RCC_GPIOC_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    if (__HAL_RCC_USART3_IS_CLK_DISABLED()) {
        __HAL_RCC_USART3_CLK_ENABLE();
    }

    // config pins
    gpio_struct.Pin = GPIO_PIN_10;
    gpio_struct.Alternate = GPIO_AF7_USART3;
    gpio_struct.Mode = MODE_AF;
    gpio_struct.Pull = GPIO_PULLUP;
    gpio_struct.Speed = GPIO_SPEED_FREQ_LOW; // "GPIO Output Maximum Frequency"
    HAL_GPIO_Init(GPIOB, &gpio_struct);
    gpio_struct.Pin = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOC, &gpio_struct);

    // uart config
    uart_struct.BaudRate = 115200;
    uart_struct.HwFlowCtl = UART_HWCONTROL_NONE;
    uart_struct.Mode = UART_MODE_TX_RX;
    uart_struct.Parity = UART_PARITY_NONE;
    uart_struct.WordLength = UART_WORDLENGTH_8B;
    uart_struct.StopBits = UART_STOPBITS_1;
    returnInfo = CONVERT_RETURN(HAL_UART_Init(&uart_struct));

    __HAL_USART_ENABLE_IT(&device, USART_IT_RXNE); //rx
    __HAL_USART_ENABLE_IT(&device, USART_IT_TC); //tx
    __HAL_USART_ENABLE(&device); // turn interrupts on

    // 2, 0 - usart3_preempt prio, usart3_sub prio
    // custom def'd in BPS repo. Should we do the same here?
    HAL_NVIC_SetPriority(USART3_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);

    rx_queues[USART_Handle_To_Int(device)] = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));
    tx_queues[USART_Handle_To_Int(device)] = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));

    return returnInfo;
}

/**
 * @brief   Reads a message from the specified UART device
 * @param   usart which usart to read from (2 or 3)
 * @param   len number of bytes that will be returned
 * @return  len bytes from the recieve queue
 */
char* BSP_UART_Read(USART_HandleTypeDef usart, uint32_t len) {
    QueueHandle_t rx_queue = rx_queues[USART_Handle_To_Int(usart)];

    char bytesToRecieve[RX_SIZE];
    HAL_USART_Receive_IT(&usart, *bytesToRecieve, RX_SIZE);

    if (uxQueueSpacesAvailable(rx_queue) != 0) {
        for (int i = 0; i < RX_SIZE; i++) {
            BaseType_t queueStatus = xQueueSendToBack(rx_queue, bytesToRecieve[i], portMAX_DELAY);
            
            if (queueStatus != pdPASS) {
                msgDropped = true;
            }
        }
    }
    
    char returnArr[len];
    for (int i = 0; i < len; i++) {
        xQueueReceive(rx_queue, &returnArr[i], portMAX_DELAY);
    }
    
    return returnArr;
}


/**
 * @brief   Continues message recieving until recieve queue is full
 * @param   huart : handle of UART that called the interrupt
 */
void HAL_USART_RxCpltCallback(USART_HandleTypeDef *huart) {
    QueueHandle_t rx_queue = rx_queues[USART_Handle_To_Int(*huart)];
    uint8_t packetSize = uxQueueMessagesWaiting < RX_SIZE ? uxQueueMessagesWaiting : RX_SIZE;
    
    if (uxQueueSpacesAvailable(rx_queue) == 0) {
        msgDropped = true;
        return;
    }

    char recievedBytes[packetSize];
    HAL_USART_Receive_IT(huart, &recievedBytes, packetSize);

    for (int i = 0; i < packetSize; i++) {
        xQueueSendToBack(rx_queue, &recievedBytes[i], portMAX_DELAY);
    }
}

/**
 * @brief   Writes a message to the specified UART device
 * @param   usart which usart to write to (1-3) 
 * @param   data pointer to the array containing the message to sned
 * @param   len length of the message in bytes
 * @return  Status of write operation (WRITE_SUCCESS OR WRITE_FAIL)
 */
UART_Write_Status BSP_UART_Write(USART_HandleTypeDef usart, char* data, uint32_t len) {
    QueueHandle_t tx_queue = tx_queues[USART_Handle_To_Int(usart)];
    for (int i = 0; i < len; i++) {
        BaseType_t queueStatus = xQueueSendToBack(tx_queue, data[i], portMAX_DELAY);
        if (queueStatus != pdPASS) return WRITE_FAIL;
    }

    uint8_t packetSize = uxQueueMessagesWaiting(tx_queue) < TX_SIZE ? uxQueueMessagesWaiting(tx_queue) : TX_SIZE;
    char bytesToSend[packetSize];

    for (int i = 0; i < packetSize; i++) {
        xQueueReceive(tx_queue, &bytesToSend[i], portMAX_DELAY);
    }
    
    HAL_USART_Transmit_IT(&usart, &bytesToSend, packetSize);
}

/**
 * @brief   Continues message tranmission on UART device until transmit queue is empty
 * @param   huart handle of UART that called the interrupt
 */
void HAL_USART_TxCpltCallback(USART_HandleTypeDef *huart) {
    QueueHandle_t tx_queue = tx_queues[USART_Handle_To_Int(*huart)];
    uint8_t packetSize = (uxQueueMessagesWaiting(tx_queue) < TX_SIZE) ? uxQueueMessagesWaiting(tx_queue) : TX_SIZE;
    
    if (packetSize == 0) {
        return;
    }

    char bytesToSend[packetSize];

    for (int i = 0; i < packetSize; i++) {
        xQueueReceive(tx_queue, &bytesToSend[i], portMAX_DELAY);
    }
    
    HAL_USART_Transmit_IT(huart, &bytesToSend, packetSize);
}

/**
 * @brief   Convert usart handles to the index in Tx and Rx queue arrays
 * @param   usart the handle to translate to a number
 * @return  Index of the usart handle in the queue array
 */
int USART_Handle_To_Int(USART_HandleTypeDef usart) {
    if (usart.Instance == USART1) {
        return 0;
    } else if (usart.Instance == USART2) {
        return 1;
    } else if (usart.Instance == USART3) {
        return 2;
    } else {
        return -1; // Handle is not recognized, send invalid index so code seg faults.
    }
}