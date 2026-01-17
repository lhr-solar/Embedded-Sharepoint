#include "sdcard.h"
#include <stdint.h>
#include "stm32xx_hal.h"

extern SPI_HandleTypeDef hspi2; 

#define CMD0 0   // Reset
#define CMD17 17 // Read single block
#define CMD24 24 // Write single block
#define DATA_TOKEN_CMD9  0xFE
#define DATA_TOKEN_CMD17 0xFE
#define DATA_TOKEN_CMD18 0xFE
#define DATA_TOKEN_CMD24 0xFE
#define DATA_TOKEN_CMD25 0xFC

/* CS */
void SD_Select(sd_handle_t *sd)
{
    HAL_GPIO_WritePin(sd->cs_port, sd->cs_pin, GPIO_PIN_RESET);
}

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
uint8_t SD_ReadR1(sd_handle_t *sd)
{
    uint8_t r1;
    // make sure FF is transmitted during receive
    uint8_t tx = 0xFF;
    while (1) {
        HAL_SPI_TransmitReceive(sd->hspi, &tx, &r1, 1, HAL_MAX_DELAY);
        if ((r1 & 0x80) == 0)     // MSB must be 0 → valid R1
            return r1;
    }
}

/* basic byte tx/rx (no CS handling) */
uint8_t SD_SPI_TxRx(sd_handle_t *sd, uint8_t tx)
{
    uint8_t rx = 0xFF;
    HAL_SPI_TransmitReceive(sd->hspi, &tx, &rx, 1, HAL_MAX_DELAY);
    return rx;
}

