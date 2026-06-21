#include "bl_can.h"
#include "bl_config.h"
#include "bl_flash.h"
#include "bl_jump.h"
#include "bl_protocol.h"
#include "bl_status.h"
#include "bl_target.h"

#include <stddef.h>
#include "stm32g4xx.h"

/* SRAMCAN layout — matches STM32 HAL fixed message RAM map */
#define BL_CAN_FLS_NBR   28U
#define BL_CAN_FLE_NBR   8U
#define BL_CAN_RF0_NBR   3U
#define BL_CAN_RF1_NBR   3U
#define BL_CAN_TEF_NBR   3U
#define BL_CAN_TFQ_NBR   3U
#define BL_CAN_FLS_SIZE  4U
#define BL_CAN_FLE_SIZE  8U
#define BL_CAN_RF0_SIZE  72U
#define BL_CAN_RF1_SIZE  72U
#define BL_CAN_TEF_SIZE  8U
#define BL_CAN_TFQ_SIZE  72U

#define BL_CAN_FLSSA  0U
#define BL_CAN_FLESA  (BL_CAN_FLSSA + (BL_CAN_FLS_NBR * BL_CAN_FLS_SIZE))
#define BL_CAN_RF0SA  (BL_CAN_FLESA + (BL_CAN_FLE_NBR * BL_CAN_FLE_SIZE))
#define BL_CAN_RF1SA  (BL_CAN_RF0SA + (BL_CAN_RF0_NBR * BL_CAN_RF0_SIZE))
#define BL_CAN_TEFSA  (BL_CAN_RF1SA + (BL_CAN_RF1_NBR * BL_CAN_RF1_SIZE))
#define BL_CAN_TFQSA  (BL_CAN_TEFSA + (BL_CAN_TEF_NBR * BL_CAN_TEF_SIZE))
#define BL_CAN_SRAM_SIZE (BL_CAN_TFQSA + (BL_CAN_TFQ_NBR * BL_CAN_TFQ_SIZE))

#define BL_CAN_STD_ID      0U
#define BL_CAN_DATA_FRAME  0U
#define BL_CAN_DLC8        8U

static uint32_t s_msg_ram;
static uint32_t s_rx_fifo0;
static uint32_t s_tx_fifo;

static uint32_t s_write_addr;
static uint8_t s_data_buf[BL_DATA_BLOCK_BYTES];
static uint32_t s_data_len;
static uint32_t s_verify_len;
static uint32_t s_verify_crc;

static int bl_can_for_me(uint8_t target)
{
    return (target == BL_DEVICE_ID_BCAST) || (target == bl_config_device_id());
}

static void bl_can_gpio_init(void)
{
    /* ponytail: set FDCAN1 AF on board CAN pins (e.g. PB8/PB9 AF9); verify on HW */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
}

static void bl_can_ram_init(void)
{
    s_msg_ram = SRAMCAN_BASE;
    s_rx_fifo0 = s_msg_ram + BL_CAN_RF0SA;
    s_tx_fifo = s_msg_ram + BL_CAN_TFQSA;

    for (uint32_t a = s_msg_ram; a < (s_msg_ram + BL_CAN_SRAM_SIZE); a += 4U) {
        *(volatile uint32_t *)a = 0U;
    }

    /* Range filter: accept CMD..DATA into RX FIFO0 */
    *(volatile uint32_t *)(s_msg_ram + BL_CAN_FLSSA) =
        ((BL_CAN_STD_ID << 30) | (1U << 27) | (BL_CAN_ID_CMD << 16) | BL_CAN_ID_DATA);
}

static void bl_can_periph_init(void)
{
    FDCAN_GlobalTypeDef *can = FDCAN1;

    RCC->APB1ENR1 |= RCC_APB1ENR1_FDCANEN;
    bl_can_gpio_init();

    can->CCCR = FDCAN_CCCR_INIT;
    while ((can->CCCR & FDCAN_CCCR_INIT) == 0U) {
    }
    can->CCCR |= FDCAN_CCCR_CCE;

    /* Classic CAN @ ~500 kbit/s from 16 MHz HSI — ponytail: tune NBTP for bus/HSE on HW */
    can->NBTP = ((0U << FDCAN_NBTP_NSJW_Pos) | (12U << FDCAN_NBTP_NTSEG1_Pos) |
                 (1U << FDCAN_NBTP_NTSEG2_Pos) | (1U << FDCAN_NBTP_NBRP_Pos));
    can->CCCR &= ~(FDCAN_CCCR_FDOE | FDCAN_CCCR_BRSE);

    can->RXGFC = (1U << FDCAN_RXGFC_LSS_Pos) | FDCAN_RXGFC_RRFE | FDCAN_RXGFC_RRFS |
                 (2U << FDCAN_RXGFC_ANFS_Pos) | (2U << FDCAN_RXGFC_ANFE_Pos);

    bl_can_ram_init();

    can->CCCR &= ~FDCAN_CCCR_INIT;
}

