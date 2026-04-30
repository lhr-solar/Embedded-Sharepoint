/*
 * Resident-bootloader integration test: soft LED breathe, periodic UART line,
 * and classic CAN / CAN-FD (classic frame) on standard ID 0. UART task still
 * accepts the ES_BLT magic command to reboot into the bootloader.
 */
#include "stm32xx_hal.h"
#include "UART.h"
#include "uart_bootloader.h"

#if defined(FDCAN1)
#include "CAN_FD.h"
#elif defined(CAN1)
#include "CAN.h"
#endif

#include <stdio.h>
#include <string.h>

#if defined(FDCAN1) || defined(FDCAN2) || defined(FDCAN3)
#define FDCAN_NVIC_PRIO (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)
#endif

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

#if defined(STM32L431xx) && defined(USART1)
#define BOOT_COMMAND_UART husart1
#elif defined(USART3)
#define BOOT_COMMAND_UART husart3
#elif defined(USART2)
#define BOOT_COMMAND_UART husart2
#elif defined(USART1)
#define BOOT_COMMAND_UART husart1
#else
#error "No UART available for bootloader command test."
#endif

#define SPAM_PERIOD_MS (400U)

#ifndef BOOTLOADER_TEST_UART_SPAM_LINESIZE
#define BOOTLOADER_TEST_UART_SPAM_LINESIZE (72U)
#endif

static StaticTask_t s_breathe_task_buffer;
static StaticTask_t s_spam_task_buffer;
static StaticTask_t s_uart_task_buffer;
static StackType_t s_breathe_stack[configMINIMAL_STACK_SIZE + 24U];
static StackType_t s_spam_stack[configMINIMAL_STACK_SIZE + 48U];
static StackType_t s_uart_stack[configMINIMAL_STACK_SIZE];

#if defined(STM32G474xx)
/* Same clock tree as test/tests/can_fd_test.c (80 MHz class, HSI PLL). */
static void G474_SystemClockConfig(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}
#endif

#if defined(STM32G473xx)
static void G473_SystemClockConfig(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN = 10;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}
#endif

static void heartbeat_clock_init(void) {
    switch ((uint32_t)LED_PORT) {
        case (uint32_t)GPIOA:
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case (uint32_t)GPIOB:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case (uint32_t)GPIOC:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
        case (uint32_t)GPIOD:
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
        default:
            break;
    }
}

static void led_init(void) {
    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = LED_PIN,
    };

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
#endif
    BOOT_COMMAND_UART->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (uart_init(BOOT_COMMAND_UART) != UART_OK) {
        Error_Handler();
    }
}

/* Slow triangle PWM using GPIO and FreeRTOS delays (~2.5 s per full cycle). */
static void led_breathe_task(void *argument) {
    (void)argument;

    for (;;) {
        for (uint32_t t = 0; t < 100U; t++) {
            uint32_t br = (t < 50U) ? t : (99U - t);
            uint32_t on_ms = (br * 18U) / 50U + 1U;
            HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
            vTaskDelay(pdMS_TO_TICKS(on_ms));
            HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
            uint32_t off_ms = (21U > on_ms) ? (21U - on_ms) : 1U;
            vTaskDelay(pdMS_TO_TICKS(off_ms));
        }
    }
}

#if defined(FDCAN1)
static can_status_t boottest_fdcan_init(void) {
    hfdcan1->Instance = FDCAN1;
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

    FDCAN_FilterTypeDef sFilterConfig = {0};
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig.FilterID1 = 0x000;
    sFilterConfig.FilterID2 = 0x000;

    if (can_fd_init(hfdcan1, &sFilterConfig) != CAN_OK) {
        return CAN_ERR;
    }
    return can_fd_start(hfdcan1);
}
#endif

