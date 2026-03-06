/* CAN test

- Setups up CANs in loopback mode
- Send 4 messages (since there are 3 mailboxes, 1 ends up going in the can1 send queue)
- receives the 4 messages and verifies correctness
- Flashes LED if successful on all CANs

- Included ClockConfig / CAN_MspInit is for PSOM

*/
#include "stm32xx_hal.h"
#include "CAN.h"

StaticTask_t task_buffer;
StackType_t task_stack[configMINIMAL_STACK_SIZE];

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

static void task(void *pvParameters) {
  // create payload to send
  CAN_TxHeaderTypeDef tx_header = {0};   
  tx_header.StdId = 0x1;
  tx_header.RTR = CAN_RTR_DATA;
  tx_header.IDE = CAN_ID_STD;
  tx_header.DLC = 2;
  tx_header.TransmitGlobalTime = DISABLE;

  // send two payloads to 0x1
  uint8_t tx_data[8] = {0};
  tx_data[0] = 0x01;
  tx_data[1] = 0x00;
  if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) != CAN_OK) Error_Handler();
  #ifdef CAN2
  if (can_send(hcan2, &tx_header, tx_data, portMAX_DELAY) != CAN_OK) Error_Handler();
  #endif /* CAN2 */

  tx_data[0] = 0x02;
  if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) != CAN_OK) Error_Handler();
  #ifdef CAN2
  if (can_send(hcan2, &tx_header, tx_data, portMAX_DELAY) != CAN_OK) Error_Handler();
  #endif /* CAN2 */

  // send two payloads to 0x3
  tx_data[0] = 0x03;
  tx_header.StdId = 0x003;
  if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) != CAN_OK) Error_Handler();
  #ifdef CAN2
  if (can_send(hcan2, &tx_header, tx_data, portMAX_DELAY) != CAN_OK) Error_Handler();
  #endif /* CAN2 */

  tx_data[0] = 0x04;
  if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) != CAN_OK) Error_Handler();
  #ifdef CAN2
  if (can_send(hcan2, &tx_header, tx_data, portMAX_DELAY) != CAN_OK) Error_Handler();
  #endif /* CAN2 */

  // receive what was sent to 0x1
  CAN_RxHeaderTypeDef rx_header = {0};
  uint8_t rx_data[8] = {0};
  can_status_t status;

  // CAN1
  status = can_recv(hcan1, 0x1, &rx_header, rx_data, portMAX_DELAY);
  if (status != CAN_OK && rx_data[0] != 0x1) Error_Handler();
  status = can_recv(hcan1, 0x1, &rx_header, rx_data, portMAX_DELAY);
  if (status != CAN_OK && rx_data[0] != 0x2) Error_Handler();

  #ifdef CAN2
  // CAN2
  status = can_recv(hcan2, 0x1, &rx_header, rx_data, portMAX_DELAY);
  if (status != CAN_OK && rx_data[0] != 0x1) Error_Handler();
  status = can_recv(hcan2, 0x1, &rx_header, rx_data, portMAX_DELAY);
  if (status != CAN_OK && rx_data[0] != 0x2) Error_Handler();
  #endif /* CAN2 */

  // make sure we don't receive from wrong ID and nonblocking works
  // CAN1
  status = can_recv(hcan1, 0x1, &rx_header, rx_data, 0);
  if (status != CAN_EMPTY) Error_Handler();
  status = can_recv(hcan1, 0x1, &rx_header, rx_data, 0);
  if (status != CAN_EMPTY) Error_Handler();

  #ifdef CAN2
  // CAN2
  status = can_recv(hcan2, 0x1, &rx_header, rx_data, 0);
  if (status != CAN_EMPTY) Error_Handler();
  status = can_recv(hcan2, 0x1, &rx_header, rx_data, 0);
  if (status != CAN_EMPTY) Error_Handler();
  #endif /* CAN2 */

  // receive the rest
  // CAN1
  status = can_recv(hcan1, 0x3, &rx_header, rx_data, portMAX_DELAY);
  if (status != CAN_OK && rx_data[0] != 0x3) Error_Handler();
  status = can_recv(hcan1, 0x3, &rx_header, rx_data, portMAX_DELAY);
  if (status != CAN_OK && rx_data[0] != 0x4) Error_Handler();

  #ifdef CAN2
  // CAN2
  status = can_recv(hcan2, 0x3, &rx_header, rx_data, portMAX_DELAY);
  if (status != CAN_OK && rx_data[0] != 0x3) Error_Handler();
  status = can_recv(hcan2, 0x3, &rx_header, rx_data, portMAX_DELAY);
  if (status != CAN_OK && rx_data[0] != 0x4) Error_Handler();
  #endif /* CAN2 */

  // TEST QUEUE OVERWRITE ============================================

  // send one payload to 0x4
  tx_data[0] = 0x04;
  tx_header.StdId = 0x004;
  if (can_send(hcan1, &tx_header, tx_data, true) != CAN_OK) Error_Handler();

  // receive what was sent to 0x4
  status = can_recv(hcan1, 0x4, &rx_header, rx_data, true);
  if (status != CAN_OK || rx_data[0] != 0x4) Error_Handler();
  
  // send two payloads to 0x4, only the last one should be received
  tx_data[0] = 0x05;
  if (can_send(hcan1, &tx_header, tx_data, true) != CAN_OK) Error_Handler();
  tx_data[0] = 0x06;
  if (can_send(hcan1, &tx_header, tx_data, true) != CAN_OK) Error_Handler();
  tx_data[0] = 0x07;
  if (can_send(hcan1, &tx_header, tx_data, true) != CAN_OK) Error_Handler();
  tx_data[0] = 0x08;
  if (can_send(hcan1, &tx_header, tx_data, true) != CAN_OK) Error_Handler();
  tx_data[0] = 0x09;
  if (can_send(hcan1, &tx_header, tx_data, true) != CAN_OK) Error_Handler();

  HAL_Delay(200);

  // receive the rest in order
  status = can_recv(hcan1, 0x4, &rx_header, rx_data, true);
  if (status != CAN_OK || rx_data[0] != 0x6) Error_Handler();
  status = can_recv(hcan1, 0x4, &rx_header, rx_data, true);
  if (status != CAN_OK || rx_data[0] != 0x7) Error_Handler();
  status = can_recv(hcan1, 0x4, &rx_header, rx_data, true);
  if (status != CAN_OK || rx_data[0] != 0x8) Error_Handler();
  status = can_recv(hcan1, 0x4, &rx_header, rx_data, true);
  if (status != CAN_OK || rx_data[0] != 0x9) Error_Handler();

  success_handler();
}

