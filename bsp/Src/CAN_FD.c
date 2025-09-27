#include "CAN_FD.h"

#ifdef FDCAN1
static FDCAN_HandleTypeDef hfdcan1_ = {.Instance = FDCAN1};
FDCAN_HandleTypeDef* hfdcan1 = &hfdcan1_;
#endif


can_status_t can_fd_init(FDCAN_HandleTypeDef* handle, FDCAN_FilterTypeDef* filter){
    return CAN_ERR;

}

can_status_t can_fd_deinit(FDCAN_HandleTypeDef* handle){
    return CAN_ERR;

}

can_status_t can_fd_start(FDCAN_HandleTypeDef* handle){
    if (HAL_FDCAN_Start(handle) != HAL_OK){
        return CAN_ERR;
    }
    return CAN_OK;
}

can_status_t can_fd_send(FDCAN_HandleTypeDef* handle, FDCAN_TxHeaderTypeDef* header, uint8_t data[], TickType_t delay_ticks){

    // disable interrupts while we check the status of the can mailboxes, so other interrupts done change it
    portENTER_CRITICAL();
    // Check if there is a free can mailbox to send a message
    if(HAL_FDCAN_GetTxFifoFreeLevel(handle) >= 1){
        // if the mailbox is free, add the message to the fifo
        if(HAL_FDCAN_AddMessageToTxFifoQ(handle, header, data) != HAL_OK){
            // enable interrupts
            portEXIT_CRITICAL();
            return CAN_OK;
        }
        // enable interrupts
        portEXIT_CRITICAL();
    }
    else{
        // enable interrupts
        portEXIT_CRITICAL();
        return CAN_EMPTY;
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
}
