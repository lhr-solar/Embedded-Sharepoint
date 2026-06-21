#ifndef BL_BRIDGE_H
#define BL_BRIDGE_H

#include "can_fd.h"

/*
 * CAN bootloader bridge (multi-FDCAN gateway).
 *
 * A gateway app that owns several FDCAN buses relays resident-bootloader traffic
 * so child nodes on a child bus stay reachable from the host on the main bus.
 * Forwarding is one-directional, keyed off the fixed bl_protocol.h IDs:
 *
 *   - CMD (0x010) / DATA (0x011) received on the main bus   -> every child bus.
 *   - RESP_BASE+id / STATUS_BASE+id received on a child bus -> the main bus.
 *   - CMD/DATA seen on a child are never relayed upstream and RESP/STATUS are
 *     never pushed down (boot instructions must originate on main). The ID
 *     ranges do not overlap, so direction is enforced by bus + ID range.
 *
 * Wire it by calling bl_bridge_on_rx() from the project's
 * can_fd_rx_callback_hook (driver __weak hook, see can_fd.h). The bridge is
 * inert until the project defines a strong bl_bridge_config; the __weak default
 * is {0} (main == NULL -> no-op) and --gc-sections strips it when unused.
 */

/*
 * Buses are named by their FDCAN peripheral instance (FDCAN1/FDCAN2/...), not by
 * HAL handle pointer: an instance is an address constant, so the project can
 * declare the whole config `const` (handle objects such as hfdcan1 are runtime
 * pointers and cannot initialise a static const). The bridge resolves an
 * instance to its driver handle internally.
 */
typedef struct {
    FDCAN_GlobalTypeDef* main;             // host-facing bus, e.g. FDCAN1
    FDCAN_GlobalTypeDef* const* children;  // child buses, e.g. {FDCAN2, FDCAN3}
    uint8_t child_count;
} bl_bridge_config_t;

// Project-defined gateway topology. Default (__weak) is {0} -> bridge disabled.
extern const bl_bridge_config_t bl_bridge_config;

/*
 * Forward one received frame per the rules above. Call from inside
 * can_fd_rx_callback_hook (ISR context) and hand the BaseType_t you pass here to
 * portYIELD_FROM_ISR() afterwards. Safe to call for every frame: non-bootloader
 * IDs and frames on unconfigured buses are ignored.
 */
void bl_bridge_on_rx(FDCAN_HandleTypeDef* hfdcan, const can_rx_payload_t* frame,
                     BaseType_t* higherPriorityTaskWoken);

#endif /* BL_BRIDGE_H */
