#ifndef SDCARD_H
#define SDCARD_H

#include "stm32xx_hal.h"
#include <stdint.h>
#include <stddef.h>

typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
} sd_handle_t;

/* Low-level helpers */
uint8_t SD_SPI_Init(sd_handle_t *sd);
void SD_SendDummyClocks(sd_handle_t *sd);
uint8_t SD_SPI_TxRx(sd_handle_t *sd, uint8_t tx);

int SD_WaitNotBusy(sd_handle_t *sd);
int SD_WaitDataToken(sd_handle_t *sd, uint8_t token);
int SD_ReadBytes(sd_handle_t *sd, uint8_t *buff, size_t len);
uint8_t SD_ReadR1(sd_handle_t *sd);

void SD_Select(sd_handle_t *sd);
void SD_Deselect(sd_handle_t *sd);

/* Command layer */
uint8_t SD_SendCommand(sd_handle_t *sd, uint8_t cmd, uint32_t arg, uint8_t crc);

/* High-level operations */
int SD_Init(sd_handle_t *sd);
int SD_ReadSingleBlock(sd_handle_t *sd, uint32_t blockNum, uint8_t *buffer);
int SD_WriteSingleBlock(sd_handle_t *sd, uint32_t blockNum, const uint8_t *buffer);


uint8_t SD_ReadSector(sd_handle_t *sd, uint32_t sector, uint8_t *buffer);
uint8_t SD_WriteSector(sd_handle_t *sd, uint32_t sector, const uint8_t *buffer);

// Read Multiple Blocks
int SD_ReadBegin(sd_handle_t *sd, uint32_t blockNum);
int SD_ReadData(sd_handle_t *sd, uint8_t* buff); // sizeof(buff) == 512!
int SD_ReadEnd(sd_handle_t *sd);

// Write Multiple Blocks
int SD_WriteBegin(sd_handle_t *sd, uint32_t blockNum);
int SD_WriteData(sd_handle_t *sd, const uint8_t* buff); // sizeof(buff) == 512!
int SD_WriteEnd(sd_handle_t *sd);


#endif /* SDCARD_H */

