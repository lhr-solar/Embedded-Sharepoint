#include "sdcard.h"
#include "stm32l4xx_hal.h"
#include <stdint.h>

extern SPI_HandleTypeDef hspi2; 

#define CMD0 0   // Reset
#define CMD17 17 // Read single block
#define CMD24 24 // Write single block

uint8_t SD_SPI_Init(sd_handle_t *sd) {

    /*Creates a structure to configure GPIO pins.*/
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable peripheral clocks */
    __HAL_RCC_SPI2_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE(); //?


    /** SPI2 GPIO Configuration
     *  PB10 ------> SPI2_SCK
     *  PC2  ------> SPI2_MISO
     *  PC3  ------> SPI2_MOSI
     */

    //SPI2_MISO: PC2   SPI2_MOSI: PC3
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    //SPI2_SCK:  PB10
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* --- CS pin setup --- */
    HAL_GPIO_WritePin(sd->cs_port, sd->cs_pin, GPIO_PIN_SET); // CS high (inactive)
    GPIO_InitStruct.Pin = sd->cs_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(sd->cs_port, &GPIO_InitStruct);

    /* --- SPI peripheral configuration --- */
    sd->hspi->Instance = SPI2;
    sd->hspi->Init.Mode = SPI_MODE_MASTER;
    sd->hspi->Init.Direction = SPI_DIRECTION_2LINES;
    sd->hspi->Init.DataSize = SPI_DATASIZE_8BIT;
    sd->hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
    sd->hspi->Init.CLKPhase = SPI_PHASE_1EDGE;
    sd->hspi->Init.NSS = SPI_NSS_SOFT;
    sd->hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16; //before: 16
    sd->hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
    sd->hspi->Init.TIMode = SPI_TIMODE_DISABLE;
    sd->hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    sd->hspi->Init.CRCPolynomial = 7;
    sd->hspi->Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    sd->hspi->Init.NSSPMode = SPI_NSS_PULSE_DISABLE;

    if (HAL_SPI_Init(sd->hspi) != HAL_OK) { // starts the SPI hardware with the config above.
        return 1; // failed
    }
    return 0; // success
}

uint8_t SD_Transmit(sd_handle_t *sd, uint8_t data) {
    // uint8_t (1 byte) received in datat;
    // recieve 1 byte from SD card
    // return received;

    uint8_t received = 0;
    HAL_SPI_TransmitReceive(sd->hspi, &data, &received, 1, HAL_MAX_DELAY);
    return received;
}

void SD_SendDummyClocks(sd_handle_t *sd) { //wakes up sd card
    SD_Deselect(sd);
    for (int i = 0; i < 10; i++) SD_Transmit(sd, 0xFF);
}

uint8_t SD_SendCommand(sd_handle_t *sd, uint8_t cmd, uint32_t arg, uint8_t crc) {
    
    SD_Deselect(sd);
    SD_Transmit(sd, 0xFF); //use 0xFF to generate 8 SPI clock cycles.
    SD_Select(sd);

    SD_Transmit(sd, 0x40 | cmd); //argument command 4 byte
    SD_Transmit(sd, arg >> 24);
    SD_Transmit(sd, arg >> 16);
    SD_Transmit(sd, arg >> 8);
    SD_Transmit(sd, arg);
    SD_Transmit(sd, crc);

    uint8_t response;
    for (int i = 0; i < 8; i++) {
        response = SD_Transmit(sd, 0xFF);
        if (!(response & 0x80)) break;
    }
    return response;
}

uint8_t SD_Init(sd_handle_t *sd) {
    SD_SendDummyClocks(sd);

    uint8_t response;
    int timeout = 0xFFFF;

    // CMD0: go to idle state
    do {
        response = SD_SendCommand(sd, 0, 0, 0x95);
    } while (response != 0x01 && --timeout);

    return (timeout == 0) ? 1 : 0; // 0=OK, 1=Fail
}


// // Pull CS low to select SD card
void SD_Select(sd_handle_t *sd) {
    HAL_GPIO_WritePin(sd->cs_port, sd->cs_pin, GPIO_PIN_RESET); // LOW = active
}

// // Pull CS high to deselect SD card
void SD_Deselect(sd_handle_t *sd) {
    HAL_GPIO_WritePin(sd->cs_port, sd->cs_pin, GPIO_PIN_SET);   // HIGH = inactive
}

uint8_t SD_ReadSector(sd_handle_t *sd, uint32_t sector, uint8_t *buffer) {
    if (SD_SendCommand(sd, CMD17, sector*512, 0x01) != 0x00) return 1;

    while (SD_Transmit(sd, 0xFF) != 0xFE);

    for (int i = 0; i < 512; i++) buffer[i] = SD_Transmit(sd, 0xFF);

    SD_Transmit(sd, 0xFF);
    SD_Transmit(sd, 0xFF);

    return 0;
}

uint8_t SD_WriteSector(sd_handle_t *sd, uint32_t sector, const uint8_t *buffer) {
    if (SD_SendCommand(sd, CMD24, sector*512, 0x01) != 0x00) return 1;

    SD_Transmit(sd, 0xFE);

    for (int i = 0; i < 512; i++) SD_Transmit(sd, buffer[i]);

    SD_Transmit(sd, 0xFF);
    SD_Transmit(sd, 0xFF);

    uint8_t response = SD_Transmit(sd, 0xFF);
    if ((response & 0x1F) != 0x05) return 2;

    while (SD_Transmit(sd, 0xFF) == 0);

    return 0;
}
