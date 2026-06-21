#ifndef BL_PROTOCOL_H
#define BL_PROTOCOL_H

#include <stdint.h>

/* CAN IDs (classic 2.0, 8-byte payload) — canonical: bl_protocol.schema.json */
#define BL_CAN_ID_CMD         0x010U
#define BL_CAN_ID_DATA        0x011U
#define BL_CAN_ID_RESP_BASE   0x100U
#define BL_CAN_ID_STATUS_BASE 0x180U

#define BL_DEVICE_ID_BCAST    0xFFU
#define BL_DEVICE_ID_MAX      119U

#define BL_DATA_BLOCK_BYTES   256U

/* Command opcodes */
#define BL_OP_ENTER        1U
#define BL_OP_ERASE        2U
#define BL_OP_SET_ADDR     3U
#define BL_OP_WRITE_BLOCK  4U
#define BL_OP_VERIFY_CRC32 5U
#define BL_OP_COMMIT       6U
#define BL_OP_RESET        7U
#define BL_OP_GET_INFO     8U

/* Response codes */
#define BL_RSP_ACK      0U
#define BL_RSP_NACK     1U
#define BL_RSP_BUSY     2U
#define BL_RSP_ERR      3U
#define BL_RSP_CRC_OK   16U
#define BL_RSP_CRC_FAIL 17U

/* Status / fault codes (byte0 on STATUS_BASE + device_id) */
#define BL_FAULT_NONE          0U
#define BL_FAULT_CRC_MISMATCH  1U
#define BL_FAULT_FLASH         2U
#define BL_FAULT_RANGE         3U
#define BL_FAULT_STATE         4U

/* App<->BL handoff via TAMP backup register BKP0R (persists across warm reset). */
#define BL_BKP_UPDATE  (1U << 0)
#define BL_BKP_DFU     (1U << 1)
#define BL_BKP_FAULT   (1U << 2)  /* entry was fault-driven (set with UPDATE) */
#define BL_BKP_SLOT_B  (1U << 8)

#endif /* BL_PROTOCOL_H */
