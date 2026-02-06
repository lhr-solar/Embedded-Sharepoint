#ifndef SDCARD_H
#define SDCARD_H

#include "stm32xx_hal.h"
#include <stdint.h>
#include <stddef.h>

// thread safe:
#include "FreeRTOS.h"
#include "semphr.h"

/* SD Card Command Definitions */
#define SD_CMD_BASE     0x40  // Start bit (0100 0000)
#define DATA_TOKEN_CMD18 0xFE
#define DATA_TOKEN_CMD25 0xFC
#define SD_DUMMY_BYTE   0xFF
#define SD_BLOCK_SIZE   512   

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
} SD_Status;

/* Low-level helpers */

/**
 * @brief Sends dummy clocks (0xFF) to the SD card with CS High to ensure it is ready or to force SPI mode.
 * @note This is a mandatory requirement from the SD Physical Layer Spec.
 * The card requires at least 74 clock cycles with CS De-asserted (High) 
 * to stabilize power and switch from native "SD Bus Mode" to "SPI Mode".
 * @param sd Pointer to the SD handle structure.
 */
void SD_SendDummyClocks(sd_handle_t *sd);

/**
 * @brief Waits for the SD card to return 0xFF (Not Busy).
 * Includes RTOS yield (vTaskDelay) to allow other tasks to run while waiting.
 * @param sd Pointer to the SD handle structure.
 * @return SD_OK on success, or specific error code on failure.
 */
int8_t SD_WaitNotBusy(sd_handle_t *sd);

/**
 * @brief Waits for a specific data token (e.g., 0xFE) from the card.
 * @param sd Pointer to the SD handle structure.
 * @param token The expected token byte.
 * @return SD_OK on success, or specific error code on failure.
 */
int8_t SD_WaitDataToken(sd_handle_t *sd, uint8_t token);

/**
 * @brief Reads a raw stream of bytes from the SD card.
 * @param sd Pointer to the SD handle structure.
 * @param buff Pointer to the buffer to store received data.
 * @param len Number of bytes to read.
 * @return SD_OK on success, or specific error code on failure.
 */
int8_t SD_ReadBytes(sd_handle_t *sd, uint8_t *buff, size_t len);

/**
 * @brief Reads the R1 response byte from the SD card.
 * @param sd Pointer to the SD handle structure.
 * @return The R1 response byte.
 */
uint8_t SD_ReadR1(sd_handle_t *sd);

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

/* Command layer */

/**
 * @brief Sends a command to the SD card and reads the R1 response.
 * @param sd Pointer to the SD handle structure.
 * @param cmd The command index (e.g., SD_CMD0).
 * @param arg The 32-bit argument for the command.
 * @param crc The CRC byte for the command.
 * @return The R1 response byte.
 */
uint8_t SD_SendCommand(sd_handle_t *sd, uint8_t cmd, uint32_t arg, uint8_t crc);

/* High-level operations */

/**
 * @brief Initializes the SD card driver, sets up the mutex, and performs the SD startup sequence.
 * @note The user MUST initialize the SPI Peripheral (hspi) and all associated GPIOs (SCK, MISO, MOSI, CS) before calling this function.
 * The CS pin should be configured as Output Push-Pull, initially High.
 * @param sd Pointer to the SD handle structure.
 * @return SD_OK on success, or specific error code on failure.
 */
int8_t SD_Init(sd_handle_t *sd);

/**
 * @brief Reads a single 512-byte block from the SD card (Thread Safe).
 * @param sd Pointer to the SD handle structure.
 * @param blockNum The block address to read.
 * @param buffer Pointer to the buffer (must be at least 512 bytes).
 * @return SD_OK on success, or specific error code on failure.
 */
int8_t SD_ReadSingleBlock(sd_handle_t *sd, uint32_t blockNum, uint8_t *buffer);

/**
 * @brief Writes a single 512-byte block to the SD card (Thread Safe).
 * @param sd Pointer to the SD handle structure.
 * @param blockNum The block address to write to.
 * @param buffer Pointer to the data to write (512 bytes).
 * @return SD_OK on success, or specific error code on failure.
 */
int8_t SD_WriteSingleBlock(sd_handle_t *sd, uint32_t blockNum, const uint8_t *buffer);

// Read/Write Sector are wrappers for SingleBlock in typical FatFS implementations, passes request to SD_ReadSingleBlock and SD_WriteSIngleBlock
uint8_t SD_ReadSector(sd_handle_t *sd, uint32_t sector, uint8_t *buffer);
uint8_t SD_WriteSector(sd_handle_t *sd, uint32_t sector, const uint8_t *buffer);

// Read Multiple Blocks

/**
 * @brief Starts a multi-block read operation. Locks the mutex.
 * @param sd Pointer to the SD handle structure.
 * @param blockNum The starting block address.
 * @return SD_OK on success, or specific error code on failure.
 */
int8_t SD_ReadBegin(sd_handle_t *sd, uint32_t blockNum);

/**
 * @brief Reads the next block of data during a multi-block read.
 * @param sd Pointer to the SD handle structure.
 * @param buff Pointer to the buffer (512 bytes).
 * @return SD_OK on success, or specific error code on failure.
 */
int8_t SD_ReadData(sd_handle_t *sd, uint8_t* buff); 

/**
 * @brief Ends a multi-block read operation. Unlocks the mutex.
 * @param sd Pointer to the SD handle structure.
 * @return SD_OK on success, or specific error code on failure.
 */
int8_t SD_ReadEnd(sd_handle_t *sd);

// Write Multiple Blocks

/**
 * @brief Starts a multi-block write operation. Locks the mutex.
 * @param sd Pointer to the SD handle structure.
 * @param blockNum The starting block address.
 * @return SD_OK on success, or specific error code on failure.
 */
int8_t SD_WriteBegin(sd_handle_t *sd, uint32_t blockNum);

/**
 * @brief Writes the next block of data during a multi-block write.
 * @param sd Pointer to the SD handle structure.
 * @param buff Pointer to the data to write (512 bytes).
 * @return SD_OK on success, or specific error code on failure.
 */
int8_t SD_WriteData(sd_handle_t *sd, const uint8_t* buff); 

/**
 * @brief Ends a multi-block write operation. Unlocks the mutex.
 * @param sd Pointer to the SD handle structure.
 * @return SD_OK on success, or specific error code on failure.
 */
int8_t SD_WriteEnd(sd_handle_t *sd);


#endif /* SDCARD_H */

