#include "stm32xx_hal.h"
#include "UART.h"
#include "uart_bootloader.h"

#if defined(STM32L432xx)
#define LED_PIN GPIO_PIN_3
#define LED_PORT GPIOB
#elif defined(STM32L431xx)
#define LED_PIN GPIO_PIN_11
#define LED_PORT GPIOB
#elif defined(STM32G473xx)
#define LED_PIN GPIO_PIN_3
#define LED_PORT GPIOC
#else
#define LED_PIN GPIO_PIN_5
#define LED_PORT GPIOA
#endif

#if defined(USART3)
#define BOOT_COMMAND_UART husart3
#elif defined(USART2)
#define BOOT_COMMAND_UART husart2
#elif defined(USART1)
#define BOOT_COMMAND_UART husart1
#else
#error "No UART available for bootloader command test."
#endif

static StaticTask_t s_blinky_task_buffer;
static StaticTask_t s_uart_task_buffer;
static StackType_t s_blinky_task_stack[configMINIMAL_STACK_SIZE];
static StackType_t s_uart_task_stack[configMINIMAL_STACK_SIZE];

static void heartbeat_clock_init(void) {
    if (LED_PORT == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (LED_PORT == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (LED_PORT == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
#ifdef GPIOD
    else if (LED_PORT == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
#endif
}

static void led_init(void) {
    GPIO_InitTypeDef led_config = {0};
    led_config.Mode = GPIO_MODE_OUTPUT_PP;
    led_config.Pull = GPIO_NOPULL;
    led_config.Pin = LED_PIN;

    heartbeat_clock_init();
    HAL_GPIO_Init(LED_PORT, &led_config);
}

static void boot_command_uart_init(void) {
    BOOT_COMMAND_UART->Init.BaudRate = 115200;
    BOOT_COMMAND_UART->Init.WordLength = UART_WORDLENGTH_8B;
    BOOT_COMMAND_UART->Init.StopBits = UART_STOPBITS_1;
    BOOT_COMMAND_UART->Init.Parity = UART_PARITY_NONE;
    BOOT_COMMAND_UART->Init.Mode = UART_MODE_TX_RX;
    BOOT_COMMAND_UART->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    BOOT_COMMAND_UART->Init.OverSampling = UART_OVERSAMPLING_16;
#if defined(STM32L4xx) || defined(STM32G4xx)
    BOOT_COMMAND_UART->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    BOOT_COMMAND_UART->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
#endif

    if (uart_init(BOOT_COMMAND_UART) != UART_OK) {
        Error_Handler();
    }
}

static void blinky_task(void *argument) {
    (void)argument;

    while (1) {
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

static void uart_task(void *argument) {
    (void)argument;

    while (1) {
        (void)uart_bootloader_service(BOOT_COMMAND_UART, portMAX_DELAY);
    }
}

int main(void) {
    uart_bootloader_init_app_vector_table();
    HAL_Init();
    SystemClock_Config();

    /* Resident bootloader jumps with IRQs globally masked; enable before UART / FreeRTOS. */
    __enable_irq();

    led_init();
    boot_command_uart_init();

    xTaskCreateStatic(blinky_task,
                      "BLINK",
                      configMINIMAL_STACK_SIZE,
                      NULL,
                      tskIDLE_PRIORITY + 1,
                      s_blinky_task_stack,
                      &s_blinky_task_buffer);

    xTaskCreateStatic(uart_task,
                      "BOOTUART",
                      configMINIMAL_STACK_SIZE,
                      NULL,
                      tskIDLE_PRIORITY + 2,
                      s_uart_task_stack,
                      &s_uart_task_buffer);

    vTaskStartScheduler();

    while (1) {}
}
