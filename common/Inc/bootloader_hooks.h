#ifndef BOOTLOADER_HOOKS_H
#define BOOTLOADER_HOOKS_H

#include <stdint.h>

#include "bl_protocol.h"

/*
 * App-side counterpart to the resident CAN/DFU bootloader. These helpers set the
 * TAMP BKP0R handoff flags (see BL_BKP_* in bl_protocol.h) and warm-reset into
 * the bootloader, which reads the flags on boot.
 *
 * Wiring from the FDCAN RX hook (G4 uses FDCAN; can_fd.c provides a __weak default):
 *
 *   #include "bootloader_hooks.h"
 *   #include "can_fd.h"
 *
 *   void can_fd_rx_callback_hook(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs,
 *                                can_rx_payload_t recv_payload) {
 *       (void)hfdcan;
 *       (void)RxFifo0ITs;
 *       BootloaderHandleCanFrame(recv_payload.header.Identifier,
 *                                           recv_payload.data,
 *                                           FDCAN_BYTES_FROM_DLC(recv_payload.header.DataLength),
 *                                           MY_DEVICE_ID);
 *   }
 *
 * recv_payload is passed by value; .header.Identifier is the CAN id and
 * FDCAN_BYTES_FROM_DLC(.header.DataLength) yields the payload byte count.
 */

/* Set BKP0R UPDATE flag, then system reset. Never returns. */
void BootloaderRequestUpdate(void);

/*
 * Recover into the bootloader after an unrecoverable fault: set BKP0R
 * UPDATE+FAULT, then system reset. Never returns. Call from a fault handler
 * (HardFault etc.), a failed assert, or a watchdog pre-reset hook. The BL stays
 * resident (it does NOT relaunch the faulting app) so a host can reflash, and it
 * shows the "breathe" status pattern. The default HardFault_Handler in
 * common/Src/stubs.c already calls this; wire it into your own fault paths too.
 */
void BootloaderRebootFromFault(void);

/* Set BKP0R DFU flag, then system reset. Never returns. */
void EnterRomDfu(void);

/* __weak, default 0. App may override to force bootloader entry at boot. */
int BootloaderShouldEnter(void);

/*
 * Returns 1 if the frame is a BL ENTER command targeting this device (and then
 * triggers BootloaderRequestUpdate, which resets); else returns 0.
 */
int BootloaderHandleCanFrame(uint32_t can_id, const uint8_t *data, uint32_t len,
                                        uint8_t my_device_id);

#endif /* BOOTLOADER_HOOKS_H */
