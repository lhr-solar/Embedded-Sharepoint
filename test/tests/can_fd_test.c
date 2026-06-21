#include "can_fd.h"
#include "can_msgs.h"


#include "stm32xx_hal.h"

StaticTask_t task_buffer;
StackType_t task_stack[512];

// LSOM has heartbeat pin defined as PC3
// Most other nucleos have a heartbeat for A5
#define LED_PIN GPIO_PIN_3
#define LED_PORT GPIOC

// Some pins broken out by LSOM
#define CAN_TX_PROFILE_PORT GPIOA
#define CAN_TX_PROFILE_PIN GPIO_PIN_10

#define CAN_RX_PROFILE_PORT GPIOB
#define CAN_RX_PROFILE_PIN GPIO_PIN_4

void can_profile_pins_init() {
  GPIO_InitTypeDef tx_pin_config = {.Mode = GPIO_MODE_OUTPUT_PP,
                                    .Pull = GPIO_NOPULL,
                                    .Pin = CAN_TX_PROFILE_PIN};

  switch ((uint32_t)CAN_TX_PROFILE_PORT) {
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

  HAL_GPIO_Init(CAN_TX_PROFILE_PORT, &tx_pin_config);

  GPIO_InitTypeDef rx_pin_config = {.Mode = GPIO_MODE_OUTPUT_PP,
                                    .Pull = GPIO_NOPULL,
                                    .Pin = CAN_RX_PROFILE_PIN};

  switch ((uint32_t)CAN_RX_PROFILE_PORT) {
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
  HAL_GPIO_Init(CAN_RX_PROFILE_PORT, &rx_pin_config);
}

// Initialize clock for heartbeat LED port
void Heartbeat_Init() {
  GPIO_InitTypeDef led_config = {
      .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Pin = LED_PIN};

  switch ((uint32_t)LED_PORT) {
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

  HAL_GPIO_Init(LED_PORT, &led_config);
}

void G474_SystemClockConfig() {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}

void G473_SystemClockConfig(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}

void Error_Handler() {
  while (1) {
    // HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    HAL_Delay(1000);
  }
}

void Success_Handler() { HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET); }


static void task(void *pvParameters) {

    int test_id = 0x321;
    FDCAN_TxHeaderTypeDef tx_header = {0};   
    tx_header.Identifier = test_id;
    tx_header.IdType = FDCAN_STANDARD_ID;
    tx_header.TxFrameType = FDCAN_DATA_FRAME;
    tx_header.DataLength = FDCAN_DLC_BYTES_8;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch = FDCAN_BRS_OFF;
    tx_header.FDFormat = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;
    tx_header.MessageMarker = 0;

  // send x1234 to 0x321
  uint8_t tx_data[8] = {0};
  tx_data[0] = 0x12;
  tx_data[1] = 0x34;
  tx_data[2] = 0x56;
  tx_data[3] = 0x78;
  tx_data[4] = 0x9A;
  tx_data[5] = 0xBC;
  tx_data[6] = 0xDE;
  tx_data[7] = 0xFF;

  FDCAN_RxHeaderTypeDef fdcan1_rx_header = {0};
  uint8_t fdcan1_rx_data[8] = {0};

  while (1) {
#ifdef FDCAN1
    if (can_fd_send(hfdcan1, &tx_header, tx_data, portMAX_DELAY) == CAN_ERR) {
      Error_Handler();
    }

    // note that for can_fd_recv to work, the ID used must be added the
    // can1_recv_entires.h file
    if (can_fd_recv(hfdcan1, test_id, &fdcan1_rx_header, fdcan1_rx_data,
                    portMAX_DELAY) != CAN_OK) {
      Error_Handler();
    }

    for (uint8_t i = 0; i < 8; i++) {
      if (fdcan1_rx_data[i] != tx_data[i]) {
        Error_Handler();
      }
    }

#endif

#ifdef FDCAN2
    FDCAN_RxHeaderTypeDef fdcan2_rx_header = {0};
    uint8_t fdcan2_rx_data[8] = {0};

    if (can_fd_send(hfdcan2, &tx_header, tx_data, portMAX_DELAY) == CAN_ERR) {
      Error_Handler();
    }

    // note that for can_fd_recv to work, the ID used must be added the
    // can2_recv_entires.h file
    if (can_fd_recv(hfdcan2, test_id, &fdcan2_rx_header, fdcan2_rx_data,
                    portMAX_DELAY) != CAN_OK) {
      Error_Handler();
    }

    for (uint8_t i = 0; i < 8; i++) {
      if (fdcan2_rx_data[i] != tx_data[i]) {
        Error_Handler();
      }
    }
#endif

#ifdef FDCAN3
    FDCAN_RxHeaderTypeDef fdcan3_rx_header = {0};
    uint8_t fdcan3_rx_data[8] = {0};

    if (can_fd_send(hfdcan3, &tx_header, tx_data, portMAX_DELAY) == CAN_ERR) {
      Error_Handler();
    }

    // note that for can_fd_recv to work, the ID used must be added the
    // can3_recv_entires.h file
    if (can_fd_recv(hfdcan3, test_id, &fdcan3_rx_header, fdcan3_rx_data,
                    portMAX_DELAY) != CAN_OK) {
      Error_Handler();
    }

    for (uint8_t i = 0; i < 8; i++) {
      if (fdcan3_rx_data[i] != tx_data[i]) {
        Error_Handler();
      }
    }

#endif

    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void can_fd_tx_callback_hook(FDCAN_HandleTypeDef* hfdcan,
                             const can_tx_payload_t* payload) {
  HAL_GPIO_TogglePin(CAN_TX_PROFILE_PORT, CAN_TX_PROFILE_PIN);
}

void can_fd_rx_callback_hook(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs,
                             can_rx_payload_t recv_payload) {
  HAL_GPIO_TogglePin(CAN_RX_PROFILE_PORT, CAN_RX_PROFILE_PIN);
}

int main(void) {
  HAL_Init();

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

// System clock config can change depending on the target MCU, since the clock
// tree can be different If you need to use a different MCU, go to cubemx and
// generate a new system clock config function with the system clock being 80
// Mhz It especially varies with nucleo vs direct MCU G473_SystemClockConfig();
#ifdef STM32G474xx
  G474_SystemClockConfig();
#elif defined(STM32G473xx)
  G473_SystemClockConfig();
#else
  SystemClock_Config();
#endif

  Heartbeat_Init();         // enable LED for LED_PORT
  can_profile_pins_init();  // enable pins we toggle during interrupts

#ifdef FDCAN1
  hfdcan1->Instance = FDCAN1;
  hfdcan1->Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan1->Init.FrameFormat = FDCAN_FRAME_CLASSIC;

  // internal loopback shorts the CAN RX and TX internally.
  // In production, this should be set to FDCAN_MODE_NORMAL
  hfdcan1->Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1->Init.AutoRetransmission = ENABLE;
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
  hfdcan1->Init.StdFiltersNbr = 1;

  // ExtFiltersNB bits sets # of ID allowed
  // 0 = no extended ID allowed (all extended ID rejected)
  // 1 =  filter enabled to match to ID
  // each CAN needs its own filter
  hfdcan1->Init.ExtFiltersNbr = 0;
  hfdcan1->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

  // Hardware filter is derived from can1_recv_entries.h inside can_fd_init.
  if (can_fd_init(hfdcan1) != CAN_OK) {
    Error_Handler();
  }

  if (can_fd_start(hfdcan1) != CAN_OK) {
    Error_Handler();
  }
#endif

#ifdef FDCAN2

  hfdcan2->Instance = FDCAN2;
  hfdcan2->Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan2->Init.FrameFormat = FDCAN_FRAME_CLASSIC;

  // internal loopback shorts the CAN RX and TX internally.
  // In production, this should be set to FDCAN_MODE_NORMAL
  hfdcan2->Init.Mode = FDCAN_MODE_INTERNAL_LOOPBACK;
  hfdcan2->Init.AutoRetransmission = ENABLE;
  hfdcan2->Init.TransmitPause = DISABLE;
  hfdcan2->Init.ProtocolException = DISABLE;
  hfdcan2->Init.NominalPrescaler = 20;
  hfdcan2->Init.NominalSyncJumpWidth = 1;
  hfdcan2->Init.NominalTimeSeg1 = 13;
  hfdcan2->Init.NominalTimeSeg2 = 2;
  hfdcan2->Init.DataPrescaler = 1;
  hfdcan2->Init.DataSyncJumpWidth = 1;
  hfdcan2->Init.DataTimeSeg1 = 1;
  hfdcan2->Init.DataTimeSeg2 = 1;
  hfdcan2->Init.StdFiltersNbr = 1;

  // ExtFiltersNB bits sets # of ID allowed
  // 0 = no extended ID allowed (all extended ID rejected)
  // 1 =  filter enabled to match to ID
  // each CAN needs its own filter
  hfdcan2->Init.ExtFiltersNbr = 0;
  hfdcan2->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

  // Hardware filter is derived from can2_recv_entries.h inside can_fd_init.
  if (can_fd_init(hfdcan2) != CAN_OK) {
    Error_Handler();
  }

  if (can_fd_start(hfdcan2) != CAN_OK) {
    Error_Handler();
  }

#endif

#ifdef FDCAN3

  hfdcan3->Instance = FDCAN3;
  hfdcan3->Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan3->Init.FrameFormat = FDCAN_FRAME_CLASSIC;

  // internal loopback shorts the CAN RX and TX internally.
  // In production, this should be set to FDCAN_MODE_NORMAL
  hfdcan3->Init.Mode = FDCAN_MODE_INTERNAL_LOOPBACK;
  hfdcan3->Init.AutoRetransmission = ENABLE;
  hfdcan3->Init.TransmitPause = DISABLE;
  hfdcan3->Init.ProtocolException = DISABLE;
  hfdcan3->Init.NominalPrescaler = 20;
  hfdcan3->Init.NominalSyncJumpWidth = 1;
  hfdcan3->Init.NominalTimeSeg1 = 13;
  hfdcan3->Init.NominalTimeSeg2 = 2;
  hfdcan3->Init.DataPrescaler = 1;
  hfdcan3->Init.DataSyncJumpWidth = 1;
  hfdcan3->Init.DataTimeSeg1 = 1;
  hfdcan3->Init.DataTimeSeg2 = 1;
  hfdcan3->Init.StdFiltersNbr = 1;

  // ExtFiltersNB bits sets # of ID allowed
  // 0 = no extended ID allowed (all extended ID rejected)
  // 1 =  filter enabled to match to ID
  // each CAN needs its own filter
  hfdcan3->Init.ExtFiltersNbr = 0;
  hfdcan3->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

  // Hardware filter is derived from can3_recv_entries.h inside can_fd_init.
  if (can_fd_init(hfdcan3) != CAN_OK) {
    Error_Handler();
  }

  if (can_fd_start(hfdcan3) != CAN_OK) {
    Error_Handler();
  }

#endif

  // you can only send CAN messages within a FreeRTOS task
  xTaskCreateStatic(task, "task", 512, NULL, tskIDLE_PRIORITY + 2, task_stack,
                    &task_buffer);

  vTaskStartScheduler();
  while (1) {
  }

  Error_Handler();
  return 0;
}

// Toggle LED on CAN error
void can_fd_error_callback_hook(FDCAN_HandleTypeDef* hfdcan, uint32_t ErrorStatusITs) {
  HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
}
