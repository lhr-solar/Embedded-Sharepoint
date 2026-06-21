#if defined(STM32G431xx)

#include "board.h"
#include "can_fd.h"
#include "uart.h"
#include "adc.h"
#include "stm32xx_hal.h"

/*
 * ponytail: FDCAN1 pin map + bit timing are lifted from the legacy defaults;
 * verify against the STM32G431CBT board schematic. The G431CBT exposes FDCAN1
 * only (PA11/PA12, AF9).
 */
#define FDCAN_NVIC_PRIO (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)

/* Nominal bit timing in time quanta: SYNC(1) + SEG1 + SEG2 = bit length. */
#define BOARD_FDCAN_NOM_PRESCALER 20
#define BOARD_FDCAN_NOM_SEG1      13
#define BOARD_FDCAN_NOM_SEG2      2
_Static_assert(1 + BOARD_FDCAN_NOM_SEG1 + BOARD_FDCAN_NOM_SEG2 == 16,
               "FDCAN nominal bit must total 16 Tq (SYNC+SEG1+SEG2)");

/* Shared FDCAN peripheral-clock users: enable once, disable on last teardown. */
static uint32_t HAL_RCC_FDCAN_CLK_ENABLED = 0;

/* ---- strong low-level init: pins + clock + NVIC (overrides driver __weak) ---- */
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* fdcanHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    if (fdcanHandle->Instance == FDCAN1) {
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
        PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
            Error_Handler();
        }

        /* FDCAN1 clock enable */
        HAL_RCC_FDCAN_CLK_ENABLED++;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 1) {
            __HAL_RCC_FDCAN_CLK_ENABLE();
        }

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**FDCAN1 GPIO Configuration
        PA11     ------> FDCAN1_RX
        PA12     ------> FDCAN1_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* FDCAN1 interrupt Init */
        HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
    }
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* fdcanHandle) {
    if (fdcanHandle->Instance == FDCAN1) {
        /* Peripheral clock disable */
        HAL_RCC_FDCAN_CLK_ENABLED--;
        if (HAL_RCC_FDCAN_CLK_ENABLED == 0) {
            __HAL_RCC_FDCAN_CLK_DISABLE();
        }

        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);

        HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
        HAL_NVIC_DisableIRQ(FDCAN1_IT1_IRQn);
    }
}

/* ---- board CAN bring-up: compile-time const config ---- */

/* Classic CAN; bit timing per the board clock tree (see static_assert above). */
static const FDCAN_InitTypeDef fdcan_init_cfg = {
    .ClockDivider = FDCAN_CLOCK_DIV1,
    .FrameFormat = FDCAN_FRAME_CLASSIC,
    .Mode = FDCAN_MODE_NORMAL,
    .AutoRetransmission = ENABLE,
    .TransmitPause = DISABLE,
    .ProtocolException = DISABLE,
    .NominalPrescaler = BOARD_FDCAN_NOM_PRESCALER,
    .NominalSyncJumpWidth = 1,
    .NominalTimeSeg1 = BOARD_FDCAN_NOM_SEG1,
    .NominalTimeSeg2 = BOARD_FDCAN_NOM_SEG2,
    .DataPrescaler = 1,
    .DataSyncJumpWidth = 1,
    .DataTimeSeg1 = 1,
    .DataTimeSeg2 = 1,
    .StdFiltersNbr = 1,
    .ExtFiltersNbr = 0,
    .TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION,
};

void board_fdcan1_init(void) {
    hfdcan1->Instance = FDCAN1;
    hfdcan1->Init = fdcan_init_cfg;
    can_fd_init(hfdcan1);  // filter derived from can1_recv_entries.h
    can_fd_start(hfdcan1);
}

/*
 * Default UART pin map for this board (__weak; a board variant or a test may
 * override with a strong HAL_UART_MspGPIOInit to remap). The driver's
 * HAL_UART_MspInit() calls this after enabling the peripheral clock and NVIC.
 *
 * ponytail: pins lifted from the legacy defaults; verify against the schematic.
 */
__weak void HAL_UART_MspGPIOInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef init = {0};

#ifdef UART4
    if (huart->Instance == UART4) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /* PA0 -> UART4_TX, PA1 -> UART4_RX */
        init.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        init.Mode = GPIO_MODE_AF_PP;
        init.Pull = GPIO_NOPULL;
        init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        init.Alternate = GPIO_AF8_UART4;
        HAL_GPIO_Init(GPIOA, &init);
    }
#endif /* UART4 */

