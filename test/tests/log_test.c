// A simple echo application to test input and output over serial
#include "UART.h"
#include "task_print.h"
#include "stm32xx_hal.h"
#include <stdio.h>

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
#define LOGGING_LEVEL 2
#include "log.h"

StaticTask_t initTaskBuffer;
StackType_t initTaskStack[configMINIMAL_STACK_SIZE];

StaticTask_t txTaskBuffer1;
StackType_t txTaskStack1[configMINIMAL_STACK_SIZE * 4];

StaticTask_t txTaskBuffer2;
StackType_t txTaskStack2[configMINIMAL_STACK_SIZE * 4];

StaticTask_t txTaskBuffer3;
StackType_t txTaskStack3[configMINIMAL_STACK_SIZE * 4];

StaticTask_t txTaskBuffer4;
StackType_t txTaskStack4[configMINIMAL_STACK_SIZE * 4];

StaticTask_t txTaskBuffer5;
StackType_t txTaskStack5[configMINIMAL_STACK_SIZE * 4];

StaticTask_t printTaskBuffer;
StackType_t printTaskStack[configMINIMAL_STACK_SIZE * 4];

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
        log(L_FATAL, "%s",  "FATAL");
        log(L_ERROR, "%s",  "ERROR");
        log(L_WARN, "%s",  "WARN");
        log(L_INFO, "%s",  "INFO");
        log(L_DEBUG, "%s",  "DEBUG");
        log(L_TRACE, "%s",  "TRACE");
        vTaskDelay(pdMS_TO_TICKS(1000));
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

    task_print_init(husart2);

    xTaskCreateStatic(task_print,
                      "Print Task",
                      configMINIMAL_STACK_SIZE,
                      husart2,
                      tskIDLE_PRIORITY + 2,
                      printTaskStack,
                      &printTaskBuffer);

    xTaskCreateStatic(TxTask, 
                      "TX1",
                      configMINIMAL_STACK_SIZE*4,
                      NULL,
                      tskIDLE_PRIORITY + 2,
                      txTaskStack1,
                      &txTaskBuffer1);
    xTaskCreateStatic(TxTask, 
                      "TX2",
                      configMINIMAL_STACK_SIZE*4,
                      NULL,
                      tskIDLE_PRIORITY + 2,
                      txTaskStack2,
                      &txTaskBuffer2);
    xTaskCreateStatic(TxTask, 
                      "TX3",
                      configMINIMAL_STACK_SIZE*4,
                      NULL,
                      tskIDLE_PRIORITY + 2,
                      txTaskStack3,
                      &txTaskBuffer3);
    xTaskCreateStatic(TxTask, 
                      "TX4",
                      configMINIMAL_STACK_SIZE*4,
                      NULL,
                      tskIDLE_PRIORITY + 2,
                      txTaskStack4,
                      &txTaskBuffer4);
//    xTaskCreateStatic(TxTask, 
//                      "TX5",
//                      configMINIMAL_STACK_SIZE*4,
//                      NULL,
//                      tskIDLE_PRIORITY + 2,
//                      txTaskStack5,
//                      &txTaskBuffer5);

    vTaskDelete(NULL);

    while(1);
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
