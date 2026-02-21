#include "sdcard.h"
#include "fatfs.h"
#include "ff.h"
#include <string.h>

static sd_handle_t *g_sd_handle = NULL;

/* RTOS INTERRUPT*/ 
static sd_status_t SD_SPI_TransferWait_IT(sd_handle_t *sd, uint8_t *tx, uint8_t *rx, uint16_t len, TickType_t timeout) {
    if (len == 0) return SD_OK;

    xSemaphoreTake(sd->spi_comp_sem, 0);
    g_sd_handle = sd;
    HAL_StatusTypeDef res;
    
    if (tx == NULL) {
        if (len <= 16) {
            uint8_t dummy[16];
            memset(dummy, SD_DUMMY_BYTE, len);
            res = HAL_SPI_TransmitReceive_IT(sd->hspi, dummy, rx, len);
        } else {
            uint8_t dummy = SD_DUMMY_BYTE;
            for(int i=0; i<len; i++) {
                if (HAL_SPI_TransmitReceive(sd->hspi, &dummy, &rx[i], 1, timeout) != HAL_OK) return SD_ERR_SPI;
            }
            return SD_OK; 
        }
    } else if (rx == NULL) {
        res = HAL_SPI_Transmit_IT(sd->hspi, tx, len); 
    } else {
        res = HAL_SPI_TransmitReceive_IT(sd->hspi, tx, rx, len); 
    }

    if (res != HAL_OK) return SD_ERR_SPI;

    if (xSemaphoreTake(sd->spi_comp_sem, timeout) != pdTRUE) {
        HAL_SPI_Abort(sd->hspi);
        return SD_TIMEOUT;
    }

    return SD_OK;
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (g_sd_handle != NULL && g_sd_handle->hspi == hspi){        
        xSemaphoreGiveFromISR(g_sd_handle->spi_comp_sem, &xHigherPriorityTaskWoken);
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) { 
    HAL_SPI_TxRxCpltCallback(hspi); 
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) { 
    HAL_SPI_TxRxCpltCallback(hspi); 
}

/* LOW LEVEL HELPERS */
void SD_Select(sd_handle_t *sd) { 
    if (sd == NULL) 
        return;
    HAL_GPIO_WritePin(sd->cs_port, sd->cs_pin, GPIO_PIN_RESET); 
}

void SD_Deselect(sd_handle_t *sd) { 
    if (sd == NULL) 
        return;
    HAL_GPIO_WritePin(sd->cs_port, sd->cs_pin, GPIO_PIN_SET); 
}

static void SD_SendDummyClocks(sd_handle_t *sd, TickType_t timeout) {
    SD_Deselect(sd);
    uint8_t tx[SD_DUMMY_CLOCKS_COUNT];
    memset(tx, SD_DUMMY_BYTE, SD_DUMMY_CLOCKS_COUNT);
    SD_SPI_TransferWait_IT(sd, tx, NULL, SD_DUMMY_CLOCKS_COUNT, timeout);
}

static sd_status_t SD_ReadBytes(sd_handle_t *sd, uint8_t *buff, size_t len, TickType_t timeout) {
    if (buff == NULL || len == 0) 
        return SD_ERROR;
    return SD_SPI_TransferWait_IT(sd, NULL, buff, len, timeout);
}

static uint8_t SD_ReadR1(sd_handle_t *sd, TickType_t timeout) {
    uint8_t r1;
    uint8_t tx = SD_DUMMY_BYTE;
    for(int i=0; i<8; i++) { 
        if (SD_SPI_TransferWait_IT(sd, &tx, &r1, 1, timeout) != SD_OK) 
            return 0xFF;
        if ((r1 & SD_R1_IDLE_MASK) == 0) 
            return r1; 
    }
    return SD_DUMMY_BYTE; 
}

static sd_status_t SD_WaitNotBusy(sd_handle_t *sd, TickType_t timeout) {
    uint8_t busy;
    for(int i=0; i<500; i++) {
        if(SD_ReadBytes(sd, &busy, 1, timeout) != SD_OK) 
            return SD_ERR_READ;
        if(busy == SD_DUMMY_BYTE) 
            return SD_OK; 
        vTaskDelay(pdMS_TO_TICKS(1)); 
    }
    return SD_TIMEOUT;
}

static sd_status_t SD_WaitDataToken(sd_handle_t *sd, uint8_t token, TickType_t timeout) { 
    uint8_t fb;
    uint8_t tx = SD_DUMMY_BYTE;  
    for (int i=0; i<1000; i++) { 
        if (SD_SPI_TransferWait_IT(sd, &tx, &fb, 1, timeout) != SD_OK) 
            return SD_ERROR;
        if (fb == token) 
            return SD_OK;
        if (fb != SD_DUMMY_BYTE) 
            return SD_ERROR; 
    }
    return SD_TIMEOUT;
}

static uint8_t SD_SendCommand(sd_handle_t *sd, uint8_t cmd, uint32_t arg, uint8_t crc, TickType_t timeout) {
    uint8_t packet[6];
    packet[0] = SD_CMD_BASE | (cmd & 0x3F);
    packet[1] = (arg >> 24) & 0xFF;
    packet[2] = (arg >> 16) & 0xFF;
    packet[3] = (arg >> 8) & 0xFF;
    packet[4] = arg & 0xFF;
    packet[5] = crc;

    SD_Select(sd);
    if (cmd != SD_CMD0) {
        if (SD_WaitNotBusy(sd, timeout) != SD_OK) {
            SD_Deselect(sd);
            return SD_DUMMY_BYTE;
        }
    }
    SD_SPI_TransferWait_IT(sd, packet, NULL, 6, timeout);
    return SD_ReadR1(sd, timeout);
}


sd_status_t SD_Init(sd_handle_t *sd, TickType_t timeout) {
    uint8_t res;
    uint8_t resp[4];

    SD_Deselect(sd);
    SD_SendDummyClocks(sd, timeout); 

    SD_Select(sd);
    uint8_t cmd0[] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
    SD_SPI_TransferWait_IT(sd, cmd0, NULL, 6, timeout);
    if(SD_ReadR1(sd, timeout) != 0x01) { 
        SD_Deselect(sd); 
        return SD_ERROR; 
    }
    SD_Deselect(sd);

    if(SD_SendCommand(sd, SD_CMD8, 0x000001AA, 0x87, timeout) != 0x01) { 
        SD_Deselect(sd); 
        return SD_ERR_VOLTAGE; 
    }
    if(SD_ReadBytes(sd, resp, 4, timeout) != SD_OK) { 
        SD_Deselect(sd); 
        return SD_ERR_READ; }

    // RTOS-safe timeout loop
    TickType_t start_tick = xTaskGetTickCount();
    for(;;) {
        if((xTaskGetTickCount() - start_tick) > pdMS_TO_TICKS(SD_DEFAULT_TIMEOUT_MS)) { 
            SD_Deselect(sd); 
            return SD_TIMEOUT; 
        }
        if(SD_SendCommand(sd, SD_CMD55, 0, 0x65, timeout) > 0x01) { 
            SD_Deselect(sd); 
            return SD_ERROR; 
        }
        SD_Deselect(sd);
        res = SD_SendCommand(sd, SD_ACMD41, 0x40000000, 0x77, timeout); 
        SD_Deselect(sd);
        if(res == 0x00) 
            break; 
        if(res > 0x01) 
            return SD_ERR_INIT_CARD; 
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    if(SD_SendCommand(sd, SD_CMD58, 0, 0xFD, timeout) != 0x00) { 
        SD_Deselect(sd); 
        return SD_ERROR; 
    }
    if(SD_ReadBytes(sd, resp, 4, timeout) != SD_OK) { 
        SD_Deselect(sd);
        return SD_ERR_READ; 
    }
    SD_Deselect(sd);

    if((resp[0] & SD_R1_IDLE_MASK) == 0) 
        return SD_ERROR;
    
    return SD_OK;
}

sd_status_t SD_ReadSingleBlock(sd_handle_t *sd, uint32_t blockNum, uint8_t *buffer, TickType_t timeout) {
    if (xSemaphoreTake(sd->mutex, portMAX_DELAY) != pdTRUE) 
        return SD_ERR_LOCK_TIMEOUT;
    if (SD_SendCommand(sd, SD_CMD17, blockNum, 0x01, timeout) != 0x00) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERROR; 
    }
    if (SD_WaitDataToken(sd, DATA_TOKEN_CMD18, timeout) != SD_OK) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERROR; 
    }
    if (SD_ReadBytes(sd, buffer, SD_BLOCK_SIZE, timeout) != SD_OK) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERR_READ; 
    }  
    uint8_t crc[2];
    SD_ReadBytes(sd, crc, 2, timeout);
    SD_Deselect(sd);
    xSemaphoreGive(sd->mutex);
    return SD_OK;
}

