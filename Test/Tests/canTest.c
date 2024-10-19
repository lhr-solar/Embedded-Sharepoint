#include <stdint.h>
#include <stm32xx_hal.h> // Adjust based on your STM32 family

CAN_HandleTypeDef hcan;

void SystemClock_Config(void);
void CAN_Init(void);
void CAN_Tx(void);
void CAN_Rx(void);
void Error_Handler(void);
void LED_Init(void); // Initialize LEDs



void LED_Init(void) {
    // Initialize PB3, PB4, and PB5 as output pins for LEDs
    __HAL_RCC_GPIOB_CLK_ENABLE(); // Enable clock for GPIOB
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void CAN_Init(void) {
    // Initialize CAN peripheral in loopback mode
    hcan.Instance = CAN1;
    hcan.Init.Prescaler = 16;  // Adjust based on baud rate
    hcan.Init.Mode = CAN_MODE_LOOPBACK; // Set CAN to loopback mode
    hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan.Init.TimeSeg1 = CAN_BS1_1TQ;
    hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
    hcan.Init.AutoBusOff = DISABLE;
    hcan.Init.AutoWakeUp = DISABLE;
    hcan.Init.AutoRetransmission = DISABLE;
    hcan.Init.ReceiveFifoLocked = DISABLE;
    hcan.Init.TransmitFifoPriority = DISABLE;

    if (HAL_CAN_Init(&hcan) != HAL_OK) {
        Error_Handler();
    }

    // Start the CAN module
    if (HAL_CAN_Start(&hcan) != HAL_OK) {
        Error_Handler();
    }

    // Activate notification for RX messages (optional)
    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        Error_Handler();
    }
}

void CAN_Tx(void) {
    CAN_TxHeaderTypeDef TxHeader;
    uint8_t TxData[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}; // Example data
    uint32_t TxMailbox;

    TxHeader.DLC = 8; // Data length
    TxHeader.IDE = CAN_ID_STD;  // Standard CAN ID
    TxHeader.StdId = 0x123;  // Example CAN ID
    TxHeader.RTR = CAN_RTR_DATA;  // Data frame

    if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) == HAL_OK) {
        // Transmission successful, toggle PB3 LED
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
    } else {
        // Transmission failed, turn on PB5 LED (error indicator)
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    }
}

void CAN_Rx(void) {
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];

    // Poll for a received message
    if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {
        // Reception successful, toggle PB4 LED
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
    } else {
        // Reception failed, turn on PB5 LED (error indicator)
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    }
}

void SystemClock_Config(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE(); // Enable clock for GPIOB

}

void Error_Handler(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    while (1); 
    //test
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    LED_Init();  // Initialize GPIO pins for LEDs
    CAN_Init();  // Initialize CAN peripheral

    while (1) {
        CAN_Tx();     
        CAN_Rx();
        
        HAL_Delay(1000); // 1-second delay between transmissions
    }


    return 0;
}