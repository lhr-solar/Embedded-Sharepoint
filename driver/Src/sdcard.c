#include "sdcard.h"
#include <stdint.h>
#include "stm32xx_hal.h"

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
    while (1) {
        HAL_SPI_TransmitReceive(sd->hspi, &tx, &r1, 1, HAL_MAX_DELAY);
        if ((r1 & 0x80) == 0)     // MSB must be 0 → valid R1
            return r1;
    }
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

    uint8_t tx = SD_DUMMY_BYTE;
    uint8_t rx;
    for (int i = 0; i < 10; i++) {
        HAL_SPI_TransmitReceive(sd->hspi, &tx, &rx, sizeof(tx), HAL_MAX_DELAY);
    }
    
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
    do {
        if(SD_ReadBytes(sd, &busy, sizeof(busy)) < 0) {
            return SD_ERR_READ;
        }

        // thread safe:
        // If busy (0x00), let the RTOS switch to another task for 1ms
        if (busy != SD_DUMMY_BYTE) {
            vTaskDelay(1); // Yield
        }
     
    } while(busy != SD_DUMMY_BYTE);

    return SD_OK;
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

    for (;;) {
        HAL_SPI_TransmitReceive(sd->hspi, &tx, &fb, 1, HAL_MAX_DELAY);

        if (fb == token)
            break;          // found correct data token

        if (fb != SD_DUMMY_BYTE)
            return SD_ERROR;      // unexpected token → error
    }

    return SD_OK;
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

    if (cmd != 0) {
        if (SD_WaitNotBusy(sd) < 0) {
            SD_Deselect(sd);
            return SD_DUMMY_BYTE;
        }
    }

    /* Send the 6-byte command */
    HAL_SPI_Transmit(sd->hspi, packet, sizeof(packet), HAL_MAX_DELAY);

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

    // thread safe RTOS init:
    // Create a Mutex statically 
    sd->mutex = xSemaphoreCreateMutexStatic(&sd->mutexBuffer);
    if (sd->mutex == NULL) {
        return SD_ERR_MUTEX; // Error creating mutex
    }

    uint8_t res;
    uint8_t resp[4];

    // Step 1: Dummy Clocks 
    SD_Deselect(sd);
    SD_SendDummyClocks(sd); 

    // Step 2: CMD0 (Go Idle) 
    SD_Select(sd);
    uint8_t cmd0[] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
    HAL_SPI_Transmit(sd->hspi, cmd0, sizeof(cmd0), HAL_MAX_DELAY);
    if(SD_ReadR1(sd) != 0x01) {
        SD_Deselect(sd);
        return SD_ERROR; 
    }
    SD_Deselect(sd);

    // Step 3: CMD8 (Voltage Check) 
    uint8_t r1 = SD_SendCommand(sd, 8, 0x000001AA, 0x87); // Capture return
    if(r1 != 0x01) {
        SD_Deselect(sd);
        return SD_ERR_VOLTAGE;
    }
    if(SD_ReadBytes(sd, resp, sizeof(resp)) < 0) {
        SD_Deselect(sd);
        return SD_ERR_READ; 
    }

    // Step 4: ACMD41 Loop (Initialize) 
    uint32_t tickStart = HAL_GetTick();
    
    for(;;) {
        if((HAL_GetTick() - tickStart) > 1000) {
             SD_Deselect(sd);
             return SD_TIMEOUT;
        }

        // Capture return directly
        // Send CMD55
        res = SD_SendCommand(sd, 55, 0, 0x65); 
        if(res > 0x01) { 
            SD_Deselect(sd);
            return SD_ERROR; 
        }
        SD_Deselect(sd);

        // Send ACMD41
        res = SD_SendCommand(sd, 41, 0x40000000, 0x77); 
        SD_Deselect(sd);
        
        if(res == 0x00) 
            break; // Success!
        if(res > 0x01) 
            return SD_ERR_INIT_CARD; // Error
        
        HAL_Delay(10);
    }

    // Step 5: CMD58 (Read OCR) 
    res = SD_SendCommand(sd, 58, 0, 0xFD);
    if(res != 0x00) {
        SD_Deselect(sd);
        return SD_ERROR;
    }

    if(SD_ReadBytes(sd, resp, sizeof(resp)) < 0) {
        SD_Deselect(sd);
        return SD_ERR_READ;
    }
    SD_Deselect(sd);

    // Check Power Up bit (Bit 31) only
    if((resp[0] & 0x80) == 0) {
        return SD_ERROR;
    }
    
    return 0; // Success
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

    // thread safe:
    // Take lock Wait forever (portMAX_DELAY) until it's free.
    if (xSemaphoreTake(sd->mutex, portMAX_DELAY) != pdTRUE) {
        return SD_ERR_LOCK_TIMEOUT; // RTOS error
    }
    
    uint8_t resp;
    resp = SD_SendCommand(sd, SD_CMD17, blockNum, 0x01); /* CMD17 */

    // thread safe:
    if (resp != 0x00) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERROR; 
    }

    if (SD_WaitDataToken(sd, 0xFE) < 0) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERROR; 
    }
    
    if (SD_ReadBytes(sd, buffer, SD_BLOCK_SIZE) < 0) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERR_READ; 
    }

    uint8_t crc[2];
    if (SD_ReadBytes(sd, crc, 2) < 0) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERR_READ; 
    }

    SD_Deselect(sd);

    // Release the lock
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
    // thread safe:
    // Take the lock
    if (xSemaphoreTake(sd->mutex, portMAX_DELAY) != pdTRUE) {
        return SD_ERR_LOCK_TIMEOUT;
    }

    uint8_t resp;
    resp = SD_SendCommand(sd, SD_CMD24, blockNum, 0x01); /* CMD24 */

    // thread safe:
    if (resp != 0x00) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERROR; 
    }

    uint8_t token = 0xFE;
    HAL_SPI_Transmit(sd->hspi, &token, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(sd->hspi, (uint8_t*)buffer, SD_BLOCK_SIZE, HAL_MAX_DELAY);

    uint8_t crc[2] = {0xFF, 0xFF};
    HAL_SPI_Transmit(sd->hspi, crc, 2, HAL_MAX_DELAY);

    uint8_t dataResp;
    if (SD_ReadBytes(sd, &dataResp, 1) < 0) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERR_READ; 
    }
    
    if ((dataResp & 0x1F) != 0x05) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERR_WRITE; 
    }

    if (SD_WaitNotBusy(sd) < 0) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_TIMEOUT; 
    }

    SD_Deselect(sd);
    
    // Release the lock
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

    uint8_t cmd[] = {
        SD_CMD_BASE | SD_CMD18,
        (blockNum >> 24) & 0xFF,
        (blockNum >> 16) & 0xFF,
        (blockNum >> 8) & 0xFF,
        blockNum & 0xFF,
        (0x7F << 1) | 1 
    };
    HAL_SPI_Transmit(sd->hspi, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);

    if(SD_ReadR1(sd) != 0x00) {
        SD_Deselect(sd);
        xSemaphoreGive(sd->mutex); // Unlock on fail
        return SD_ERROR;
    }

    SD_Deselect(sd);
    return 0;
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

    /* CMD12 (STOP_TRANSMISSION) */
    static const uint8_t cmd[] = { SD_CMD_BASE | SD_CMD12 /* CMD12 */, 0x00, 0x00, 0x00, 0x00 /* ARG */, (0x7F << 1) | 1 };
    HAL_SPI_Transmit(sd->hspi, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);

    uint8_t stuffByte;
    if(SD_ReadBytes(sd, &stuffByte, sizeof(stuffByte)) < 0) {
        SD_Deselect(sd);
        
        // thread safe:
        xSemaphoreGive(sd->mutex); // Unlock!

        return SD_ERR_READ;
    }

    if(SD_ReadR1(sd) != 0x00) {
        SD_Deselect(sd);

        // thread safe:
        xSemaphoreGive(sd->mutex); // Unlock!

        return SD_ERR_READ;
    }
    
    SD_Deselect(sd);

    // thread safe:
    xSemaphoreGive(sd->mutex); // Unlock!

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
    uint8_t cmd[] = {
        SD_CMD_BASE | SD_CMD25 /* CMD25 */,
        (blockNum >> 24) & 0xFF, /* ARG */
        (blockNum >> 16) & 0xFF,
        (blockNum >> 8) & 0xFF,
        blockNum & 0xFF,
        (0x7F << 1) | 1 /* CRC7 + end bit */
    };
    HAL_SPI_Transmit(sd->hspi, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);

    if(SD_ReadR1(sd) != 0x00) {
        SD_Deselect(sd);

        // thread safe:
        xSemaphoreGive(sd->mutex); // Unlock!

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

    uint8_t dataToken = DATA_TOKEN_CMD25;
    uint8_t crc[2] = { 0xFF, 0xFF };
    HAL_SPI_Transmit(sd->hspi, &dataToken, sizeof(dataToken), HAL_MAX_DELAY);
    HAL_SPI_Transmit(sd->hspi, (uint8_t*)buff, SD_BLOCK_SIZE, HAL_MAX_DELAY);
    HAL_SPI_Transmit(sd->hspi, crc, sizeof(crc), HAL_MAX_DELAY);

    /*
        dataResp:
        xxx0abc1
            010 - Data accepted
            101 - Data rejected due to CRC error
            110 - Data rejected due to write error
    */
    uint8_t dataResp;
    SD_ReadBytes(sd, &dataResp, sizeof(dataResp));
    if((dataResp & 0x1F) != 0x05) { // data rejected
        SD_Deselect(sd);
        return SD_ERR_WRITE;
    }

    if(SD_WaitNotBusy(sd) < 0) {
        SD_Deselect(sd);
        return SD_TIMEOUT;
    }

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

    uint8_t stopTran = 0xFD; // stop transaction token for CMD25
    HAL_SPI_Transmit(sd->hspi, &stopTran, sizeof(stopTran), HAL_MAX_DELAY);

    // skip one byte before readyng "busy"
    uint8_t skipByte;
    SD_ReadBytes(sd, &skipByte, sizeof(skipByte));

    if(SD_WaitNotBusy(sd) < 0) {
        SD_Deselect(sd);
        
        // thread safe:
        xSemaphoreGive(sd->mutex); // Unlock!

        return SD_TIMEOUT;
    }

    SD_Deselect(sd);
    // thread safe:
    xSemaphoreGive(sd->mutex); // Unlock!
    return SD_OK;
}