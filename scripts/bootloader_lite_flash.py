#!/usr/bin/env python3
"""
bootloader_lite_flash.py

Put a board running the `bootloader_lite` helper into the STM32 system-memory
(ROM) bootloader by sending the "BOOT" magic word, wait for its "BOOTACK" reply,
then (optionally) flash a binary over the ROM UART bootloader with
STM32_Programmer_CLI.

Serial-port and STM32_Programmer_CLI detection match the rest of the
Embedded-Sharepoint flash tooling (bootloader_script_utils).

Examples:
    python3 scripts/bootloader_lite_flash.py                       # just enter the bootloader
    python3 scripts/bootloader_lite_flash.py --bin build/stm32g473xx.bin
    python3 scripts/bootloader_lite_flash.py --port /dev/cu.usbserial-1420 --bin app.bin
"""
import argparse
import sys
import time

from bootloader_script_utils import (
    default_stm32prog,
    detect_port,
    port_help,
    run_stm32prog,
    validate_file,
)

MAGIC = b"BOOT"
ACK = b"BOOTACK"
ACK_TIMEOUT_S = 3.0
FLASH_ADDRESS = "0x08000000"
# Give the MCU a moment to reach the ROM bootloader before STM32_Programmer connects.
ENTER_TO_FLASH_DELAY_S = 0.3


def send_and_wait_ack(port: str, baud: int, timeout: float, wait_ack: bool) -> bool:
    try:
        import serial
    except ImportError:
        print("pyserial is required: pip install pyserial", file=sys.stderr)
        return False

    try:
        with serial.Serial(port=port, baudrate=baud, timeout=0.1) as uart:
            uart.reset_input_buffer()
            uart.write(MAGIC)
            uart.flush()
            print(f"Sent {MAGIC!r} to {port} @ {baud} baud.")
            if not wait_ack:
                return True
            deadline = time.monotonic() + timeout
            buf = bytearray()
            while time.monotonic() < deadline:
                chunk = uart.read(256)
                if chunk:
                    buf += chunk
                    if ACK in buf:
                        print(f"ACK ({ACK.decode()}) received - entering ROM bootloader.")
                        return True
            print(
                f"ERROR: no ACK ({ACK.decode()}) within {timeout:.0f}s. "
                "Is the board running bootloader_lite, on the right port and baud?",
                file=sys.stderr,
            )
            return False
    except serial.SerialException as exc:
        print(f"Failed to open/write {port}: {exc}", file=sys.stderr)
        return False


def main() -> int:
    ap = argparse.ArgumentParser(
        description="Send BOOT to enter the STM32 ROM bootloader, then optionally flash.")
    ap.add_argument("--port", help="Serial device (autodetected if omitted)")
    ap.add_argument("--baud", default=115200, type=int, help="UART baud (default 115200)")
    ap.add_argument("--bin", help="Binary to flash after entering the bootloader (omit to just enter)")
    ap.add_argument("--address", default=FLASH_ADDRESS, help=f"Flash address (default {FLASH_ADDRESS})")
    ap.add_argument("--ack-timeout", default=ACK_TIMEOUT_S, type=float,
                    help=f"Seconds to wait for BOOTACK (default {ACK_TIMEOUT_S})")
    ap.add_argument("--no-ack", action="store_true", help="Don't wait for the board's ACK")
    ap.add_argument("--stm32prog", default=default_stm32prog(), help="Path to STM32_Programmer_CLI")
    args = ap.parse_args()

    port = args.port or detect_port()
    if port is None:
        print(port_help(), file=sys.stderr)
        return 1

    if not send_and_wait_ack(port, args.baud, args.ack_timeout, not args.no_ack):
        return 1

    if not args.bin:
        print("Board should now be in the ROM bootloader. (No --bin; not flashing.)")
        return 0

    if not validate_file(args.bin, "Binary"):
        return 1
    if not validate_file(args.stm32prog, "STM32_Programmer_CLI", executable=True):
        return 1

    time.sleep(ENTER_TO_FLASH_DELAY_S)
    conn = [f"port={port}", f"br={args.baud}"]
    try:
        run_stm32prog(args.stm32prog, ["-c", *conn, "-w", args.bin, args.address, "-v", "-g", args.address])
    except Exception as exc:
        rc = getattr(exc, "returncode", None)
        if isinstance(rc, int):
            return rc
        print(f"Flash failed: {exc}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
