#include "bl_bridge.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "bl_protocol.h"

/*
 * Inert default: a gateway project provides a strong
 * `const bl_bridge_config_t bl_bridge_config = { ... }`. With main == NULL the
 * bridge no-ops, and --gc-sections drops this object plus bl_bridge_on_rx when
 * the project never references them (e.g. a board with no resident bootloader).
 */
__weak const bl_bridge_config_t bl_bridge_config = {0};

// Resolve an FDCAN peripheral instance to the driver's handle for that bus.
static FDCAN_HandleTypeDef* bl_bridge_handle(const FDCAN_GlobalTypeDef* instance) {
    if (instance == NULL) {
        return NULL;
    }
#ifdef FDCAN1
    if (instance == FDCAN1) {
        return hfdcan1;
    }
#endif
#ifdef FDCAN2
    if (instance == FDCAN2) {
        return hfdcan2;
    }
#endif
#ifdef FDCAN3
    if (instance == FDCAN3) {
        return hfdcan3;
    }
#endif
    return NULL;
}

// Device replies: RESP_BASE+id and STATUS_BASE+id, id in 0..BL_DEVICE_ID_MAX.
static bool bl_bridge_is_device_reply(uint32_t id) {
    return (id >= BL_CAN_ID_RESP_BASE && id <= BL_CAN_ID_RESP_BASE + BL_DEVICE_ID_MAX) ||
           (id >= BL_CAN_ID_STATUS_BASE && id <= BL_CAN_ID_STATUS_BASE + BL_DEVICE_ID_MAX);
}

void bl_bridge_on_rx(FDCAN_HandleTypeDef* hfdcan, const can_rx_payload_t* frame,
                     BaseType_t* higherPriorityTaskWoken) {
    const bl_bridge_config_t* cfg = &bl_bridge_config;
    if (cfg->main == NULL || hfdcan == NULL || frame == NULL) {
        return;
    }

    const uint32_t id = frame->header.Identifier;

    // Mirror the received frame into a classic-CAN TX header. RX and TX
    // DataLength share the FDCAN DLC encoding, so it copies straight across.
    FDCAN_TxHeaderTypeDef tx = {
        .Identifier = id,
        .IdType = FDCAN_STANDARD_ID,
        .TxFrameType = FDCAN_DATA_FRAME,
        .DataLength = frame->header.DataLength,
        .ErrorStateIndicator = FDCAN_ESI_ACTIVE,
        .BitRateSwitch = FDCAN_BRS_OFF,
        .FDFormat = FDCAN_CLASSIC_CAN,
        .TxEventFifoControl = FDCAN_NO_TX_EVENTS,
        .MessageMarker = 0,
    };
    uint8_t* data = (uint8_t*)frame->data;  // can_fd_send_isr only reads it

    if (hfdcan->Instance == cfg->main) {
        // Host -> devices: fan host commands out to every child bus. A 256-byte
        // block arrives as several DATA frames, each forwarded individually.
        if (id == BL_CAN_ID_CMD || id == BL_CAN_ID_DATA) {
            for (uint8_t i = 0; i < cfg->child_count; i++) {
                FDCAN_HandleTypeDef* child = bl_bridge_handle(cfg->children[i]);
                if (child != NULL) {
                    can_fd_send_isr(child, &tx, data, higherPriorityTaskWoken);
                }
            }
        }
        return;
    }

    // Devices -> host: forward a child's RESP/STATUS up to the main bus. CMD/DATA
    // seen on a child are dropped (boot instructions only originate on main).
    if (bl_bridge_is_device_reply(id)) {
        for (uint8_t i = 0; i < cfg->child_count; i++) {
            if (hfdcan->Instance == cfg->children[i]) {
                FDCAN_HandleTypeDef* main = bl_bridge_handle(cfg->main);
                if (main != NULL) {
                    can_fd_send_isr(main, &tx, data, higherPriorityTaskWoken);
                }
                return;
            }
        }
    }
}
