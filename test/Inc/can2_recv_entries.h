#include "can_msgs.h"
#include "bl_can_filter.h"
// CAN_RECV_ENTRY(ID, SIZE, CIRCULAR)

CAN_RECV_ENTRY(0x001, 5, false)
CAN_RECV_ENTRY(0x002, 8, false)
CAN_RECV_ENTRY(0x003, 3, false)
CAN_RECV_ENTRY(0x004, 4, true)
CAN_RECV_ENTRY(BPS_FAULT_ID, 4, true)

// CAN bootloader bridge child-bus example (see bl_bridge.h / bl_can_filter.h):
// when this bus carries child nodes 0 and 1, accept their RESP/STATUS replies so
// the gateway can forward them up to the main bus. Harmless on non-gateway apps.
BL_RECV_ENTRIES_CHILD(0)
BL_RECV_ENTRIES_CHILD(1)