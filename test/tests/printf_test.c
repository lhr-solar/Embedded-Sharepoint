// A simple echo application to test input and output over serial
#include "FreeRTOS.h"
#include "UART.h"
#include "stm32xx_hal.h"
#include "printf.h"

StaticTask_t txTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE];

StaticTask_t rxTaskBuffer;
StackType_t rxTaskStack[configMINIMAL_STACK_SIZE];

StaticTask_t initTaskBuffer;
StackType_t initTaskStack[configMINIMAL_STACK_SIZE];

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

// Semaphore for the buffer
StaticSemaphore_t buf_sem4_buffer;
SemaphoreHandle_t buf_sem4;

#define BUF_LEN 65
char buf[BUF_LEN];

int test;

void RxTask(void *argument){
    while(1){
        scanf("%d", &test);
        printf("helo world %d\n\r", test);
        vTaskDelay(pdMS_TO_TICKS(100));
        //xSemaphoreGive(buf_sem4);
    }   
}

// void TxTask(void *argument){
//     while(1){
//         xSemaphoreTake(buf_sem4, portMAX_DELAY);
//         printf("%s", buf);
//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }

void InitTask(void *argument){
    husart2->Init.BaudRate = 115200;
    husart2->Init.WordLength = UART_WORDLENGTH_8B;
    husart2->Init.StopBits = UART_STOPBITS_1;
    husart2->Init.Parity = UART_PARITY_NONE;
    husart2->Init.Mode = UART_MODE_TX_RX;
    husart2->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart2->Init.OverSampling = UART_OVERSAMPLING_16;
    
    printf_init(husart2);
    // xSemaphoreCreateBinaryStatic(&buf_sem4_buffer);

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

    while(1){}
}

int main(void) {
    HAL_Init();
    SystemClock_Config();

    xTaskCreateStatic(InitTask, 
                     "Init",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     tskIDLE_PRIORITY + 2,
                     initTaskStack,
                     &initTaskBuffer);

    vTaskStartScheduler();

    while (1) {
    }
}
