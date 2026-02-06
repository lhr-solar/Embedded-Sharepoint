#include "stm32xx_hal.h"
#include "CAN_FD.h"

StaticTask_t task_buffer;
StackType_t task_stack[512];

#define CAN_LOOPBACK_ENABLED

// If you only want to disable loop, uncomment the line below
// #undef CAN_LOOPBACK_ENABLED


// Most other nucleos have a heartbeat for A5
#define LED_PIN GPIO_PIN_3
#define LED_PORT GPIOC

#define CAN_TX_PROFILE_PORT GPIOA
#define CAN_TX_PROFILE_PIN GPIO_PIN_10

#define CAN_RX_PROFILE_PORT GPIOB
#define CAN_RX_PROFILE_PIN GPIO_PIN_4

void can_profile_pins_init(){
    GPIO_InitTypeDef tx_pin_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = CAN_TX_PROFILE_PIN
    };

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

    GPIO_InitTypeDef rx_pin_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = CAN_RX_PROFILE_PIN
    };

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
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = LED_PIN
    };

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


void G474_SystemClockConfig(){

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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

}

void G473_SystemClockConfig(void)
{
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(){
    while(1){
        // HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        HAL_Delay(1000);
    }
}

void Success_Handler(){
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
}


static void task(void *pvParameters) {


    FDCAN_TxHeaderTypeDef tx_header = {0};   
    tx_header.Identifier = 0x321;
    tx_header.IdType = FDCAN_STANDARD_ID;
    tx_header.TxFrameType = FDCAN_DATA_FRAME;
    tx_header.DataLength = FDCAN_DLC_BYTES_8;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch = FDCAN_BRS_OFF;
    tx_header.FDFormat = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;
    tx_header.MessageMarker = 0;

    // send x1234 to 0x11
    uint8_t tx_data[8] = {0};
    tx_data[0] = 0x12;
    tx_data[1] = 0x34;
    tx_data[2] = 0x56;
    tx_data[3] = 0x78;
    tx_data[4] = 0x9A;
    tx_data[5] = 0xBC;
    tx_data[6] = 0xDE;
    tx_data[7] = 0xFF;

    while(1){

#ifdef FDCAN1
        if (can_fd_send(hfdcan1, &tx_header, tx_data, portMAX_DELAY) == CAN_ERR){
            Error_Handler();
        }

        // to do: implement reading
#endif

#ifdef FDCAN2
        if (can_fd_send(hfdcan2, &tx_header, tx_data, portMAX_DELAY) == CAN_ERR){
            Error_Handler();
        }

        // to do: implement reading
#endif

#ifdef FDCAN3
        if (can_fd_send(hfdcan3, &tx_header, tx_data, portMAX_DELAY) == CAN_ERR){
            Error_Handler();
        }

        // to do: implement reading
#endif
        
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void can_fd_tx_complete_hook(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndexes){
    static uint8_t trippleToggle = 0; // toggle twice at the start, and then toggle once at the end (trippleToggle = 1)
    if(trippleToggle == 0){
        HAL_GPIO_TogglePin(CAN_TX_PROFILE_PORT, CAN_TX_PROFILE_PIN);
    }
    trippleToggle = 1;
    HAL_GPIO_TogglePin(CAN_TX_PROFILE_PORT, CAN_TX_PROFILE_PIN);
}

void can_fd_rx_callback_hook(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs){
    static uint8_t trippleToggle = 0; // toggle twice at the start, and then toggle once at the end (trippleToggle = 1)
    if(trippleToggle == 0){
        HAL_GPIO_TogglePin(CAN_RX_PROFILE_PORT, CAN_RX_PROFILE_PIN);
    }
    trippleToggle = 1;
    HAL_GPIO_TogglePin(CAN_RX_PROFILE_PORT, CAN_RX_PROFILE_PIN);
}

int main(void) {
    HAL_Init();

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    // System clock config can change depending on the target MCU, since the clock tree can be different
    // If you need to use a different MCU, go to cubemx and generate a new system clock config function with the system clock being 80 Mhz
    // It especially varies with nucleo vs direct MCU
    // G473_SystemClockConfig();
    #ifdef STM32G474xx
        G474_SystemClockConfig();
    #elif defined(STM32G473xx)
        G473_SystemClockConfig();
    #else
        SystemClock_Config();
    #endif



    Heartbeat_Init(); // enable LED for LED_PORT
    can_profile_pins_init(); // enable pins we toggle during interrupts

#ifdef FDCAN1
    hfdcan1->Instance = FDCAN1;
    hfdcan1->Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan1->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
    hfdcan1->Init.Mode = FDCAN_MODE_INTERNAL_LOOPBACK;
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
    hfdcan1->Init.StdFiltersNbr = 1;
    hfdcan1->Init.ExtFiltersNbr = 0;
    hfdcan1->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;


    // FDCAN1 Filter Config
    FDCAN_FilterTypeDef sFilterConfig;
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; // directs frames to FIFO0
    sFilterConfig.FilterID1 = 0;
    sFilterConfig.FilterID2 = 0x7FF;
    
    if(can_fd_init(hfdcan1, &sFilterConfig) != CAN_OK){
        Error_Handler();
    }

    if(can_fd_start(hfdcan1) != CAN_OK){
        Error_Handler();
    }
#endif

#ifdef FDCAN2

    hfdcan2->Instance = FDCAN2; 
    hfdcan2->Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan2->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
    hfdcan2->Init.Mode = FDCAN_MODE_EXTERNAL_LOOPBACK;
    hfdcan2->Init.AutoRetransmission = DISABLE;
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
    hfdcan2->Init.ExtFiltersNbr = 0;
    hfdcan2->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

    // FDCAN2 Filter Config
    FDCAN_FilterTypeDef sFilterConfig2;
    sFilterConfig2.IdType = FDCAN_STANDARD_ID;
    sFilterConfig2.FilterIndex = 0;
    sFilterConfig2.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig2.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; // directs frames to FIFO0
    sFilterConfig2.FilterID1 = 0;
    sFilterConfig2.FilterID2 = 0x7FF;

    if(can_fd_init(hfdcan2, &sFilterConfig2) != CAN_OK){
        Error_Handler();
    }

    if(can_fd_start(hfdcan2) != CAN_OK){
        Error_Handler();
    }

#endif

#ifdef FDCAN3

    hfdcan3->Instance = FDCAN3;
    hfdcan3->Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan3->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
    hfdcan3->Init.Mode = FDCAN_MODE_INTERNAL_LOOPBACK;
    hfdcan3->Init.AutoRetransmission = DISABLE;
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
    hfdcan3->Init.ExtFiltersNbr = 0;
    hfdcan3->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

    // FDCAN3 Filter Config
    FDCAN_FilterTypeDef sFilterConfig3;
    sFilterConfig3.IdType = FDCAN_STANDARD_ID;
    sFilterConfig3.FilterIndex = 0;
    sFilterConfig3.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig3.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; // directs frames to FIFO0
    sFilterConfig3.FilterID1 = 0;
    sFilterConfig3.FilterID2 = 0x7FF;

    if(can_fd_init(hfdcan3, &sFilterConfig3) != CAN_OK){
        Error_Handler();
    }

    if(can_fd_start(hfdcan3) != CAN_OK){
        Error_Handler();
    }

#endif


    // you can only send CAN messages within a FreeRTOS task
    xTaskCreateStatic(
                task,
                "task",
                512,
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

static uint32_t HAL_RCC_FDCAN_CLK_ENABLED=0;

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(fdcanHandle->Instance==FDCAN1)
  {
 
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* FDCAN1 clock enable */
    HAL_RCC_FDCAN_CLK_ENABLED++;
    if(HAL_RCC_FDCAN_CLK_ENABLED==1){
      __HAL_RCC_FDCAN_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**FDCAN1 GPIO Configuration
    PA11     ------> FDCAN1_RX
    PA12     ------> FDCAN1_TX
    */ 
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* FDCAN1 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
    HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
  }
  else if(fdcanHandle->Instance==FDCAN2)
  {

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* FDCAN2 clock enable */
    HAL_RCC_FDCAN_CLK_ENABLED++;
    if(HAL_RCC_FDCAN_CLK_ENABLED==1){
      __HAL_RCC_FDCAN_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**FDCAN2 GPIO Configuration
    PB12     ------> FDCAN2_RX
    PB13     ------> FDCAN2_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* FDCAN2 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
    HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);
  
  }
  else if(fdcanHandle->Instance==FDCAN3)
  {
  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* FDCAN3 clock enable */
    HAL_RCC_FDCAN_CLK_ENABLED++;
    if(HAL_RCC_FDCAN_CLK_ENABLED==1){
      __HAL_RCC_FDCAN_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**FDCAN3 GPIO Configuration
    PA8     ------> FDCAN3_RX
    PA15     ------> FDCAN3_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF11_FDCAN3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* FDCAN3 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN3_IT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN3_IT0_IRQn);
    HAL_NVIC_SetPriority(FDCAN3_IT1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN3_IT1_IRQn);

  }
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  if(fdcanHandle->Instance==FDCAN1)
  {
    /* Peripheral clock disable */
    HAL_RCC_FDCAN_CLK_ENABLED--;
    if(HAL_RCC_FDCAN_CLK_ENABLED==0){
      __HAL_RCC_FDCAN_CLK_DISABLE();
    }

    /**FDCAN1 GPIO Configuration
    PA11     ------> FDCAN1_RX
    PA12     ------> FDCAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* FDCAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
    HAL_NVIC_DisableIRQ(FDCAN1_IT1_IRQn);
  /* USER CODE BEGIN FDCAN1_MspDeInit 1 */

  /* USER CODE END FDCAN1_MspDeInit 1 */
  }
  else if(fdcanHandle->Instance==FDCAN2)
  {
    /* Peripheral clock disable */
    HAL_RCC_FDCAN_CLK_ENABLED--;
    if(HAL_RCC_FDCAN_CLK_ENABLED==0){
      __HAL_RCC_FDCAN_CLK_DISABLE();
    }

    /**FDCAN2 GPIO Configuration
    PB12     ------> FDCAN2_RX
    PB13     ------> FDCAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);

    /* FDCAN2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
    HAL_NVIC_DisableIRQ(FDCAN2_IT1_IRQn);
  /* USER CODE BEGIN FDCAN2_MspDeInit 1 */

  /* USER CODE END FDCAN2_MspDeInit 1 */
  }
  else if(fdcanHandle->Instance==FDCAN3)
  {
    /* Peripheral clock disable */
    HAL_RCC_FDCAN_CLK_ENABLED--;
    if(HAL_RCC_FDCAN_CLK_ENABLED==0){
      __HAL_RCC_FDCAN_CLK_DISABLE();
    }

    /**FDCAN3 GPIO Configuration
    PA8     ------> FDCAN3_RX
    PA15     ------> FDCAN3_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8|GPIO_PIN_15);

    /* FDCAN3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(FDCAN3_IT0_IRQn);
    HAL_NVIC_DisableIRQ(FDCAN3_IT1_IRQn);
  }
}
