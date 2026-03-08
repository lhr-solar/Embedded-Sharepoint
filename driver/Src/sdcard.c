#include "sdcard.h"
#include "fatfs.h"
#include "ff.h"
#include <string.h>

static sd_handle_t *g_sd_handle = NULL;
static const uint8_t g_large_dummy_buffer[SD_BLOCK_SIZE] = { [0 ... SD_BLOCK_SIZE-1] = SD_DUMMY_BYTE }; // dummy buffer array used to generate SPI clock cycles during large reads
//memory allocation for worker task
StaticTask_t xWorkerBuffer;
StackType_t xWorkerStack[2048]; 

/**
 * @brief starts an interrupt-driven SPI transfer and blocks the calling task until complete.
 * 1.takes a byte from the tx memory spot
 * 2.pushes it out the MOSI wire, and simultaneously pulls a byte from the MISO wire to park it in the rx memory spot
 * 3.then pauses the task until the hardware says the "swap" is finished.
 * @param sd      Pointer to the SD handle structure.
 * @param tx      Pointer to the transmit buffer (NULL for receive-only).
 * @param rx      Pointer to the receive buffer (NULL for transmit-only).
 * @param len     Number of bytes to transfer.
 * @param timeout Maximum ticks to wait for the transfer to complete.
 * @return sd_status_t SD_OK on success, SD_TIMEOUT or SD_ERR_SPI on failure.
 */
static sd_status_t SD_SPI_TransferWait_IT(sd_handle_t *sd, uint8_t *tx, uint8_t *rx, uint16_t len, TickType_t timeout) {
    if (len == 0) {
        return SD_OK;
    }

    //only wait if the SPI is actually busy.
    //otherwise, clears old signal before starting
    if (HAL_SPI_GetState(sd->hspi) != HAL_SPI_STATE_READY) {
        xSemaphoreTake(sd->spi_comp_sem, timeout); 
    } else {
        xSemaphoreTake(sd->spi_comp_sem, 0); 
    }
    // xSemaphoreTake(sd->spi_comp_sem, 0);
    g_sd_handle = sd;
    HAL_StatusTypeDef res;
    
    //recieve data only
    //sends dummy byte to provide clock cycle
    if (tx == NULL) {
        if (len <= SD_SMALL_TRANSFER_THRESHOLD) {
            uint8_t dummy[SD_SMALL_TRANSFER_THRESHOLD];
            memset(dummy, SD_DUMMY_BYTE, len); //SD Card requires 0xFF on MOSI WHILE Reading
            res = HAL_SPI_TransmitReceive_IT(sd->hspi, dummy, rx, len);
        } 
        else {
            //if read is too large
            // uint8_t dummy = SD_DUMMY_BYTE;
            // for(int i=0; i<len; i++) {
            //     if (HAL_SPI_TransmitReceive(sd->hspi, &dummy, &rx[i], 1, timeout) != HAL_OK) 
            //         return SD_ERR_SPI;
            // }
            // return SD_OK; 
            res = HAL_SPI_TransmitReceive_IT(sd->hspi, (uint8_t*)g_large_dummy_buffer, rx, len);
        }
    } 
    //transmit only
    else if (rx == NULL) {
        res = HAL_SPI_Transmit_IT(sd->hspi, tx, len); 
    } 
    //recieve and transmit (full duplex)
    else {
        res = HAL_SPI_TransmitReceive_IT(sd->hspi, tx, rx, len); 
    }

    if (res != HAL_OK) {
        return SD_ERR_SPI;
    }

    //put task to sleep so cpu could do other stuff until spi hardware wake this task up
    if (xSemaphoreTake(sd->spi_comp_sem, timeout) != pdTRUE) {
        //if take too long
        HAL_SPI_Abort(sd->hspi);
        return SD_TIMEOUT;
    }

    return SD_OK;
}

