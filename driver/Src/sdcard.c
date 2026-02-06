#include "sdcard.h"
#include <stdint.h>
#include "stm32xx_hal.h"
#include <string.h>

// Global pointer so the ISR knows which handle to signal
static sd_handle_t *g_sd_handle = NULL;

/**
 * @brief  Performs an SPI transfer using Interrupts and waits for completion.
 * @note   Replaces HAL_SPI_TransmitReceive(..., HAL_MAX_DELAY).
 * Instead of freezing the CPU, this function puts the task to SLEEP.
 * * @param  sd   Pointer to SD handle
 * @param  tx   Pointer to TX data (can be NULL for Read-Only)
 * @param  rx   Pointer to RX data (can be NULL for Write-Only)
 * @param  len  Number of bytes to transfer
 * @return SD_OK if successful, SD_TIMEOUT if interrupt never fired.
 */
static SD_Status SD_SPI_TransferWait(sd_handle_t *sd, uint8_t *tx, uint8_t *rx, uint16_t len) {
    if (len == 0) 
        return SD_OK;

    // Clear Semaphore, so no old signal
    // Done flag 0
    xSemaphoreTake(sd->spi_comp_sem, 0);

    // Set global handle for ISR
    g_sd_handle = sd;

    // Start Non-Blocking Transfer
    HAL_StatusTypeDef res;
    if (tx == NULL) {
        res = HAL_SPI_Receive_IT(sd->hspi, rx, len); // READ ONLY: HAL sends dummy bytes (0x00 or 0xFF) automatically while reading
    } else if (rx == NULL) {
        res = HAL_SPI_Transmit_IT(sd->hspi, tx, len); // WRITE ONLY: HAL sends data and ignores the incoming bytes
    } else {
        res = HAL_SPI_TransmitReceive_IT(sd->hspi, tx, rx, len); // Full Duplex: sends TX while reading RX
    }

    if (res != HAL_OK) 
        return SD_ERR_SPI;

    // Sleep until ISR is up (CPU run other tasks)
    if (xSemaphoreTake(sd->spi_comp_sem, pdMS_TO_TICKS(1000)) != pdTRUE) {
        HAL_SPI_Abort(sd->hspi);
        return SD_TIMEOUT;
    }

    // Interruppt fired and data ready
    return SD_OK;
}

// Called automatically by HAL when the SPI hardware finishes a transfer
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Check if this interrupt belongs to SD Card driver
    if (g_sd_handle && g_sd_handle->hspi == hspi) {
        //Wakes up the task
        xSemaphoreGiveFromISR(g_sd_handle->spi_comp_sem, &xHigherPriorityTaskWoken);
    }
    
    // If the SD task is high priority, switch to it immediately
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Map specific callbacks to the general one
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) { 
    HAL_SPI_TxRxCpltCallback(hspi); 
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) { 
    HAL_SPI_TxRxCpltCallback(hspi); 
}

/* CS */
/**
 * @brief Asserts the Chip Select pin (sets it LOW).
 * @param sd Pointer to the SD handle structure.
 */
void SD_Select(sd_handle_t *sd)
{
    HAL_GPIO_WritePin(sd->cs_port, sd->cs_pin, GPIO_PIN_RESET);
}

/**
 * @brief De-asserts the Chip Select pin (sets it HIGH).
 * @param sd Pointer to the SD handle structure.
 */
void SD_Deselect(sd_handle_t *sd)
{
    HAL_GPIO_WritePin(sd->cs_port, sd->cs_pin, GPIO_PIN_SET);

}

// /* read R1 response (poll until first non 0xFF or MSB==0) */
/*
R1: 0abcdefg
     ||||||`- 1th bit (g): card is in idle state
     |||||`-- 2th bit (f): erase sequence cleared
     ||||`--- 3th bit (e): illigal command detected
     |||`---- 4th bit (d): crc check error
     ||`----- 5th bit (c): error in the sequence of erase commands
     |`------ 6th bit (b): misaligned addres used in command
     `------- 7th bit (a): command argument outside allowed range
             (8th bit is always zero)
*/
/**
 * @brief Reads the R1 response byte from the SD card.
 * @param sd Pointer to the SD handle structure.
 * @return The R1 response byte.
 */
uint8_t SD_ReadR1(sd_handle_t *sd)
{
    uint8_t r1;
    // make sure FF is transmitted during receive
    uint8_t tx = SD_DUMMY_BYTE;
    // Loop using interrupts for each byte
    for(int i=0; i<8; i++) { 
        if (SD_SPI_TransferWait(sd, &tx, &r1, 1) != SD_OK) 
            return 0xFF;
        if ((r1 & 0x80) == 0) 
            return r1; 
    }
    return SD_DUMMY_BYTE; 
}


