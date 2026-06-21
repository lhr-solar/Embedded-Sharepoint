/*
 * CAN bootloader bridge (multi-FDCAN gateway) demo + compile validation.
 *
 * Models the smallest real gateway: FDCAN1 is the host-facing main bus and
 * FDCAN2 carries the child nodes. The bridge runs entirely in the RX ISR hook,
 * so the app just brings up both buses and relays in can_fd_rx_callback_hook.
 *
 * Filters: CMD/DATA on FDCAN1 are auto-accepted (CAN1_BL_RECV_ENTRIES in
 * can_fd.c); FDCAN2 accepts each child's RESP/STATUS via BL_RECV_ENTRIES_CHILD
 * in can2_recv_entries.h. A CMD injected on the child bus is rejected by FDCAN2's
 * filter and, even if seen, dropped by bl_bridge_on_rx (directional guard), so it
 * never reaches the main bus.
 *
 * Needs a child FDCAN bus, so it [CONFIG]-skips on single-bus parts (e.g. g431).
 */
#include "bl_bridge.h"

#if !defined(FDCAN2)
#error "[CONFIG] CAN bootloader bridge needs a child FDCAN bus; this part only has FDCAN1."
#endif

#include "FreeRTOS.h"
#include "task.h"

#include "board.h"
#include "stm32xx_hal.h"

// Gateway topology: host on FDCAN1 (main), child nodes on FDCAN2. Instances are
// address constants, so the whole config is a compile-time const (see bl_bridge.h).
static FDCAN_GlobalTypeDef* const bl_bridge_children[] = {FDCAN2};
const bl_bridge_config_t bl_bridge_config = {
    .main = FDCAN1,
    .children = bl_bridge_children,
    .child_count = 1,
};

// Relay every received frame; the bridge ignores non-bootloader IDs and buses.
void can_fd_rx_callback_hook(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs,
                             can_rx_payload_t recv_payload) {
    (void)RxFifo0ITs;
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    bl_bridge_on_rx(hfdcan, &recv_payload, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void Error_Handler(void) {
    while (1) {
        HAL_Delay(1000);
    }
}

static StaticTask_t bridge_task_buffer;
static StackType_t bridge_task_stack[256];

static void bridge_task(void* pv) {
    (void)pv;
    // Forwarding happens in the RX ISR; the gateway itself just idles.
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();

    board_fdcan1_init();  // main bus (host-facing)
    board_fdcan2_init();  // child bus

    xTaskCreateStatic(bridge_task, "bridge", 256, NULL, tskIDLE_PRIORITY + 1,
                      bridge_task_stack, &bridge_task_buffer);

    vTaskStartScheduler();
    while (1) {
    }
    return 0;
}
