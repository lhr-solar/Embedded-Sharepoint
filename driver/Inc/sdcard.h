#pragma once

#include "stm32xx_hal.h"
#include <stdint.h>
#include <stddef.h>

// thread safe:
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

// configuration
#define SD_DEFAULT_TIMEOUT_MS   1000
#define SD_DEFAULT_TIMEOUT_TICKS pdMS_TO_TICKS(SD_DEFAULT_TIMEOUT_MS)

/* SD Card Command Definitions */
#define SD_CMD_BASE     0x40  // Start bit (0100 0000)
#define DATA_TOKEN_CMD18 0xFE
#define DATA_TOKEN_CMD25 0xFC
#define SD_DUMMY_BYTE   0xFF
#define SD_BLOCK_SIZE   512   

#define SD_R1_IDLE_MASK 0x80
#define SD_DUMMY_CLOCKS_COUNT  10

#define SD_CMD0         0     // GO_IDLE_STATE
#define SD_CMD8         8     // SEND_IF_COND
#define SD_CMD12        12    // STOP_TRANSMISSION
#define SD_CMD17        17    // READ_SINGLE_BLOCK
#define SD_CMD18        18    // READ_MULTIPLE_BLOCK
#define SD_CMD24        24    // WRITE_BLOCK
#define SD_CMD25        25    // WRITE_MULTIPLE_BLOCK
#define SD_CMD41        41    // SD_SEND_OP_COND
#define SD_CMD55        55    // APP_CMD
#define SD_CMD58        58    // READ_OCR

#define SD_QUEUE_LENGTH  10
#define SD_MAX_FILENAME_LEN  16
#define SD_DEFAULT_TIMEOUT_MS  1000
#define SD_FORMAT_BUFFER_SIZE   512  

typedef enum {
    SD_JOB_WRITE_ASYNC,
    SD_JOB_MOUNT,
    SD_JOB_FORMAT
} sd_job_type_t;

typedef struct {
    sd_job_type_t type;
    char filename[SD_MAX_FILENAME_LEN];
    char *data;
    uint32_t len;
} sd_job_t;

/**
 * @brief SD Card Handle Structure
 * Holds hardware configuration, pin definitions, and synchronization objects.
 */
typedef struct {
    SPI_HandleTypeDef *hspi;       // Pointer to the HAL SPI Handle (e.g., &hspi1)
    
    // Chip Select (CS)
    GPIO_TypeDef *cs_port;         // GPIO Port for Chip Select
    uint16_t cs_pin;               // GPIO Pin for Chip Select

    // Thread Safety
    SemaphoreHandle_t mutex;       // Mutex for thread-safe access
    StaticSemaphore_t mutexBuffer; // Static memory buffer for the mutex

    // Interrupt Synchronization (Sleep until SPI finishes)
    SemaphoreHandle_t spi_comp_sem;       
    StaticSemaphore_t spi_comp_buffer;

    // Queue
    QueueHandle_t job_queue;
    uint8_t queue_storage[SD_QUEUE_LENGTH * sizeof(sd_job_t)];
    StaticQueue_t queue_buffer;

} sd_handle_t;



/* Status Codes (Readable Errors) */
typedef enum {
    SD_OK               = 0,
    SD_ERROR            = 1,   // Generic Error
    SD_BUSY             = 2,   // Card is busy
    SD_TIMEOUT          = 3,   // Operation timed out
    SD_ERR_SPI          = 4,   // SPI Transmission failed
    SD_ERR_VOLTAGE      = 5,   // CMD8 Voltage check failed
    SD_ERR_INIT_CARD    = 6,   // ACMD41 Initialization failed
    SD_ERR_READ         = 7,   // Read operation failed
    SD_ERR_WRITE        = 8,   // Write operation failed
    SD_ERR_MUTEX        = 9,   // RTOS Mutex creation failed
    SD_ERR_LOCK_TIMEOUT = 10,  // Could not acquire Mutex in time
    SD_ERR_QUEUE_FULL   = 11,
    SD_ERR_MALLOC       = 12
} sd_status_t;

/* USER API */

sd_status_t USER_SD_Card_Init(sd_handle_t *sd);

