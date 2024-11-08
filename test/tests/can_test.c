#include "stm32xx_hal.h"
#include "CAN.h"

StaticTask_t task_buffer;
StackType_t taskStack[configMINIMAL_STACK_SIZE];

static void task(void *pvParameters) {
    // create payload
    payload_t payload = {0};
    payload.header.StdId = 0x11;
    payload.header.RTR = CAN_RTR_DATA;
    payload.header.IDE = CAN_ID_STD;
    payload.header.DLC = 2;
    payload.header.TransmitGlobalTime = DISABLE;

    // send payload
    payload.data[0] = 0x01;
    payload.data[1] = 0x00;
    volatile CAN_Status_e status1 = can_send(can1_handle, &payload.header, payload.data, true);
    payload.data[0] = 0x02;
    volatile CAN_Status_e status2 = can_send(can1_handle, &payload.header, payload.data, true);
    payload.data[0] = 0x03;
    volatile CAN_Status_e status3 = can_send(can1_handle, &payload.header, payload.data, true);
    payload.data[0] = 0x04;
    volatile CAN_Status_e status4 = can_send(can1_handle, &payload.header, payload.data, true);

    (void)status1;
    (void)status2;
    (void)status3;
    (void)status4;

    while(1) {}
}

int main(void) {
    // initialize the HAL and system clock
    HAL_Init();
    // SystemClock_Config();

    // create filter
    CAN_FilterTypeDef  sFilterConfig;
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    // setup can init structure
    can1_handle->Init.Prescaler = 5;
    can1_handle->Init.Mode = CAN_MODE_LOOPBACK;
    can1_handle->Init.SyncJumpWidth = CAN_SJW_1TQ;
    can1_handle->Init.TimeSeg1 = CAN_BS1_6TQ;
    can1_handle->Init.TimeSeg2 = CAN_BS2_2TQ;
    can1_handle->Init.TimeTriggeredMode = DISABLE;
    can1_handle->Init.AutoBusOff = DISABLE;
    can1_handle->Init.AutoWakeUp = DISABLE;
    can1_handle->Init.AutoRetransmission = ENABLE;
    can1_handle->Init.ReceiveFifoLocked = DISABLE;
    can1_handle->Init.TransmitFifoPriority = DISABLE;

    // initialize CAN
    can_init(can1_handle, &sFilterConfig);

    xTaskCreateStatic( /* The function that implements the task. */
                    task,
                    /* Text name for the task, just to help debugging. */
                    "task",
                    /* The size (in words) of the stack that should be created
                    for the task. */
                    configMINIMAL_STACK_SIZE,
                 /* A parameter that can be passed into the task. Not used
                    in this simple demo. */
                    NULL,
                 /* The priority to assign to the task. tskIDLE_PRIORITY
                    (which is 0) is the lowest priority. configMAX_PRIORITIES - 1
                    is the highest priority. */
                    tskIDLE_PRIORITY + 2,
                 /* Used to obtain a handle to the created task. Not used in
                    this simple demo, so set to NULL. */
                    taskStack,
                    /* Buffer for static allocation */
                    &task_buffer );

    vTaskStartScheduler();

    while(1) {}

    return 0;
}
