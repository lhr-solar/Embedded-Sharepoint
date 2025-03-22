/* USART test

- Sets up USART loopback mode (the TX line is connected to the RX line)
- Sends messages and verifies correctness
*/
#include "FreeRTOS.h"
#include "task.h"
#include "stm32xx_hal.h"
#include "USART.h"
#include <string.h>

/* Private defines */
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

/* Private function prototypes */
static void MX_GPIO_Init(void);
static void MX_USART1_Init(void);
void TxTask(void *argument);
void RxTask(void *argument);
void Error_Handler(void);

/* Private variables */
extern USART_HandleTypeDef* husart1;

// Static task creation resources
StaticTask_t txTaskBuffer;
StaticTask_t rxTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE];
StackType_t rxTaskStack[configMINIMAL_STACK_SIZE];

// Static queue creation resources
StaticQueue_t xRxStaticQueue;
uint8_t ucRxQueueStorageArea[128];
QueueHandle_t xRxQueue;

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_Init();

    // Initialize USART BSP
    usart_status_t status = usart_init(husart1);
    if (status != USART_OK) {
        Error_Handler();
    }

    // Create the tasks statically
    // xTaskCreateStatic(TxTask, 
    //                  "TX",
    //                  configMINIMAL_STACK_SIZE,
    //                  NULL,
    //                  tskIDLE_PRIORITY + 2,
    //                  txTaskStack,
    //                  &txTaskBuffer);

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

static void MX_USART1_Init(void)
{
    husart1->Instance = USART1;
    husart1->Init.BaudRate = 115200;
    husart1->Init.WordLength = USART_WORDLENGTH_8B;
    husart1->Init.StopBits = USART_STOPBITS_1;
    husart1->Init.Parity = USART_PARITY_NONE;
    husart1->Init.Mode = USART_MODE_TX_RX;
    husart1->Init.CLKPolarity = USART_POLARITY_LOW;
    husart1->Init.CLKPhase = USART_PHASE_1EDGE;
    husart1->Init.CLKLastBit = USART_LASTBIT_DISABLE;
    
    if (HAL_USART_Init(husart1) != HAL_OK) {
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
    uint8_t testData[] = "USART Test Message 123\r\n";
    const uint8_t msgLen = sizeof(testData) - 1;
    uint32_t txCount = 0;
    
    while(1) {
        // Send test message
        usart_status_t status = usart_send(husart1, testData, msgLen, portMAX_DELAY);
        
        if (status == USART_SENT) {
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
        usart_status_t status = usart_recv(husart1, rxBuffer, sizeof(rxBuffer), 0);
        
        if (status == USART_RECV) {
            rxCount++;
            // Echo received data back
            usart_send(husart1, rxBuffer, sizeof(rxBuffer), portMAX_DELAY);
            
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