#ifdef UART5
    if (huart->Instance == UART5) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        /* PC12 -> UART5_TX, PD2 -> UART5_RX */
        init.Pin = GPIO_PIN_12;
        init.Mode = GPIO_MODE_AF_PP;
        init.Pull = GPIO_NOPULL;
        init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        init.Alternate = GPIO_AF8_UART5;
        HAL_GPIO_Init(GPIOC, &init);

        init.Pin = GPIO_PIN_2;
        HAL_GPIO_Init(GPIOD, &init);
    }
#endif /* UART5 */

#ifdef USART1
    if (huart->Instance == USART1) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /* PA9 -> USART1_TX, PA10 -> USART1_RX */
        init.Pin = GPIO_PIN_9 | GPIO_PIN_10;
        init.Mode = GPIO_MODE_AF_PP;
        init.Pull = GPIO_NOPULL;
        init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        init.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &init);
    }
#endif /* USART1 */

#if defined(USART2) && defined(GPIOD)
    if (huart->Instance == USART2) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
        /* PD5 -> USART2_TX, PD6 -> USART2_RX */
        init.Pin = GPIO_PIN_5 | GPIO_PIN_6;
        init.Mode = GPIO_MODE_AF_PP;
        init.Pull = GPIO_NOPULL;
        init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        init.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOD, &init);
    }
#endif /* USART2 && GPIOD */

#ifdef USART3
    if (huart->Instance == USART3) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
        /* PC10 -> USART3_TX, PC11 -> USART3_RX */
        init.Pin = GPIO_PIN_10 | GPIO_PIN_11;
        init.Mode = GPIO_MODE_AF_PP;
        init.Pull = GPIO_NOPULL;
        init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        init.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOC, &init);
    }
#endif /* USART3 */

#ifdef LPUART1
    if (huart->Instance == LPUART1) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /* PA2 -> LPUART1_TX, PA3 -> LPUART1_RX */
        init.Pin = GPIO_PIN_2 | GPIO_PIN_3;
        init.Mode = GPIO_MODE_AF_PP;
        init.Pull = GPIO_NOPULL;
        init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        init.Alternate = GPIO_AF8_LPUART1;
        HAL_GPIO_Init(GPIOA, &init);
    }
#endif /* LPUART1 */
}

/*
 * Default ADC low-level init for this board (__weak; an app or test may override
 * with a strong HAL_ADC_MspInit to remap). Selects the ADC kernel-clock source
 * and enables the NVIC line; analog-pin selection stays with the app (channels
 * are app-specific and pins are Hi-Z at reset). The G431CBT has ADC1/ADC2 only;
 * the ADC345 blocks compile out via CMSIS instance macros.
 */
#define ADC_NVIC_PRIO (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)

__weak void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    if (hadc->Instance == ADC1 || hadc->Instance == ADC2) {
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
        PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) Error_Handler();
        __HAL_RCC_ADC12_CLK_ENABLE();
    }
#if defined(ADC3) || defined(ADC4) || defined(ADC5)
    else {
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC345;
        PeriphClkInit.Adc345ClockSelection = RCC_ADC345CLKSOURCE_SYSCLK;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) Error_Handler();
        __HAL_RCC_ADC345_CLK_ENABLE();
    }
#endif

#if defined(ADC1) || defined(ADC2)
    HAL_NVIC_SetPriority(ADC1_2_IRQn, ADC_NVIC_PRIO, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
#endif
#if defined(ADC3)
    HAL_NVIC_SetPriority(ADC3_IRQn, ADC_NVIC_PRIO, 0);
    HAL_NVIC_EnableIRQ(ADC3_IRQn);
#endif
#if defined(ADC4)
    HAL_NVIC_SetPriority(ADC4_IRQn, ADC_NVIC_PRIO, 0);
    HAL_NVIC_EnableIRQ(ADC4_IRQn);
#endif
#if defined(ADC5)
    HAL_NVIC_SetPriority(ADC5_IRQn, ADC_NVIC_PRIO, 0);
    HAL_NVIC_EnableIRQ(ADC5_IRQn);
#endif
}

__weak void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc) {
    if (hadc->Instance == ADC1 || hadc->Instance == ADC2) {
        __HAL_RCC_ADC12_CLK_DISABLE();
    }
#if defined(ADC3) || defined(ADC4) || defined(ADC5)
    else {
        __HAL_RCC_ADC345_CLK_DISABLE();
    }
#endif

#if defined(ADC1) || defined(ADC2)
    HAL_NVIC_DisableIRQ(ADC1_2_IRQn);
#endif
#if defined(ADC3)
    HAL_NVIC_DisableIRQ(ADC3_IRQn);
#endif
#if defined(ADC4)
    HAL_NVIC_DisableIRQ(ADC4_IRQn);
#endif
#if defined(ADC5)
    HAL_NVIC_DisableIRQ(ADC5_IRQn);
#endif
}

void board_init(void) {
    board_fdcan1_init();
}

#endif /* STM32G431xx */
