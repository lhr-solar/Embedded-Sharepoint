/* UART test

- Setups up UART loopback mode (the TX line is connected to the RX line)
- Send messages and verify correctness
*/
#include "stm32xx_hal.h"
#include "UART.h"

void Error_Handler(void)
{
    __disable_irq();
    while (1) {
    }
}

#if defined(UART4)

/* Private defines */
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

/* Private function prototypes */
void Clock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
void TxTask(void *argument);
void RxTask(void *argument);
void Error_Handler(void);  // Add this line

/* Private variables */
extern UART_HandleTypeDef* huart4;

// Static task creation resources
StaticTask_t txTaskBuffer;
StaticTask_t rxTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE];
StackType_t rxTaskStack[configMINIMAL_STACK_SIZE];

// Static queue creation resources
StaticQueue_t xRxStaticQueue;
uint8_t ucRxQueueStorageArea[128];
QueueHandle_t xRxQueue;

static void MX_UART4_Init(void)
{
    huart4->Instance = UART4;
    huart4->Init.BaudRate = 115200;
    huart4->Init.WordLength = UART_WORDLENGTH_8B;
    huart4->Init.StopBits = UART_STOPBITS_1;
    huart4->Init.Parity = UART_PARITY_NONE;
    huart4->Init.Mode = UART_MODE_TX_RX;
    huart4->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart4->Init.OverSampling = UART_OVERSAMPLING_16;

    #ifdef STM32L4xx
    huart4->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart4->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    #endif /* STM32L4xx */
    
    if (HAL_UART_Init(huart4) != HAL_OK) {
        Error_Handler();
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

    /* Configure LED pin */
    GPIO_InitStruct.Pin = LD2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);
}

void TxTask(void *argument)
{
    const TickType_t xDelay = pdMS_TO_TICKS(1000);  // 1 second delay
    uint8_t testData[] = "Test Message 123\r\n";
    const uint8_t msgLen = sizeof(testData) - 1;
    uint32_t txCount = 0;
    
    while(1) {
        // Send test message
        uart_status_t status = uart_send(huart4, testData, msgLen, portMAX_DELAY);
        
        if (status == UART_SENT) {
            txCount++;
            // Toggle LED to indicate successful transmission
            HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        }
        
        vTaskDelay(xDelay);
    }
}

void RxTask(void *argument)
{
    const TickType_t xDelay = pdMS_TO_TICKS(100);  // 100ms polling interval
    uint8_t rxBuffer[32];
    uint32_t rxCount = 0;
    
    while(1) {
        // Try to receive data
        uart_status_t status = uart_recv(huart4, rxBuffer, sizeof(rxBuffer), 0);
        
        if (status == UART_RECV) {
            rxCount++;
            // Echo received data back
            uart_send(huart4, rxBuffer, sizeof(rxBuffer), portMAX_DELAY);
            
            // Toggle LED to indicate successful reception
            HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        }
        
        vTaskDelay(xDelay);
    }
}


void Clock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

#ifdef STM32L4xx
  // L4 series configuration
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;

#else
  // F4 series configuration
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
#endif

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

#ifdef STM32L4xx
  // L4 series specific clock configuration
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
#else
  // F4 series specific clock configuration
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
#endif
  {
    Error_Handler();
  }
}
#endif

int main(void) {
  #if defined(UART4)
  HAL_Init();
  Clock_Config();
  MX_GPIO_Init();
  MX_UART4_Init();

  
  // Initialize UART BSP
  uart_status_t status = uart_init(huart4);
  if (status != UART_OK) {
      Error_Handler();
  }

  // Create the tasks statically
  xTaskCreateStatic(TxTask, 
                   "TX",
                   configMINIMAL_STACK_SIZE,
                   NULL,
                   tskIDLE_PRIORITY + 2,
                   txTaskStack,
                   &txTaskBuffer);

  xTaskCreateStatic(RxTask,
                   "RX", 
                   configMINIMAL_STACK_SIZE,
                   NULL,
                   tskIDLE_PRIORITY + 2,
                   rxTaskStack,
                   &rxTaskBuffer);

  // Start the scheduler
  vTaskStartScheduler();
  #endif

  Error_Handler();
  return 0;
}