/* UART echo test - Send back what it receives
- Connect UART TX (PA0) to AD2(Or other UART device) RX
- Connect UART RX (PA0) to AD2(Or other UART device) TX
- AD2 sends message, UART Peripheral receives it and sends it back (echo)
- LED should stay on once message received (and echoed back consequently)
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
static void MX_UART4_Init(void);
void TxTask(void *argument);
void RxTask(void *argument);
void Error_Handler(void);  
void EchoTask(void *argument);

/* Private variables */
extern UART_HandleTypeDef* huart4;

// Static task creation resources
StaticTask_t echoTaskBuffer;
StackType_t echoTaskStack[configMINIMAL_STACK_SIZE];


int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init(); // Initialize LED gpio
    MX_UART4_Init(); // Initalize UART settings, buad rate, parity bits, etc. 
    // Initialize UART BSP
    uart_status_t status = uart_init(huart4);
    if (status != UART_OK) {
        Error_Handler();
    }

    // Create the tasks statically
    xTaskCreateStatic(
        EchoTask,
        "Echo",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        echoTaskStack,
        &echoTaskBuffer
    );

    // Start the scheduler
    vTaskStartScheduler();

    while (1) {
        // Should never get here
    }
}

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

void EchoTask(void *argument) {
  uint8_t rxBuffer[32]; // Buffer to store received data

  // Initial LED blink to indicate startup
  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
  vTaskDelay(pdMS_TO_TICKS(500));
  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

  while(1) {
      // We are expecting the AD2/other UART device  to send an 8-byte message, Ex: AD2 sends "Test1234"(not null-terminated)
      if(uart_recv(huart4, rxBuffer, 8, pdMS_TO_TICKS(10)) == UART_RECV) {

        // Turn LED on to indicate reception
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
        // Send the received data back; On AD2, we should see the message we sent echoed back
        uart_send(huart4, rxBuffer, 8, portMAX_DELAY);

        // Small Delay
        vTaskDelay(pdMS_TO_TICKS(50));
      }
      // Small delay
      vTaskDelay(pdMS_TO_TICKS(10));
  }
}


void Error_Handler(void)
{
    __disable_irq();
    while (1) {
    }
}