void bl_can_init(void)
{
    s_write_addr = bl_config_active_slot_base();
    s_data_len = 0U;
    bl_can_periph_init();
}

static int bl_can_tx_std(uint16_t id, const uint8_t *data, uint8_t len)
{
    FDCAN_GlobalTypeDef *can = FDCAN1;

    if ((can->TXFQS & FDCAN_TXFQS_TFQF) != 0U) {
        return -1;
    }

    uint32_t put = (can->TXFQS & FDCAN_TXFQS_TFQPI) >> FDCAN_TXFQS_TFQPI_Pos;
    volatile uint32_t *elem = (volatile uint32_t *)(s_tx_fifo + (put * BL_CAN_TFQ_SIZE));

    elem[0] = (uint32_t)(id << 18);
    elem[1] = (uint32_t)(BL_CAN_DLC8 << 16);
    elem[2] = (uint32_t)data[0] | ((uint32_t)data[1] << 8) | ((uint32_t)data[2] << 16) |
              ((uint32_t)data[3] << 24);
    elem[3] = (uint32_t)data[4] | ((uint32_t)data[5] << 8) | ((uint32_t)data[6] << 16) |
              ((uint32_t)data[7] << 24);

    can->TXBAR = (1U << put);
    return 0;
}

int bl_can_send_resp(uint8_t code, const uint8_t *extra, uint8_t extra_len)
{
    uint8_t frame[8];
    frame[0] = code;
    for (uint8_t i = 0; i < 7U; i++) {
        frame[i + 1U] = (extra != NULL && i < extra_len) ? extra[i] : 0U;
    }
    return bl_can_tx_std((uint16_t)(BL_CAN_ID_RESP_BASE + bl_config_device_id()), frame, 8U);
}

int bl_can_send_status(const uint8_t *payload, uint8_t len)
{
    uint8_t frame[8] = {0};
    for (uint8_t i = 0; i < len && i < 8U; i++) {
        frame[i] = payload[i];
    }
    return bl_can_tx_std((uint16_t)(BL_CAN_ID_STATUS_BASE + bl_config_device_id()), frame, 8U);
}

static int bl_can_rx(uint16_t *id, uint8_t *data, uint8_t *len)
{
    FDCAN_GlobalTypeDef *can = FDCAN1;

    if ((can->RXF0S & FDCAN_RXF0S_F0FL) == 0U) {
        return -1;
    }

    uint32_t idx = (can->RXF0S & FDCAN_RXF0S_F0GI) >> FDCAN_RXF0S_F0GI_Pos;
    volatile uint32_t *elem = (volatile uint32_t *)(s_rx_fifo0 + (idx * BL_CAN_RF0_SIZE));

    *id = (uint16_t)((elem[0] >> 18) & 0x7FFU);
    uint32_t dlc = (elem[1] >> 16) & 0xFU;
    uint8_t n = (dlc > 8U) ? 8U : (uint8_t)dlc;
    *len = n;
    uint32_t w0 = elem[2];
    uint32_t w1 = elem[3];
    for (uint8_t i = 0; i < n; i++) {
        if (i < 4U) {
            data[i] = (uint8_t)((w0 >> (8U * i)) & 0xFFU);
        } else {
            data[i] = (uint8_t)((w1 >> (8U * (i - 4U))) & 0xFFU);
        }
    }

    can->RXF0A = idx;
    return 0;
}

static void bl_erase_active_slot(void)
{
    uint32_t base = bl_config_active_slot_base();
    uint32_t size = BL_SLOT_A_SIZE;
#if defined(STM32G491xx)
    if (base == BL_SLOT_B_BASE) {
        size = BL_SLOT_B_SIZE;
    }
#endif
    for (uint32_t off = 0; off < size; off += BL_PAGE_SIZE) {
        (void)bl_flash_page_erase(base + off);
    }
}

