/* CAN test

- Setups up CANs in loopback mode
- Send 4 messages (since there are 3 mailboxes, 1 ends up going in the can1 send
queue)
- receives the 4 messages and verifies correctness
- Flashes LED if successful on all CANs

- Included ClockConfig / CAN_MspInit is for PSOM

*/
#include "CAN.h"
#include "stm32xx_hal.h"

#if defined(STM32L431xx)
// PeripheralSOM
#define HEARTBEAT_LED_PORT GPIOB
#define HEARTBEAT_LED_PIN GPIO_PIN_11
#else
// Most Nucleos
#define HEARTBEAT_LED_PORT GPIOA
#define HEARTBEAT_LED_PIN GPIO_PIN_5
#endif

StaticTask_t task_buffer;
StackType_t task_stack[configMINIMAL_STACK_SIZE];

// Initialize clock for heartbeat LED port
void Heartbeat_Clock_Init() {
  switch ((uint32_t)HEARTBEAT_LED_PORT) {
    case (uint32_t)GPIOA:
      __HAL_RCC_GPIOA_CLK_ENABLE();
      break;
    case (uint32_t)GPIOB:
      __HAL_RCC_GPIOB_CLK_ENABLE();
      break;
    case (uint32_t)GPIOC:
      __HAL_RCC_GPIOC_CLK_ENABLE();
      break;
  }
}

void Error_Handler(void) {
  while (1) {
    HAL_GPIO_TogglePin(HEARTBEAT_LED_PORT, HEARTBEAT_LED_PIN);
    HAL_Delay(1000);
  }
}

static void task(void* pvParameters) {
  // create payload to send

  CAN_TxHeaderTypeDef tx_header = {0};
  tx_header.StdId = 0x1;
  tx_header.RTR = CAN_RTR_DATA;
  tx_header.IDE = CAN_ID_STD;
  tx_header.DLC = 8;
  tx_header.TransmitGlobalTime = DISABLE;

  CAN_RxHeaderTypeDef rx_header = {0};
  uint8_t rx_data[8] = {0};
  //   can_status_t status;

  uint8_t tx_data[8] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xFF};

  while (1) {
    // sending and recieving the same data from pcan or use internal loopback
    if (can_send(hcan1, &tx_header, tx_data, portMAX_DELAY) != CAN_OK)
      Error_Handler();
    vTaskDelay(100);

    // send can with id of 0x2 with dummy data matching what we wanted
    if (can_recv(hcan1, 0x2, &rx_header, rx_data, portMAX_DELAY) != CAN_OK)
      Error_Handler();

    for (uint8_t i = 0; i < rx_header.DLC; i++) {
      if (rx_data[i] != tx_data[i]) {
        Error_Handler();
      }
    }

    vTaskDelay(100);

    HAL_GPIO_TogglePin(HEARTBEAT_LED_PORT, HEARTBEAT_LED_PIN);
  }
}

int main(void) {
#if defined(CAN1)
  // initialize the HAL and system clock
  if (HAL_Init() != HAL_OK) Error_Handler();
  SystemClock_Config();

  GPIO_InitTypeDef led_init = {.Mode = GPIO_MODE_OUTPUT_PP,
                               .Pull = GPIO_NOPULL,
                               .Pin = HEARTBEAT_LED_PIN};

  Heartbeat_Clock_Init();
  HAL_GPIO_Init(HEARTBEAT_LED_PORT, &led_init);

  // create filter
  CAN_FilterTypeDef sFilterConfig;
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
  //   hcan1->Init.Mode = CAN_MODE_LOOPBACK;
  hcan1->Init.Mode = CAN_MODE_NORMAL;
  hcan1->Init.TimeTriggeredMode = DISABLE;
  hcan1->Init.AutoBusOff = DISABLE;
  hcan1->Init.AutoWakeUp = DISABLE;
  hcan1->Init.AutoRetransmission = ENABLE;
  hcan1->Init.ReceiveFifoLocked = DISABLE;

  // If TransmitFifoPriority is disabled, the hardware selects the mailbox based
  // on the message ID priority. If enabled, the hardware uses a FIFO mechanism
  // to select the mailbox based on the order of transmission requests.
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

  xTaskCreateStatic(task, "task", configMINIMAL_STACK_SIZE, NULL,
                    tskIDLE_PRIORITY + 2, task_stack, &task_buffer);

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
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
  if (hcan->Instance == CAN1) {
    /* Peripheral clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_TX_IRQn,
                         configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn,
                         configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  }
}

/**
 * @brief CAN MSP De-Initialization for PSOM.
 * @param hcan: CAN handle pointer
 * @retval None
 */
void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan) {
  if (hcan->Instance == CAN1) {
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8 | GPIO_PIN_9);

    /* CAN1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  }
}
#endif