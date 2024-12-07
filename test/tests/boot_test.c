#include "stm32xx_hal.h"
#include "stdint.h"
#include "stdbool.h"

#include "FreeRTOS.h"
#include "task.h"

#define SIZEOF(x) (sizeof(x)/sizeof(x[0]))
static uint8_t deadbeef[4] = {0xDE, 0xAD, 0xBE, 0xEF};

static uint8_t buf[SIZEOF(deadbeef)*4] = {0};
static uint8_t bufind = 0;

static USART_HandleTypeDef USART1_Handle = {
    .Instance = USART1,
    .Init.BaudRate = 9600,
    .Init.WordLength = USART_WORDLENGTH_9B,
    .Init.StopBits = USART_STOPBITS_1,
    .Init.Parity = USART_PARITY_EVEN,
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
        return; //error
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(LED_PORT, &led_config);
    
    // Turn on LED with HAL
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);

    // USART initialization
    __HAL_USART_ENABLE(&USART1_Handle);
    __USART1_CLK_ENABLE();
    if(HAL_USART_Init(&USART1_Handle) == HAL_ERROR){
        return; //error 
    }

    // Put USART in asynchronous mode
    USART1_Handle.Instance->CR2 &= ~(USART_CR2_CLKEN);
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
    bufind = 0;
    while(1){
        // Receive 4 bytes at a time
        if(HAL_USART_Receive(&USART1_Handle, &(buf[bufind]), 4, 100) == HAL_OK){
            bufind += 4;
        }

        if(bufind == SIZEOF(buf)){
            // Find the 0xDE
            for(uint8_t i=0; i<SIZEOF(buf); i++){
                if(buf[i] == 0xDE){
                    bufind = i;
                    break;
                }
            }

            if(bufind+(SIZEOF(deadbeef)*3) >= SIZEOF(buf)){
                // No 0xDE found
                bufind = 0;
                continue;
            }

            bool found_three = true;
            for(; bufind<SIZEOF(buf); bufind+=SIZEOF(deadbeef)){
                if(memcmp(&buf[bufind], deadbeef, SIZEOF(deadbeef)) != 0){
                    found_three = false;
                }
            }

            if(found_three){
                NVIC_SystemReset(); // jump to bootloader
            }
            bufind = 0;
        }

        vTaskDelay(100);
    }
}

int main(){
    init();
    return 0;
}