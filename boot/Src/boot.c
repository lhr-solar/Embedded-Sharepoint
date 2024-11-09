#include "boot.h"

#include "flash.h"

#include <stdint.h>
#include "stm32xx_hal.h"
#include "cmsis_gcc.h"

static uint8_t transaction_buf[MAX_BUFFER_SIZE] = {0};

static GPIO_InitTypeDef GPIO_InitCfg = {
    .Pin = LED_PIN,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_LOW
};

static UART_HandleTypeDef USART1_Cfg = {
    .Instance = USART1,
    .Init.BaudRate = 115200,
    .Init.WordLength = UART_WORDLENGTH_8B,
    .Init.StopBits = UART_STOPBITS_1,
    .Init.Parity = UART_PARITY_EVEN,
    .Init.Mode = UART_MODE_TX_RX,
    .Init.HwFlowCtl = UART_HWCONTROL_NONE,
    .Init.OverSampling = UART_OVERSAMPLING_16
};

void boot_init(){
    // Disable interrupts
    __disable_irq();

    if(HAL_Init() == HAL_ERROR){
        error_condition();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(GPIOA, &GPIO_InitCfg);
    
    // Turn on LED with HAL
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);

    // UART initialization
    if(HAL_UART_Init(&USART1_Cfg) == HAL_ERROR){
        error_condition();
    }
}

void boot_deinit(){
    // Turn off LED with HAL
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);

    HAL_GPIO_DeInit(LED_PORT, LED_PIN);
    __HAL_RCC_GPIOA_CLK_DISABLE();

    HAL_DeInit();
}

inline void error_condition(){
    // TODO: record/report the error to the user?
    boot_deinit();
    startapp();
}

void boot(){
    // Initialize
    boot_init();

    uint8_t init_cmd;
    if(HAL_UART_Receive(&USART1_Cfg, &init_cmd, 1, 0)){
        // Successfully entered bootloader
        
    }

    // Deinitialize
    boot_deinit();

    // Call the application's entry point.
    startapp();
}