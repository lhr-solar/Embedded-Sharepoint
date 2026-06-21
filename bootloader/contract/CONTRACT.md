# Bootloader Contract (P1)

Human-readable source of truth for the resident CAN/DFU bootloader on STM32 G4 targets.
Machine-readable companions: `bl_protocol.schema.json`, `node_registry.schema.json`, `memory_map.schema.json`, and per-MCU maps in `../memory_map/`.

## Boot and recovery flow

```
Reset
  └─> Resident bootloader @ flash base (0x08000000)
        ├─ update requested?  (CAN ENTER token OR app CRC invalid OR DFU GPIO)
        │     └─> Enter update mode (stay in BL)
        └─ else
              └─> Relocate VTOR to app_vector_base, jump to application
```

**Update mode entry triggers**

| Trigger | Path | Behavior |
|---------|------|----------|
| CAN `ENTER` (or magic token on bus) | CAN | Flash target slot, verify CRC32 read-back, commit |
| App image CRC invalid | CAN fallback | Stay in BL; host must reflash (no alternate slot on single-bank parts) |
| App fault (`UPDATE`+`FAULT`) | CAN | Crash recovery: stay in BL (don't relaunch the faulting app); host reflashes |
| DFU pin / flag asserted | USB | Set ROM-DFU flag or GPIO, reset into STM32 system memory; `dfu-util` on host |

The resident bootloader contains **no USB stack**. USB updates always hand off to the STM32 ROM bootloader in system memory.

**CAN update sequence (happy path):** `ENTER` → `ERASE` (slot) → `SET_ADDR` → stream `DATA` frames → `WRITE_BLOCK` (repeat) → `VERIFY_CRC32` → `COMMIT` → optional `RESET`.

**USB update sequence:** Assert DFU entry (pin or persisted flag) → reset → ROM DFU. No on-chip USB code in the resident BL.

## App→BL handoff (TAMP backup registers)

The application requests bootloader entry by setting flag bits in TAMP `BKP0R` and issuing a warm reset; the bits persist across the reset and the BL reads them on boot. Backup-register writes require unlocking (`RCC` PWREN+RTCAPBEN, then `PWR->CR1` DBP).

| Register | Field | Meaning |
|----------|-------|---------|
| `BKP0R` bit0 | `UPDATE` | Enter CAN update mode (stay in BL) |
| `BKP0R` bit1 | `DFU` | Enter STM32 ROM DFU (system memory) |
| `BKP0R` bit2 | `FAULT` | Entry was fault-driven (set together with `UPDATE`) |
| `BKP0R` bit8 | `SLOT_B` | Active slot = B (dual-bank parts only) |
| `BKP1R` | committed length | Committed app image length (bytes) |
| `BKP2R` | committed CRC | Committed app image CRC32 |

Flag bit macros (`BL_BKP_*`) are canonical in `bl_protocol.h`; app-side helpers live in `common/Inc/bootloader_hooks.h`.

## Fault recovery (app → BL)

An application returns to the bootloader after an unrecoverable fault by setting `UPDATE`+`FAULT` and warm-resetting (`BootloaderRebootFromFault()`). Because `UPDATE` is set, the BL stays resident instead of relaunching the faulting image, so a host can reflash; the `FAULT` bit records that the entry was crash-driven (a fresh `ENTER`/`BootloaderRequestUpdate` leaves it clear). The BL clears `FAULT` on `COMMIT`. The default `HardFault_Handler` (in `common/Src/stubs.c`) calls `BootloaderRebootFromFault()` when no debugger is attached (otherwise it breaks for inspection); apps can also call it from asserts or a watchdog pre-reset path. See `docs/Bootloader.md`.

## Status LED

When resident, the BL drives an optional board status LED via `bl_indicator` (`bootloader/Inc/bl_indicator.h`):

| Pattern | Meaning |
|---------|---------|
| Breathe (smooth pulse) | Resident by request or fault recovery (`UPDATE` or `FAULT` set) |
| Hard blink (~5 Hz) | Resident only because no valid app exists (invalid-image fallback or fresh/blank MCU) |

The pattern engine is board-agnostic; the LED pin is user-defined via weak hooks (`bl_indicator_led_set` / `bl_indicator_led_init`), so a board without a status LED builds with the feature inert.

## Update state machine

```
IDLE ──ERASE──> ERASING ──done──> IDLE
IDLE ──WRITE_BLOCK──> WRITING ──block programmed──> IDLE
IDLE ──VERIFY_CRC32──> VERIFYING ──CRC_OK──> IDLE (ready for COMMIT)
                              └──CRC_FAIL──> FAULT (stay in BL, STATUS fault)
IDLE ──COMMIT──> COMMIT ──success──> IDLE (or jump to app on RESET)
Any ──error──> FAULT (broadcast on STATUS_BASE + device_id)
```

`BUSY` is returned on `RESP_BASE + device_id` while a long operation (erase, program, verify) is in progress.

## CAN addressing

Classic CAN 2.0, 8-byte frames. Host→device commands use **shared** IDs (all bootloaders listen). Device→host replies use **per-device** IDs derived from `device_id` (0..119).

| Direction | Name | ID | Notes |
|-----------|------|-----|-------|
| Host → all | `CMD` | `0x010` | Byte0 = opcode; byte1 = target `device_id` (`0xFF` = broadcast) |
| Host → all | `DATA` | `0x011` | Up to 8 payload bytes per frame; accumulated into 256-byte blocks |
| Device → host | Response | `RESP_BASE + device_id` | `RESP_BASE = 0x100` |
| Device → host | Status/fault | `STATUS_BASE + device_id` | `STATUS_BASE = 0x180` |

Collision guard: max response ID = `0x100 + 119 = 0x177` < `0x180` = min status ID.

Opcode and response code tables are canonical in `bl_protocol.schema.json`.

## Safeguards

1. **BL region isolation:** Bootloader occupies page-aligned flash at the base of bank 1. Slot bases begin on page boundaries immediately after `bl_region`. An application erase/program cannot overlap the BL because slots start above `bl_region.size` and the BL linker script reserves that range.
2. **Device-authoritative CRC32:** On `VERIFY_CRC32`, the device re-reads the programmed flash range, computes CRC32, and compares to the host-supplied expected value. The host does not trust its own write path alone.
3. **CRC failure:** On mismatch, the device stays in the bootloader, replies `CRC_FAIL` on `RESP_BASE + device_id`, and may broadcast a fault on `STATUS_BASE + device_id`. Image is not committed.
4. **Single-bank fallback (stm32g431cbt):** One app slot only. If app CRC is invalid at boot, the BL stays resident; recovery is CAN reflash or USB ROM DFU—there is no B-slot swap.

## CRC32 rule

CRC-32 in the STM32 hardware CRC peripheral's **default, non-reflected** configuration (a.k.a. CRC-32/MPEG-2 — NOT the reflected zlib/Ethernet variant):

- Polynomial: `0x04C11DB7`
- Initial value: `0xFFFFFFFF`
- Input reflection: **none**; Output reflection: **none**; Final XOR: **none**
- Processed **byte-wise, MSB-first, in ascending flash-address order** over the application image region (from slot base for `length` bytes supplied in `VERIFY_CRC32`, typically the full programmed image)

Firmware and host tools MUST produce identical values over the same bytes. The device may use the STM32 hardware CRC unit (configured for byte input, no reversal) or an equivalent software routine. Host tools MUST NOT use Python's `zlib.crc32` (reflected Ethernet CRC-32); use a non-reflected CRC-32/MPEG-2 implementation. A reference byte-wise definition both sides can share:

```
crc = 0xFFFFFFFF
for each byte b (ascending address):
    crc ^= (b << 24)
    repeat 8 times:
        crc = (crc << 1) ^ 0x04C11DB7  if (crc & 0x80000000) else (crc << 1)
    crc &= 0xFFFFFFFF
# no final reflection, no final XOR
```

## Memory maps

| MCU | Map file | BL | Slots | Notes |
|-----|----------|-----|-------|-------|
| stm32g491vet | `../memory_map/stm32g491vet.json` | 16 KB @ 0x08000000 | A (240 KB), B (256 KB) | Dual-bank; default app vector = Slot A |
| stm32g431cbt | `../memory_map/stm32g431cbt.json` | 8 KB @ 0x08000000 | APP (120 KB) | Single-bank |

ponytail: On stm32g491vet, Slot A (240 KB) and Slot B (256 KB) are intentionally asymmetric for bring-up; equalize sizes later if symmetric A/B swap is required.

## Node registry

Per-board `device_id`, `board_type`, optional 96-bit UID, and optional `bus` live in deployment registries validated against `node_registry.schema.json`. See `../registry/example.yaml` for shape only—real registries live downstream in product repos.

On a multi-FDCAN gateway (see [CAN bootloader bridge](../../docs/CanSetup.md)), a child node's `bus` names the child FDCAN bus it sits on. `validate_registry.py --recv BUS=FILE` then cross-checks that each such node has a RESP+STATUS receive filter (`BL_RECV_ENTRIES_CHILD(device_id)`) in that bus's recv-entries header, catching a child that was registered but never given a hardware filter on the gateway.

## References

- Protocol descriptor schema: `bl_protocol.schema.json`
- Node registry schema: `node_registry.schema.json`
- Memory map schema: `memory_map.schema.json`
- Example registry: `../registry/example.yaml`
- Registry validator: `../registry/validate_registry.py`
