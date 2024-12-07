#include "boot.h"

#include "flash.h"

#include <stdint.h>
#include <string.h>
#include "stm32xx_hal.h"
#include "cmsis_gcc.h"

#define SIZEOF(x) (sizeof(x)/sizeof(x[0]))

#define STX (0x02)
#define ACK (0x06)
// static uint8_t transaction_buf[MAX_BUFFER_SIZE + 3] = {0}; // Add 3 bytes for STX, HEADER
// static uint8_t* buf_ptr = transaction_buf;

#define FLASH_CMD_OFFSET (0x5)

#define INIT_RECV_CT (3) // Number of successful init commands required to start booloader
#define INIT_RECV_TIMEOUT (HAL_MAX_DELAY) // Number of ticks before timing out of init check

static uint8_t init_cmd[4] = {0xDE, 0xAD, 0xBE, 0xEF};

#define USART1_TX_PIN GPIO_PIN_9

#define USART1_TX_PORT GPIOA

#define USART1_RX_PIN GPIO_PIN_10
#define USART1_RX_PORT GPIOA

static GPIO_InitTypeDef GPIO_InitCfg = {
    .Pin = LED_PIN,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_LOW
};

static GPIO_InitTypeDef USART1_GPIO_TXCfg = {
    .Pin = USART1_TX_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FAST,
    .Alternate = GPIO_AF7_USART1
};

static GPIO_InitTypeDef USART1_GPIO_RXCfg = {
    .Pin = USART1_RX_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FAST,
    .Alternate = GPIO_AF7_USART1
};

static USART_HandleTypeDef USART1_Handle = {
    .Instance = USART1,
    .Init.BaudRate = 9600,
    .Init.WordLength = USART_WORDLENGTH_9B,
    .Init.StopBits = USART_STOPBITS_1,
    .Init.Parity = USART_PARITY_EVEN,
    .Init.Mode = USART_MODE_TX_RX
};

static inline void startapp_with_err(error_code_t ec){
    // TODO: record/report the error to the user?
    boot_deinit();
    startapp();
}

/*
CMD Structure:
Start of Transmission:
STX (1 byte) RX
ACK (1 bytes) TX

Header:
CMD (1 byte) RX
DATA_SIZE (1 byte) RX
ACK (1 byte) TX

Data:
DATA (X bytes) RX
ACK (1 byte) TX
*/

static inline error_code_t uart_ack(){
    // Acknowledge
    uint8_t ack = ACK;
    return HAL_USART_Transmit(&USART1_Handle, &ack, 1, INIT_RECV_TIMEOUT);
}

/*
Start of Transmission:
STX (1 byte) RX
ACK (1 bytes) TX
*/
// static error_code_t uart_stx(){
//     // Get STX (start of text character)
//     error_code_t ret = HAL_USART_Receive(&USART1_Handle, buf_ptr, 1, INIT_RECV_TIMEOUT);
//     if(ret == BLDR_OK){
//         if(buf_ptr[0] == STX){
//             ret = uart_ack();
//             if(ret != BLDR_OK) return ret;
//         } else {
//             ret = BLDR_FAIL_STX;
//         }
//     } else{
//         return ret;
//     }

//     buf_ptr += 1;
//     return ret;
// }

/*
Header:
CMD (1 byte) RX
DATA_SIZE (1 byte) RX
ACK (1 byte) TX
*/
// static error_code_t uart_header(uint8_t *cmd, uint8_t *data_size){
//     // Get CMD and DATA_SIZE
//     error_code_t ret = HAL_USART_Receive(&USART1_Handle, buf_ptr, 2, INIT_RECV_TIMEOUT);
//     if(ret != BLDR_OK) return ret;
    
//     *cmd = buf_ptr[0];
//     *data_size = buf_ptr[1];
    
//     ret = uart_ack();
//     if(ret != BLDR_OK) return ret;

//     buf_ptr += 2;
//     return ret;
// }

/*
Data:
DATA (X bytes) RX
ACK (1 byte) TX
*/
// static error_code_t uart_data(uint8_t *data, uint8_t data_size){
//     // Get DATA
//     error_code_t ret = HAL_USART_Receive(&USART1_Handle, buf_ptr, data_size, INIT_RECV_TIMEOUT);
//     if(ret != BLDR_OK) return ret;
    