/**
 * @brief Sends dummy clocks (0xFF) to the SD card with CS High to ensure it is ready or to force SPI mode.
 * @note This is a mandatory requirement from the SD Physical Layer Spec.
 * The card requires at least 74 clock cycles with CS De-asserted (High) 
 * to stabilize power and switch from native "SD Bus Mode" to "SPI Mode".
 * @param sd Pointer to the SD handle structure.
 */
void SD_SendDummyClocks(sd_handle_t *sd)
{
    /* Ensure CS high and send >= 80 clocks (10 bytes of 0xFF) */
    SD_Deselect(sd);

    // Create a buffer of 10 bytes (80 clocks)
    uint8_t tx[10];
    memset(tx, SD_DUMMY_BYTE, 10);

    // Send all 80 clocks in one go
    SD_SPI_TransferWait(sd, tx, NULL, 10);

}

/**
 * @brief Reads a raw stream of bytes from the SD card.
 * @param sd Pointer to the SD handle structure.
 * @param buff Pointer to the buffer to store received data.
 * @param len Number of bytes to read.
 * @return SD_OK on success, or specific error code on failure.
 */
/* read N bytes by sending 0xFF */
int8_t SD_ReadBytes(sd_handle_t *sd, uint8_t *buff, size_t len)
{
    if (buff == NULL) {
        return SD_ERROR; // Error: Buffer is null
    }
    
    if (len == 0) {
        return SD_ERROR; // Error: Length is zero
    }

    uint8_t tx = SD_DUMMY_BYTE; 
    
    while (len--) {
        if (HAL_SPI_TransmitReceive(sd->hspi, &tx, buff, 1, HAL_MAX_DELAY) != HAL_OK) {
            return SD_ERR_SPI; // Error: SPI failure
        }
        buff++;
    }

    return SD_OK; // Success

}

/**
 * @brief Waits for the SD card to return 0xFF (Not Busy).
 * Includes RTOS yield (vTaskDelay) to allow other tasks to run while waiting.
 * @param sd Pointer to the SD handle structure.
 * @return SD_OK on success, or specific error code on failure.
 */
/* wait for not-busy (card returns 0xFF when ready) */
int8_t SD_WaitNotBusy(sd_handle_t *sd)
{
    uint8_t busy;
    for(int i=0; i<500; i++) {
        if(SD_SPI_TransferWait(sd, NULL, &busy, 1) != SD_OK) 
            return SD_ERR_READ;
        if(busy == 0xFF) // line is high -> idle/ready
            return SD_OK; // Card is ready
        vTaskDelay(1); // Explicitly yield 1 tick to let other tasks run if not ready
    }
    return SD_TIMEOUT;
}

/**
 * @brief Waits for a specific data token (e.g., 0xFE) from the card.
 * @param sd Pointer to the SD handle structure.
 * @param token The expected token byte.
 * @return SD_OK on success, or specific error code on failure.
 */
/* wait for a data token (e.g. 0xFE) */
int8_t SD_WaitDataToken(sd_handle_t *sd, uint8_t token) { 
    uint8_t fb;
    uint8_t tx = SD_DUMMY_BYTE;  

    for (int i=0; i<1000; i++) { // Safety timeout
        if (SD_SPI_TransferWait(sd, &tx, &fb, 1) != SD_OK) 
            return SD_ERROR;
        if (fb == token) 
            return SD_OK;
        if (fb != 0xFF) 
            return SD_ERROR; 
    }
    return SD_TIMEOUT;
}

/**
 * @brief Sends a command to the SD card and reads the R1 response.
 * @param sd Pointer to the SD handle structure.
 * @param cmd The command index (e.g., SD_CMD0).
 * @param arg The 32-bit argument for the command.
 * @param crc The CRC byte for the command.
 * @return The R1 response byte.
 */
/* send a command packet and return R1 */
uint8_t SD_SendCommand(sd_handle_t *sd, uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t packet[6];
    packet[0] = SD_CMD_BASE | (cmd & 0x3F);
    packet[1] = (arg >> 24) & 0xFF;
    packet[2] = (arg >> 16) & 0xFF;
    packet[3] = (arg >> 8) & 0xFF;
    packet[4] = arg & 0xFF;
    packet[5] = crc;

    SD_Select(sd);

    // Commands require the card to be reayd first
    if (cmd != 0) {
        if (SD_WaitNotBusy(sd) != SD_OK) {
            SD_Deselect(sd);
            return SD_DUMMY_BYTE;
        }
    }

    // Send 6 bytes efficiently via Interrupt
    SD_SPI_TransferWait(sd, packet, NULL, 6);
    return SD_ReadR1(sd);
}

