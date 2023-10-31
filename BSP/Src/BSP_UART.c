#include "BSP_UART.h"

static USART_TypeDef *handles[NUM_UART] = {USART1, USART2, USART3};

static UART_HandleTypeDef for_interrupts;

//TODO This function needs to be reviewed, and queue creation will likely happen here
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
 * @brief   Reads a message from the specified UART device
 * @param   usart : which usart to read from (2 or 3)
 * @param   len number of bytes that will be returned
 * @return  len bytes from the recieve queue
 */
char* BSP_UART_Read(UART_t device, uint32_t len);

/**
 * @brief   Continues message recieving until recieve queue is full
 * @param   huart : handle of UART that called the interrupt
 */
void HAL_USART_RxCpltCallback(UART_HandleTypeDef *huart);

/**
 * @brief   Writes a message to the specified UART device
 * @param   usart : which usart to write to (1-3) 
 * @param   data pointer to the array containing the message to sned
 * @param   len length of the message in bytes
 * @return  Status of write operation (WRITE_SUCCESS OR WRITE_FAIL)
 */
Write_Status BSP_UART_Write(UART_t usart, char* data, uint32_t len);

/**
 * @brief   Continues message tranmission on UART device until transmit queue is empty
 * @param   huart : handle of UART that called the interrupt
 */
void HAL_USART_TxCpltCallback(UART_HandleTypeDef *huart);