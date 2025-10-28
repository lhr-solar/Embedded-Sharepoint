#include "diskio.h"
#include "sdcard.h"
#include "stm32l4xx_hal.h"

// External SPI handle defined elsewhere (CubeMX auto-generates it)
extern SPI_HandleTypeDef hspi2;

// Create an sd_handle_t instance to store CS pin and SPI config
static sd_handle_t hsd = {
    .hspi = &hspi2,
    .cs_port = GPIOB,      // change this if your CS pin is not on GPIOB
    .cs_pin = GPIO_PIN_12  // change this to match your actual SD_CS pin
};

// Disk status
static DSTATUS Stat = STA_NOINIT;

// ---------------------------------------------------------------------------
// Required FatFs low-level disk I/O functions
// ---------------------------------------------------------------------------

// Initialize SD card
DSTATUS disk_initialize(BYTE pdrv) {
    if (pdrv != 0) return STA_NOINIT;  // Only support drive 0

    if (SD_SPI_Init(&hsd) != 0) return STA_NOINIT;
    if (SD_Init(&hsd) != 0) return STA_NOINIT;

    Stat &= ~STA_NOINIT;
    return Stat;
}

// Get current status
DSTATUS disk_status(BYTE pdrv) {
    if (pdrv != 0) return STA_NOINIT;
    return Stat;
}

// Read one or more sectors
DRESULT disk_read(BYTE pdrv, BYTE* buff, LBA_t sector, UINT count) {
    if (pdrv != 0 || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    for (UINT i = 0; i < count; i++) {
        if (SD_ReadSector(&hsd, sector + i, buff + (512 * i)) != 0)
            return RES_ERROR;
    }
    return RES_OK;
}

// Write one or more sectors
#if _USE_WRITE
DRESULT disk_write(BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count) {
    if (pdrv != 0 || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    for (UINT i = 0; i < count; i++) {
        if (SD_WriteSector(&hsd, sector + i, buff + (512 * i)) != 0)
            return RES_ERROR;
    }
    return RES_OK;
}
#endif

// Optional control commands
#if _USE_IOCTL
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void* buff) {
    if (pdrv != 0) return RES_PARERR;
    // Minimal stub (FatFs rarely needs these)
    switch (cmd) {
        case CTRL_SYNC:
            return RES_OK;
        case GET_SECTOR_COUNT:
        case GET_SECTOR_SIZE:
        case GET_BLOCK_SIZE:
            return RES_OK;
    }
    return RES_PARERR;
}
#endif