/* Queues a Write */
sd_status_t USER_SD_Card_Write_Async(sd_handle_t *sd, const char *filename, const char *data);

/* Reads immediately (Sync) */
sd_status_t USER_SD_Card_Read_Sync(sd_handle_t *sd, const char *filename, char *buffer, uint32_t len);

void USER_SD_Card_Worker_Task(void *params);


/* Low-level helpers */

/**
 * @brief Asserts the Chip Select pin (sets it LOW).
 * @param sd Pointer to the SD handle structure.
 */
void SD_Select(sd_handle_t *sd);

/**
 * @brief De-asserts the Chip Select pin (sets it HIGH).
 * @param sd Pointer to the SD handle structure.
 */
void SD_Deselect(sd_handle_t *sd);

/* High-level operations */

/**
 * @brief Initializes the SD card driver, sets up the mutex, and performs the SD startup sequence.
 * @note The user MUST initialize the SPI Peripheral (hspi) and all associated GPIOs (SCK, MISO, MOSI, CS) before calling this function.
 * The CS pin should be configured as Output Push-Pull, initially High.
 * @param sd Pointer to the SD handle structure.
 * @return SD_OK on success, or specific error code on failure.
 */
sd_status_t SD_Init(sd_handle_t *sd, TickType_t timeout);

/**
 * @brief Reads a single 512-byte block from the SD card (Thread Safe).
 * @param sd Pointer to the SD handle structure.
 * @param blockNum The block address to read.
 * @param buffer Pointer to the buffer (must be at least 512 bytes).
 * @return SD_OK on success, or specific error code on failure.
 */
sd_status_t SD_ReadSingleBlock(sd_handle_t *sd, uint32_t blockNum, uint8_t *buffer, TickType_t timeout);

/**
 * @brief Writes a single 512-byte block to the SD card (Thread Safe).
 * @param sd Pointer to the SD handle structure.
 * @param blockNum The block address to write to.
 * @param buffer Pointer to the data to write (512 bytes).
 * @return SD_OK on success, or specific error code on failure.
 */
sd_status_t SD_WriteSingleBlock(sd_handle_t *sd, uint32_t blockNum, const uint8_t *buffer, TickType_t timeout);

// Read Multiple Blocks

/**
 * @brief Starts a multi-block read operation. Locks the mutex.
 * @param sd Pointer to the SD handle structure.
 * @param blockNum The starting block address.
 * @return SD_OK on success, or specific error code on failure.
 */
sd_status_t SD_ReadBegin(sd_handle_t *sd, uint32_t blockNum, TickType_t timeout);

/**
 * @brief Reads the next block of data during a multi-block read.
 * @param sd Pointer to the SD handle structure.
 * @param buff Pointer to the buffer (512 bytes).
 * @return SD_OK on success, or specific error code on failure.
 */
sd_status_t SD_ReadData(sd_handle_t *sd, uint8_t* buff, TickType_t timeout); 

/**
 * @brief Ends a multi-block read operation. Unlocks the mutex.
 * @param sd Pointer to the SD handle structure.
 * @return SD_OK on success, or specific error code on failure.
 */
sd_status_t SD_ReadEnd(sd_handle_t *sd, TickType_t timeout);

// Write Multiple Blocks

/**
 * @brief Starts a multi-block write operation. Locks the mutex.
 * @param sd Pointer to the SD handle structure.
 * @param blockNum The starting block address.
 * @return SD_OK on success, or specific error code on failure.
 */
sd_status_t SD_WriteBegin(sd_handle_t *sd, uint32_t blockNum, TickType_t timeout);

/**
 * @brief Writes the next block of data during a multi-block write.
 * @param sd Pointer to the SD handle structure.
 * @param buff Pointer to the data to write (512 bytes).
 * @return SD_OK on success, or specific error code on failure.
 */
sd_status_t SD_WriteData(sd_handle_t *sd, const uint8_t* buff, TickType_t timeout); 

/**
 * @brief Ends a multi-block write operation. Unlocks the mutex.
 * @param sd Pointer to the SD handle structure.
 * @return SD_OK on success, or specific error code on failure.
 */
sd_status_t SD_WriteEnd(sd_handle_t *sd, TickType_t timeout);


//#endif /* SDCARD_H */

