#include "stm32xx_hal.h"
#include "boot_config.h"
#include "boot.h"

static GPIO_InitTypeDef LED_InitCfg= {
    .Pin = LED_PIN,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_LOW
};

static GPIO_InitTypeDef UART_GPIO_TxCfg = {
    .Pin = BOOT_UART_TX_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FAST,
    .Alternate = BOOT_UART_AF
};

static GPIO_InitTypeDef UART_GPIO_RxCfg = {
    .Pin = BOOT_UART_RX_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FAST,
    .Alternate = BOOT_UART_AF
};


// UART configuration
void boot_led_init(){
    GPIO_CLK_ENABLE(LED_PORT);
    HAL_GPIO_Init(LED_PORT, &LED_InitCfg);
    
    // Turn on LED with HAL
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
}

void boot_uart_init(){
    GPIO_CLK_ENABLE(BOOT_UART_TX_PORT);
    GPIO_CLK_ENABLE(BOOT_UART_RX_PORT);

    HAL_GPIO_Init(BOOT_UART_TX_PORT, &UART_GPIO_TxCfg);
    HAL_GPIO_Init(BOOT_UART_RX_PORT, &UART_GPIO_RxCfg);

    BOOT_UART_CLOCK_ENABLE();
    __HAL_UART_ENABLE(&UART_Handle);

    if(HAL_UART_Init(&UART_Handle) == HAL_ERROR){
        startapp_with_err(BLDR_FAIL_INIT);
    }
    
    // Boot runs on busy-waits
    HAL_NVIC_DisableIRQ(BOOT_UART_IRQN);
}

void boot_led_deinit(){
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);

    HAL_GPIO_DeInit(LED_PORT, LED_PIN);
    GPIO_CLK_DISABLE(LED_PORT);
}

void boot_uart_deinit(){
    HAL_UART_DeInit(&UART_Handle);

    __HAL_UART_DISABLE(&UART_Handle);
    BOOT_UART_CLOCK_DISABLE();

    HAL_GPIO_DeInit(BOOT_UART_TX_PORT, BOOT_UART_TX_PIN);
    HAL_GPIO_DeInit(BOOT_UART_RX_PORT, BOOT_UART_RX_PIN);

    GPIO_CLK_DISABLE(BOOT_UART_TX_PORT);
    GPIO_CLK_DISABLE(BOOT_UART_RX_PORT);
}

