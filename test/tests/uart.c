/* UART test

- Setups up UART loopback mode (the TX line is connected to the RX line)
- Send messages and verify correctness
*/
#include "FreeRTOS.h"
#include "task.h"
#include "stm32xx_hal.h"
#include "UART.h"
#include <string.h>

/* Private defines */
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

/* Private function prototypes */
static void MX_GPIO_Init(void);
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

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

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

    
    // Initialize UART BSP
    uart_status_t status = uart_init(huart4);
    if (status != UART_OK) {
        Error_Handler();
    }

    // Create the tasks statically
  //  xTaskCreateStatic(TxTask, 
  //                   "TX",
  //                   configMINIMAL_STACK_SIZE,
  //                   NULL,
  //                   tskIDLE_PRIORITY + 2,
  //                   txTaskStack,
  //                   &txTaskBuffer);

    xTaskCreateStatic(RxTask,
                     "RX", 
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     tskIDLE_PRIORITY + 2,
                     rxTaskStack,
                     &rxTaskBuffer);

    // Start the scheduler
    vTaskStartScheduler();

    while (1) {
        // Should never get here
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

void Error_Handler(void)
{
    __disable_irq();
    while (1) {
    }
}
