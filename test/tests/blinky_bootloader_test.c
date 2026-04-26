#include "stm32xx_hal.h"
#include "UART.h"
#include "bootloader_command.h"

#if defined(FDCAN1)
#include "CAN_FD.h"
#elif defined(CAN1)
#include "CAN.h"
#endif

#define BOOT_COMMAND_CAN_NVIC_PRIO (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)

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
static StaticTask_t s_can_task_buffer;
static StackType_t s_blinky_task_stack[configMINIMAL_STACK_SIZE];
static StackType_t s_uart_task_stack[configMINIMAL_STACK_SIZE];
static StackType_t s_can_task_stack[configMINIMAL_STACK_SIZE];

static void heartbeat_clock_init(void) {
    if (LED_PORT == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (LED_PORT == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (LED_PORT == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (LED_PORT == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
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

static void boot_command_can_init(void) {
#if defined(FDCAN1)
    hfdcan1->Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan1->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
    hfdcan1->Init.Mode = FDCAN_MODE_NORMAL;
    hfdcan1->Init.AutoRetransmission = ENABLE;
    hfdcan1->Init.TransmitPause = DISABLE;
    hfdcan1->Init.ProtocolException = DISABLE;
    hfdcan1->Init.NominalPrescaler = 20;
    hfdcan1->Init.NominalSyncJumpWidth = 1;
    hfdcan1->Init.NominalTimeSeg1 = 13;
    hfdcan1->Init.NominalTimeSeg2 = 2;
    hfdcan1->Init.DataPrescaler = 1;
    hfdcan1->Init.DataSyncJumpWidth = 1;
    hfdcan1->Init.DataTimeSeg1 = 1;
    hfdcan1->Init.DataTimeSeg2 = 1;
    hfdcan1->Init.StdFiltersNbr = 1;
    hfdcan1->Init.ExtFiltersNbr = 0;
    hfdcan1->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

    FDCAN_FilterTypeDef filter = {0};
    filter.IdType = FDCAN_STANDARD_ID;
    filter.FilterIndex = 0;
    filter.FilterType = FDCAN_FILTER_MASK;
    filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    filter.FilterID1 = BOOTLOADER_CAN_COMMAND_ID;
    filter.FilterID2 = 0x7FFU;

    if ((can_fd_init(hfdcan1, &filter) != CAN_OK) || (can_fd_start(hfdcan1) != CAN_OK)) {
        Error_Handler();
    }
#elif defined(CAN1)
    CAN_FilterTypeDef filter = {0};
    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh = (uint16_t)(BOOTLOADER_CAN_COMMAND_ID << 5U);
    filter.FilterIdLow = 0U;
    filter.FilterMaskIdHigh = (uint16_t)(0x7FFU << 5U);
    filter.FilterMaskIdLow = 0U;
    filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    filter.FilterActivation = ENABLE;
    filter.SlaveStartFilterBank = 14;

    hcan1->Init.Prescaler = 20;
    hcan1->Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1->Init.TimeSeg1 = CAN_BS1_13TQ;
    hcan1->Init.TimeSeg2 = CAN_BS2_2TQ;
    hcan1->Init.Mode = CAN_MODE_NORMAL;
    hcan1->Init.TimeTriggeredMode = DISABLE;
    hcan1->Init.AutoBusOff = ENABLE;
    hcan1->Init.AutoWakeUp = DISABLE;
    hcan1->Init.AutoRetransmission = ENABLE;
    hcan1->Init.ReceiveFifoLocked = DISABLE;
    hcan1->Init.TransmitFifoPriority = ENABLE;

    if ((can_init(hcan1, &filter) != CAN_OK) || (can_start(hcan1) != CAN_OK)) {
        Error_Handler();
    }
#endif
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
        (void)uart_boot_command_service(BOOT_COMMAND_UART, portMAX_DELAY);
    }
}

static void can_task(void *argument) {
    (void)argument;

    while (1) {
        vTaskDelay(portMAX_DELAY);
    }
}

int main(void) {
    bootloader_command_init_app_vector_table();
    HAL_Init();
    SystemClock_Config();

    led_init();
    boot_command_uart_init();
    boot_command_can_init();

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

#if defined(FDCAN1) || defined(CAN1)
    xTaskCreateStatic(can_task,
                      "BOOTCAN",
                      configMINIMAL_STACK_SIZE,
                      NULL,
                      tskIDLE_PRIORITY + 2,
                      s_can_task_stack,
                      &s_can_task_buffer);
#endif

    vTaskStartScheduler();

    while (1) {}
}

#if defined(FDCAN1)
static uint32_t s_fdcan_clock_users = 0U;

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *fdcanHandle) {
    if (fdcanHandle->Instance != FDCAN1) {
        return;
    }

    RCC_PeriphCLKInitTypeDef periph_clk = {0};
    periph_clk.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    periph_clk.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&periph_clk) != HAL_OK) {
        Error_Handler();
    }

    s_fdcan_clock_users++;
    if (s_fdcan_clock_users == 1U) {
        __HAL_RCC_FDCAN_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = GPIO_AF9_FDCAN1;
    HAL_GPIO_Init(GPIOA, &gpio);

    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, BOOT_COMMAND_CAN_NVIC_PRIO, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
    HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, BOOT_COMMAND_CAN_NVIC_PRIO, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
}
#elif defined(CAN1)
void HAL_CAN_MspInit(CAN_HandleTypeDef *canHandle) {
    if (canHandle->Instance != CAN1) {
        return;
    }

    __HAL_RCC_CAN1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOB, &gpio);

    HAL_NVIC_SetPriority(CAN1_TX_IRQn, BOOT_COMMAND_CAN_NVIC_PRIO, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, BOOT_COMMAND_CAN_NVIC_PRIO, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
}
#endif
