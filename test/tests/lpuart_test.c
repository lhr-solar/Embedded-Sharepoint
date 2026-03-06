/* LPUART test

- Setups up LPUART loopback mode (the TX line is connected to the RX line)
- Send messages and verify correctness
*/
#include "stm32xx_hal.h"
#include "UART.h"

void TxTask(void *argument);
void RxTask(void *argument);

// Static task creation resources
StaticTask_t txTaskBuffer;
StaticTask_t rxTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE];
StackType_t rxTaskStack[configMINIMAL_STACK_SIZE];

// Heartbeat pin on stm32l432kcu is PB3
#if defined(STM32L432xx)
    #define LED_PIN GPIO_PIN_3
    #define LED_PORT GPIOB
#elif defined(STM32L431xx)
    #define LED_PIN GPIO_PIN_11
    #define LED_PORT GPIOB
#elif defined(STM32G473xx)
    // LSOM
    #define LED_PIN GPIO_PIN_3
    #define LED_PORT GPIOC
#else
// Most other nucleos have a heartbeat for A5
    #define LED_PIN GPIO_PIN_5
    #define LED_PORT GPIOA
#endif

// Initialize clock for heartbeat LED port
void Heartbeat_Clock_Init() {
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
}

void TxTask(void *argument){

    const TickType_t xDelay = pdMS_TO_TICKS(1000);  // 1 second delay
    uint8_t testData[] = "Test Message 123\r\n";
    const uint8_t msgLen = sizeof(testData) - 1;
    uint32_t txCount = 0;
    
    while(1) {
        // // Send test message
        uart_status_t status = uart_send(hlpuart1, testData, msgLen, portMAX_DELAY);
        
        if (status == UART_SENT) {
            txCount++;
            // Toggle LED to indicate successful transmission
            HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        }
        
        vTaskDelay(xDelay);
    }
}

void RxTask(void *argument){

}

int main(void) {

    HAL_Init();
    SystemClock_Config();

    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = LED_PIN
    };
    
    Heartbeat_Clock_Init(); // enable clock for LED_PORT
    HAL_GPIO_Init(LED_PORT, &led_config); // initialize GPIOA with led_config

    hlpuart1->Instance = LPUART1;
    hlpuart1->Init.BaudRate = 115200;
    hlpuart1->Init.WordLength = UART_WORDLENGTH_8B;
    hlpuart1->Init.StopBits = UART_STOPBITS_1;
    hlpuart1->Init.Parity = UART_PARITY_NONE;
    hlpuart1->Init.Mode = UART_MODE_TX_RX;
    hlpuart1->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    hlpuart1->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    hlpuart1->Init.ClockPrescaler = UART_PRESCALER_DIV1;
    hlpuart1->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(hlpuart1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(hlpuart1) != HAL_OK)
    {
        Error_Handler();
    }

    // Initialize UART BSP
    uart_status_t status = uart_init(hlpuart1);
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

    // xTaskCreateStatic(RxTask,
    //                  "RX", 
    //                  configMINIMAL_STACK_SIZE,
    //                  NULL,
    //                  tskIDLE_PRIORITY + 2,
    //                  rxTaskStack,
    //                  &rxTaskBuffer);

    // Start the scheduler
    vTaskStartScheduler();

    while (1) {
        // Should never get here
    }
}


void HAL_UART_MspGPIOInit(UART_HandleTypeDef* huart){
    GPIO_InitTypeDef GPIO_InitStruct = {0}; 
#ifdef LPUART1
    if(huart->Instance == LPUART1){
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**LPUART1 GPIO Configuration
        PB10     ------> LPUART1_RX
        PB11     ------> LPUART1_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
#endif
}

void Error_Handler(void)
{
    while (1) {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
    }
}