sd_status_t SD_WriteSingleBlock(sd_handle_t *sd, uint32_t blockNum, const uint8_t *buffer, TickType_t timeout) {
    if (xSemaphoreTake(sd->mutex, portMAX_DELAY) != pdTRUE) 
        return SD_ERR_LOCK_TIMEOUT;
    if (SD_SendCommand(sd, SD_CMD24, blockNum, 0x01, timeout) != 0x00) { 
        SD_Deselect(sd); 
        xSemaphoreGive(sd->mutex); 
        return SD_ERROR; 
    }
    
    uint8_t token = DATA_TOKEN_CMD18;
    SD_SPI_TransferWait_IT(sd, &token, NULL, 1, timeout);
    SD_SPI_TransferWait_IT(sd, (uint8_t*)buffer, NULL, SD_BLOCK_SIZE, timeout);
    uint8_t crc[] = {0xFF, 0xFF};
    SD_SPI_TransferWait_IT(sd, crc, NULL, 2, timeout);
    
    uint8_t dataResp;
    SD_ReadBytes(sd, &dataResp, 1, timeout);
    if ((dataResp & 0x1F) != 0x05) { SD_Deselect(sd); xSemaphoreGive(sd->mutex); return SD_ERR_WRITE; }
    if (SD_WaitNotBusy(sd, timeout) != SD_OK) { SD_Deselect(sd); xSemaphoreGive(sd->mutex); return SD_TIMEOUT; }
    
    SD_Deselect(sd);
    xSemaphoreGive(sd->mutex);
    return SD_OK;
}

