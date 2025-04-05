/* CAN test

- Setups up CANs in loopback mode
- Send 4 messages (since there are 3 mailboxes, 1 ends up going in the can1 send queue)
- receives the 4 messages and verifies correctness
- Flashes LED if successful on all CANs

*/

#include "stm32xx_hal.h"
#include "can_utils.h"
#include "CAN.h"

StaticTask_t task_buffer;
StackType_t task_stack[configMINIMAL_STACK_SIZE];

static void error_handler(void) {
  while(1) {}
}

static void success_handler(void) {
  GPIO_InitTypeDef led_init = {
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Pin = GPIO_PIN_5
  };
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  HAL_GPIO_Init(GPIOA, &led_init);

  while(1){
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_Delay(500);
  }
}

static void can_task(void *pvParameters) {
    CAN_RxHeaderTypeDef rx_header = {0};
    uint8_t rx_data[8] = {0};
    can_status_t status;
    status = can_recv(hcan1, CANUTIL_BPS_Trip, &rx_header, rx_data, portMAX_DELAY);
    if (status != CAN_RECV) {
       volatile uint64_t reading = CANUTIL_GET_VALUE_BPS_Trip(*rx_data);
       for (int i)
  }
}

static void supp_task(void *pvParameters) {
  // testing supp voltage CAN loopback
  CAN_TxHeaderTypeDef tx_header = {0};   
  tx_header.StdId = CANUTIL_Supplemental_Voltage;
  tx_header.RTR = CAN_RTR_DATA;
  tx_header.IDE = CAN_ID_STD;
  tx_header.DLC = 2;
  tx_header.TransmitGlobalTime = DISABLE;

  // send two bytes to supp voltage
  uint8_t tx_data[8] = {0};
  tx_data[0] = 0xAB; 
  tx_data[1] = 0xCD;
  if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) != CAN_SENT) error_handler();

  // receive supp voltage
  CAN_RxHeaderTypeDef rx_header = {0};
  uint8_t rx_data[8] = {0};
  can_status_t status;

  status = can_recv(hcan1, CANUTIL_Supplemental_Voltage, &rx_header, rx_data, portMAX_DELAY);
  if (status != CAN_RECV && rx_data[0] != 0xAB) error_handler();
  if (status != CAN_RECV && rx_data[1] != 0xCD) error_handler();
  
  success_handler();
}

int main(void) {
  // initialize the HAL and system clock
  if (HAL_Init() != HAL_OK) error_handler();
  SystemClock_Config();

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
  //hcan1->Init.Mode = CAN_MODE_LOOPBACK;
  hcan1->Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1->Init.TimeSeg1 = CAN_BS1_6TQ;
  hcan1->Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1->Init.TimeTriggeredMode = DISABLE;
  hcan1->Init.AutoBusOff = DISABLE;
  hcan1->Init.AutoWakeUp = DISABLE;
  hcan1->Init.AutoRetransmission = ENABLE;
  hcan1->Init.ReceiveFifoLocked = DISABLE;

  // If TransmitFifoPriority is disabled, the hardware selects the mailbox based on the message ID priority. 
  // If enabled, the hardware uses a FIFO mechanism to select the mailbox based on the order of transmission requests.
  hcan1->Init.TransmitFifoPriority = ENABLE;

  // initialize CAN1
  if (can_init(hcan1, &sFilterConfig) != CAN_OK) error_handler();
  if (can_start(hcan1) != CAN_OK) error_handler();

  #ifdef CAN2
  // setup can2 init
  hcan2->Init.Prescaler = 40;
  hcan2->Init.Mode = CAN_MODE_NORMAL;

  hcan2->Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2->Init.TimeSeg1 = CAN_BS1_6TQ;
  hcan2->Init.TimeSeg2 = CAN_BS2_2TQ;

  hcan2->Init.TimeTriggeredMode = DISABLE;
  hcan2->Init.AutoBusOff = DISABLE;
  hcan2->Init.AutoWakeUp = DISABLE;
  hcan2->Init.AutoRetransmission = ENABLE;
  hcan2->Init.ReceiveFifoLocked = DISABLE;
  hcan2->Init.TransmitFifoPriority = DISABLE;

  // initialize CAN
  sFilterConfig.FilterBank = 14;
  if (can_init(hcan2, &sFilterConfig) != CAN_OK) error_handler();
  if (can_start(hcan2) != CAN_OK) error_handler();
  #endif /* CAN2 */

  xTaskCreateStatic(
                supp_task,
                "task",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                task_stack,
                &task_buffer);

  vTaskStartScheduler();

  error_handler();

  return 0;
}
