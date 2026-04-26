#include "bootloader_runtime.h"

#include "bootloader_command.h"
#include "bootloader_config.h"
#include "bootloader_hal.h"

static UART_HandleTypeDef s_huart = {
    .Instance = BOOTLOADER_UART_INSTANCE,
};

static bootloader_transport_t s_active_transport = BOOTLOADER_TRANSPORT_UART;

#if BOOTLOADER_CAN_ENABLE
static CAN_HandleTypeDef s_hcan = {
    .Instance = BOOTLOADER_CAN_INSTANCE,
};
#endif

static void bootloader_copy_bytes(uint8_t *dst, const uint8_t *src, size_t len) {
    for (size_t i = 0; i < len; i++) {
        dst[i] = src[i];
    }
}

static void bootloader_uart_gpio_init(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef init = {0};

    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

#if defined(USART1)
    if (huart->Instance == USART1) {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        init.Pin = GPIO_PIN_9 | GPIO_PIN_10;
        init.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &init);
        return;
    }
#endif

#if defined(USART2)
    if (huart->Instance == USART2) {
        __HAL_RCC_USART2_CLK_ENABLE();
#if defined(GPIOD)
        __HAL_RCC_GPIOD_CLK_ENABLE();
        init.Pin = GPIO_PIN_5 | GPIO_PIN_6;
        init.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOD, &init);
#else
        __HAL_RCC_GPIOA_CLK_ENABLE();
        init.Pin = GPIO_PIN_2 | GPIO_PIN_3;
        init.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOA, &init);
#endif
        return;
    }
#endif

#if defined(USART3)
    if (huart->Instance == USART3) {
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        init.Pin = GPIO_PIN_10 | GPIO_PIN_11;
        init.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOC, &init);
        return;
    }
#endif
}

static void bootloader_uart_init(UART_HandleTypeDef *huart) {
    bootloader_uart_gpio_init(huart);

    huart->Init.BaudRate = BOOTLOADER_UART_BAUD;
    /* AN3155 UART uses 8E1; HAL needs 9-bit word length when parity is enabled. */
    huart->Init.WordLength = UART_WORDLENGTH_9B;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Parity = UART_PARITY_EVEN;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;
    huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    (void)HAL_UART_Init(huart);
}