int main(void) {
  #if defined(CAN1)
  // initialize the HAL and system clock
  if (HAL_Init() != HAL_OK) Error_Handler();
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
  // Baud rate is 250 kbit/s
  hcan1->Init.Prescaler = 20;
  hcan1->Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1->Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan1->Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1->Init.Mode = CAN_MODE_LOOPBACK;
  hcan1->Init.TimeTriggeredMode = DISABLE;
  hcan1->Init.AutoBusOff = DISABLE;
  hcan1->Init.AutoWakeUp = DISABLE;
  hcan1->Init.AutoRetransmission = ENABLE;
  hcan1->Init.ReceiveFifoLocked = DISABLE;

  // If TransmitFifoPriority is disabled, the hardware selects the mailbox based on the message ID priority. 
  // If enabled, the hardware uses a FIFO mechanism to select the mailbox based on the order of transmission requests.
  hcan1->Init.TransmitFifoPriority = ENABLE;

  // initialize CAN1
  if (can_init(hcan1, &sFilterConfig) != CAN_OK) Error_Handler();
  if (can_start(hcan1) != CAN_OK) Error_Handler();

  #ifdef CAN2
  // setup can2 init
  // Baud rate is 250 kbit/s
  hcan2->Init.Prescaler = 20;
  hcan2->Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2->Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan2->Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan2->Init.Mode = CAN_MODE_LOOPBACK;
  hcan2->Init.TimeTriggeredMode = DISABLE;
  hcan2->Init.AutoBusOff = DISABLE;
  hcan2->Init.AutoWakeUp = DISABLE;
  hcan2->Init.AutoRetransmission = ENABLE;
  hcan2->Init.ReceiveFifoLocked = DISABLE;
  hcan2->Init.TransmitFifoPriority = DISABLE;

  // initialize CAN
  sFilterConfig.FilterBank = 14;
  if (can_init(hcan2, &sFilterConfig) != CAN_OK) Error_Handler();
  if (can_start(hcan2) != CAN_OK) Error_Handler();
  #endif /* CAN2 */

  xTaskCreateStatic(
                task,
                "task",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                task_stack,
                &task_buffer);

  vTaskStartScheduler();
  #endif

  Error_Handler();

  return 0;
}


/* -------- Config / CAN Init for PSOM -------- */
#ifdef STM32L431xx

/**
 * SystemClock_Config for PSOM (80MHz)
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
* @brief CAN MSP Initialization for PSOM.
* @param hcan: CAN handle pointer
* @retval None
*/
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hcan->Instance==CAN1) {
    /* Peripheral clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  }
}

/**
* @brief CAN MSP De-Initialization for PSOM.
* @param hcan: CAN handle pointer
* @retval None
*/
void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan) {
  if(hcan->Instance==CAN1) {
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* CAN1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  }
}
#endif