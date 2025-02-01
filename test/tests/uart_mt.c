/* UART multithreaded test

- Setups up UART loopback mode (the TX line is connected to the RX line)
- Send messages and verify correctness
*/
#include "FreeRTOS.h"
#include "task.h"
#include "stm32xx_hal.h"
#include <string.h>
#include "UART.h"

/* Private function prototypes */
void Clock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
void TxTask(void *argument);
void RxTask(void *argument);

/* Private variables */
UART_HandleTypeDef huart4;

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
const uint8_t testPattern[] = {0xA5, 0x5A, 0xB4, 0x4B}; // Easily recognizable pattern

int main(void) {
    HAL_Init();
    Clock_Config();
    MX_GPIO_Init();
    MX_UART4_Init();

    // Create the RX queue statically
    xRxQueue = xQueueCreateStatic(128,
                                 sizeof(uint8_t),
                                 ucRxQueueStorageArea,
                                 &xRxStaticQueue);
    
    // Initialize UART BSP
    uart_status_t status = uart_init(&huart4, &xRxQueue);
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

    vTaskStartScheduler();
    while(1);
}

void TxTask(void *argument)
{
    const TickType_t fastDelay = pdMS_TO_TICKS(1);   // 1ms between transmissions
    const TickType_t slowDelay = pdMS_TO_TICKS(100); // 100ms pause between bursts
    uint32_t txCount = 0;
    uint32_t successCount = 0;
    
    while(1) {
        // Send burst of data to test queue
        for(int i = 0; i < 20; i++) {  // Send enough data to fill queue
            uart_status_t status = uart_send(&huart4, testPattern, sizeof(testPattern), false);
            txCount++;
            
            if(status == UART_SENT) {
                successCount++;
                HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
            }
            
            vTaskDelay(fastDelay); // Small delay between sends
        }
        
        // Delay to allow queue to clear
        vTaskDelay(slowDelay);
        
        // Status check every 100 transmissions
        if(txCount % 100 == 0) {
            // Could add debug output here if needed
            HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, (successCount == txCount) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
    }
}

void RxTask(void *argument)
{
    const TickType_t pollDelay = pdMS_TO_TICKS(10);  // 10ms between queue checks
    uint8_t rxBuffer[32];  // Larger buffer to test multiple bytes
    uint32_t rxCount = 0;
    uint32_t patternCount = 0;
    
    while(1) {
        uart_status_t status = uart_recv(&huart4, rxBuffer, sizeof(testPattern), false);
        
        if(status == UART_RECV) {
            rxCount++;
            
            // Check if received data matches test pattern
            if(memcmp(rxBuffer, testPattern, sizeof(testPattern)) == 0) {
                patternCount++;
                HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
            }
            
            // Test queue overflow handling by immediately trying to receive more
            while(uart_recv(&huart4, rxBuffer, sizeof(testPattern), false) == UART_RECV) {
                rxCount++;
            }
        }
        
        // Status check every 100 receptions
        if(rxCount % 100 == 0 && rxCount > 0) {
            // Could add debug output here if needed
            HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, (patternCount > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
        
        vTaskDelay(pollDelay);
    }
}


static void MX_UART4_Init(void)
{
    huart4.Instance = UART4;
    huart4.Init.BaudRate = 115200;
    huart4.Init.WordLength = UART_WORDLENGTH_8B;
    huart4.Init.StopBits = UART_STOPBITS_1;
    huart4.Init.Parity = UART_PARITY_NONE;
    huart4.Init.Mode = UART_MODE_TX_RX;
    huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart4.Init.OverSampling = UART_OVERSAMPLING_16;
    huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    
    if (HAL_UART_Init(&huart4) != HAL_OK) {
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



void UART4_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart4);
}

void SystemClock_Config(void)
{
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

void Error_Handler(void)
{
    __disable_irq();
    while (1) {
    }
}