/**
 * @brief Initializes the SD card driver, sets up the mutex, and performs the SD startup sequence.
 * @note The user MUST initialize the SPI Peripheral (hspi) and all associated GPIOs (SCK, MISO, MOSI, CS) before calling this function.
 * The CS pin should be configured as Output Push-Pull, initially High.
 * @param sd Pointer to the SD handle structure.
 * @return SD_OK on success, or specific error code on failure.
 */
int8_t SD_Init(sd_handle_t *sd) {

    // 1. Initialize Thread Safety Objects
    // create mutex
    sd->mutex = xSemaphoreCreateMutexStatic(&sd->mutexBuffer);
    if (sd->mutex == NULL) 
        return SD_ERR_MUTEX; 

    // 2. Initialize Interrupt Synchronization Object (NEW)
    // create semaphore
    sd->spi_comp_sem = xSemaphoreCreateBinaryStatic(&sd->spi_comp_buffer);
    if (sd->spi_comp_sem == NULL) 
        return SD_ERR_MUTEX;

    uint8_t resp[4];
    
    SD_Deselect(sd);
    SD_SendDummyClocks(sd); 

    // send CMD0
    SD_Select(sd);
    uint8_t cmd0[] = {0x40, 0, 0, 0, 0, 0x95};
    SD_SPI_TransferWait(sd, cmd0, NULL, 6); // Interrupt transfer
    
    if(SD_ReadR1(sd) != 0x01) { 
        SD_Deselect(sd); 
        return SD_ERROR; 
    } 
    SD_Deselect(sd);

    // CMD8: Check voltage
    if(SD_SendCommand(sd, 8, 0x000001AA, 0x87) != 0x01) { 
        SD_Deselect(sd); 
        return SD_ERR_VOLTAGE; 
    }
    
    // Read 4 bytes via Interrupt
    if(SD_SPI_TransferWait(sd, NULL, resp, 4) != SD_OK) { 
        SD_Deselect(sd); 
        return SD_ERR_READ; 
    }

    // ACMD41: Initialize Card (Loop until Ready)
    uint32_t tickStart = HAL_GetTick();
    uint8_t res;
    for(;;) {
        if((HAL_GetTick() - tickStart) > 1000) { 
            SD_Deselect(sd); 
            return SD_TIMEOUT; 
        }
        if(SD_SendCommand(sd, 55, 0, 0x65) > 0x01) { 
            SD_Deselect(sd); 
            return SD_ERROR; 
        }
        SD_Deselect(sd);
        res = SD_SendCommand(sd, 41, 0x40000000, 0x77); 
        SD_Deselect(sd);
        if(res == 0x00) 
            break; 
        if(res > 0x01) 
            return SD_ERR_INIT_CARD; 
        vTaskDelay(10);
    }
    
    // CMD58: Read OCR (Operation Conditions)
    if(SD_SendCommand(sd, 58, 0, 0xFD) != 0x00) { 
        SD_Deselect(sd); 
        return SD_ERROR; 
    }
    if(SD_SPI_TransferWait(sd, NULL, resp, 4) != SD_OK) { 
        SD_Deselect(sd); 
        return SD_ERR_READ; 
    }
    SD_Deselect(sd);
    if((resp[0] & 0x80) == 0) 
        return SD_ERROR; 
    
    return SD_OK;
}

/**
 * @brief Reads a single 512-byte block from the SD card (Thread Safe).
 * @param sd Pointer to the SD handle structure.
 * @param blockNum The block address to read.
 * @param buffer Pointer to the buffer (must be at least 512 bytes).
 * @return 0 on success, negative error code on failure.
 */
