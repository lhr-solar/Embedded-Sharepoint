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
    if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) != CAN_SENT) Error_Handler();  

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
    if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) != CAN_SENT) Error_Handler();  

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
    if (status != CAN_RECV && rx_data[0] != 0x1) Error_Handler();
    status = can_recv(hcan1, 0x1, &rx_header, rx_data, portMAX_DELAY);
    if (status != CAN_RECV && rx_data[0] != 0x2) Error_Handler();

    vTaskDelay(200);
  }
}

int main(void){
  // initialize the HAL and system clock
  if (HAL_Init() != HAL_OK) Error_Handler();
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
  if (can_init(hcan1, &sFilterConfig) != CAN_OK) Error_Handler();
  if (can_start(hcan1) != CAN_OK) Error_Handler();

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

  Error_Handler();
  return 0;
}

/* -------- CAN MSP Init / DeInit -------- */

#if defined(STM32F4xx)
static inline void HAL_CAN_MspF4Init(CAN_HandleTypeDef* hcan){
  GPIO_InitTypeDef init = {0};
  
  // CAN1
  if (hcan->Instance == CAN1) {
    // enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* enable gpio
    PA11 -> CAN1_RX
    PA12 -> CAN1_TX
    */
    init.Pin = GPIO_PIN_11;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_PULLUP;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &init);

    init.Pin = GPIO_PIN_12;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &init);
  }

  // CAN2
  #ifdef CAN2
  else if (hcan->Instance == CAN2) {
    // enable clocks
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* enable gpio
    PB12 -> CAN2_RX
    PB13 -> CAN2_TX
    */
    init.Pin = GPIO_PIN_12;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_PULLUP;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &init);

    init.Pin = GPIO_PIN_13;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &init);
  }
  #endif /* CAN2 */

  // CAN3
  #ifdef CAN3
  else if (hcan->Instance == CAN3) {
    // enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* enable gpio
    PA8  -> CAN3_RX
    PA15 -> CAN3_TX
    */
    init.Pin = GPIO_PIN_8;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_PULLUP;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF11_CAN3;
    HAL_GPIO_Init(GPIOA, &init);

    init.Pin = GPIO_PIN_15;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF11_CAN3;
    HAL_GPIO_Init(GPIOA, &init);
  }
  #endif /* CAN3 */
}
#endif


#if defined(STM32L4xx)
static inline void HAL_CAN_MspL4Init(CAN_HandleTypeDef* hcan){
  GPIO_InitTypeDef init = {0};
  // CAN1
  if (hcan->Instance == CAN1) {
    // enable clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* enable gpio
    PA11 -> CAN1_RX
    PA12 -> CAN1_TX
    */
    init.Pin = GPIO_PIN_11;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_PULLUP;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &init);

    init.Pin = GPIO_PIN_12;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &init);
  }
}
#endif

// CAN MSP deinit
void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan) {
  // CAN1
  if (hcan->Instance == CAN1) {
    // disable clocks
    __HAL_RCC_CAN1_CLK_DISABLE();

    /* disable gpio
    PA11 -> CAN1_RX
    PA12 -> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);

    // disable interrupts
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  }

  // CAN2
  #ifdef CAN2
  else if (hcan->Instance == CAN2) {
    // disable clocks
    __HAL_RCC_CAN2_CLK_DISABLE();

    /* disable gpio
    PB12 -> CAN2_RX
    PB13 -> CAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);

    // disable interrupts
    HAL_NVIC_DisableIRQ(CAN2_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
  }
  #endif /* CAN2 */

  // CAN3
  #ifdef CAN3
  else if (hcan->Instance == CAN3) {
    // disable clocks
    __HAL_RCC_CAN3_CLK_DISABLE();

    /* disable gpio
    PA8  -> CAN3_RX
    PB15 -> CAN3_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);

    // disable interrupts
    HAL_NVIC_DisableIRQ(CAN3_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN3_RX0_IRQn);
  }
  #endif /* CAN3 */
}

// CAN MSP init
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan) {
  IRQn_Type txIRQ = 0;
  IRQn_Type rxIRQ = 0;
  if (hcan->Instance == CAN1) {
    __HAL_RCC_CAN1_CLK_ENABLE();
    txIRQ = CAN1_TX_IRQn;
    rxIRQ = CAN1_RX0_IRQn;
  }
  #ifdef CAN2
  else if(hcan->Instance == CAN2){
    __HAL_RCC_CAN2_CLK_ENABLE();
    txIRQ = CAN2_TX_IRQn;
    rxIRQ = CAN2_RX0_IRQn;
  }
  #endif
  #ifdef CAN3
  else if(hcan->Instance == CAN3){
    __HAL_RCC_CAN3_CLK_ENABLE();
    txIRQ = CAN3_TX_IRQn;
    rxIRQ = CAN3_RX0_IRQn;
  }
  #endif

  // configure GPIO pins for CAN
  #if defined(STM32F4xx)
  HAL_CAN_MspF4Init(hcan);
  #elif defined(STM32L4xx)
  HAL_CAN_MspL4Init(hcan);
  #endif

  // enable can interrupts
  if(txIRQ !=0 && rxIRQ !=0){
    HAL_NVIC_SetPriority(txIRQ, 5, 0);
    HAL_NVIC_EnableIRQ(txIRQ);
    HAL_NVIC_SetPriority(rxIRQ, 5, 0);
    HAL_NVIC_EnableIRQ(rxIRQ);
  }
}