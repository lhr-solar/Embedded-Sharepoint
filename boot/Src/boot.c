#include "boot.h"
#include "boot_config.h"

#include "flash.h"

#include <stdint.h>
#include <string.h>
#include "stm32xx_hal.h"
#include "cmsis_gcc.h"

#define SHARED_MEM_LEN (1020)
extern uint8_t _estack;
uint8_t* shared_mem;

#define SIZEOF(x) (sizeof(x)/sizeof(x[0]))

#define STX (0x02)
#define ACK (0x06)

#define INIT_RECV_CT (3) // Number of successful init commands required to start booloader
#define INIT_RECV_TIMEOUT (pdMS_TO_TICKS(10000)) // Number of ticks before timing out of init check
#define RX_TIMEOUT (HAL_MAX_DELAY) // Number of ticks before timing out of command
#define TX_TIMEOUT (pdMS_TO_TICKS(1000))

static uint8_t init_cmd[4] = {0xDE, 0xAD, 0xBE, 0xEF};

#define USART1_TX_PIN GPIO_PIN_9
#define USART1_TX_PORT GPIOA
#define USART1_RX_PIN GPIO_PIN_10
#define USART1_RX_PORT GPIOA

#define USART2_TX_PIN GPIO_PIN_2
#define USART2_TX_PORT GPIOA
#define USART2_RX_PIN GPIO_PIN_3
#define USART2_RX_PORT GPIOA

static GPIO_InitTypeDef GPIO_InitCfg = {
    .Pin = LED_PIN,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_LOW
};

static GPIO_InitTypeDef USART2_GPIO_TxCfg = {
    .Pin = USART2_TX_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FAST,
    .Alternate = GPIO_AF7_USART2
};

static GPIO_InitTypeDef USART2_GPIO_RxCfg = {
    .Pin = USART2_RX_PIN,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FAST,
    .Alternate = GPIO_AF7_USART2
};

static USART_HandleTypeDef USART2_Handle = {
    .Instance = USART2,
    .Init.BaudRate = 9600,
    .Init.WordLength = USART_WORDLENGTH_9B,
    .Init.StopBits = USART_STOPBITS_1,
    .Init.Parity = USART_PARITY_EVEN,
    .Init.Mode = USART_MODE_TX_RX,
    .State = HAL_USART_STATE_RESET
};

static inline void startapp_with_err(error_code_t ec){
    shared_mem[0] = ec;
    boot_deinit();
    startapp();
}

static inline error_code_t uart_ack(){
    // Acknowledge
    uint8_t ack = ACK;
    return HAL_USART_Transmit(&USART2_Handle, &ack, 1, TX_TIMEOUT);
}

/*
Start of Transmission:
STX (1 byte) RX
ACK (1 bytes) TX
*/
static error_code_t uart_stx(){
    uint8_t buf = 0;

    // Get STX (start of text character)
    error_code_t ret = HAL_USART_Receive(&USART2_Handle, &buf, 1, RX_TIMEOUT);
    if(ret == BLDR_OK){
        if(buf == STX){
            ret = uart_ack();
            if(ret != BLDR_OK) return ret;
        } else {
            ret = BLDR_FAIL_STX;
        }
    } else{
        return ret;
    }

    return ret;
}

/*
Header:
CMD (1 byte) RX
DATA_SIZE (2 bytes) RX
ADDRESS (4 bytes) RX
ACK (1 byte) TX
*/
static error_code_t uart_header(uint8_t *cmd, uint16_t *data_size, uint32_t *address){
    uint8_t buf[7] = {0};
    
    // Get CMD and DATA_SIZE
    error_code_t ret = HAL_USART_Receive(&USART2_Handle, buf, 7, RX_TIMEOUT);
    if(ret != BLDR_OK) return ret;
    
    *cmd = buf[0];
    *data_size = (buf[1] << 8) | buf[2];
    *address = (buf[3] << 24) | (buf[4] << 16) | (buf[5] << 8) | buf[6];
    
    ret = uart_ack();
    if(ret != BLDR_OK) return ret;

    return ret;
}

/*
Data:
DATA (X bytes) RX
ACK (1 byte) TX
*/
static error_code_t uart_data(uint8_t *data, uint16_t data_size){
    uint8_t buf[data_size];
    
    // Get DATA
    error_code_t ret = HAL_USART_Receive(&USART2_Handle, buf, data_size, RX_TIMEOUT);
    if(ret != BLDR_OK) return ret;
    
    memcpy(data, buf, data_size);
    
    ret = uart_ack();
    if(ret != BLDR_OK) return ret;

    return ret;
}

