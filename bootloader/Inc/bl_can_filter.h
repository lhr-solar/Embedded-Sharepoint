#ifndef BL_CAN_FILTER_H
#define BL_CAN_FILTER_H

#include "bl_protocol.h"

/*
 * Hardware receive-filter helpers for buses that carry resident-bootloader
 * traffic on a gateway (see bl_bridge.h). The FDCAN acceptance filter is derived
 * from each bus's can<N>_recv_entries.h (one element per CAN_RECV_ENTRY ID), so
 * the bridge can only forward frames the peripheral is told to accept. These
 * macros expand to the CAN_RECV_ENTRY lines a gateway needs; use them inside a
 * recv-entries file (which is included where CAN_RECV_ENTRY is defined).
 */

/*
 * Main (host-facing) bus: accept the two shared host->device IDs so the gateway
 * receives the commands it forwards down.
 *
 * NOTE: FDCAN1 already auto-registers CMD/DATA (CAN1_BL_RECV_ENTRIES in
 * can_fd.c), so use this only on a main bus that is NOT FDCAN1 -- adding it to
 * can1_recv_entries.h would declare the same queue storage twice (a compile
 * error).
 */
#define BL_RECV_ENTRIES_MAIN                \
    CAN_RECV_ENTRY(BL_CAN_ID_CMD, 8, false) \
    CAN_RECV_ENTRY(BL_CAN_ID_DATA, 16, false)

/*
 * Child bus: accept one child node's RESP + STATUS so the gateway receives the
 * replies it forwards up. List one BL_RECV_ENTRIES_CHILD(dev) per child node on
 * this bus. `dev` is the node's device id (0..BL_DEVICE_ID_MAX) and must be a
 * single token (an integer literal or a macro that expands to one): it names the
 * queue storage, while the arithmetic CAN ID stays an expression via
 * CAN_RECV_ENTRY_KEYED.
 */
#define BL_RECV_ENTRIES_CHILD(dev)                                             \
    CAN_RECV_ENTRY_KEYED(bl_resp_##dev, BL_CAN_ID_RESP_BASE + (dev), 4, false) \
    CAN_RECV_ENTRY_KEYED(bl_status_##dev, BL_CAN_ID_STATUS_BASE + (dev), 4, true)

#endif /* BL_CAN_FILTER_H */
