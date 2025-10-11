#include "UART.h"
#include "stm32xx_hal.h"

#include <stdio.h>
#include "task_print.h"

StaticTask_t initTaskBuffer;
StackType_t initTaskStack[configMINIMAL_STACK_SIZE];

StaticTask_t txTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE * 4];

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
        printf("Hello World! %s %d %f\n\r", "Test String", 5, 4.4);
        vTaskDelay(pdMS_TO_TICKS(100));
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
                      "TX",
                      configMINIMAL_STACK_SIZE*4,
                      NULL,
                      tskIDLE_PRIORITY + 2,
                      txTaskStack,
                      &txTaskBuffer);

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
