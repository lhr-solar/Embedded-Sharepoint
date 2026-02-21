#pragma once

#include "stm32xx_hal.h"
#include <stdint.h>
#include <stddef.h>

// Thread Safe RTOS includes
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

/* --- SD Card Command Definitions --- */
#define SD_CMD_BASE       0x40  
#define DATA_TOKEN_CMD18  0xFE
#define DATA_TOKEN_CMD25  0xFC
#define SD_DUMMY_BYTE     0xFF
#define SD_BLOCK_SIZE     512   

// Magic Number Replacements
#define SD_R1_IDLE_MASK         0x80
#define SD_DUMMY_CLOCKS_COUNT   10
#define SD_DEFAULT_TIMEOUT_MS   1000

#define SD_CMD0         0     // GO_IDLE_STATE
#define SD_CMD8         8     // SEND_IF_COND
#define SD_CMD12        12    // STOP_TRANSMISSION
#define SD_CMD17        17    // READ_SINGLE_BLOCK
#define SD_CMD18        18    // READ_MULTIPLE_BLOCK
#define SD_CMD24        24    // WRITE_BLOCK
#define SD_CMD25        25    // WRITE_MULTIPLE_BLOCK
#define SD_ACMD41       41    // SD_SEND_OP_COND
#define SD_CMD55        55    // APP_CMD
#define SD_CMD58        58    // READ_OCR

/* Configuration */
#define SD_MAX_FILENAME_LEN 13
#define SD_DATA_BUFFER_LEN  64
#define SD_QUEUE_LENGTH     5

/**
 * @brief Custom error status enum
 */
typedef enum {
    SD_OK = 0,
    SD_ERROR = -1,
    SD_TIMEOUT = -2,
    SD_ERR_SPI = -3,
    SD_ERR_INIT_CARD = -4,
    SD_ERR_VOLTAGE = -5,
    SD_ERR_READ = -6,
    SD_ERR_WRITE = -7,
    SD_ERR_MUTEX = -8,
    SD_ERR_QUEUE_FULL = -9,
    SD_ERR_LOCK_TIMEOUT = -10
} sd_status_t;

/**
 * @brief Enum for Worker Task Job Types
 */
typedef enum {
    SD_JOB_MOUNT,
    SD_JOB_WRITE_ASYNC,
    SD_JOB_FORMAT
} sd_job_type_t;

/**
 * @brief Job structure passed through the RTOS Queue
 */
typedef struct {
    sd_job_type_t type;
    char filename[SD_MAX_FILENAME_LEN];
    char data[SD_DATA_BUFFER_LEN];
    uint16_t len;
} sd_job_t;

/**
 * @brief Main Handle Structure for SD Card
 */
typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;

    SemaphoreHandle_t mutex;       
    StaticSemaphore_t mutexBuffer; 

    SemaphoreHandle_t spi_comp_sem;       
    StaticSemaphore_t spi_comp_buffer;    

    QueueHandle_t job_queue;
    StaticQueue_t queue_buffer;
    uint8_t queue_storage[SD_QUEUE_LENGTH * sizeof(sd_job_t)];
} sd_handle_t;

/* Core Driver */
sd_status_t SD_Init(sd_handle_t *sd, TickType_t timeout);
sd_status_t SD_ReadSingleBlock(sd_handle_t *sd, uint32_t blockNum, uint8_t *buffer, TickType_t timeout);
sd_status_t SD_WriteSingleBlock(sd_handle_t *sd, uint32_t blockNum, const uint8_t *buffer, TickType_t timeout);

/* User Function */
sd_status_t USER_SD_Card_Init(sd_handle_t *sd);
sd_status_t USER_SD_Card_Write_Async(sd_handle_t *sd, const char *filename, const char *data, TickType_t delay_ticks);
void USER_SD_Card_Worker_Task(void *params);
