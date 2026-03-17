

# File sdcard.h

[**File List**](files.md) **>** [**driver**](dir_1284b95147afa48b330f502c8bbc0529.md) **>** [**Inc**](dir_63ffe06a04b97e03324a2ad2f10a5002.md) **>** [**sdcard.h**](sdcard_8h.md)

[Go to the documentation of this file](sdcard_8h.md)


```C++
#pragma once

#include "stm32xx_hal.h"
#include <stdint.h>
#include <stddef.h>

// Thread Safe RTOS includes
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "queue_ex.h"

/* --- SD Card Command Definitions --- */
#define SD_CMD_BASE       0x40  // All SPI commands start with 01 followed by 6-bit index
#define DATA_TOKEN_CMD18  0xFE  // Standard Start Block token for single/multiple block reads
#define DATA_TOKEN_CMD25  0xFC  // Start Block token for multiple block writes
#define SD_DUMMY_BYTE     0xFF  // Byte sent to provide clock cycles while waiting for card output
#define SD_BLOCK_SIZE     512   // Standard SD card sector size in bytes

#define SD_R1_IDLE_MASK             0x80   // Mask to check if the MSB of R1 response is 0
#define SD_DUMMY_CLOCKS_COUNT       10     // Minimum dummy bytes (80 clocks) for power-up sequence
#define SD_DEFAULT_TIMEOUT_MS       1000   // Default time to wait for card operations
#define SD_SMALL_TRANSFER_THRESHOLD 16 // Threshold for stack-allocated dummy buffers
#define SD_R1_POLL_RETRIES          8   // Number of bytes to poll for an R1 response
#define SD_BUSY_WAIT_RETRIES        500 // loop time

#define SD_CMD_PACKET_SIZE  6    // Standard SD SPI command packet size (1 index + 4 arg + 1 CRC)
#define SD_CMD_INDEX_MASK   0x3F // Mask to ensure command index is 6 bits
#define SD_BYTE_SHIFT_24    24
#define SD_BYTE_SHIFT_16    16
#define SD_BYTE_SHIFT_8     8
#define SD_BYTE_MASK        0xFF

#define SD_R1_ERROR_CODE    0xFF  // Card failed to respond with a valid R1 byte

#define SD_TIMEOUT_MS    1000 //timeout for standard SD card operations (1 second)

#define SD_DEFAULT_SECTOR_COUNT  3934208 // Default for 2GB Card

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
#define SD_MAX_FILENAME_LEN 13 // Max length for FatFs 8.3 filenames
#define SD_DATA_BUFFER_LEN  64 // Max string size for asynchronous logging
#define SD_QUEUE_LENGTH     5  // Number of jobs the worker task can hold

typedef enum {
    SD_OK = 0x00, // Operation successful
    SD_ERROR = 0x01, // General failure
    SD_TIMEOUT = 0x02, // Operation timed out
    SD_ERR_SPI = 0x03, // SPI hardware error
    SD_ERR_INIT_CARD = 0x04, // Initialization sequence failed
    SD_ERR_VOLTAGE = 0x05, // Incompatible card voltage
    SD_ERR_READ = 0x06, // Block read failure
    SD_ERR_WRITE = 0x07, // Block write failure
    SD_ERR_MUTEX = 0x08, // Failed to acquire hardware lock
    SD_ERR_QUEUE_FULL = 0x09, // Async job queue is full
    SD_ERR_LOCK_TIMEOUT = 0x0A // Mutex lock timed out
} sd_status_t;

typedef enum {
    SD_JOB_MOUNT, // Mount the file system
    SD_JOB_WRITE_ASYNC, // Write text data to a file
    SD_JOB_FORMAT // Format the SD card
} sd_job_type_t;

typedef struct {
    sd_job_type_t type; // The type of job to execute
    char filename[SD_MAX_FILENAME_LEN]; // Target filename
    char data[SD_DATA_BUFFER_LEN]; // Data to be written
    uint16_t len; // Length of the data string
} sd_job_t;

typedef struct {
    SPI_HandleTypeDef *hspi; // SPI peripheral handle
    GPIO_TypeDef *cs_port; // GPIO port for Chip Select
    uint16_t cs_pin; // GPIO pin for Chip Select

    SemaphoreHandle_t mutex; // Mutex to prevent simultaneous SPI access
    StaticSemaphore_t mutexBuffer; // Static buffer for mutex allocation

    SemaphoreHandle_t spi_comp_sem; // Semaphore to signal SPI DMA/IT completion
    StaticSemaphore_t spi_comp_buffer; // Static buffer for completion semaphore

    QueueHandle_t job_queue; // Inbox for the background worker
    StaticQueue_t queue_buffer; // Static buffer for the job queue
    uint8_t queue_storage[SD_QUEUE_LENGTH * sizeof(sd_job_t)]; // Queue memory

    TaskHandle_t worker_task_handle; // tracks worker task
} sd_handle_t;

void sdcard_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi, BaseType_t* xHigherPriorityTaskWoken);

sd_status_t SD_Init(sd_handle_t *sd, TickType_t timeout);

sd_status_t SD_ReadSingleBlock(sd_handle_t *sd, uint32_t blockNum, uint8_t *buffer, TickType_t timeout);

sd_status_t SD_WriteSingleBlock(sd_handle_t *sd, uint32_t blockNum, const uint8_t *buffer, TickType_t timeout);

sd_status_t USER_SD_Card_Init(sd_handle_t *sd, UBaseType_t priority);

sd_status_t USER_SD_Card_Write_Async(sd_handle_t *sd, const char *filename, const char *data, TickType_t delay_ticks);

void USER_SD_Card_Worker_Task(void *params);
```