#if defined(CAN1)
/* Filter + bit timing match test/tests/can_test.c (PSOM / L431); NORMAL for bus traffic. */
static can_status_t boottest_can_init(void) {
    CAN_FilterTypeDef sFilterConfig = {0};
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

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

    if (can_init(hcan1, &sFilterConfig) != CAN_OK) {
        return CAN_ERR;
    }
    return can_start(hcan1);
}
#endif

static void spam_task(void *argument) {
    (void)argument;

    uint32_t seq = 0U;
    char line[BOOTLOADER_TEST_UART_SPAM_LINESIZE];

    for (;;) {
        int n = snprintf(line, sizeof(line), "ES_BLT_BOOTLOADER_TEST seq=%lu\r\n", (unsigned long)seq);
        if (n > 0 && n < (int)sizeof(line)) {
            (void)uart_send(BOOT_COMMAND_UART, (const uint8_t *)line, (uint16_t)n, pdMS_TO_TICKS(50));
        }
        seq++;

#if defined(FDCAN1)
        {
            uint8_t payload[8];
            memset(payload, 0, sizeof(payload));
            payload[0] = (uint8_t)(seq & 0xFFU);
            payload[1] = (uint8_t)((seq >> 8) & 0xFFU);

            FDCAN_TxHeaderTypeDef tx_header = {0};
            tx_header.Identifier = 0U;
            tx_header.IdType = FDCAN_STANDARD_ID;
            tx_header.TxFrameType = FDCAN_DATA_FRAME;
            tx_header.DataLength = FDCAN_DLC_BYTES_8;
            tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
            tx_header.BitRateSwitch = FDCAN_BRS_OFF;
            tx_header.FDFormat = FDCAN_CLASSIC_CAN;
            tx_header.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;
            tx_header.MessageMarker = 0;

            (void)can_fd_send(hfdcan1, &tx_header, payload, pdMS_TO_TICKS(50));
        }
#elif defined(CAN1)
        {
            uint8_t payload[8];
            memset(payload, 0, sizeof(payload));
            payload[0] = (uint8_t)(seq & 0xFFU);
            payload[1] = (uint8_t)((seq >> 8) & 0xFFU);

            CAN_TxHeaderTypeDef tx_header = {0};
            tx_header.StdId = 0U;
            tx_header.RTR = CAN_RTR_DATA;
            tx_header.IDE = CAN_ID_STD;
            tx_header.DLC = 8U;
            tx_header.TransmitGlobalTime = DISABLE;

            (void)can_send(hcan1, &tx_header, payload, pdMS_TO_TICKS(50));
        }
#endif

        vTaskDelay(pdMS_TO_TICKS(SPAM_PERIOD_MS));
    }
}

static void uart_task(void *argument) {
    (void)argument;

    for (;;) {
        (void)uart_bootloader_service(BOOT_COMMAND_UART, portMAX_DELAY);
    }
}

int main(void) {
    uart_bootloader_init_app_vector_table();
    HAL_Init();

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

#if defined(STM32G474xx)
    G474_SystemClockConfig();
#elif defined(STM32G473xx)
    G473_SystemClockConfig();
#else
    /* L431: strong PSOM SystemClock_Config() below; other targets: weak stm32*xx_hal_init.c */
    SystemClock_Config();
#endif

    /* Resident bootloader jumps with IRQs globally masked; turn them on before UART/CAN ISRs. */
    __enable_irq();

    led_init();
    boot_command_uart_init();

#if defined(FDCAN1)
    if (boottest_fdcan_init() != CAN_OK) {
        Error_Handler();
    }
#elif defined(CAN1)
    if (boottest_can_init() != CAN_OK) {
        Error_Handler();
    }
#endif

    xTaskCreateStatic(led_breathe_task,
                      "BREATHE",
                      sizeof(s_breathe_stack) / sizeof(StackType_t),
                      NULL,
                      tskIDLE_PRIORITY + 1,
                      s_breathe_stack,
                      &s_breathe_task_buffer);

    xTaskCreateStatic(spam_task,
                      "SPAM",
                      sizeof(s_spam_stack) / sizeof(StackType_t),
                      NULL,
                      tskIDLE_PRIORITY + 1,
                      s_spam_stack,
                      &s_spam_task_buffer);

    xTaskCreateStatic(uart_task,
                      "BOOTUART",
                      sizeof(s_uart_stack) / sizeof(StackType_t),
                      NULL,
                      tskIDLE_PRIORITY + 2,
                      s_uart_stack,
                      &s_uart_task_buffer);

    vTaskStartScheduler();

    while (1) {
    }
}