static void bl_handle_cmd(const uint8_t *cmd, uint8_t len)
{
    if (len < 2U || !bl_can_for_me(cmd[1])) {
        return;
    }

    uint8_t op = cmd[0];

    switch (op) {
    case BL_OP_ENTER:
        bl_config_set_update_requested(1);
        (void)bl_can_send_resp(BL_RSP_ACK, &op, 1U);
        break;

    case BL_OP_ERASE:
        (void)bl_can_send_resp(BL_RSP_BUSY, &op, 1U);
        bl_erase_active_slot();
        s_write_addr = bl_config_active_slot_base();
        s_data_len = 0U;
        (void)bl_can_send_resp(BL_RSP_ACK, &op, 1U);
        break;

    case BL_OP_SET_ADDR:
        if (len < 6U) {
            (void)bl_can_send_resp(BL_RSP_NACK, &op, 1U);
            break;
        }
        s_write_addr = (uint32_t)cmd[2] | ((uint32_t)cmd[3] << 8) |
                       ((uint32_t)cmd[4] << 16) | ((uint32_t)cmd[5] << 24);
        if (bl_addr_in_bl_region(s_write_addr, BL_DATA_BLOCK_BYTES)) {
            (void)bl_can_send_resp(BL_RSP_ERR, &op, 1U);
            bl_status_fault(BL_FAULT_RANGE);
            break;
        }
        s_data_len = 0U;
        (void)bl_can_send_resp(BL_RSP_ACK, &op, 1U);
        break;

    case BL_OP_WRITE_BLOCK:
        if (s_data_len != BL_DATA_BLOCK_BYTES) {
            (void)bl_can_send_resp(BL_RSP_NACK, &op, 1U);
            bl_status_fault(BL_FAULT_STATE);
            break;
        }
        (void)bl_can_send_resp(BL_RSP_BUSY, &op, 1U);
        if (bl_flash_program_block(s_write_addr, s_data_buf, BL_DATA_BLOCK_BYTES) != 0) {
            (void)bl_can_send_resp(BL_RSP_ERR, &op, 1U);
            bl_status_fault(BL_FAULT_FLASH);
            break;
        }
        s_write_addr += BL_DATA_BLOCK_BYTES;
        s_data_len = 0U;
        (void)bl_can_send_resp(BL_RSP_ACK, &op, 1U);
        break;

    case BL_OP_VERIFY_CRC32:
        if (len < 10U) {
            (void)bl_can_send_resp(BL_RSP_NACK, &op, 1U);
            break;
        }
        s_verify_crc = (uint32_t)cmd[2] | ((uint32_t)cmd[3] << 8) |
                       ((uint32_t)cmd[4] << 16) | ((uint32_t)cmd[5] << 24);
        s_verify_len = (uint32_t)cmd[6] | ((uint32_t)cmd[7] << 8) |
                       ((uint32_t)cmd[8] << 16) | ((uint32_t)cmd[9] << 24);
        (void)bl_can_send_resp(BL_RSP_BUSY, &op, 1U);
        {
            uint32_t got = bl_flash_crc32(bl_config_active_slot_base(), s_verify_len);
            if (got == s_verify_crc) {
                (void)bl_can_send_resp(BL_RSP_CRC_OK, &op, 1U);
            } else {
                (void)bl_can_send_resp(BL_RSP_CRC_FAIL, &op, 1U);
                bl_status_fault(BL_FAULT_CRC_MISMATCH);
            }
        }
        break;

    case BL_OP_COMMIT:
        bl_config_set_committed(s_verify_len, s_verify_crc);
        bl_config_set_update_requested(0);
        bl_config_clear_fault();
        (void)bl_can_send_resp(BL_RSP_ACK, &op, 1U);
        break;

    case BL_OP_RESET:
        (void)bl_can_send_resp(BL_RSP_ACK, &op, 1U);
        if (bl_config_app_valid()) {
            bl_jump_to_app(bl_config_active_slot_base());
        }
        bl_system_reset();
        break;

    case BL_OP_GET_INFO: {
        uint8_t info[7];
        uint8_t uid[12];
        info[0] = bl_config_device_id();
        info[1] = 0U; /* BL protocol version */
        bl_config_read_uid(uid);
        for (uint8_t i = 0; i < 5U; i++) {
            info[2U + i] = uid[i];
        }
        (void)bl_can_send_resp(BL_RSP_ACK, info, 7U);
        break;
    }

    default:
        (void)bl_can_send_resp(BL_RSP_NACK, &op, 1U);
        break;
    }
}

static void bl_handle_data(const uint8_t *payload, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++) {
        if (s_data_len < BL_DATA_BLOCK_BYTES) {
            s_data_buf[s_data_len++] = payload[i];
        }
    }
}

void bl_can_run(void)
{
    uint16_t id;
    uint8_t data[8];
    uint8_t len;

    if (bl_can_rx(&id, data, &len) != 0) {
        return;
    }

    if (id == BL_CAN_ID_CMD) {
        bl_handle_cmd(data, len);
    } else if (id == BL_CAN_ID_DATA) {
        bl_handle_data(data, len);
    }
}