int8_t SD_ReadSingleBlock(sd_handle_t *sd, uint32_t blockNum, uint8_t *buffer)
{
    // 1. Acquire Mutex (Queueing behavior)
    if (xSemaphoreTake(sd->mutex, portMAX_DELAY) != pdTRUE) 
        return SD_ERR_LOCK_TIMEOUT;
    
    // 2. Send Command CMD17 (Read Block)
    if (SD_SendCommand(sd, 17, blockNum, 0x01) != 0x00) { 
        SD_Deselect(sd); xSemaphoreGive(sd->mutex); 
        return SD_ERROR; 
    }
    
    // 3. Wait for the card to find the data (Token 0xFE)
    if (SD_WaitDataToken(sd, 0xFE) != SD_OK) { 
        SD_Deselect(sd); xSemaphoreGive(sd->mutex); 
        return SD_ERROR; 
    }
    
    // 4. READ DATA (Interrupt Driven) reads 512 bytes
    if (SD_ReadBytes(sd, buffer, SD_BLOCK_SIZE) != SD_OK) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERR_READ; 
    }

    // 5. Read CRC (2 bytes) 
    uint8_t crc[2];
    SD_SPI_TransferWait(sd, NULL, crc, 2);
    
    // 6. unlock, clean
    SD_Deselect(sd);
    xSemaphoreGive(sd->mutex);
    return SD_OK;
}

/**
 * @brief Writes a single 512-byte block to the SD card (Thread Safe).
 * @param sd Pointer to the SD handle structure.
 * @param blockNum The block address to write to.
 * @param buffer Pointer to the data to write (512 bytes).
 * @return 0 on success, negative error code on failure.
 */
int8_t SD_WriteSingleBlock(sd_handle_t *sd, uint32_t blockNum, const uint8_t *buffer)
{
    // 1. lock mutex
    if (xSemaphoreTake(sd->mutex, portMAX_DELAY) != pdTRUE) 
        return SD_ERR_LOCK_TIMEOUT;
    
    // 2. Send Command CMD24 (Write Block)
    if (SD_SendCommand(sd, 24, blockNum, 0x01) != 0x00) { 
        SD_Deselect(sd); xSemaphoreGive(sd->mutex); 
        return SD_ERROR; 
    }
    
    // 3. Send Start Token (0xFE)
    uint8_t token = 0xFE;
    SD_SPI_TransferWait(sd, &token, NULL, 1);

    // 4. WRITE DATA (Interrupt Driven)
    if (SD_SPI_TransferWait(sd, (uint8_t*)buffer, NULL, SD_BLOCK_SIZE) != SD_OK) {
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERR_WRITE; 
    }
    
    // 5. Send Dummy CRC
    uint8_t crc[] = {0xFF, 0xFF};
    SD_SPI_TransferWait(sd, crc, NULL, 2);
    
    // 6. Receive Data Response
    uint8_t dataResp;
    HAL_SPI_Receive(sd->hspi, &dataResp, 1, 100); 
    
    // Check if data was accepted (xxx00101)
    if ((dataResp & 0x1F) != 0x05) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERR_WRITE; 
    }
    
    // 7. Wait for Flash Programming 
    if (SD_WaitNotBusy(sd) != SD_OK) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_TIMEOUT; 
    }
    
    // 8. unlock and clean
    SD_Deselect(sd);
    xSemaphoreGive(sd->mutex);
    return SD_OK;
}

/**
 * @brief Starts a multi-block read operation. Locks the mutex.
 * @param sd Pointer to the SD handle structure.
 * @param blockNum The starting block address.
 * @return 0 on success, negative error code on failure.
 */
int8_t SD_ReadBegin(sd_handle_t *sd, uint32_t blockNum) {
   
    // thread safe:
    // 1. Lock Mutex
    if (xSemaphoreTake(sd->mutex, portMAX_DELAY) != pdTRUE) return SD_ERR_LOCK_TIMEOUT;

    SD_Select(sd);

    if(SD_WaitNotBusy(sd) < 0) { 
        SD_Deselect(sd);
        xSemaphoreGive(sd->mutex); // Unlock on fail
        return SD_TIMEOUT;
    }

    uint8_t cmd[] = { SD_CMD_BASE | SD_CMD18, (blockNum >> 24) & 0xFF, (blockNum >> 16) & 0xFF, (blockNum >> 8) & 0xFF, blockNum & 0xFF, 0xFF };
    SD_SPI_TransferWait(sd, cmd, NULL, 6);
    
    if(SD_ReadR1(sd) != 0x00) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERROR; 
    }
    SD_Deselect(sd);
    return SD_OK;
}

/**
 * @brief Reads the next block of data during a multi-block read.
 * @param sd Pointer to the SD handle structure.
 * @param buff Pointer to the buffer (512 bytes).
 * @return 0 on success, negative error code on failure.
 */
