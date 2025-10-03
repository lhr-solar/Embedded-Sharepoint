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
// L_INFO: 4
// L_DEBUG: 5
// L_TRACE: 6
#define LOGGING_LEVEL 4
#include "log.h"

StaticTask_t initTaskBuffer;
StackType_t initTaskStack[configMINIMAL_STACK_SIZE];

StaticTask_t txTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE*2];

StaticTask_t txTaskBuffer2;
StackType_t txTaskStack2[configMINIMAL_STACK_SIZE*2];

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
    while(1){
        log(L_FATAL, "TxTask1 Fatal");
        log(L_ERROR, "TxTask1 Error");
        log(L_WARN, "TxTask1 Warn");
        log(L_INFO, "TxTask1 Info");
        log(L_DEBUG, "TxTask1 Debug");
        log(L_TRACE, "TxTask1 Trace");
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}

void TxTask2(void *argument){
    while(1){
        log(L_FATAL, "TxTask2 Fatal");
        log(L_ERROR, "TxTask2 Error");
        log(L_WARN, "TxTask2 Warn");
        log(L_INFO, "TxTask2 Info");
        log(L_DEBUG, "TxTask2 Debug");
        log(L_TRACE, "TxTask2 Trace");
        taskYIELD();
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void InitTask(void *argument){
    husart2->Init.BaudRate = 115200;
    husart2->Init.WordLength = UART_WORDLENGTH_8B;
    husart2->Init.StopBits = UART_STOPBITS_1;
    husart2->Init.Parity = UART_PARITY_NONE;
    husart2->Init.Mode = UART_MODE_TX_RX;
    husart2->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart2->Init.OverSampling = UART_OVERSAMPLING_16;
    
    printf_init(husart2);

    xTaskCreateStatic(TxTask, 
                     "TX",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     tskIDLE_PRIORITY + 1,
                     txTaskStack,
                     &txTaskBuffer);

    xTaskCreateStatic(TxTask2, 
                     "TX2",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     tskIDLE_PRIORITY + 2,
                     txTaskStack2,
                     &txTaskBuffer2);

    vTaskDelete(NULL);
}

int main(void) {
    HAL_Init();
    SystemClock_Config();

    xTaskCreateStatic(InitTask, 
                     "Init",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     tskIDLE_PRIORITY + 3,
                     initTaskStack,
                     &initTaskBuffer);

    vTaskStartScheduler();

    while (1) {
    }
}
