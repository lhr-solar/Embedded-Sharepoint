#include "CAN_FD.h"
#include "CAN_Common.h"

// Define CAN FD handles
#ifdef FDCAN1
static FDCAN_HandleTypeDef hfdcan1_ = {.Instance = FDCAN1};
FDCAN_HandleTypeDef* hfdcan1 = &hfdcan1_;
#endif

#ifdef FDCAN2
static FDCAN_HandleTypeDef hfdcan2_ = {.Instance = FDCAN2};
FDCAN_HandleTypeDef* hfdcan2 = &hfdcan2_;
#endif

#ifdef FDCAN3
static FDCAN_HandleTypeDef hfdcan3_ = {.Instance = FDCAN3};
FDCAN_HandleTypeDef* hfdcan3 = &hfdcan3_;
#endif

#ifdef FDCAN1
    #ifndef FDCAN1_SEND_QUEUE_SIZE
        #define FDCAN1_SEND_QUEUE_SIZE (10)
    #endif
    
    static QueueHandle_t fdcan1_send_queue = NULL;
    static StaticQueue_t fdcan1_send_queue_buffer;
    static uint8_t fdcan1_send_queue_storage[FDCAN1_SEND_QUEUE_SIZE * sizeof(can_tx_payload_t)];
#endif

#ifdef FDCAN2
    #ifndef FDCAN2_SEND_QUEUE_SIZE
        #define FDCAN2_SEND_QUEUE_SIZE (10)
    #endif

    static QueueHandle_t fdcan2_send_queue = NULL;
    static StaticQueue_t fdcan2_send_queue_buffer;
    static uint8_t fdcan2_send_queue_storage[FDCAN2_SEND_QUEUE_SIZE * sizeof(can_tx_payload_t)];
#endif

#ifdef FDCAN3
    #ifndef FDCAN3_SEND_QUEUE_SIZE
        #define FDCAN3_SEND_QUEUE_SIZE (10)
    #endif

    static QueueHandle_t fdcan3_send_queue = NULL;
    static StaticQueue_t fdcan3_send_queue_buffer;
    static uint8_t fdcan3_send_queue_storage[FDCAN3_SEND_QUEUE_SIZE * sizeof(can_tx_payload_t)];
#endif


can_status_t can_fd_init(FDCAN_HandleTypeDef* handle, FDCAN_FilterTypeDef* filter){

    if (HAL_FDCAN_Init(handle) != HAL_OK)
    {
        return CAN_ERR;
    }

    if (HAL_FDCAN_ConfigFilter(handle, filter) != HAL_OK)
    {
        return CAN_ERR;
    }

    for (int i = 0; i < can1_recv_entry_count; i++) {
      can1_recv_entries[i].queue = xQueueCreateStatic(
          can1_recv_entries[i].size, sizeof(rx_payload_t),
          can1_recv_entries[i].storage, &can1_recv_entries[i].buffer);
    }


#ifdef FDCAN1
    if(handle->Instance == FDCAN1){
        fdcan1_send_queue =
        xQueueCreateStatic(FDCAN1_SEND_QUEUE_SIZE, sizeof(can_tx_payload_t),
                           fdcan1_send_queue_storage, &fdcan1_send_queue_buffer);
    }
#endif
#ifdef FDCAN2
    if(handle->Instance == FDCAN2){
        fdcan2_send_queue =
        xQueueCreateStatic(FDCAN2_SEND_QUEUE_SIZE, sizeof(can_tx_payload_t),
                           fdcan2_send_queue_storage, &fdcan2_send_queue_buffer);
    }
#endif
#ifdef FDCAN3
    if(handle->Instance == FDCAN3){
        fdcan3_send_queue =
        xQueueCreateStatic(FDCAN3_SEND_QUEUE_SIZE, sizeof(can_tx_payload_t),
                           fdcan3_send_queue_storage, &fdcan3_send_queue_buffer);
    }
#endif
    return CAN_OK;
}

can_status_t can_fd_deinit(FDCAN_HandleTypeDef* handle){
    return CAN_ERR;
}

can_status_t can_fd_start(FDCAN_HandleTypeDef* handle){

    // activate interrupts for rx and tx interrupts
    if(HAL_FDCAN_ActivateNotification(handle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_TX_COMPLETE, 0) != HAL_OK){
        return CAN_ERR;
    }

    // start the can peripheral
    if (HAL_FDCAN_Start(handle) != HAL_OK){
        return CAN_ERR;
    }
    return CAN_OK;
}

can_status_t can_fd_send(FDCAN_HandleTypeDef* handle, FDCAN_TxHeaderTypeDef* header, uint8_t data[], TickType_t delay_ticks){

    if(handle == NULL || header == NULL || data == NULL){
        return CAN_ERR;
    }

    // disable interrupts while we check the status of the can mailboxes, so other interrupts done change it
    portENTER_CRITICAL();
    // Check if there is a free can mailbox to send a message
    if(HAL_FDCAN_GetTxFifoFreeLevel(handle) >= 1){
        // if the mailbox is free, add the message to the hardware fifo
        if(HAL_FDCAN_AddMessageToTxFifoQ(handle, header, data) != HAL_OK){
            // If adding to the can fd mailbox was not succesful

            // enable interrupts
            portEXIT_CRITICAL();
            return CAN_ERR;
        }
        // enable interrupts
        portEXIT_CRITICAL();
        return CAN_SENT;
    }

    // hardware mailbox is full, so must add to the queue
    else{
        // enable interrupts
        portEXIT_CRITICAL();

        can_tx_payload_t payload = {0};
        payload.header = *header;
        for (int i = 0; i < CAN_DATA_SIZE; i++) {
            payload.data[i] = data[i];
        }
#ifdef FDCAN1
        if (handle->Instance == FDCAN1) {
            if (xQueueSend(fdcan1_send_queue, &payload, delay_ticks) != pdTRUE) {
                return CAN_ERR;
            }
        }
#endif

    }
    return CAN_OK;
}

__weak void HAL_FDCAN_MspGpioInit(FDCAN_HandleTypeDef* hfdcan){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
#ifdef FDCAN1
    if(hfdcan->Instance==FDCAN1){
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /**
        FDCAN1 GPIO Configuration
        PA11     ------> FDCAN1_RX
        PA12     ------> FDCAN1_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
#endif /* FDCAN1 */

}

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* hfdcan){
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

#ifdef FDCAN1
    if(hfdcan->Instance==FDCAN1)
    {
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
        PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
        {
            Error_Handler();
        }
        __HAL_RCC_FDCAN_CLK_ENABLE();
    
    }
#endif /* FDCAN1 */ 
    HAL_FDCAN_MspGpioInit(hfdcan);

    /* FDCAN1 interrupt Init */
    // TODO: make this configurable depending on the FDCAN instance
    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
    HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
}


void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  {

  }
}

void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes)
{

    BaseType_t higherPriorityTaskWoken = pdFALSE;
    can_tx_payload_t payload = {0};

#ifdef FDCAN1
    if (hfdcan->Instance == FDCAN1){
        // check if data in the queue to send
        if (xQueueReceiveFromISR(fdcan1_send_queue, &payload, NULL) == pdTRUE) {
            HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &payload.header, payload.data);
        }
    }
#endif

  portYIELD_FROM_ISR(higherPriorityTaskWoken);

}
