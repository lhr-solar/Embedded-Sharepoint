// A simple echo application to test input and output over serial
#include "UART.h"
#include "projdefs.h"
#include "stm32xx_hal.h"
#include <stdio.h>
#include "printf.h"

// Enables logging
#define LOGGING_ENABLE 1

// MUST BE DEFINED AS AN INTEGER
// Logging levels:
// L_FATAL: 1
// L_ERROR: 2
// L_WARN: 3
// L_DEBUG: 4
// L_TRACE: 5
#define LOGGING_LEVEL 4
#include "log.h"

StaticTask_t txTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE];

void HAL_UART_MspGPIOInit(UART_HandleTypeDef *huart){
    GPIO_InitTypeDef init = {0};
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

void TxTask(void *argument){
    husart2->Init.BaudRate = 115200;
    husart2->Init.WordLength = UART_WORDLENGTH_8B;
    husart2->Init.StopBits = UART_STOPBITS_1;
    husart2->Init.Parity = UART_PARITY_NONE;
    husart2->Init.Mode = UART_MODE_TX_RX;
    husart2->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart2->Init.OverSampling = UART_OVERSAMPLING_16;
    
    printf_init(husart2);

    while(1){
        log(L_FATAL, "Test Fatal");
        log(L_ERROR, "Test Error");
        log(L_WARN, "Test Warn");
        log(L_INFO, "Test Info");
        log(L_DEBUG, "Test Debug");
        log(L_TRACE, "Test Trace");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();

    xTaskCreateStatic(TxTask, 
                     "TX",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     tskIDLE_PRIORITY + 2,
                     txTaskStack,
                     &txTaskBuffer);

    vTaskStartScheduler();

    while (1) {
    }
}