void sdcard_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi, BaseType_t* xHigherPriorityTaskWoken){
    if (g_sd_handle != NULL && g_sd_handle->hspi == hspi){        
        xSemaphoreGiveFromISR(g_sd_handle->spi_comp_sem, xHigherPriorityTaskWoken);
    }
}

/**
 * @brief Asserts the Chip Select (CS) line (Logic Low).
 * @param sd Pointer to the SD handle.
 */
void SD_Select(sd_handle_t *sd) { 
    if (sd == NULL) {
        return;
    }
    HAL_GPIO_WritePin(sd->cs_port, sd->cs_pin, GPIO_PIN_RESET); 
}

/**
 * @brief De-asserts the Chip Select (CS) line (Logic High).
 * @param sd Pointer to the SD handle.
 */
void SD_Deselect(sd_handle_t *sd) { 
    if (sd == NULL) {
        return;
    }
    HAL_GPIO_WritePin(sd->cs_port, sd->cs_pin, GPIO_PIN_SET); 
}

/**
 * @brief Sends at least 74 clock cycles with CS high to enter SPI mode.
 * @param sd Pointer to the SD handle.
 * @param timeout Max wait time for the SPI transfer.
 */
static void SD_SendDummyClocks(sd_handle_t *sd, TickType_t timeout) {
    if (sd == NULL){
        return;
    }

    SD_Deselect(sd);
    uint8_t tx[SD_DUMMY_CLOCKS_COUNT];
    memset(tx, SD_DUMMY_BYTE, SD_DUMMY_CLOCKS_COUNT);
    SD_SPI_TransferWait_IT(sd, tx, NULL, SD_DUMMY_CLOCKS_COUNT, timeout);
}

/**
 * @brief Reads a specific number of raw bytes from the SPI bus.
 * @param sd Pointer to the SD handle.
 * @param buff Destination buffer for the read data.
 * @param len Number of bytes to read.
 * @param timeout Max wait time.
 * @return sd_status_t SD_OK or SD_ERROR.
 */
static sd_status_t SD_ReadBytes(sd_handle_t *sd, uint8_t *buff, size_t len, TickType_t timeout) {
    if (buff == NULL || len == 0) {
        return SD_ERROR;
    }
    return SD_SPI_TransferWait_IT(sd, NULL, buff, len, timeout);
}

/**
 * @brief Polls the SPI bus for the R1 response byte from the SD card until card stop sending dummy byte.
 * @param sd Pointer to the SD handle.
 * @param timeout Max wait time.
 * @return uint8_t The R1 response byte, or 0xFF if a timeout occurred.
 */
static uint8_t SD_ReadR1(sd_handle_t *sd, TickType_t timeout) {
    uint8_t r1;
    uint8_t tx = SD_DUMMY_BYTE;
    for(int i=0; i<SD_R1_POLL_RETRIES; i++) { 
        if (SD_SPI_TransferWait_IT(sd, &tx, &r1, 1, timeout) != SD_OK) {
            return SD_R1_ERROR_CODE; // no response from bus
        }
        if ((r1 & SD_R1_IDLE_MASK) == 0) {
            return r1; 
        }
    }
    return SD_DUMMY_BYTE; 
}

/**
 * @brief Polls the SD card until it releases the MISO line (Logic High).
 * * During write operations, the SD card holds MISO low while the internal flash controller is busy.
 * * if card sends 0xFF: not busy
 * * if card sends anything but 0xFF: still busy
 * @param sd Pointer to the SD handle.
 * @param timeout Max wait time.
 * @return sd_status_t SD_OK if card becomes ready, SD_TIMEOUT otherwise.
 */
static sd_status_t SD_WaitNotBusy(sd_handle_t *sd, TickType_t timeout) {
    uint8_t busy;
    for(int i=0; i<SD_BUSY_WAIT_RETRIES; i++) {
        if(SD_ReadBytes(sd, &busy, 1, timeout) != SD_OK) {
            return SD_ERR_READ;
        }
        if(busy == SD_DUMMY_BYTE) {
            return SD_OK; 
        }
        //yields the CPU to other FreeRTOS tasks while the SD card's physical flash memory is busy with internal write 
        vTaskDelay(pdMS_TO_TICKS(1)); 
    }
    return SD_TIMEOUT;
}

