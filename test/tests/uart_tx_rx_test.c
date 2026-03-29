/* UART TX/RX queue test
* Tests:
* - TX queue functionality under high load
* - RX queue functionality when UART is busy
* - Queue overflow conditions
*/
#include "stm32xx_hal.h"
#include "UART.h"

#include <string.h>

/* Private defines */
#define TEST_PATTERN_SIZE 64  // Larger pattern to ensure queue gets filled, pattern size represents a message
#define TX_BURST_SIZE 100     // Number of messages to send in a burst

#ifdef STM32F4xx
#define huart husart2
#define HEARTBEAT_PIN GPIO_PIN_5
#define HEARTBEAT_GPIO_PORT GPIOA
#endif

#ifdef STM32G4xx
#define huart husart3
#define HEARTBEAT_PIN GPIO_PIN_11
#define HEARTBEAT_GPIO_PORT GPIOC
#endif

// Test data
static uint8_t testPattern[TEST_PATTERN_SIZE];

/* Private function prototypes */
static void MX_GPIO_Init(void); // Initialize LED gpio 
void TxTask(void *argument);
void RxTask(void *argument);

// Static task creation resources
StaticTask_t txTaskBuffer;
StaticTask_t rxTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE];
StackType_t rxTaskStack[configMINIMAL_STACK_SIZE];

// Test data
static uint8_t testPattern[TEST_PATTERN_SIZE];

void HAL_UART_MspGPIOInit(UART_HandleTypeDef *huart){
    GPIO_InitTypeDef init = {0};
    UNUSED(init);

#ifdef STM32F4xx
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* enable port A USART2 gpio
    PA2 -> USART2_TX
    PA3 -> USART2_RX
    */
    init.Pin = GPIO_PIN_2|GPIO_PIN_3;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &init);
#endif

#ifdef STM32G4xx
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* enable port C USART3 gpio
    PC10 -> USART3_TX
    PC11 -> USART3_RX
    */
    init.Pin = GPIO_PIN_10|GPIO_PIN_11;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOC, &init);
#endif
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    
#ifdef STM32F4xx
    husart2->Init.BaudRate = 115200;
    husart2->Init.WordLength = UART_WORDLENGTH_8B;
    husart2->Init.StopBits = UART_STOPBITS_1;
    husart2->Init.Parity = UART_PARITY_NONE;
    husart2->Init.Mode = UART_MODE_TX_RX;
    husart2->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart2->Init.OverSampling = UART_OVERSAMPLING_16;
#endif

#ifdef STM32G4xx
    husart3->Init.BaudRate = 115200;
    husart3->Init.WordLength = UART_WORDLENGTH_8B;
    husart3->Init.StopBits = UART_STOPBITS_1;
    husart3->Init.Parity = UART_PARITY_NONE;
    husart3->Init.Mode = UART_MODE_TX_RX;
    husart3->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart3->Init.OverSampling = UART_OVERSAMPLING_16;
#endif
    
    if (HAL_UART_Init(huart) != HAL_OK) {
      Error_Handler();
    }

    // Initialize test pattern
    for(int i = 0; i < TEST_PATTERN_SIZE; i++) {
        testPattern[i] = ((char)(i & 0xFF)) + 'A';
    }
    
    // Initialize UART BSP
    uart_status_t status = uart_init(huart);
    if (status != UART_OK) {
        Error_Handler();
    }

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

    vTaskStartScheduler();
    while(1);
}

void TxTask(void *argument)
{
  const TickType_t fastDelay = pdMS_TO_TICKS(5);    // 1ms between transmissions, could be 0 for maximum stress
  const TickType_t burstDelay = pdMS_TO_TICKS(500); // 250ms between bursts, good for now
  
  volatile uint32_t txCount = 0;
  volatile uint32_t queueFullCount = 0;
  UNUSED(txCount); UNUSED(queueFullCount);
  
  while(1) {
    // Rapid burst transmission to test queue
    for(int i = 0; i < TX_BURST_SIZE; i++) {
      uart_status_t status = uart_send(huart, testPattern, TEST_PATTERN_SIZE, 0);
      txCount++;
      
      if(status == UART_ERR) {
        queueFullCount++;  // Track when queue gets full
        HAL_GPIO_WritePin(HEARTBEAT_GPIO_PORT, HEARTBEAT_PIN, GPIO_PIN_SET);
      } else {
        HAL_GPIO_WritePin(HEARTBEAT_GPIO_PORT, HEARTBEAT_PIN, GPIO_PIN_RESET);
      }
      
      vTaskDelay(fastDelay);
    }
    
    // Allow some time for queue to process
    vTaskDelay(burstDelay);
  }
}

void RxTask(void *argument)
{
  const TickType_t pollDelay = pdMS_TO_TICKS(5);  // 5ms polling rate
  uint8_t rxBuffer[TEST_PATTERN_SIZE];
  uint32_t rxCount = 0;
  uint32_t rxEmptyCount = 0;
  uint32_t patternMatchCount = 0;
  
  while(1) {
    uart_status_t status = uart_recv(huart, rxBuffer, TEST_PATTERN_SIZE, 0);
    
    if(status == UART_OK) {
      rxCount++;
      
      // Check pattern match
      if(memcmp(rxBuffer, testPattern, TEST_PATTERN_SIZE) == 0) {
        patternMatchCount++;
        HAL_GPIO_TogglePin(HEARTBEAT_GPIO_PORT, HEARTBEAT_PIN);
      }
      
      // Immediate retry to test queue emptying
      while(uart_recv(huart, rxBuffer, TEST_PATTERN_SIZE, 0) == UART_OK) {
        rxCount++;
      }
    }
    else if(status == UART_EMPTY) {
      rxEmptyCount++;
    }
    
    vTaskDelay(pollDelay);
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  /* GPIO Ports Clocks Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(HEARTBEAT_GPIO_PORT, HEARTBEAT_PIN, GPIO_PIN_RESET);
  
  /* Configure LED pin */
  GPIO_InitStruct.Pin = HEARTBEAT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(HEARTBEAT_GPIO_PORT, &GPIO_InitStruct);
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}

#ifdef STM32L4xx
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }
}
#endif
#ifdef STM32G4xx
void SystemClock_Config(void)
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
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 20;
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
#endif
