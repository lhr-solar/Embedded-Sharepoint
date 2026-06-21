#include "bootloader_hooks.h"
#include "bl_protocol.h"
#include "stm32xx_hal.h"

/* Mirrors bl_config.c bl_backup_enable(): unlock TAMP backup register writes. */
static void backup_write_enable(void)
{
    RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_RTCAPBEN;
    PWR->CR1 |= PWR_CR1_DBP;
}

void BootloaderRequestUpdate(void)
{
    backup_write_enable();
    TAMP->BKP0R |= BL_BKP_UPDATE;
    __DSB();
    NVIC_SystemReset();
}

void BootloaderRebootFromFault(void)
{
    backup_write_enable();
    TAMP->BKP0R |= (BL_BKP_UPDATE | BL_BKP_FAULT);
    __DSB();
    NVIC_SystemReset();
}

void EnterRomDfu(void)
{
    backup_write_enable();
    TAMP->BKP0R |= BL_BKP_DFU;
    __DSB();
    NVIC_SystemReset();
}

int BootloaderHandleCanFrame(uint32_t can_id, const uint8_t *data, uint32_t len,
                                        uint8_t my_device_id)
{
    if (can_id == BL_CAN_ID_CMD && len >= 2U && data[0] == BL_OP_ENTER &&
        (data[1] == my_device_id || data[1] == BL_DEVICE_ID_BCAST)) {
        BootloaderRequestUpdate();
        return 1;
    }
    return 0;
}

__weak int BootloaderShouldEnter(void)
{
    return 0;
}