/**
 * @brief Waits for a specific Data Token to be sent by the card.
 * @param sd Pointer to the SD handle.
 * @param token The specific byte value to wait for.
 * @param timeout Max wait time.
 * @return sd_status_t SD_OK if token found, SD_TIMEOUT or SD_ERROR otherwise.
 */

static sd_status_t SD_WaitDataToken(sd_handle_t *sd, uint8_t token, TickType_t timeout) { 
    uint8_t fb;
    uint8_t tx = SD_DUMMY_BYTE;  
    for (int i=0; i<1000; i++) { 
        if (SD_SPI_TransferWait_IT(sd, &tx, &fb, 1, timeout) != SD_OK) {
            return SD_ERROR;
        }
        if (fb == token) {
            return SD_OK;
        }
        if (fb != SD_DUMMY_BYTE) {
            return SD_ERROR; 
        }
    }
    return SD_TIMEOUT;
}

/**
 * @brief Formats and sends a standard 6-byte SPI command to the SD card.
 * @param sd Pointer to the SD handle.
 * @param cmd Command index (0-63).
 * @param arg 32-bit command argument.
 * @param crc 7-bit CRC + 1-bit end bit.
 * @param timeout Max wait time.
 * @return uint8_t The R1 response from the card.
 */
static uint8_t SD_SendCommand(sd_handle_t *sd, uint8_t cmd, uint32_t arg, uint8_t crc, TickType_t timeout) {
    uint8_t packet[SD_CMD_PACKET_SIZE];
    packet[0] = SD_CMD_BASE | (cmd & SD_CMD_INDEX_MASK);
    packet[1] = (arg >> SD_BYTE_SHIFT_24) & SD_BYTE_MASK;
    packet[2] = (arg >> SD_BYTE_SHIFT_16) & SD_BYTE_MASK;
    packet[3] = (arg >> SD_BYTE_SHIFT_8)  & SD_BYTE_MASK;
    packet[4] = arg & SD_BYTE_MASK;
    packet[5] = crc;

    SD_Select(sd);
    if (cmd != SD_CMD0) {
        if (SD_WaitNotBusy(sd, timeout) != SD_OK) {
            SD_Deselect(sd);
            return SD_DUMMY_BYTE; //send dummy to provide clocks so card can send back response
        }
    }
    SD_SPI_TransferWait_IT(sd, packet, NULL, SD_CMD_PACKET_SIZE, timeout);
    return SD_ReadR1(sd, timeout);

}


