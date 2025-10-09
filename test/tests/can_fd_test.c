#include "stm32xx_hal.h"
#include "CAN_FD.h"

StaticTask_t task_buffer;
StackType_t task_stack[configMINIMAL_STACK_SIZE];

#define CAN_LOOPBACK_ENABLED

// If you only want to disable loop, uncomment the line below
// #undef CAN_LOOPBACK_ENABLED

void G474_SystemClockConfig(){

}

void G473_SystemClockConfig(){

}

void Error_Handler(){
    while(1){

    }
}


static void task(void *pvParameters) {

}

int main(void) {
    HAL_Init();

    // System clock config can change depending on the target MCU, since the clock tree can be different
    // If you need to use a different MCU, go to cubemx and generate a new system clock config function with the system clock being 80 Mhz
    // It especially varies with nucleo vs direct MCU
    #ifdef STM32G474xx
        G474_SystemClockConfig();
    #elif defined(STM32G473xx)
        G473_SystemClockConfig();
    #else
        SystemClock_Config();
    #endif

    // CANFD1 Filter Config
    FDCAN_FilterTypeDef sFilterConfig;
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; // directs frames to FIFO0
    sFilterConfig.FilterID1 = 0x22;
    sFilterConfig.FilterID2 = 0x22;

    // if (HAL_FDCAN_ConfigFilter(hfdcan1, &sFilterConfig) != HAL_OK)
    // {
    //     /* Filter configuration Error */
    //     Error_Handler();
    // }

    hfdcan1->Instance = FDCAN1;
    hfdcan1->Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan1->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
    #ifdef CAN_LOOPBACK_ENABLED
    hfdcan1->Init.Mode = FDCAN_MODE_INTERNAL_LOOPBACK;
    #else
    hfdcan1->Init.Mode = FDCAN_MODE_NORMAL;
    #endif
    hfdcan1->Init.AutoRetransmission = DISABLE;
    hfdcan1->Init.TransmitPause = DISABLE;
    hfdcan1->Init.ProtocolException = DISABLE;
    hfdcan1->Init.NominalPrescaler = 20;
    hfdcan1->Init.NominalSyncJumpWidth = 1;
    hfdcan1->Init.NominalTimeSeg1 = 13;
    hfdcan1->Init.NominalTimeSeg2 = 2;
    hfdcan1->Init.DataPrescaler = 1;
    hfdcan1->Init.DataSyncJumpWidth = 1;
    hfdcan1->Init.DataTimeSeg1 = 1;
    hfdcan1->Init.DataTimeSeg2 = 1;
    hfdcan1->Init.StdFiltersNbr = 0;
    hfdcan1->Init.ExtFiltersNbr = 0;
    hfdcan1->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
    // if (HAL_FDCAN_Init(hfdcan1) != HAL_OK)
    // {
    //     Error_Handler();
    // }

    if(can_fd_init(hfdcan1, &sFilterConfig) != CAN_OK){
        Error_Handler();
    }
    if(can_fd_start(hfdcan1) != CAN_OK){
        Error_Handler();
    }

    // you can only send CAN messages within a FreeRTOS task
    xTaskCreateStatic(
                task,
                "task",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                task_stack,
                &task_buffer);

    
    vTaskStartScheduler();
    while(1){

    }
    
    Error_Handler();
    return 0;
}