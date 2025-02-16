/* CAN Multithreaded Test 

- Setups up CANs in loopback mode
- Send 2 messages to 0x1 from task1 and 2 messages to 0x1 from task2
- Receives the 4 messages in task3
- Read values in GDB to verify correctness

*/
#include "stm32xx_hal.h"
#include "CAN.h"

StaticTask_t task1_buffer;
StackType_t task1_stack[configMINIMAL_STACK_SIZE];

StaticTask_t task2_buffer;
StackType_t task2_stack[configMINIMAL_STACK_SIZE];

StaticTask_t task3_buffer;
StackType_t task3_stack[configMINIMAL_STACK_SIZE];

static void error_handler(void) {
  while(1) {}
}

static void task1(void *pvParameters){
  while(1){
    // create payload to send
    CAN_TxHeaderTypeDef tx_header = {0};   
    tx_header.StdId = 0x1;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.IDE = CAN_ID_STD;
    tx_header.DLC = 2;
    tx_header.TransmitGlobalTime = DISABLE;

    // send two payloads to 0x1
    uint8_t tx_data[8] = {0};
    tx_data[0] = 0xDE;
    tx_data[1] = 0xAD;
    if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) != CAN_SENT) error_handler();  

    vTaskDelay(400);
  }
}

static void task2(void *pvParameters){
  while(1){
    // create payload to send
    CAN_TxHeaderTypeDef tx_header = {0};   
    tx_header.StdId = 0x1;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.IDE = CAN_ID_STD;
    tx_header.DLC = 2;
    tx_header.TransmitGlobalTime = DISABLE;

    // send two payloads to 0x1
    uint8_t tx_data[8] = {0};
    tx_data[0] = 0xBE;
    tx_data[1] = 0xEF;
    if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) != CAN_SENT) error_handler();  

    vTaskDelay(400);
  }
}

static void task3(void *pvParameters){
  while(1){
    CAN_RxHeaderTypeDef rx_header = {0};
    uint8_t rx_data[8] = {0};
    can_status_t status;

    // receive what was sent to 0x1
    status = can_recv(hcan1, 0x1, &rx_header, rx_data, portMAX_DELAY);
    if (status != CAN_RECV && rx_data[0] != 0x1) error_handler();
    status = can_recv(hcan1, 0x1, &rx_header, rx_data, portMAX_DELAY);
    if (status != CAN_RECV && rx_data[0] != 0x2) error_handler();

    vTaskDelay(200);
  }
}

int main(void){
  // initialize the HAL and system clock
  if (HAL_Init() != HAL_OK) error_handler();
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

  // setup can1 init
  hcan1->Init.Prescaler = 5;
  hcan1->Init.Mode = CAN_MODE_LOOPBACK;
  hcan1->Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1->Init.TimeSeg1 = CAN_BS1_6TQ;
  hcan1->Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1->Init.TimeTriggeredMode = DISABLE;
  hcan1->Init.AutoBusOff = DISABLE;
  hcan1->Init.AutoWakeUp = DISABLE;
  hcan1->Init.AutoRetransmission = ENABLE;
  hcan1->Init.ReceiveFifoLocked = DISABLE;
  hcan1->Init.TransmitFifoPriority = DISABLE;

  // initialize CAN1
  if (can_init(hcan1, &sFilterConfig) != CAN_OK) error_handler();
  if (can_start(hcan1) != CAN_OK) error_handler();

  xTaskCreateStatic(
    task1,
    "task1",
    configMINIMAL_STACK_SIZE,
    NULL,
    tskIDLE_PRIORITY + 2,
    task1_stack,
    &task1_buffer);

  xTaskCreateStatic(
    task2,
    "task2",
    configMINIMAL_STACK_SIZE,
    NULL,
    tskIDLE_PRIORITY + 2,
    task2_stack,
    &task2_buffer);
  
  xTaskCreateStatic(
    task3,
    "task3",
    configMINIMAL_STACK_SIZE,
    NULL,
    tskIDLE_PRIORITY + 2,
    task3_stack,
    &task3_buffer);


  vTaskStartScheduler();

  error_handler();
}