/* USER FUNCTIONS */
sd_status_t USER_SD_Card_Init(sd_handle_t *sd) {
    sd->mutex = xSemaphoreCreateMutexStatic(&sd->mutexBuffer);
    sd->spi_comp_sem = xSemaphoreCreateBinaryStatic(&sd->spi_comp_buffer);
    sd->job_queue = xQueueCreateStatic(SD_QUEUE_LENGTH, sizeof(sd_job_t), sd->queue_storage, &sd->queue_buffer);

    if (sd->mutex == NULL || sd->spi_comp_sem == NULL || sd->job_queue == NULL) 
        return SD_ERR_MUTEX;
    
    MX_FATFS_Init();
    return SD_OK;
}

sd_status_t USER_SD_Card_Write_Async(sd_handle_t *sd, const char *filename, const char *data, TickType_t delay_ticks) {
    if (sd->job_queue == NULL) 
        return SD_ERR_SPI;

    sd_job_t job;
    job.type = SD_JOB_WRITE_ASYNC;
    
    strncpy(job.filename, filename, SD_MAX_FILENAME_LEN);
    job.filename[SD_MAX_FILENAME_LEN-1] = '\0';

    uint32_t len = strlen(data);
    if (len >= SD_DATA_BUFFER_LEN) 
        len = SD_DATA_BUFFER_LEN - 1; 
    job.len = len;

    strncpy(job.data, data, SD_DATA_BUFFER_LEN);
    job.data[SD_DATA_BUFFER_LEN-1] = '\0'; 

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
    
    while (SD_Init(sd, pdMS_TO_TICKS(1000)) != SD_OK) { vTaskDelay(pdMS_TO_TICKS(500)); }

    f_mount(&fs, "", 1); 

    for(;;) {
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