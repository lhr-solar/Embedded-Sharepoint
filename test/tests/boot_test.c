#include "stm32xx_hal.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"

#include "boot_config.h"

#include "FreeRTOS.h"
#include "task.h"

uint8_t* shared_mem;

#define USART1_TX_PIN GPIO_PIN_9
#define USART1_TX_PORT GPIOA
#define USART1_RX_PIN GPIO_PIN_10
#define USART1_RX_PORT GPIOA

static GPIO_InitTypeDef USART2_GPIO_TxCfg = {
    .Pin = USART1_TX_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FAST,
    .Alternate = GPIO_AF7_USART2
};

static GPIO_InitTypeDef USART2_GPIO_RxCfg = {
    .Pin = USART1_RX_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FAST,
    .Alternate = GPIO_AF7_USART2
};

static USART_HandleTypeDef USART2_Handle = {
    .Instance = USART1,
    .Init.BaudRate = 9600,
    .Init.WordLength = USART_WORDLENGTH_8B,
    .Init.StopBits = USART_STOPBITS_1,
    .Init.Parity = USART_PARITY_NONE,
    .Init.Mode = USART_MODE_TX_RX
};

static GPIO_InitTypeDef led_config = {
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Pin = GPIO_PIN_5
};

static StaticTask_t blinkyTaskTCB;
static StackType_t blinkyTaskStack[configMINIMAL_STACK_SIZE];

static StaticTask_t uartTaskTCB;
static StackType_t uartTaskStack[configMINIMAL_STACK_SIZE];

void blinkyTask(void *pvParameters);
void uartTask(void *pvParameters);

void init(){
    SystemClock_Config();

    if(HAL_Init() == HAL_ERROR){
        while(1){} //error
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(LED_PORT, &led_config);
    
    // Turn on LED with HAL
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);

    // USART initialization
    HAL_GPIO_Init(USART1_TX_PORT, &USART2_GPIO_TxCfg);
    HAL_GPIO_Init(USART1_RX_PORT, &USART2_GPIO_RxCfg);
    __HAL_USART_ENABLE(&USART2_Handle);
    __USART1_CLK_ENABLE();
    if(HAL_USART_Init(&USART2_Handle) == HAL_ERROR){
        while(1){} //error
    }

    // Put USART in asynchronous mode
    USART2_Handle.Instance->CR2 &= ~(USART_CR2_CLKEN);
    HAL_NVIC_DisableIRQ(USART1_IRQn);

    // Create tasks
    xTaskCreateStatic(blinkyTask, 
        "Blinky", 
        configMINIMAL_STACK_SIZE, 
        NULL, 
        configMAX_PRIORITIES - 1, 
        blinkyTaskStack, 
        &blinkyTaskTCB
    );

    xTaskCreateStatic(uartTask, 
        "UART", 
        configMINIMAL_STACK_SIZE, 
        NULL, 
        configMAX_PRIORITIES - 2, 
        uartTaskStack, 
        &uartTaskTCB
    );

    shared_mem = (uint8_t*)(&_estack) + 4; // Start of shared memory (+4 to avoid stack collision)

    vTaskStartScheduler();

    while(1);
}

void blinkyTask(void *pvParameters){
    while(1){
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        vTaskDelay(1000);
    }
}

void uartTask(void *pvParameters){
    while(1){
        const char *err_code_label = "Error Code:";
        const char *endln = "\n\r";
        char err_code = shared_mem[0] + '0';

        HAL_USART_Transmit(&USART2_Handle, (unsigned char *)err_code_label, strlen(err_code_label), portMAX_DELAY);
        HAL_USART_Transmit(&USART2_Handle, (unsigned char *)&err_code, 1, portMAX_DELAY);
        HAL_USART_Transmit(&USART2_Handle, (unsigned char *)endln, 2, portMAX_DELAY);
        vTaskDelay(1000);
    }
}

int main(){
    init();
    while(1){}
    return 0;
}