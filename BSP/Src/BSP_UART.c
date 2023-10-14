#include "BSP_UART.h"

static uint8_t tx_buffer[TX_SIZE];
static uint8_t rx_buffer[RX_SIZE + 1];

static USART_TypeDef *handles[NUM_UART] = {USART2, USART3};

// Some uart helpers
static void clear_buffer(uint8_t* buf, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        buf[i] = 0;
    }
}
static void clear_terminators() {
    for (uint16_t i = 0; i < RX_SIZE; i++) {
        if (rx_buffer[i] == '\r' || rx_buffer[i] == '\n') {
            rx_buffer[i] = 0;
            break;
        } else if (rx_buffer[i] == '\0') break;
    }
}

static UART_HandleTypeDef for_interrupts;

BSP_Status BSP_UART_Init() {
    GPIO_InitTypeDef gpio_struct;
    UART_InitTypeDef uart_struct;
    for_interrupts.Instance = USART3;

    BSP_Status returnInfo;

    // Init clocks to port b, c, and usart3

    // check if they're disabled
    // if so, enable
    if (__HAL_RCC_GPIOB_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    if (__HAL_RCC_GPIOC_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    if (__HAL_RCC_USART3_IS_CLK_DISABLED()) {
        __HAL_RCC_USART3_CLK_ENABLE();
    }

    // config pins
    gpio_struct.Pin = GPIO_PIN_10;
    gpio_struct.Alternate = GPIO_AF7_USART3;
    gpio_struct.Mode = MODE_AF;
    gpio_struct.Pull = GPIO_PULLUP;
    gpio_struct.Speed = GPIO_SPEED_FREQ_LOW; // "GPIO Output Maximum Frequency"
    HAL_GPIO_Init(GPIOB, &gpio_struct);
    gpio_struct.Pin = GPIO_PIN_5;
    HAL_GPIO_Init(GPIOC, &gpio_struct);

    // uart config
    uart_struct.BaudRate = 115200;
    uart_struct.HwFlowCtl = UART_HWCONTROL_NONE;
    uart_struct.Mode = UART_MODE_TX_RX;
    uart_struct.Parity = UART_PARITY_NONE;
    uart_struct.WordLength = UART_WORDLENGTH_8B;
    uart_struct.StopBits = UART_STOPBITS_1;
    returnInfo = CONVERT_RETURN(HAL_UART_Init(&uart_struct));

    __HAL_USART_ENABLE_IT(&for_interrupts, USART_IT_RXNE); //rx
    __HAL_USART_ENABLE_IT(&for_interrupts, USART_IT_TC); //tx
    __HAL_USART_ENABLE(&for_interrupts); // turn interrupts on

    // 2, 0 - usart3_preempt prio, usart3_sub prio
    // custom def'd in BPS repo. Should we do the same here?
    HAL_NVIC_SetPriority(USART3_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);

    return returnInfo;
}

/**
 * @brief   Reads a line of input from UART into the rx buffer
 * 
 * @param   usart : which usart to read from (2 or 3)
 * 
 * @return  Pointer to the data read; ALWAYS null terminated
 */
char* BSP_UART_ReadLine(UART_t usart) {
    USART_TypeDef *usart_handle = handles[usart];

    clear_buffer(rx_buffer, RX_SIZE);
    // HAL doesnt have USART_ITConfig(usart_handle, USART_IT_RXNE, RESET); that I can find
    // so we'll toggle interrupts for now.
    __HAL_USART_DISABLE_IT(&for_interrupts, USART_IT_RXNE);

    BSP_Status returnInfo = CONVERT_RETURN(HAL_USART_Receive_IT(usart_handle, rx_buffer, RX_SIZE));

    __HAL_USART_ENABLE_IT(&for_interrupts, USART_IT_RXNE);
    clear_terminators();
    rx_buffer[RX_SIZE] = 0; // always end the RX array with a null terminator just in case.
    if (!IS_OK(returnInfo)) {
        return NULL;
    }
    return rx_buffer;
}

/**
 * @brief   Write a line of input into UART
 * 
 * @param   usart : which usart to write to (2 or 3) 
 * @param   input : data to write to usart
 * @param   len : length of data
 */
BSP_Status BSP_UART_WriteLine(UART_t usart, const char* input, uint32_t len) {
    USART_TypeDef *usart_handle = handles[usart];
   
    __HAL_USART_DISABLE_IT(&for_interrupts, USART_IT_TC);
    BSP_Status returnInfo = CONVERT_RETURN(HAL_USART_Transmit_IT(usart_handle, input, len));
    // re-enable interrupts
    __HAL_USART_ENABLE_IT(&for_interrupts, USART_IT_RXNE);
    return returnInfo;
}

void HAL_UART_IRQHandler(UART_HandleTypeDef *huart) {
    // TODO Convert these
    // CPU_SR_ALLOC();
    // CPU_CRITICAL_ENTER();
    // OSIntEnter();
    // CPU_CRITICAL_EXIT();
    
    USART_TypeDef *usart_handle;
    if (huart == handles[UART_2]) {
        usart_handle = handles[UART_2];
    }
    else if (huart == handles[UART_3]) {
        usart_handle = handles[UART_3];
    }

    if (__HAL_GET_FLAG(usart_handle, USART_FLAG_RXNE) != 0) {
        
    }
}