// Testing if queue overwrite works with queues of size >= 1
// Written on STM32F446RET

#include <stdio.h>
#include <string.h>
#include "stm32xx_hal.h"

StaticTask_t task_buffer;
StackType_t task_stack[configMINIMAL_STACK_SIZE];

#define QUEUE_SIZE 10

static QueueHandle_t queue_handle = NULL;
static StaticQueue_t queue_buffer;
static uint8_t queue_storage[QUEUE_SIZE * sizeof(uint32_t)];

#define USART2_TX_PIN GPIO_PIN_2
#define USART2_TX_PORT GPIOA
#define USART2_RX_PIN GPIO_PIN_3
#define USART2_RX_PORT GPIOA

static GPIO_InitTypeDef USART2_GPIO_TXCfg = {
    .Pin = USART2_TX_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FAST,
    .Alternate = GPIO_AF7_USART2
};

static GPIO_InitTypeDef USART2_GPIO_RXCfg = {
    .Pin = USART2_RX_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FAST,
    .Alternate = GPIO_AF7_USART2
};

static GPIO_InitTypeDef led_config = {
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Pin = GPIO_PIN_5
};

static USART_HandleTypeDef USART2_Handle = {
    .Instance = USART2,
    .Init.BaudRate = 9600,
    .Init.WordLength = USART_WORDLENGTH_8B,
    .Init.StopBits = USART_STOPBITS_1,
    .Init.Parity = USART_PARITY_NONE,
    .Init.Mode = USART_MODE_TX_RX,
    .State = HAL_USART_STATE_RESET
};

static void error_handler(void) {
    __disable_irq();
    while(1) {}
}

static void task(void *pvParameters){
    uint32_t data = 0xAA;
    xQueueSend(queue_handle, &data, pdMS_TO_TICKS(250));

    data = 0xCC;
    xQueueSend(queue_handle, &data, pdMS_TO_TICKS(250));
    
    data = 0xBB;
    xQueueOverwrite(queue_handle, &data);

    // Pop all elements from the queue
    while(xQueueReceive(queue_handle, &data, 0) == pdTRUE){      
        // Print
        char buffer[50];
        sprintf(buffer, "Data: 0x%lx\n\r", data);
        HAL_USART_Transmit(&USART2_Handle, (uint8_t *)buffer, strlen(buffer), 1000);
    }

    while(1){}
}

int main(){
    __disable_irq();

    if (HAL_Init() != HAL_OK) error_handler();
    SystemClock_Config();

    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable clock for GPIOA
    HAL_GPIO_Init(GPIOA, &led_config); // initialize GPIOA with led_config

    // USART initialization
    HAL_GPIO_Init(USART2_TX_PORT, &USART2_GPIO_TXCfg);
    HAL_GPIO_Init(USART2_RX_PORT, &USART2_GPIO_RXCfg);
    __HAL_USART_ENABLE(&USART2_Handle);
    __USART2_CLK_ENABLE();
    // __USART1_CLK_ENABLE();
    if(HAL_USART_Init(&USART2_Handle) == HAL_ERROR){
        error_handler();
    }

    // Put USART in asynchronous mode
    USART2_Handle.Instance->CR2 &= ~(USART_CR2_CLKEN);
    HAL_NVIC_DisableIRQ(USART1_IRQn);

    // Systick
    HAL_NVIC_EnableIRQ(SysTick_IRQn);

    __enable_irq();

    xTaskCreateStatic(
        task,
        "task",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        task_stack,
        &task_buffer
    );
    
    queue_handle = xQueueCreateStatic(QUEUE_SIZE, sizeof(uint32_t), queue_storage, &queue_buffer);

    vTaskStartScheduler();
}