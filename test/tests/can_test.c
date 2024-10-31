#include "stm32xx_hal.h"
#include "CAN.h"

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

    // create payload
    payload_t payload = {0};
    payload.header.StdId = 0x11;
    payload.header.RTR = CAN_RTR_DATA;
    payload.header.IDE = CAN_ID_STD;
    payload.header.DLC = 2;
    payload.header.TransmitGlobalTime = DISABLE;

    payload.data[0] = 0x01;
    payload.data[1] = 0x00;

    // HAL_CAN_Stop(can1_handle);

    // send payload
    volatile CAN_Status_e status1 = can_send(can1_handle, &payload.header, payload.data, true);
    payload.data[0] = 0x02;
    volatile CAN_Status_e status2 = can_send(can1_handle, &payload.header, payload.data, true);
    payload.data[0] = 0x03;
    volatile CAN_Status_e status3 = can_send(can1_handle, &payload.header, payload.data, true);

    // for (int i = 0; i < 999999; i++) {}
    payload.data[0] = 0x04;
    volatile CAN_Status_e status4 = can_send(can1_handle, &payload.header, payload.data, true);

    (void)status1;
    (void)status2;
    (void)status3;
    (void)status4;

    // HAL_CAN_Start(can1_handle);

    while(1) {}

    return 0;
}
