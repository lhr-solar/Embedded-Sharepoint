#include "portmacro.h"
#include "stm32xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "boot_shared.h"

#include "FreeRTOS.h"
#include "task.h"

shared_bootmem_t* shared_mem;

static GPIO_InitTypeDef UART_GPIO_TxCfg = {
    .Pin = DBG_UART_TX_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FAST,
    .Alternate = DBG_UART_AF
};

static GPIO_InitTypeDef UART_GPIO_RxCfg = {
    .Pin = DBG_UART_RX_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FAST,
    .Alternate = DBG_UART_AF
};

static UART_HandleTypeDef UART_Handle = {
    .Instance = DBG_UART_INST,
    .Init.BaudRate = 9600,
    .Init.WordLength = UART_WORDLENGTH_8B,
    .Init.StopBits = UART_STOPBITS_1,
    .Init.Parity = UART_PARITY_NONE,
    .Init.Mode = UART_MODE_TX_RX,
    .gState = HAL_UART_STATE_RESET
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
    __disable_irq();
    SystemClock_Config();

    if(HAL_Init() == HAL_ERROR){
        while(1){} //error
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(LED_PORT, &led_config);
    
    // Turn on LED with HAL
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);

    // USART initialization
    HAL_GPIO_Init(DBG_UART_TX_PORT, &UART_GPIO_TxCfg);
    HAL_GPIO_Init(DBG_UART_RX_PORT, &UART_GPIO_RxCfg);
    
    __HAL_UART_ENABLE(&UART_Handle);
    DBG_UART_CLOCK_ENABLE();

    if(HAL_UART_Init(&UART_Handle) == HAL_ERROR){
        while(1){} //error
    }

    HAL_NVIC_DisableIRQ(DBG_UART_IRQN);

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

    shared_mem = (shared_bootmem_t*)(&_estack) + 4; // Start of shared memory (+4 to avoid stack collision)

    if(shared_mem->magic_num == BOOT_MAGIC_NUM){
	const char *boot_success = "Booted from bootloader!\n\r";
	HAL_UART_Transmit(&UART_Handle, boot_success, strlen(boot_success), portMAX_DELAY);
    } else {
	const char *reg_boot = "Booted from typical boot sequence!\n\r";
	HAL_UART_Transmit(&UART_Handle, reg_boot, strlen(reg_boot), portMAX_DELAY);
    }

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
        char err_code = shared_mem->err_code + '0';

        HAL_UART_Transmit(&UART_Handle, (unsigned char *)err_code_label, strlen(err_code_label), portMAX_DELAY);
        HAL_UART_Transmit(&UART_Handle, (unsigned char *)&err_code, 1, portMAX_DELAY);
        HAL_UART_Transmit(&UART_Handle, (unsigned char *)endln, 2, portMAX_DELAY);
        vTaskDelay(1000);
    }
}

int main(){
    init();
    while(1){}
    return 0;
}
