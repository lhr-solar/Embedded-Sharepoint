#include <stdio.h>
#include "BSP_CAN.h"

// int main(void)
// {
    // CAN_TxPayload_t payload = (CAN_TxPayload_t){
    //     .header = (CAN_TxHeaderTypeDef){
    //         .StdId = 0x32,
    //     },
    //     .data = {1, 2, 3, 4, 5, 6, 7, 8}
    // };

    // /* init can */
    // CAN_CarCANInit();

    // /* send payload */
    // CAN_Send(CarCAN, payload, false);

//     while(1) {}
// }

CAN_HandleTypeDef     CanHandle;
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;

int main(void)
{
    /*##-1- Configure the CAN peripheral #######################################*/
    CanHandle.Instance = CAN1;
    CanHandle.Init.Prescaler = 16;
    CanHandle.Init.Mode = CAN_MODE_NORMAL;
    CanHandle.Init.SyncJumpWidth = CAN_SJW_1TQ;
    CanHandle.Init.TimeSeg1 = CAN_BS1_1TQ;
    CanHandle.Init.TimeSeg2 = CAN_BS2_1TQ;
    CanHandle.Init.TimeTriggeredMode = DISABLE;
    CanHandle.Init.AutoBusOff = DISABLE;
    CanHandle.Init.AutoWakeUp = DISABLE;
    CanHandle.Init.AutoRetransmission = DISABLE;
    CanHandle.Init.ReceiveFifoLocked = DISABLE;
    CanHandle.Init.TransmitFifoPriority = DISABLE;
    
    if(HAL_CAN_Init(&CanHandle) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }

    if (HAL_CAN_Start(&CanHandle) != HAL_OK)
    {
        /* Start Error */
        Error_Handler();
    }

    TxHeader.StdId = 0x11;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.DLC = 2;
    TxHeader.TransmitGlobalTime = DISABLE;
    TxData[0] = 0xCA;
    TxData[1] = 0xFE;
    
    /* Request transmission */
    if(HAL_CAN_AddTxMessage(&CanHandle, &TxHeader, TxData, &TxMailbox) != HAL_OK)
    {
        /* Transmission request Error */
        Error_Handler();
    }

    while(1) {}
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *canHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = (GPIO_InitTypeDef){0};

    if (canHandle->Instance == CAN1)
    {

        /* CAN1 clock enable */
        __HAL_RCC_CAN1_CLK_ENABLE();

        /* GPIOA clock enable */
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /**CAN1 GPIO Configuration
        PA11     ------> CAN1_RX
        PA12     ------> CAN1_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
    else if (canHandle->Instance == CAN3)
    {
        /* CAN3 clock enable */
        __HAL_RCC_CAN3_CLK_ENABLE();

        /* GPIOA clock enable */
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /**CAN3 GPIO Configuration
        PA8     ------> CAN3_RX
        PA15     ------> CAN3_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF11_CAN3;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}