//     memcpy(data, buf_ptr, data_size);
    
//     ret = uart_ack();
//     if(ret != BLDR_OK) return ret;

//     buf_ptr += data_size;
//     return ret;
// }


/*
Init command (special case):
DE AD BE EF x3 (12 bytes) RX
ACK (1 byte) TX
*/
static error_code_t uart_init(){
    // We allocate for one more than the INIT_RECV_CT to handle slight misalignment
    uint8_t buf[SIZEOF(init_cmd)*(INIT_RECV_CT + 1)] = {0};
    uint8_t* bufptr;
    
    // Get init command
    error_code_t ret = HAL_OK;
    // if(ret != BLDR_OK) return ret;
    
    // Get all 4 packets
    for(bufptr = buf; bufptr<&buf[SIZEOF(buf)-1]; bufptr+=SIZEOF(init_cmd)){
        ret = HAL_USART_Receive(&USART1_Handle, bufptr, SIZEOF(init_cmd), INIT_RECV_TIMEOUT);
        if(ret != BLDR_OK) return ret;
    }
    
    // Look for 0xDE
    uint8_t start = 0;
    for(; start<(SIZEOF(buf) - SIZEOF(init_cmd)*(INIT_RECV_CT)); start++){
        if(buf[start] == 0xDE){
            bool found = true;
            
            // Check if we received the correct init command 3 times
            for(uint8_t i=0; i<INIT_RECV_CT; i++){
                if(memcmp(&buf[start+(SIZEOF(init_cmd)*i)], init_cmd, SIZEOF(init_cmd)) != 0){
                    found = false;
                    break;
                }
            }

            if(found){
                // Send ACK
                ret = uart_ack();
                if(ret != BLDR_OK) return ret;
                break;
            } else continue;
        }
    }

    return ret;
}

error_code_t boot_init(){
    // Disable interrupts
    __disable_irq();

    SystemClock_Config();

    if(HAL_Init() == HAL_ERROR){
        startapp_with_err(BLDR_FAIL_INIT);
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(LED_PORT, &GPIO_InitCfg);
    
    // Turn on LED with HAL
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);

    // USART initialization
    HAL_GPIO_Init(USART1_TX_PORT, &USART1_GPIO_TXCfg);
    HAL_GPIO_Init(USART1_RX_PORT, &USART1_GPIO_RXCfg);
    __HAL_USART_ENABLE(&USART1_Handle);
    __USART1_CLK_ENABLE();
    if(HAL_USART_Init(&USART1_Handle) == HAL_ERROR){
        startapp_with_err(BLDR_FAIL_INIT);
    }

    // Put USART in asynchronous mode
    USART1_Handle.Instance->CR2 &= ~(USART_CR2_CLKEN);
    HAL_NVIC_DisableIRQ(USART1_IRQn);

    // Systick
    HAL_NVIC_EnableIRQ(SysTick_IRQn);

    __enable_irq();

    // TODO: Maybe put this in the app's reset handler?
    return HAL_OK;
}

void boot_deinit(){
    __disable_irq();

    // Turn off LED with HAL
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);

    HAL_GPIO_DeInit(LED_PORT, LED_PIN);
    __HAL_RCC_GPIOA_CLK_DISABLE();

    __HAL_USART_DISABLE(&USART1_Handle);
    __USART1_CLK_DISABLE();
    HAL_USART_DeInit(&USART1_Handle);

    HAL_NVIC_DisableIRQ(USART1_IRQn);
    HAL_NVIC_DisableIRQ(SysTick_IRQn);

    HAL_DeInit();
}

void boot(){
    // Initialize
    if(boot_init() != BLDR_OK){
        startapp_with_err(BLDR_FAIL_INIT);
    }

    if(uart_init() != BLDR_OK){
        startapp_with_err(BLDR_FAIL_UART_INIT);
    }
    
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
    HAL_Delay(5000); // 5s

    boot_deinit();
    startapp();
    
    return;
    // Main UART recv loop
    
}