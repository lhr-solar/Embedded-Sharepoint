#!/usr/bin/env python3
import argparse
import os
import sys
import time

from bootloader_script_utils import (
    default_binary,
    default_stm32prog,
    detect_port,
    normalize_port_for_pyserial,
    normalize_port_for_stm32prog,
    port_help,
    run_stm32prog,
    validate_file,
)

APP_BASE = "0x08010000"
DEFAULT_APP_BIN_GLOB = "build/app/stm*.bin"
BOOTLOADER_PACKET = b"ESBLT_BOOT\n"
BOOTLOADER_BYTE_DELAY_SECONDS = 0.005
# Time to wait after ESBLT_BOOT so the MCU can reset into the resident bootloader before STM32_Programmer connects.
ENTER_TO_FLASH_DELAY_S = 0.2


def default_app_bin() -> str:
    return default_binary(DEFAULT_APP_BIN_GLOB)


def request_bootloader_termios(port: str, baud: int, byte_delay: float) -> None:
    import termios

    baud_attr = getattr(termios, f"B{baud}", None)
    if baud_attr is None:
        raise RuntimeError(f"Unsupported baud rate for termios fallback: {baud}")

    fd = os.open(port, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
    try:
        attrs = termios.tcgetattr(fd)

        attrs[0] = 0  # iflag
        attrs[1] = 0  # oflag
        attrs[2] = termios.CLOCAL | termios.CREAD | termios.CS8
        attrs[3] = 0  # lflag
        attrs[4] = baud_attr
        attrs[5] = baud_attr
        attrs[6][termios.VMIN] = 0
        attrs[6][termios.VTIME] = 10

        termios.tcsetattr(fd, termios.TCSANOW, attrs)
        time.sleep(0.1)
        for byte in BOOTLOADER_PACKET:
            os.write(fd, bytes((byte,)))
            time.sleep(byte_delay)
        termios.tcdrain(fd)
    finally:
        os.close(fd)


def request_bootloader(port: str, baud: int, byte_delay: float) -> None:
    try:
        import serial
    except ImportError:
        if sys.platform == "win32":
            raise RuntimeError("pyserial is required to send the bootloader packet on Windows")
        request_bootloader_termios(port, baud, byte_delay)
        return

    with serial.Serial(port=port, baudrate=baud, timeout=1.0) as uart:
        time.sleep(0.1)
        for byte in BOOTLOADER_PACKET:
            uart.write(bytes((byte,)))
            uart.flush()
            time.sleep(byte_delay)
        uart.flush()


def main() -> int:
    parser = argparse.ArgumentParser(description="Flash app via ES_BLT AN3155-compatible UART bootloader")
    parser.add_argument("--port", help="Serial device. If omitted, common CP210x paths are autodetected.")
    parser.add_argument("--baud", default=115200, type=int, help="UART baud rate")
    parser.add_argument("--bin", default=default_app_bin(), help="App binary to flash")
    parser.add_argument("--address", default=APP_BASE, help=f"Flash address (default: {APP_BASE})")
    parser.add_argument("--enter", action="store_true", help="Send ES_BLT magic packet before flashing")
    parser.add_argument("--boot", action="store_true", help="Jump to app after flashing")
    parser.add_argument(
        "--byte-delay",
        default=BOOTLOADER_BYTE_DELAY_SECONDS,
        type=float,
        help=f"Delay between ES_BLT command bytes in seconds (default: {BOOTLOADER_BYTE_DELAY_SECONDS})",
    )
    parser.add_argument("--stm32prog", default=default_stm32prog(), help="Path to STM32_Programmer_CLI")
    args = parser.parse_args()

    port = args.port or detect_port()
    if port is None:
        print(port_help(), file=sys.stderr)
        return 1
    if not validate_file(args.bin, "Binary"):
        return 1
    if not validate_file(args.stm32prog, "STM32_Programmer_CLI", executable=True):
        return 1

    stm32prog_port = normalize_port_for_stm32prog(port, args.stm32prog)
    conn = [f"port={stm32prog_port}", f"br={args.baud}"]
    try:
        if args.enter:
            request_bootloader(normalize_port_for_pyserial(port), args.baud, args.byte_delay)
            time.sleep(ENTER_TO_FLASH_DELAY_S)
        run_stm32prog(args.stm32prog, ["-c", *conn, "-w", args.bin, args.address, "-v"])
        if args.boot:
            run_stm32prog(args.stm32prog, ["-c", *conn, "-g", args.address])
    except Exception as exc:
        returncode = getattr(exc, "returncode", None)
        if isinstance(returncode, int):
            return returncode
        print(f"UART bootloader flash failed: {exc}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