#if defined(FDCAN1) || defined(FDCAN2) || defined(FDCAN3)

static uint32_t HAL_RCC_FDCAN_CLK_ENABLED = 0;

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *fdcanHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    if (0) {
    }
#ifdef FDCAN1
    else if (fdcanHandle->Instance == FDCAN1) {
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
        PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            Error_Handler();
        }

        HAL_RCC_FDCAN_CLK_ENABLED++;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 1) {
            __HAL_RCC_FDCAN_CLK_ENABLE();
        }

        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
    }
#endif
#ifdef FDCAN2
    else if (fdcanHandle->Instance == FDCAN2) {
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
        PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            Error_Handler();
        }

        HAL_RCC_FDCAN_CLK_ENABLED++;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 1) {
            __HAL_RCC_FDCAN_CLK_ENABLE();
        }

        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);
    }
#endif
#ifdef FDCAN3
    else if (fdcanHandle->Instance == FDCAN3) {
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
        PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            Error_Handler();
        }

        HAL_RCC_FDCAN_CLK_ENABLED++;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 1) {
            __HAL_RCC_FDCAN_CLK_ENABLE();
        }

        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF11_FDCAN3;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(FDCAN3_IT0_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN3_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN3_IT1_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN3_IT1_IRQn);
    }
#endif
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef *fdcanHandle) {
#ifdef FDCAN1
    if (fdcanHandle->Instance == FDCAN1) {
        HAL_RCC_FDCAN_CLK_ENABLED--;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 0) {
            __HAL_RCC_FDCAN_CLK_DISABLE();
        }
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);
        HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN1_IT1_IRQn);
        return;
    }
#endif
#ifdef FDCAN2
    if (fdcanHandle->Instance == FDCAN2) {
        HAL_RCC_FDCAN_CLK_ENABLED--;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 0) {
            __HAL_RCC_FDCAN_CLK_DISABLE();
        }
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12 | GPIO_PIN_13);
        HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN2_IT1_IRQn);
        return;
    }
#endif
#ifdef FDCAN3
    if (fdcanHandle->Instance == FDCAN3) {
        HAL_RCC_FDCAN_CLK_ENABLED--;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 0) {
            __HAL_RCC_FDCAN_CLK_DISABLE();
        }
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8 | GPIO_PIN_15);
        HAL_NVIC_DisableIRQ(FDCAN3_IT0_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN3_IT1_IRQn);
    }
#endif
}

void can_fd_error_callback_hook(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs) {
    (void)hfdcan;
    (void)ErrorStatusITs;
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
}

#endif /* FDCAN1 || FDCAN2 || FDCAN3 */

#if defined(STM32L431xx) && defined(CAN1)

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (hcan->Instance == CAN1) {
        __HAL_RCC_CAN1_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(CAN1_TX_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
        HAL_NVIC_SetPriority(CAN1_RX0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef *hcan) {
    if (hcan->Instance == CAN1) {
        __HAL_RCC_CAN1_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8 | GPIO_PIN_9);
        HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
        HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    }
}

#endif /* STM32L431xx && CAN1 */

#if defined(STM32L431xx)
/*
 * Peripheral SOM L431: HSE -> 80 MHz (matches test/tests/can_test.c).
 * Strong definition overrides weak SystemClock_Config in stm32l4xx_hal_init.c for this image.
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
        Error_Handler();
    }

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 20;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        Error_Handler();
    }
}
#endif
