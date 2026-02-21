#include <string.h>
#include "ff_gen_drv.h"
#include "sdcard.h"

extern sd_handle_t sd;

/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* Private function prototypes */
DSTATUS USER_initialize (BYTE pdrv);
DSTATUS USER_status (BYTE pdrv);
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
  DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif 
#if _USE_IOCTL == 1
  DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif 

Diskio_drvTypeDef  USER_Driver =
{
  USER_initialize,
  USER_status,
  USER_read,
#if  _USE_WRITE
  USER_write,
#endif  
#if  _USE_IOCTL == 1
  USER_ioctl,
#endif 
};

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_initialize (BYTE pdrv)
{
    // FIX: Added pdMS_TO_TICKS(1000) timeout parameter
    if(SD_Init(&sd, pdMS_TO_TICKS(1000)) != SD_OK) {
        Stat = STA_NOINIT; 
        return Stat;
    }

    Stat = 0; // Mark drive as "Ready"
    return Stat;
}

/**
  * @brief  Gets Disk Status
  */
DSTATUS USER_status (BYTE pdrv)
{
    return Stat;
}

/**
  * @brief  Reads Sector(s) using the safe Single Block interrupt function
  */
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
    // Loop through however many sectors FatFs asks for
    for (UINT i = 0; i < count; i++) {
        // FIX: Added pdMS_TO_TICKS(1000) timeout parameter
        if (SD_ReadSingleBlock(&sd, sector + i, buff, pdMS_TO_TICKS(1000)) != SD_OK) {
            return RES_ERROR;
        }
        buff += SD_BLOCK_SIZE; // Advance the buffer by 512 bytes
    }
    return RES_OK;
}

/**
  * @brief  Writes Sector(s) using the safe Single Block interrupt function
  */
#if _USE_WRITE == 1
DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
    for (UINT i = 0; i < count; i++) {
        // FIX: Added pdMS_TO_TICKS(1000) timeout parameter
        if (SD_WriteSingleBlock(&sd, sector + i, buff, pdMS_TO_TICKS(1000)) != SD_OK) {
            return RES_ERROR;
        }
        buff += SD_BLOCK_SIZE; // Advance the buffer by 512 bytes
    }
    return RES_OK;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  */
#if _USE_IOCTL == 1
DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff)
{
  if (pdrv != 0) return RES_PARERR;

    switch (cmd) {
        case CTRL_SYNC:
            return RES_OK; 
            
        case GET_SECTOR_COUNT:
            *(DWORD*)buff = 1024 * 1024 * 2; // Dummy size
            return RES_OK;

        case GET_BLOCK_SIZE:
            *(DWORD*)buff = 1;
            return RES_OK;
    }

    return RES_PARERR;
}
#endif /* _USE_IOCTL == 1 */