sd_status_t SD_Init(sd_handle_t *sd, TickType_t timeout) {

    if (sd == NULL) {
        return SD_ERROR;
    }

    //check if mutex is given
    if (xSemaphoreTake(sd->mutex, portMAX_DELAY) != pdTRUE){
        return SD_ERR_LOCK_TIMEOUT;
    }
    uint8_t res;
    uint8_t resp[4];

    //1.wake up SD Card
    SD_Deselect(sd);
    SD_SendDummyClocks(sd, timeout); 

    //2.CMD0: reset card to idle state
    SD_Select(sd);
    uint8_t cmd0[] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
    //sends the 6 bytes of CMD0 and waits for the hardware to finish.
    SD_SPI_TransferWait_IT(sd, cmd0, NULL, 6, timeout);
    //if returns 0xFF, the card didn't hear us
    //if returns something else, the card is broken or wired wrong
    if(SD_ReadR1(sd, timeout) != 0x01) { 
        SD_Deselect(sd); 
        return SD_ERROR; 
    }
    SD_Deselect(sd);

    //3.CMD8: check voltage compatibility 
    if(SD_SendCommand(sd, SD_CMD8, 0x000001AA, 0x87, timeout) != 0x01) { 
        SD_Deselect(sd); 
        return SD_ERR_VOLTAGE; 
    }
    //read the 4 extra bytes for CMD8
    if(SD_ReadBytes(sd, resp, 4, timeout) != SD_OK) { 
        SD_Deselect(sd); 
        return SD_ERR_READ; 
    }

    //4.RTOS-safe timeout loop
    TickType_t start_tick = xTaskGetTickCount();
    for(;;) {
        if((xTaskGetTickCount() - start_tick) > pdMS_TO_TICKS(SD_DEFAULT_TIMEOUT_MS)) { 
            SD_Deselect(sd); 
            return SD_TIMEOUT; 
        }
        //CMD55:
        if(SD_SendCommand(sd, SD_CMD55, 0, 0x65, timeout) > 0x01) { 
            SD_Deselect(sd); 
            return SD_ERROR; 
        }
        SD_Deselect(sd);
        //ACMD41: statrs card's internal intitialization
        res = SD_SendCommand(sd, SD_ACMD41, 0x40000000, 0x77, timeout); 
        SD_Deselect(sd);
        if(res == 0x00) {
            break; //ready!
        }
        if(res > 0x01) {
            return SD_ERR_INIT_CARD; 
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    //5.CMD48 asks for OCR register to tell if it's a high-capacity (SDHC) or standard card.
    if(SD_SendCommand(sd, SD_CMD58, 0, 0xFD, timeout) != 0x00) { 
        SD_Deselect(sd); 
        return SD_ERROR; 
    }
    if(SD_ReadBytes(sd, resp, 4, timeout) != SD_OK) { 
        SD_Deselect(sd);
        return SD_ERR_READ; 
    }
    SD_Deselect(sd);

    if((resp[0] & SD_R1_IDLE_MASK) == 0) {
        return SD_ERROR;
    }
    
    //release mutex for other tasks
    xSemaphoreGive(sd->mutex);
    return SD_OK;
}

//fetch 512 bytes from the card
sd_status_t SD_ReadSingleBlock(sd_handle_t *sd, uint32_t blockNum, uint8_t *buffer, TickType_t timeout) {
    //check if mutex is given
    if (xSemaphoreTake(sd->mutex, portMAX_DELAY) != pdTRUE){
        return SD_ERR_LOCK_TIMEOUT;
    }
    //send CMD17 to read block
    if (SD_SendCommand(sd, SD_CMD17, blockNum, 0x01, timeout) != 0x00) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERROR; 
    }
    //waits for a data token for the card to start sending back
    if (SD_WaitDataToken(sd, DATA_TOKEN_CMD18, timeout) != SD_OK) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERROR; 
    }
    //MCU pulls the 512 bytes of file data into buffer
    if (SD_ReadBytes(sd, buffer, SD_BLOCK_SIZE, timeout) != SD_OK) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERR_READ; 
    }  
    uint8_t crc[2];
    SD_ReadBytes(sd, crc, 2, timeout);
    //release CS and mutex for other tasks
    SD_Deselect(sd);
    xSemaphoreGive(sd->mutex);
    return SD_OK;
}

//saving 512 bytes of data to the card
sd_status_t SD_WriteSingleBlock(sd_handle_t *sd, uint32_t blockNum, const uint8_t *buffer, TickType_t timeout) {
    //check if mutex is given
    if (xSemaphoreTake(sd->mutex, portMAX_DELAY) != pdTRUE) {
        return SD_ERR_LOCK_TIMEOUT;
    }
    //send CMD24 to write block
    if (SD_SendCommand(sd, SD_CMD24, blockNum, 0x01, timeout) != 0x00) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERROR; 
    }
    
    //MCU sends data token first so SD Card knows that the next byte is the real data
    uint8_t token = DATA_TOKEN_CMD18;
    //pushes 512 bytes from the buffer onto the MOSI wire and into the SD card's flash memory
    SD_SPI_TransferWait_IT(sd, &token, NULL, 1, timeout);
    SD_SPI_TransferWait_IT(sd, (uint8_t*)buffer, NULL, SD_BLOCK_SIZE, timeout);
    uint8_t crc[] = {0xFF, 0xFF};
    SD_SPI_TransferWait_IT(sd, crc, NULL, 2, timeout);
    
    uint8_t dataResp;
    SD_ReadBytes(sd, &dataResp, 1, timeout);
    if ((dataResp & 0x1F) != 0x05) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERR_WRITE; 
    }
    if (SD_WaitNotBusy(sd, timeout) != SD_OK) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_TIMEOUT; 
    }
    
    //if data is accepted
    SD_Deselect(sd);
    xSemaphoreGive(sd->mutex);
    return SD_OK;
}

