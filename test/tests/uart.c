/* UART test

- Setups up UART loopback mode (the TX line is connected to the RX line)
- Send messages and verify correctness
*/
#include "stm32xx_hal.h"
#include "UART.h"

/* Private defines */
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

/* Private function prototypes */
static void MX_GPIO_Init(void);
void TxTask(void *argument);
void RxTask(void *argument);

// Static task creation resources
StaticTask_t txTaskBuffer;
StaticTask_t rxTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE];
StackType_t rxTaskStack[configMINIMAL_STACK_SIZE];

#ifdef UART4
#define huart huart4
#else
#define huart husart1
#endif

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    huart->Init.BaudRate = 115200;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;

    #ifdef STM32L4xx
    huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    #endif /* STM32L4xx */
    
    // Initialize UART BSP
    uart_status_t status = uart_init(huart);
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

    while (1) {
        // Should never get here
    }
}

void HAL_UART_MspGPIOInit(UART_HandleTypeDef* huart){
    GPIO_InitTypeDef init = {0}; 
    #ifdef STM32F446xx
    #ifdef USART2
    if(huart->Instance == USART2) {
        //enable port A clock
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
    }
    #endif /* USART2 */
    #else
    (void)init;
    #endif /* STM32F446xx */
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
        uart_status_t status = uart_send(huart, testData, msgLen, portMAX_DELAY);
        
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
    uint8_t rxBuffer;
    uint32_t rxCount = 0;
    
    while(1) {
        // Try to receive data
        uart_status_t status = uart_recv(huart, &rxBuffer, 1, 0);
        
        if (status == UART_RECV) {
            rxCount++;
            // Echo received data back
            uart_send(huart, &rxBuffer, 1, portMAX_DELAY);
            
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
