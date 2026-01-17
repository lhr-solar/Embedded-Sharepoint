#ifndef SDCARD_H
#define SDCARD_H

#include "stm32xx_hal.h"
#include <stdint.h>
#include <stddef.h>

/* --- SD Card Command Definitions --- */
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

typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
} sd_handle_t;

/* Low-level helpers */
uint8_t SD_SPI_Init(sd_handle_t *sd);
void SD_SendDummyClocks(sd_handle_t *sd);

int8_t SD_WaitNotBusy(sd_handle_t *sd);
int8_t SD_WaitDataToken(sd_handle_t *sd, uint8_t token);
int8_t SD_ReadBytes(sd_handle_t *sd, uint8_t *buff, size_t len);
uint8_t SD_ReadR1(sd_handle_t *sd);

void SD_Select(sd_handle_t *sd);
void SD_Deselect(sd_handle_t *sd);

/* Command layer */
uint8_t SD_SendCommand(sd_handle_t *sd, uint8_t cmd, uint32_t arg, uint8_t crc);

/* High-level operations */
int8_t SD_Init(sd_handle_t *sd);
int8_t SD_ReadSingleBlock(sd_handle_t *sd, uint32_t blockNum, uint8_t *buffer);
int8_t SD_WriteSingleBlock(sd_handle_t *sd, uint32_t blockNum, const uint8_t *buffer);


uint8_t SD_ReadSector(sd_handle_t *sd, uint32_t sector, uint8_t *buffer);
uint8_t SD_WriteSector(sd_handle_t *sd, uint32_t sector, const uint8_t *buffer);

// Read Multiple Blocks
int8_t SD_ReadBegin(sd_handle_t *sd, uint32_t blockNum);
int8_t SD_ReadData(sd_handle_t *sd, uint8_t* buff); // sizeof(buff) == 512!
int8_t SD_ReadEnd(sd_handle_t *sd);

// Write Multiple Blocks
int8_t SD_WriteBegin(sd_handle_t *sd, uint32_t blockNum);
int8_t SD_WriteData(sd_handle_t *sd, const uint8_t* buff); // sizeof(buff) == 512!
int8_t SD_WriteEnd(sd_handle_t *sd);


#endif /* SDCARD_H */

