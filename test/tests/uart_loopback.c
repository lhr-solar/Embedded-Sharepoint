/* UART test
- Simple Sanity Check
- Connect TX (PA0) to RX (PA1)
- LED should stay on, indicating RX received TX data
- Disconnect TX from RX
- LED should stay off, since RX did not receive TX data since it was disconnected
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
void LoopbackTask(void *argument);

/* Private variables */
extern UART_HandleTypeDef* huart4;

// Static task creation resources
StaticTask_t loopbackTaskBuffer;
StackType_t loopbackTaskStack[configMINIMAL_STACK_SIZE];

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_UART4_Init();

    
    // Initialize UART BSP
    uart_status_t status = uart_init(huart4);
    if (status != UART_OK) {
        Error_Handler();
    }

    // Create the tasks statically
    xTaskCreateStatic(
        LoopbackTask,
        "Loopback",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        loopbackTaskStack,
        &loopbackTaskBuffer
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

void LoopbackTask(void *argument)
{
    // Initial LED blink

    uint8_t rxBuffer[32];
    uint8_t testMessage[8] = "Test123"; // Arbitrary test message
    uint8_t msgLen = 8; // Including the null terminator, strlen(testMessage) would be 7
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    vTaskDelay(pdMS_TO_TICKS(500));
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

    uart_send(huart4, testMessage, msgLen, portMAX_DELAY);


    vTaskDelay(pdMS_TO_TICKS(1000));

    // Try to receive
    if(uart_recv(huart4, rxBuffer, msgLen, pdMS_TO_TICKS(100)) == UART_RECV) {
    // Success - turn LED on
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
        // Optionally send the copy of receive data back or a success message
    //  uart_send(huart4, successMsg, strlen((char*)successMsg), portMAX_DELAY);
    } else {
        // Failure - turn LED off
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    }


}


void Error_Handler(void)
{
    __disable_irq();
    while (1) {
    }
}