uint8_t SD_SPI_Init(sd_handle_t *sd) {

    /*Creates a structure to configure GPIO pins.*/
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable peripheral clocks */
    __HAL_RCC_SPI2_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE(); 

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
    sd->hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; 
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

/* ---------- Dummy clocks ---------- */
void SD_SendDummyClocks(sd_handle_t *sd)
{
    /* Ensure CS high and send >= 80 clocks (10 bytes of 0xFF) */
    SD_Deselect(sd);

    // uint8_t high = 0xFF;
    // for(int i = 0; i < 10; i++) { // 80 clock pulses
    //     HAL_SPI_Transmit(sd->hspi, &high, sizeof(high), HAL_MAX_DELAY);
    // }
    uint8_t tx = 0xFF;
    uint8_t rx;
    for (int i = 0; i < 10; i++) {
        HAL_SPI_TransmitReceive(sd->hspi, &tx, &rx, sizeof(tx), HAL_MAX_DELAY);
    }
    
}

/* ---------- read N bytes by sending 0xFF ---------- */
int SD_ReadBytes(sd_handle_t *sd, uint8_t *buff, size_t len)
{
    uint8_t tx = 0xFF; 
    while (len--) {
        if (HAL_SPI_TransmitReceive(sd->hspi, &tx, buff, 1, HAL_MAX_DELAY) != HAL_OK) return -1;
        ++buff;
    }
    return 0;
}

/* ---------- wait for not-busy (card returns 0xFF when ready) ---------- */
int SD_WaitNotBusy(sd_handle_t *sd)
{
    // uint8_t v;
    // uint8_t tx = 0xFF;
    // for (uint32_t t = 50000; t > 0; t--) {

    //     HAL_StatusTypeDef status =
    //         HAL_SPI_TransmitReceive(sd->hspi, &tx, &v, 1, HAL_MAX_DELAY);

    //     if (status != HAL_OK)
    //         return -1;

    //     if (v == 0xFF)
    //         return 0;       // not busy
    // }

    // return -1;              // timeout
    uint8_t busy;
    //uint8_t cnt;
    do {
        if(SD_ReadBytes(sd, &busy, sizeof(busy)) < 0) {
            return -1;
        }
        // cnt++;
        // if (cnt>200){
        //     return -1;
        // }        
    } while(busy != 0xFF);

    return 0;
}

/* ---------- wait for a data token (e.g. 0xFE) ---------- */
int SD_WaitDataToken(sd_handle_t *sd, uint8_t token) { 
    uint8_t fb;
    uint8_t tx = 0xFF;   // send dummy clocks during read

    for (;;) {
        HAL_SPI_TransmitReceive(sd->hspi, &tx, &fb, 1, HAL_MAX_DELAY);

        if (fb == token)
            break;          // found correct data token

        if (fb != 0xFF)
            return -1;      // unexpected token → error
    }

    return 0;
}


/* ---------- send a command packet and return R1 ---------- */
uint8_t SD_SendCommand(sd_handle_t *sd, uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t packet[6];
    packet[0] = 0x40 | (cmd & 0x3F);
    packet[1] = (arg >> 24) & 0xFF;
    packet[2] = (arg >> 16) & 0xFF;
    packet[3] = (arg >> 8) & 0xFF;
    packet[4] = arg & 0xFF;
    packet[5] = crc;

    SD_Select(sd);

    /* --- FIX: Do NOT wait for busy if sending CMD0 (Reset) --- */
    if (cmd != 0) {
        if (SD_WaitNotBusy(sd) < 0) {
            SD_Deselect(sd);
            return 0xFF;
        }
    }
    /* --------------------------------------------------------- */

    /* Send the 6-byte command */
    HAL_SPI_Transmit(sd->hspi, packet, sizeof(packet), HAL_MAX_DELAY);

    /* Read R1 */
    uint8_t r1 = SD_ReadR1(sd);

    return r1;
}

// // SD INIT require:
// /*
// 1. to enter SPI mode, the host must send ≥74 dummy clocks with CS = high
// 2. Send CMD0, puts the SD card into idle state (Expected response = 0x01 = idle)
// 3. Send CMD8 (CHECK VOLTAGE / VERSION); send argument 0x000001AA and expect the same pattern back.
// 4. Loop sending ACMD41 until the card leaves idle
// Send CMD55 (prefix command), Send ACMD41 (init command) until card returns: 0x00 meaning: “Initialization complete. Ready to work.”
// 5. Send CMD58 to read Operating Conditions Register (supported voltage)
// */



int SD_Init(sd_handle_t *sd) {
    uint8_t res;
    uint8_t resp[4];

    // --- Step 1: Dummy Clocks ---
    SD_Deselect(sd);
    SD_SendDummyClocks(sd); 

    // --- Step 2: CMD0 (Go Idle) ---
    SD_Select(sd);
    uint8_t cmd0[] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
    HAL_SPI_Transmit(sd->hspi, cmd0, sizeof(cmd0), HAL_MAX_DELAY);
    if(SD_ReadR1(sd) != 0x01) {
        SD_Deselect(sd);
        return -1; 
    }
    SD_Deselect(sd);

    // --- Step 3: CMD8 (Voltage Check) ---
    uint8_t r1 = SD_SendCommand(sd, 8, 0x000001AA, 0x87); // FIX 1: Capture return
    if(r1 != 0x01) {
        SD_Deselect(sd);
        return -2;
    }
    if(SD_ReadBytes(sd, resp, sizeof(resp)) < 0) {
        SD_Deselect(sd);
        return -3; 
    }

    // --- Step 4: ACMD41 Loop (Initialize) ---
    uint32_t tickStart = HAL_GetTick();
    
    for(;;) {
        if((HAL_GetTick() - tickStart) > 1000) {
             SD_Deselect(sd);
             return -10;
        }

        // --- FIX 2: Capture return directly (Do not call ReadR1 again) ---
        // Send CMD55
        res = SD_SendCommand(sd, 55, 0, 0x65); 
        if(res > 0x01) { 
            SD_Deselect(sd);
            return -5; 
        }
        SD_Deselect(sd);

        // Send ACMD41
        res = SD_SendCommand(sd, 41, 0x40000000, 0x77); 
        SD_Deselect(sd);
        
        if(res == 0x00) break; // Success!
        if(res > 0x01) return -6; // Error
        
        HAL_Delay(10);
    }

    // --- Step 5: CMD58 (Read OCR) ---
    res = SD_SendCommand(sd, 58, 0, 0xFD);
    if(res != 0x00) {
        SD_Deselect(sd);
        return -7;
    }

    if(SD_ReadBytes(sd, resp, sizeof(resp)) < 0) {
        SD_Deselect(sd);
        return -8;
    }
    SD_Deselect(sd);

    // Check Power Up bit (Bit 31) only
    if((resp[0] & 0x80) == 0) {
        return -9;
    }
    
    return 0; // Success
}

/* ---------- read / write block helpers (512 bytes) ---------- */
int SD_ReadSingleBlock(sd_handle_t *sd, uint32_t blockNum, uint8_t *buffer)
{
    uint8_t resp;
    resp = SD_SendCommand(sd, 17, blockNum, 0x01); /* CMD17 */
    if (resp != 0x00) { SD_Deselect(sd); return -1; }

    if (SD_WaitDataToken(sd, 0xFE) < 0) { SD_Deselect(sd); return -2; }
    if (SD_ReadBytes(sd, buffer, 512) < 0) { SD_Deselect(sd); return -3; }

    /* read CRC (2 bytes) */
    uint8_t crc[2];
    if (SD_ReadBytes(sd, crc, 2) < 0) { SD_Deselect(sd); return -4; }

    SD_Deselect(sd);
    return 0;
}

int SD_WriteSingleBlock(sd_handle_t *sd, uint32_t blockNum, const uint8_t *buffer)
{
    uint8_t resp;
    resp = SD_SendCommand(sd, 24, blockNum, 0x01); /* CMD24 */
    if (resp != 0x00) { SD_Deselect(sd); return -1; }

    uint8_t token = 0xFE;
    HAL_SPI_Transmit(sd->hspi, &token, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(sd->hspi, (uint8_t*)buffer, 512, HAL_MAX_DELAY);

    uint8_t crc[2] = {0xFF, 0xFF};
    HAL_SPI_Transmit(sd->hspi, crc, 2, HAL_MAX_DELAY);

    uint8_t dataResp;
    if (SD_ReadBytes(sd, &dataResp, 1) < 0) { SD_Deselect(sd); return -2; }
    if ((dataResp & 0x1F) != 0x05) { SD_Deselect(sd); return -3; }

    if (SD_WaitNotBusy(sd) < 0) { SD_Deselect(sd); return -4; }

    SD_Deselect(sd);
    return 0;
}



int SD_ReadBegin(sd_handle_t *sd, uint32_t blockNum) {
    SD_Select(sd);

    if(SD_WaitNotBusy(sd) < 0) { // keep this!
        SD_Deselect(sd);
        return -1;
    }

    /* CMD18 (READ_MULTIPLE_BLOCK) command */
    uint8_t cmd[] = {
        0x40 | 0x12 /* CMD18 */,
        (blockNum >> 24) & 0xFF, /* ARG */
        (blockNum >> 16) & 0xFF,
        (blockNum >> 8) & 0xFF,
        blockNum & 0xFF,
        (0x7F << 1) | 1 /* CRC7 + end bit */
    };
    HAL_SPI_Transmit(sd->hspi, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);

    if(SD_ReadR1(sd) != 0x00) {
        SD_Deselect(sd);
        return -2;
    }

    SD_Deselect(sd);
    return 0;
}

int SD_ReadData(sd_handle_t *sd, uint8_t* buff) {
    uint8_t crc[2];
    SD_Select(sd);

    if(SD_WaitDataToken(sd, DATA_TOKEN_CMD18) < 0) {
        SD_Deselect(sd);
        return -1;
    }

    if(SD_ReadBytes(sd, buff, 512) < 0) {
        SD_Deselect(sd);
        return -2;
    }

    if(SD_ReadBytes(sd, crc, 2) < 0) {
        SD_Deselect(sd);
        return -3;
    }

    SD_Deselect(sd);
    return 0;

}

int SD_ReadEnd(sd_handle_t *sd) {
    SD_Select(sd);

    /* CMD12 (STOP_TRANSMISSION) */
    {
        static const uint8_t cmd[] = { 0x40 | 0x0C /* CMD12 */, 0x00, 0x00, 0x00, 0x00 /* ARG */, (0x7F << 1) | 1 };
        HAL_SPI_Transmit(sd->hspi, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);
    }

    uint8_t stuffByte;
    if(SD_ReadBytes(sd, &stuffByte, sizeof(stuffByte)) < 0) {
        SD_Deselect(sd);
        return -1;
    }

    if(SD_ReadR1(sd) != 0x00) {
        SD_Deselect(sd);
        return -2;
    }
    
    SD_Deselect(sd);
    return 0;
}


int SD_WriteBegin(sd_handle_t *sd, uint32_t blockNum) {
    SD_Select(sd);

    if(SD_WaitNotBusy(sd) < 0) { // keep this!
        SD_Deselect(sd);
        return -1;
    }

    /* CMD25 (WRITE_MULTIPLE_BLOCK) command */
    uint8_t cmd[] = {
        0x40 | 0x19 /* CMD25 */,
        (blockNum >> 24) & 0xFF, /* ARG */
        (blockNum >> 16) & 0xFF,
        (blockNum >> 8) & 0xFF,
        blockNum & 0xFF,
        (0x7F << 1) | 1 /* CRC7 + end bit */
    };
    HAL_SPI_Transmit(sd->hspi, (uint8_t*)cmd, sizeof(cmd), HAL_MAX_DELAY);

    if(SD_ReadR1(sd) != 0x00) {
        SD_Deselect(sd);
        return -2;
    }

    SD_Deselect(sd);
    return 0;
}

int SD_WriteData(sd_handle_t *sd, const uint8_t* buff) {
    SD_Select(sd);

    uint8_t dataToken = DATA_TOKEN_CMD25;
    uint8_t crc[2] = { 0xFF, 0xFF };
    HAL_SPI_Transmit(sd->hspi, &dataToken, sizeof(dataToken), HAL_MAX_DELAY);
    HAL_SPI_Transmit(sd->hspi, (uint8_t*)buff, 512, HAL_MAX_DELAY);
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
        return -1;
    }

    if(SD_WaitNotBusy(sd) < 0) {
        SD_Deselect(sd);
        return -2;
    }

    SD_Deselect(sd);
    return 0;
}

int SD_WriteEnd(sd_handle_t *sd) {
    SD_Select(sd);

    uint8_t stopTran = 0xFD; // stop transaction token for CMD25
    HAL_SPI_Transmit(sd->hspi, &stopTran, sizeof(stopTran), HAL_MAX_DELAY);

    // skip one byte before readyng "busy"
    uint8_t skipByte;
    SD_ReadBytes(sd, &skipByte, sizeof(skipByte));

    if(SD_WaitNotBusy(sd) < 0) {
        SD_Deselect(sd);
        return -1;
    }

    SD_Deselect(sd);
    return 0;
}