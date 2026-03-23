#include "movingAverage.h"
#include "stm32xx_hal.h"
#include "stdio.h"
#include "UART.h"
#include "printf.h"

StaticTask_t txTaskBuffer;
StackType_t txTaskStack[configMINIMAL_STACK_SIZE];

DEFINE_MOVING_AVG(MA_u16, uint16_t, uint32_t)
DEFINE_MOVING_AVG(MA_u32, uint32_t, uint64_t)
DEFINE_MOVING_AVG(MA_s32, int32_t,  int64_t)

void test_u16() {
    printf("\n--- uint16_t test ---\n\r");

    MA_u16 ma;
    MA_u16_init(&ma);

    uint16_t inputs[] = {2, 4, 6, 8, 10, 12};

    for (int i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
        uint16_t avg = MA_u16_update(&ma, inputs[i]);
        printf("input=%u -> avg=%u\n\r", inputs[i], avg);
    }
}

void test_u32() {
    printf("\n--- uint32_t test ---\n\r");

    MA_u32 ma;
    MA_u32_init(&ma);

    uint32_t inputs[] = {100, 200, 300, 400, 500, 600};

    for (int i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
        uint32_t avg = MA_u32_update(&ma, inputs[i]);
        printf("input=%lu -> avg=%lu\n\r", inputs[i], avg);
    }
}

void test_s32() {
    printf("\n--- int32_t test ---\n\r");

    MA_s32 ma;
    MA_s32_init(&ma);

    int32_t inputs[] = {-2, -4, 6, 8, 10, -10};

    for (int i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
        int32_t avg = MA_s32_update(&ma, inputs[i]);
        printf("input=%ld -> avg=%ld\n\r", inputs[i], avg);
    }
}

void TxTask(void *argument){

#ifdef USART3
    husart3->Init.BaudRate = 115200;
    husart3->Init.WordLength = UART_WORDLENGTH_8B;
    husart3->Init.StopBits = UART_STOPBITS_1;
    husart3->Init.Parity = UART_PARITY_NONE;
    husart3->Init.Mode = UART_MODE_TX_RX;
    husart3->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart3->Init.OverSampling = UART_OVERSAMPLING_16;

    printf_init(husart3);
#endif /*USART3 */

    printf("Running moving average tests...\n\r");

    test_u16();
    test_u32();
    test_s32();

    while(1){}
}



int main() {

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


    while(1){}


    return 0;
}

void HAL_UART_MspGPIOInit(UART_HandleTypeDef *huart){

#ifdef USART3
    GPIO_InitTypeDef init = {0};
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PC10     ------> USART3_TX
    PC11     ------> USART3_RX
    */
    init.Pin = GPIO_PIN_10|GPIO_PIN_11;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_LOW;
    init.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOC, &init);
#endif

}