#if BOOTLOADER_CAN_ENABLE
static void bootloader_gpio_clock_enable(GPIO_TypeDef *port) {
#if defined(GPIOA)
    if (port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
#endif
#if defined(GPIOB)
    if (port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
#endif
#if defined(GPIOC)
    if (port == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
#endif
#if defined(GPIOD)
    if (port == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
#endif
}

static void bootloader_can_gpio_init(void) {
    GPIO_InitTypeDef init = {0};
    bootloader_gpio_clock_enable(BOOTLOADER_CAN_GPIO_PORT);

    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = BOOTLOADER_CAN_GPIO_AF;
    init.Pin = BOOTLOADER_CAN_RX_PIN | BOOTLOADER_CAN_TX_PIN;
    HAL_GPIO_Init(BOOTLOADER_CAN_GPIO_PORT, &init);
}

static void bootloader_can_init(CAN_HandleTypeDef *hcan) {
    bootloader_can_gpio_init();

#if defined(CAN1)
    if (hcan->Instance == CAN1) {
        __HAL_RCC_CAN1_CLK_ENABLE();
    }
#endif
#if defined(CAN2)
    if (hcan->Instance == CAN2) {
        __HAL_RCC_CAN2_CLK_ENABLE();
    }
#endif
#if defined(CAN3)
    if (hcan->Instance == CAN3) {
        __HAL_RCC_CAN3_CLK_ENABLE();
    }
#endif

    hcan->Init.Prescaler = BOOTLOADER_CAN_STARTUP_PRESCALER;
    hcan->Init.SyncJumpWidth = BOOTLOADER_CAN_SJW;
    hcan->Init.TimeSeg1 = BOOTLOADER_CAN_TIME_SEG1;
    hcan->Init.TimeSeg2 = BOOTLOADER_CAN_TIME_SEG2;
    hcan->Init.Mode = CAN_MODE_NORMAL;
    hcan->Init.TimeTriggeredMode = DISABLE;
    hcan->Init.AutoBusOff = ENABLE;
    hcan->Init.AutoWakeUp = DISABLE;
    hcan->Init.AutoRetransmission = ENABLE;
    hcan->Init.ReceiveFifoLocked = DISABLE;
    hcan->Init.TransmitFifoPriority = ENABLE;

    (void)HAL_CAN_Init(hcan);

    CAN_FilterTypeDef filter = {0};
    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh = 0U;
    filter.FilterIdLow = 0U;
    filter.FilterMaskIdHigh = 0U;
    filter.FilterMaskIdLow = 0U;
    filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    filter.FilterActivation = ENABLE;
    filter.SlaveStartFilterBank = 14;
    (void)HAL_CAN_ConfigFilter(hcan, &filter);
    (void)HAL_CAN_Start(hcan);
}
#endif

void bootloader_runtime_init(void) {
    bootloader_uart_init(&s_huart);
#if BOOTLOADER_CAN_ENABLE
    bootloader_can_init(&s_hcan);
#endif
}

bootloader_transport_t bootloader_runtime_active_transport(void) {
    return s_active_transport;
}

bool bootloader_runtime_wait_for_handshake(uint32_t timeout_ms) {
    uint8_t byte = 0U;
    if (HAL_UART_Receive(&s_huart, &byte, 1U, timeout_ms) != HAL_OK) {
        return false;
    }
    return byte == 0x7FU;
}

bool bootloader_runtime_poll_sync(uint32_t timeout_ms) {
    uint32_t start = HAL_GetTick();

    while ((timeout_ms == 0U) || ((HAL_GetTick() - start) < timeout_ms)) {
        uint8_t byte = 0U;
        if ((HAL_UART_Receive(&s_huart, &byte, 1U, 0U) == HAL_OK) && (byte == 0x7FU)) {
            s_active_transport = BOOTLOADER_TRANSPORT_UART;
            return true;
        }

#if BOOTLOADER_CAN_ENABLE
        bootloader_can_frame_t frame = {0};
        if (bootloader_runtime_can_recv(&frame, 0U)) {
            if (bootloader_command_is_can_reset_message(frame.id, frame.data, frame.dlc)) {
                bootloader_command_clear_request();
                __DSB();
                NVIC_SystemReset();
            }
            if (frame.id == BOOTLOADER_CAN_SYNC_ID) {
                s_active_transport = BOOTLOADER_TRANSPORT_CAN;
                return true;
            }
        }
#endif
    }

    return false;
}

bool bootloader_runtime_send_bytes(const uint8_t *data, uint16_t len) {
    return HAL_UART_Transmit(&s_huart, (uint8_t *)data, len, 1000U) == HAL_OK;
}

bool bootloader_runtime_read_bytes(uint8_t *data, uint16_t len, uint32_t timeout_ms) {
    return HAL_UART_Receive(&s_huart, data, len, timeout_ms) == HAL_OK;
}

bool bootloader_runtime_can_send(uint32_t id, const uint8_t *data, uint8_t len) {
#if BOOTLOADER_CAN_ENABLE
    if ((data == NULL && len > 0U) || len > 8U || id > BOOTLOADER_CAN_MAX_STD_ID) {
        return false;
    }

    CAN_TxHeaderTypeDef header = {0};
    header.StdId = id;
    header.IDE = CAN_ID_STD;
    header.RTR = CAN_RTR_DATA;
    header.DLC = len;
    header.TransmitGlobalTime = DISABLE;

    uint32_t start = HAL_GetTick();
    while (HAL_CAN_GetTxMailboxesFreeLevel(&s_hcan) == 0U) {
        if ((HAL_GetTick() - start) >= 1000U) {
            return false;
        }
    }

    uint8_t payload[8] = {0};
    for (uint8_t i = 0U; i < len; i++) {
        payload[i] = data[i];
    }

    uint32_t mailbox = 0U;
    return HAL_CAN_AddTxMessage(&s_hcan, &header, payload, &mailbox) == HAL_OK;
#else
    (void)id;
    (void)data;
    (void)len;
    return false;
#endif
}

bool bootloader_runtime_can_recv(bootloader_can_frame_t *frame, uint32_t timeout_ms) {
#if BOOTLOADER_CAN_ENABLE
    if (frame == NULL) {
        return false;
    }

    uint32_t start = HAL_GetTick();
    do {
        if (HAL_CAN_GetRxFifoFillLevel(&s_hcan, CAN_RX_FIFO0) == 0U) {
            if (timeout_ms == 0U) {
                return false;
            }
            continue;
        }

        CAN_RxHeaderTypeDef header = {0};
        uint8_t data[8] = {0};
        if (HAL_CAN_GetRxMessage(&s_hcan, CAN_RX_FIFO0, &header, data) != HAL_OK) {
            return false;
        }

        if ((header.IDE != CAN_ID_STD) || (header.RTR != CAN_RTR_DATA) || (header.DLC > 8U)) {
            continue;
        }

        frame->id = header.StdId;
        frame->dlc = (uint8_t)header.DLC;
        for (uint8_t i = 0U; i < frame->dlc; i++) {
            frame->data[i] = data[i];
        }
        return true;
    } while ((HAL_GetTick() - start) < timeout_ms);

    return false;
#else
    (void)frame;
    (void)timeout_ms;
    return false;
#endif
}

static uint32_t bootloader_app_end_addr(void) {
    return BOOTLOADER_APP_BASE + BOOTLOADER_APP_MAX_SIZE;
}

__attribute__((unused)) static bool bootloader_flash_is_dual_bank(void) {
#if defined(FLASH_OPTR_DBANK)
    return READ_BIT(FLASH->OPTR, FLASH_OPTR_DBANK) != 0U;
#else
    return false;
#endif
}

static uint32_t bootloader_addr_to_bank(uint32_t addr) {
#if defined(FLASH_OPTR_DBANK)
    if (bootloader_flash_is_dual_bank() && ((addr - FLASH_BASE) >= FLASH_BANK_SIZE)) {
        return FLASH_BANK_2;
    }
#else
    (void)addr;
#endif
    return FLASH_BANK_1;
}

static uint32_t bootloader_addr_to_page(uint32_t addr) {
#if defined(FLASH_OPTR_DBANK)
    if (bootloader_flash_is_dual_bank()) {
        return ((addr - FLASH_BASE) % FLASH_BANK_SIZE) / FLASH_PAGE_SIZE;
    }
#endif
    return (addr - FLASH_BASE) / FLASH_PAGE_SIZE;
}

bool bootloader_runtime_erase_app(void) {
    uint32_t page_error = 0U;
    uint32_t erase_addr = BOOTLOADER_APP_BASE;
    uint32_t erase_end = bootloader_app_end_addr();

    HAL_FLASH_Unlock();

    bool ok = true;
    while (erase_addr < erase_end) {
        uint32_t bank = bootloader_addr_to_bank(erase_addr);
        uint32_t segment_end = erase_end;

#if defined(FLASH_OPTR_DBANK)
        if (bootloader_flash_is_dual_bank() && (bank == FLASH_BANK_1)) {
            uint32_t bank1_end = FLASH_BASE + FLASH_BANK_SIZE;
            if (segment_end > bank1_end) {
                segment_end = bank1_end;
            }
        }
#endif

        FLASH_EraseInitTypeDef erase = {0};
        uint32_t first_page = bootloader_addr_to_page(erase_addr);
        uint32_t last_page = bootloader_addr_to_page(segment_end - 1U);

        erase.TypeErase = FLASH_TYPEERASE_PAGES;
        erase.Page = first_page;
        erase.NbPages = (last_page - first_page) + 1U;
        erase.Banks = bank;

        if (HAL_FLASHEx_Erase(&erase, &page_error) != HAL_OK) {
            ok = false;
            break;
        }

        erase_addr = segment_end;
    }

    HAL_FLASH_Lock();
    return ok;
}

bool bootloader_runtime_write_app(uint32_t app_offset, const uint8_t *data, size_t len) {
    if ((app_offset + len) > BOOTLOADER_APP_MAX_SIZE) {
        return false;
    }

    uint32_t write_addr = BOOTLOADER_APP_BASE + app_offset;
    HAL_FLASH_Unlock();

    for (size_t i = 0; i < len; i += 8U) {
        uint64_t dword = 0xFFFFFFFFFFFFFFFFULL;
        size_t chunk = ((len - i) >= 8U) ? 8U : (len - i);
        bootloader_copy_bytes((uint8_t *)&dword, &data[i], chunk);
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, write_addr + i, dword) != HAL_OK) {
            HAL_FLASH_Lock();
            return false;
        }
    }

    HAL_FLASH_Lock();
    return true;
}

bool bootloader_runtime_is_app_valid(void) {
    uint32_t stack = *(volatile uint32_t *)BOOTLOADER_APP_BASE;
    uint32_t reset = *(volatile uint32_t *)(BOOTLOADER_APP_BASE + 4U);

    bool stack_valid = (stack & 0x2FF00000UL) == 0x20000000UL;
    bool reset_valid = (reset >= BOOTLOADER_APP_BASE) && (reset < bootloader_app_end_addr());
    return stack_valid && reset_valid;
}

static void bootloader_runtime_clear_interrupt_state(void) {
    SysTick->CTRL = 0U;
    SysTick->LOAD = 0U;
    SysTick->VAL = 0U;
    SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk | SCB_ICSR_PENDSVCLR_Msk;

    for (uint32_t i = 0U; i < (sizeof(NVIC->ICER) / sizeof(NVIC->ICER[0])); i++) {
        NVIC->ICER[i] = 0xFFFFFFFFUL;
        NVIC->ICPR[i] = 0xFFFFFFFFUL;
    }
}

void bootloader_runtime_jump_to_app(void) {
    if (!bootloader_runtime_is_app_valid()) {
        return;
    }

    uint32_t app_stack = *(volatile uint32_t *)BOOTLOADER_APP_BASE;
    uint32_t app_reset = *(volatile uint32_t *)(BOOTLOADER_APP_BASE + 4U);
    void (*app_reset_handler)(void) = (void (*)(void))app_reset;

    __disable_irq();
    (void)HAL_UART_DeInit(&s_huart);
#if BOOTLOADER_CAN_ENABLE
    (void)HAL_CAN_Stop(&s_hcan);
    (void)HAL_CAN_DeInit(&s_hcan);
#endif
    HAL_DeInit();
    HAL_RCC_DeInit();
    bootloader_runtime_clear_interrupt_state();

    SCB->VTOR = BOOTLOADER_APP_BASE;
    __set_CONTROL(0U);
    __set_PSP(0U);
    __set_MSP(app_stack);
    __DSB();
    __ISB();
    __enable_irq();
    app_reset_handler();
}
