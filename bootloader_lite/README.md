# bootloader_lite

A tiny, **opt-in** way to escape into the STM32 built-in **system-memory (ROM)
bootloader** so a board can be reflashed over UART without a resident
bootloader. It triggers on a UART magic word (`BOOT`) or on a hard fault.

This is an **interim backup** flashing path — simple on purpose — while a more
robust bootloader is developed.

## Why it's safe to bump to `bl-lite`

`bl-lite` is `main` + this folder + a flash script. The code is pure CMSIS/HAL,
defines **no** new strong symbols (no `HardFault_Handler` override), and does
nothing unless you call it. Unused code is removed by `--gc-sections`, so a
project that doesn't opt in builds and behaves exactly as on `main`.

## Opt in (one call + feed your UART)

1. Bump your Embedded-Sharepoint submodule to the `bl-lite` branch.
2. Early in `main()` (before the scheduler), install the fault → bootloader route:

```c
#include "bootloader_lite.h"

int main(void) {
    HAL_Init();
    SystemClock_Config();
    bootloader_lite_init();   // HardFault/MemManage/BusFault/UsageFault -> ROM bootloader
    ...
}
```

3. Feed received UART bytes to the matcher wherever you read the console UART.
   On a match, optionally reply so the host can confirm, then jump:

```c
// e.g. a small FreeRTOS task using the Embedded-Sharepoint bsp UART:
void Task_BootListen(void *unused) {
    (void)unused;
    uint8_t b;
    for (;;) {
        if (uart_recv(husart3, &b, 1, portMAX_DELAY) == UART_OK &&
            bootloader_lite_feed_byte(b)) {
            uart_send(husart3, (const uint8_t *)BOOTLOADER_LITE_ACK,
                      sizeof(BOOTLOADER_LITE_ACK) - 1, pdMS_TO_TICKS(200));
            vTaskDelay(pdMS_TO_TICKS(100));   // let the ACK drain
            bootloader_lite_enter_rom();      // never returns
        }
    }
}
```

That's it. `bootloader_lite_feed_byte()` and `enter_rom()` have no RTOS/BSP
dependency — feed bytes from any UART source you like.

## Opt out

Don't call any `bootloader_lite_*` function. The whole module is then garbage
collected at link time (zero flash/RAM cost).

## Flashing from the host

Port and `STM32_Programmer_CLI` detection match the other flash tooling.

```bash
make bl-send        # send "BOOT": a running app jumps into the ROM bootloader
make flash-lite     # send "BOOT", then flash the built image @ 0x08000000
# override the port if autodetect picks the wrong one:
make flash-lite BL_LITE_PORT=/dev/cu.usbserial-XXXX
```

Or call the script directly:

```bash
python3 scripts/bootloader_lite_flash.py --bin build/stm32g473xx.bin
python3 scripts/bootloader_lite_flash.py            # just enter the bootloader
```

The script sends `BOOT`, waits up to 3 s for the board's `BOOTACK`, errors if it
never arrives, then (with `--bin`) flashes over the ROM UART bootloader.

## API

| Function | Purpose |
| --- | --- |
| `bootloader_lite_init()` | Opt in: route faults to the ROM bootloader (call once, early in `main`). |
| `bootloader_lite_feed_byte(b)` | Feed UART RX bytes; returns `true` when `"BOOT"` is matched. |
| `bootloader_lite_enter_rom()` | Deinit + jump to the ROM bootloader (never returns). |
| `BOOTLOADER_LITE_COMMAND` / `BOOTLOADER_LITE_ACK` | Magic word `"BOOT"` / reply `"BOOTACK\r\n"`. |

## Notes & limitations (it's a lite/interim tool)

- **System-memory address** defaults to `0x1FFF0000` (STM32F4/G4/L4). For other
  families override `-DBOOTLOADER_LITE_SYSMEM_BASE=0x...` (see the RM "system memory").
- **Hard-fault entry** jumps from handler mode (set MSP, deinit, branch). This is
  reliable enough for UART reflashing/recovery; it is not a substitute for proper
  fault diagnostics.
- **Independent watchdog (IWDG):** if your app starts the IWDG, it keeps running
  after the jump and will reset the chip out of the bootloader after its timeout.
  Flash promptly, or don't use the IWDG when relying on this.
- **VTOR:** `bootloader_lite_init()` relocates the vector table to RAM. If your
  app also sets `SCB->VTOR`, call `bootloader_lite_init()` afterwards.