/* USER FUNCTIONS */
sd_status_t USER_SD_Card_Init(sd_handle_t *sd) {
    //creates mutex (ensure only one task at a time)
    sd->mutex = xSemaphoreCreateMutexStatic(&sd->mutexBuffer);
    //creates semaphore (tells when SPI hardware has finished moving data)
    sd->spi_comp_sem = xSemaphoreCreateBinaryStatic(&sd->spi_comp_buffer);
    //creates queue where tasks or requests are stored until it can be handled
    sd->job_queue = xQueueCreateStatic(SD_QUEUE_LENGTH, sizeof(sd_job_t), sd->queue_storage, &sd->queue_buffer);

    if (sd->mutex == NULL || sd->spi_comp_sem == NULL || sd->job_queue == NULL) {
        return SD_ERR_MUTEX;
    }
    
    // create tasks
    sd->worker_task_handle = xTaskCreateStatic(
        USER_SD_Card_Worker_Task, // The function to run
        "SD_Worker",              // Name for debugging
        2048,                     // Stack size
        (void*)sd,                // Pass the handle as the parameter
        tskIDLE_PRIORITY + 3,     // Priority
        xWorkerStack,             // The stack buffer
        &xWorkerBuffer            // The task buffer
    );

    MX_FATFS_Init();
    return SD_OK;
}

sd_status_t USER_SD_Card_Write_Async(sd_handle_t *sd, const char *filename, const char *data, TickType_t delay_ticks) {
    if (sd->job_queue == NULL) {
        return SD_ERR_SPI;
    }

    sd_job_t job;
    job.type = SD_JOB_WRITE_ASYNC;
    
    //copies filename
    strncpy(job.filename, filename, SD_MAX_FILENAME_LEN);
    job.filename[SD_MAX_FILENAME_LEN-1] = '\0';

    uint32_t len = strlen(data);
    if (len >= SD_DATA_BUFFER_LEN){
        len = SD_DATA_BUFFER_LEN - 1; 
    }
    job.len = len;

    //copies data
    strncpy(job.data, data, SD_DATA_BUFFER_LEN);
    job.data[SD_DATA_BUFFER_LEN-1] = '\0'; 

    //place in queue
    if (xQueueSend(sd->job_queue, &job, delay_ticks) != pdTRUE) {
        return SD_ERR_QUEUE_FULL;
    }
    return SD_OK;
}

void USER_SD_Card_Worker_Task(void *params) {
    sd_handle_t *sd = (sd_handle_t *)params;
    sd_job_t job;
    FIL f_obj;
    FATFS fs; 
    UINT bw;
    
    while (SD_Init(sd, pdMS_TO_TICKS(1000)) != SD_OK) { 
        vTaskDelay(pdMS_TO_TICKS(500)); 
    }

    f_mount(&fs, "", 1); 

    while (1) {
        //waits for task to be placed on queue
        if (xQueueReceive(sd->job_queue, &job, portMAX_DELAY) == pdTRUE) {
            if (job.type == SD_JOB_WRITE_ASYNC) {
                if (f_open(&f_obj, job.filename, FA_OPEN_ALWAYS | FA_WRITE | FA_OPEN_APPEND) == FR_OK) {
                    f_write(&f_obj, job.data, job.len, &bw);
                    f_close(&f_obj);
                }
            }
        }
    }
}