/*
Response Header:
DATA_SIZE (2 byte) TX
ACK (1 byte) RX

Response:
DATA (X bytes) TX
ACK (1 byte) RX
*/
static error_code_t uart_resp(uint8_t *data, uint16_t data_size){
    // Header
    HAL_USART_Transmit(&USART2_Handle, (const uint8_t*) &data_size, 2, TX_TIMEOUT);

    // Wait for ack
    uint8_t ack = 0;
    error_code_t ret = HAL_USART_Receive(&USART2_Handle, &ack, 1, RX_TIMEOUT);
    if(ret != BLDR_OK) return ret;
    if(ack != ACK) return BLDR_ERR;

    // Data
    HAL_USART_Transmit(&USART2_Handle, data, data_size, TX_TIMEOUT);

    // Wait for ack
    ret = HAL_USART_Receive(&USART2_Handle, &ack, 1, RX_TIMEOUT);
    if(ret != BLDR_OK) return ret;
    if(ack != ACK) return BLDR_ERR;

    return ret;
}

static error_code_t uart_cmd(){
    // Wait for STX
    if(uart_stx() != BLDR_OK) return BLDR_FAIL_STX;

    // Get Header
    uint8_t cmd;
    uint16_t data_size;
    uint32_t address;
    if(uart_header(&cmd, &data_size, &address) != BLDR_OK) return BLDR_FAIL_HDR;

    // Get Data
    uint8_t data[data_size];
    if(uart_data(data, data_size) != BLDR_OK) return BLDR_FAIL_DATA;

    if(cmd == CMD_BLDR_START_AFTER_UPDATE){
        return BLDR_START_AFTER_UPDATE;
    }

    // Execute command
    flash_cmd_t flash_cmd = {
        .id = cmd,
        .data_size = data_size,
        .address = address
    };
    if(!exec_flash_command(data, &flash_cmd)) return BLDR_FAIL_FLASH;

    // Send response
    if(flash_cmd.id == FLASH_READ_SINGLE || flash_cmd.id == FLASH_READ_BUF){
        if(uart_resp(data, data_size) != BLDR_OK) return BLDR_ERR;
    }

    return BLDR_OK;
}

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
    
    // Get all 4 packets
    for(bufptr = buf; bufptr<&buf[SIZEOF(buf)-1]; bufptr+=SIZEOF(init_cmd)){
        ret = HAL_USART_Receive(&USART2_Handle, bufptr, SIZEOF(init_cmd), INIT_RECV_TIMEOUT);
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
    HAL_GPIO_Init(USART2_TX_PORT, &USART2_GPIO_TxCfg);
    HAL_GPIO_Init(USART2_RX_PORT, &USART2_GPIO_RxCfg);
    __HAL_USART_ENABLE(&USART2_Handle);
    __USART2_CLK_ENABLE();
    // __USART1_CLK_ENABLE();
    if(HAL_USART_Init(&USART2_Handle) == HAL_ERROR){
        startapp_with_err(BLDR_FAIL_INIT);
    }

    // Put USART in asynchronous mode
    USART2_Handle.Instance->CR2 &= ~(USART_CR2_CLKEN);
    HAL_NVIC_DisableIRQ(USART1_IRQn);

    // Shared memory
    shared_mem = (uint8_t*)(&_estack) + 4; // Start of shared memory (+4 to avoid stack collision)

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

    __HAL_USART_DISABLE(&USART2_Handle);
    __USART1_CLK_DISABLE();
    HAL_USART_DeInit(&USART2_Handle);

    HAL_NVIC_DisableIRQ(USART1_IRQn);
    HAL_NVIC_DisableIRQ(SysTick_IRQn);

    HAL_DeInit();
}

/*
=======================
CMD Structure:
Start of Transmission:
STX (1 byte) RX
ACK (1 bytes) TX

Header:
CMD (1 byte) RX
DATA_SIZE (2 bytes) RX
ADDRESS (4 bytes) RX
ACK (1 byte) TX

Data:
DATA (X bytes) RX
ACK (1 byte) TX

*if required:*
Response Header:
DATA_SIZE (2 byte) TX
ACK (1 byte) RX

Response:
DATA (X bytes) TX
ACK (1 byte) RX
=======================
*/

void boot(){
    // Initialize
    if(boot_init() != BLDR_OK){
        boot_deinit();
        startapp_with_err(BLDR_FAIL_INIT);
    }

    if(uart_init() != BLDR_OK){
        boot_deinit();
        startapp_with_err(BLDR_REGULAR_START);
    }

    // Locked into the bootloader
    while(1){
        // Process command & execute action
        error_code_t err = uart_cmd();
        if(err != BLDR_OK){
            boot_deinit();
            startapp_with_err(err);
        }
    }
    
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
    HAL_Delay(5000); // 5s

    boot_deinit();
    startapp();
    
    return;
}