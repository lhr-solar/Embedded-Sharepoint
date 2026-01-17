#include "sdcard.h"
#include <stdint.h>
#include "stm32xx_hal.h"

extern SPI_HandleTypeDef hspi2; 


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
    uint8_t tx = SD_DUMMY_BYTE;
    while (1) {
        HAL_SPI_TransmitReceive(sd->hspi, &tx, &r1, 1, HAL_MAX_DELAY);
        if ((r1 & 0x80) == 0)     // MSB must be 0 → valid R1
            return r1;
    }
}

uint8_t SD_SPI_Init(sd_handle_t *sd) {

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 1. Enable Clocks (Uses the Macros from sdcard.h) */
    SD_SPI_CLK_ENABLE();       // Enables SPI1 or SPI2
    SD_GPIO_CLK_ENABLE();      // Enables GPIOA or (GPIOB & GPIOC)

    /* 2. Configure SCK Pin */
    GPIO_InitStruct.Pin = SD_SCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = SD_SPI_AF;
    HAL_GPIO_Init(SD_SCK_PORT, &GPIO_InitStruct);

    /* 3. Configure MISO Pin */
    GPIO_InitStruct.Pin = SD_MISO_PIN;
    HAL_GPIO_Init(SD_MISO_PORT, &GPIO_InitStruct);

    /* 4. Configure MOSI Pin */
    GPIO_InitStruct.Pin = SD_MOSI_PIN;
    HAL_GPIO_Init(SD_MOSI_PORT, &GPIO_InitStruct);

    /* 5. Configure CS Pin (Dynamic based on struct) */
    // Ensure the specific CS port clock is on (Safety check)
    if(sd->cs_port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if(sd->cs_port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if(sd->cs_port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
    
    HAL_GPIO_WritePin(sd->cs_port, sd->cs_pin, GPIO_PIN_SET); // CS High (Inactive)
    GPIO_InitStruct.Pin = sd->cs_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = 0; // No AF for CS
    HAL_GPIO_Init(sd->cs_port, &GPIO_InitStruct);

    /* 6. Configure SPI Peripheral */
    sd->hspi->Instance = SD_SPI_HANDLE;  // SPI1 or SPI2
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

    if (HAL_SPI_Init(sd->hspi) != HAL_OK) { 
        return 1; // Failed
    }
    return 0; // Success
}

/* ---------- Dummy clocks ---------- */
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

/* ---------- read N bytes by sending 0xFF ---------- */
int8_t SD_ReadBytes(sd_handle_t *sd, uint8_t *buff, size_t len)
{
    /* --- 1. Sanity Checks (Requested by Lakshay983) --- */
    if (buff == NULL) {
        return -1; // Error: Buffer is null
    }
    
    if (len == 0) {
        return -1; // Error: Length is zero
    }

    /* --- 2. Main Logic --- */
    uint8_t tx = SD_DUMMY_BYTE; // Using the macro we just made
    
    while (len--) {
        if (HAL_SPI_TransmitReceive(sd->hspi, &tx, buff, 1, HAL_MAX_DELAY) != HAL_OK) {
            return -1; // Error: SPI failure
        }
        buff++;
    }

    return 0; // Success

}

/* ---------- wait for not-busy (card returns 0xFF when ready) ---------- */
int8_t SD_WaitNotBusy(sd_handle_t *sd)
{
    uint8_t busy;
    do {
        if(SD_ReadBytes(sd, &busy, sizeof(busy)) < 0) {
            return -1;
        }
     
    } while(busy != SD_DUMMY_BYTE);

    return 0;
}

/* ---------- wait for a data token (e.g. 0xFE) ---------- */
int8_t SD_WaitDataToken(sd_handle_t *sd, uint8_t token) { 
    uint8_t fb;
    uint8_t tx = SD_DUMMY_BYTE;   // send dummy clocks during read

    for (;;) {
        HAL_SPI_TransmitReceive(sd->hspi, &tx, &fb, 1, HAL_MAX_DELAY);

        if (fb == token)
            break;          // found correct data token

        if (fb != SD_DUMMY_BYTE)
            return -1;      // unexpected token → error
    }

    return 0;
}

/* ---------- send a command packet and return R1 ---------- */
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



int8_t SD_Init(sd_handle_t *sd) {
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
        return -1; 
    }
    SD_Deselect(sd);

    // Step 3: CMD8 (Voltage Check) 
    uint8_t r1 = SD_SendCommand(sd, 8, 0x000001AA, 0x87); // Capture return
    if(r1 != 0x01) {
        SD_Deselect(sd);
        return -2;
    }
    if(SD_ReadBytes(sd, resp, sizeof(resp)) < 0) {
        SD_Deselect(sd);
        return -3; 
    }

    // Step 4: ACMD41 Loop (Initialize) 
    uint32_t tickStart = HAL_GetTick();
    
    for(;;) {
        if((HAL_GetTick() - tickStart) > 1000) {
             SD_Deselect(sd);
             return -10;
        }

        // Capture return directly
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

    // Step 5: CMD58 (Read OCR) 
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

/* read / write block helpers (512 bytes) */
int8_t SD_ReadSingleBlock(sd_handle_t *sd, uint32_t blockNum, uint8_t *buffer)
{
    uint8_t resp;
    resp = SD_SendCommand(sd, SD_CMD17, blockNum, 0x01); /* CMD17 */
    if (resp != 0x00) { SD_Deselect(sd); return -1; }

    if (SD_WaitDataToken(sd, 0xFE) < 0) { SD_Deselect(sd); return -2; }
    if (SD_ReadBytes(sd, buffer, SD_BLOCK_SIZE) < 0) { SD_Deselect(sd); return -3; }

    /* read CRC (2 bytes) */
    uint8_t crc[2];
    if (SD_ReadBytes(sd, crc, 2) < 0) { SD_Deselect(sd); return -4; }

    SD_Deselect(sd);
    return 0;
}

int8_t SD_WriteSingleBlock(sd_handle_t *sd, uint32_t blockNum, const uint8_t *buffer)
{
    uint8_t resp;
    resp = SD_SendCommand(sd, SD_CMD24, blockNum, 0x01); /* CMD24 */
    if (resp != 0x00) { SD_Deselect(sd); return -1; }

    uint8_t token = 0xFE;
    HAL_SPI_Transmit(sd->hspi, &token, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(sd->hspi, (uint8_t*)buffer, SD_BLOCK_SIZE, HAL_MAX_DELAY);

    uint8_t crc[2] = {0xFF, 0xFF};
    HAL_SPI_Transmit(sd->hspi, crc, 2, HAL_MAX_DELAY);

    uint8_t dataResp;
    if (SD_ReadBytes(sd, &dataResp, 1) < 0) { SD_Deselect(sd); return -2; }
    if ((dataResp & 0x1F) != 0x05) { SD_Deselect(sd); return -3; }

    if (SD_WaitNotBusy(sd) < 0) { SD_Deselect(sd); return -4; }

    SD_Deselect(sd);
    return 0;
}



int8_t SD_ReadBegin(sd_handle_t *sd, uint32_t blockNum) {
    SD_Select(sd);

    if(SD_WaitNotBusy(sd) < 0) { // keep this!
        SD_Deselect(sd);
        return -1;
    }

    /* CMD18 (READ_MULTIPLE_BLOCK) command */
    uint8_t cmd[] = {
        SD_CMD_BASE | SD_CMD18 /* CMD18 */,
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

int8_t SD_ReadData(sd_handle_t *sd, uint8_t* buff) {
    uint8_t crc[2];
    SD_Select(sd);

    if(SD_WaitDataToken(sd, DATA_TOKEN_CMD18) < 0) {
        SD_Deselect(sd);
        return -1;
    }

    if(SD_ReadBytes(sd, buff, SD_BLOCK_SIZE) < 0) {
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

int8_t SD_ReadEnd(sd_handle_t *sd) {
    SD_Select(sd);

    /* CMD12 (STOP_TRANSMISSION) */
    {
        static const uint8_t cmd[] = { SD_CMD_BASE | SD_CMD12 /* CMD12 */, 0x00, 0x00, 0x00, 0x00 /* ARG */, (0x7F << 1) | 1 };
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


int8_t SD_WriteBegin(sd_handle_t *sd, uint32_t blockNum) {
    SD_Select(sd);

    if(SD_WaitNotBusy(sd) < 0) { 
        SD_Deselect(sd);
        return -1;
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
        return -2;
    }

    SD_Deselect(sd);
    return 0;
}

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
        return -1;
    }

    if(SD_WaitNotBusy(sd) < 0) {
        SD_Deselect(sd);
        return -2;
    }

    SD_Deselect(sd);
    return 0;
}

int8_t SD_WriteEnd(sd_handle_t *sd) {
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