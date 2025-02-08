/* UART multithreaded queue test
 * Tests:
 * - TX queue functionality under high load
 * - RX queue functionality when UART is busy
 * - Queue overflow conditions
 */
#include "FreeRTOS.h"
#include "task.h"
#include "stm32xx_hal.h"
#include <string.h>
#include "UART.h"

/* Private defines */
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TEST_PATTERN_SIZE 32  // Larger pattern to ensure queue gets filled, pattern size represents a message
#define TX_BURST_SIZE 100     // Number of messages to send in a burst

/* Private function prototypes */
void Clock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
void TxTask(void *argument);
void RxTask(void *argument);
void Error_Handler(void);

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

// Test data
static uint8_t testPattern[TEST_PATTERN_SIZE];

int main(void) {
    HAL_Init();
    Clock_Config();
    MX_GPIO_Init();
    MX_UART4_Init();

    // Initialize test pattern
    for(int i = 0; i < TEST_PATTERN_SIZE; i++) {
        testPattern[i] = (uint8_t)(i & 0xFF);
    }

    // Create the RX queue statically - smaller size to test overflow
    xRxQueue = xQueueCreateStatic(64,   // Could be smaller, like 32, to test overflow
                                 sizeof(uint8_t),
                                 ucRxQueueStorageArea,
                                 &xRxStaticQueue);
    
    // Initialize UART BSP
    uart_status_t status = uart_init(huart4, &xRxQueue);
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
    const TickType_t fastDelay = pdMS_TO_TICKS(1);    // 1ms between transmissions, could be 0 for maximum stress
    const TickType_t burstDelay = pdMS_TO_TICKS(500); // 500ms between bursts, good for now
    uint32_t txCount = 0;
    uint32_t queueFullCount = 0;
    
    while(1) {
        // Rapid burst transmission to test queue
        for(int i = 0; i < TX_BURST_SIZE; i++) {
            uart_status_t status = uart_send(huart4, testPattern, TEST_PATTERN_SIZE, false);
            txCount++;
            
            if(status == UART_ERR) {
                queueFullCount++;  // Track when queue gets full
                HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
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
        uart_status_t status = uart_recv(huart4, rxBuffer, TEST_PATTERN_SIZE, false);
        
        if(status == UART_RECV) {
            rxCount++;
            
            // Check pattern match
            if(memcmp(rxBuffer, testPattern, TEST_PATTERN_SIZE) == 0) {
                patternMatchCount++;
                HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
            }
            
            // Immediate retry to test queue emptying
            while(uart_recv(huart4, rxBuffer, TEST_PATTERN_SIZE, false) == UART_RECV) {
                rxCount++;
            }
        }
        else if(status == UART_EMPTY) {
            rxEmptyCount++;
        }
        
        vTaskDelay(pollDelay);
    }
}

// Rest of the code (MX_UART4_Init, MX_GPIO_Init, Clock_Config, Error_Handler) same as uart.c


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
void Error_Handler(void)
{
    __disable_irq();
    while (1) {
    }
}