int8_t SD_ReadData(sd_handle_t *sd, uint8_t* buff) {
    uint8_t crc[2];
    SD_Select(sd);

    if(SD_WaitDataToken(sd, DATA_TOKEN_CMD18) < 0) {
        SD_Deselect(sd);
        return SD_ERROR;
    }

    if(SD_ReadBytes(sd, buff, SD_BLOCK_SIZE) < 0) {
        SD_Deselect(sd);
        return SD_ERR_READ;
    }

    if(SD_ReadBytes(sd, crc, 2) < 0) {
        SD_Deselect(sd);
        return SD_ERR_READ;
    }

    SD_Deselect(sd);
    return SD_OK;

}

/**
 * @brief Ends a multi-block read operation. Unlocks the mutex.
 * @param sd Pointer to the SD handle structure.
 * @return 0 on success, negative error code on failure.
 */
int8_t SD_ReadEnd(sd_handle_t *sd) {

    SD_Select(sd);
    uint8_t cmd[] = { SD_CMD_BASE | 12, 0, 0, 0, 0, 0xFF };
    SD_SPI_TransferWait(sd, cmd, NULL, 6);
    
    uint8_t stuff;
    HAL_SPI_Receive(sd->hspi, &stuff, 1, 10);
    SD_ReadR1(sd);
    SD_Deselect(sd);
    xSemaphoreGive(sd->mutex); 
    return SD_OK;
}

/**
 * @brief Starts a multi-block write operation. Locks the mutex.
 * @param sd Pointer to the SD handle structure.
 * @param blockNum The starting block address.
 * @return 0 on success, negative error code on failure.
 */
int8_t SD_WriteBegin(sd_handle_t *sd, uint32_t blockNum) {
    // thread safe: Lock Mutex
    if (xSemaphoreTake(sd->mutex, portMAX_DELAY) != pdTRUE) 
        return SD_ERR_LOCK_TIMEOUT;

    SD_Select(sd);

    if(SD_WaitNotBusy(sd) < 0) { 
        SD_Deselect(sd);

        // thread safe:
        xSemaphoreGive(sd->mutex); // Unlock!

        return SD_TIMEOUT;
    }

    /* CMD25 (WRITE_MULTIPLE_BLOCK) command */
    uint8_t cmd[] = { SD_CMD_BASE | 25, (blockNum >> 24) & 0xFF, (blockNum >> 16) & 0xFF, (blockNum >> 8) & 0xFF, blockNum & 0xFF, 0xFF };
    SD_SPI_TransferWait(sd, cmd, NULL, 6);
    if(SD_ReadR1(sd) != 0x00) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERROR; 
    }
    SD_Deselect(sd);
    return SD_OK;
}

/**
 * @brief Writes the next block of data during a multi-block write.
 * @param sd Pointer to the SD handle structure.
 * @param buff Pointer to the data to write (512 bytes).
 * @return 0 on success, negative error code on failure.
 */
int8_t SD_WriteData(sd_handle_t *sd, const uint8_t* buff) {
    SD_Select(sd);
    uint8_t token = 0xFC; 
    SD_SPI_TransferWait(sd, &token, NULL, 1);
    SD_SPI_TransferWait(sd, (uint8_t*)buff, NULL, SD_BLOCK_SIZE);
    
    uint8_t crc[] = {0xFF, 0xFF};
    SD_SPI_TransferWait(sd, crc, NULL, 2);
    
    uint8_t resp;
    HAL_SPI_Receive(sd->hspi, &resp, 1, 100);
    if((resp & 0x1F) != 0x05) { SD_Deselect(sd); return SD_ERR_WRITE; }
    if(SD_WaitNotBusy(sd) != SD_OK) { SD_Deselect(sd); return SD_TIMEOUT; }
    SD_Deselect(sd);
    return SD_OK;
}

/**
 * @brief Ends a multi-block write operation. Unlocks the mutex.
 * @param sd Pointer to the SD handle structure.
 * @return 0 on success, negative error code on failure.
 */
int8_t SD_WriteEnd(sd_handle_t *sd) {
SD_Select(sd);
    uint8_t stop = 0xFD; 
    SD_SPI_TransferWait(sd, &stop, NULL, 1);
    uint8_t skip;
    HAL_SPI_Receive(sd->hspi, &skip, 1, 10);
    
    if(SD_WaitNotBusy(sd) != SD_OK) { SD_Deselect(sd); xSemaphoreGive(sd->mutex); return SD_TIMEOUT; }
    SD_Deselect(sd);
    xSemaphoreGive(sd->mutex); 
    return SD_OK;
}