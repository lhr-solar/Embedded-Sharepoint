#!/usr/bin/env python3
import argparse
import sys
import time


BOOTLOADER_PACKET = b"ESBLT_BOOT\n"


def send_packet(port: str, baud: int, timeout: float, delay: float) -> None:
    try:
        import serial
    except ImportError as exc:
        raise RuntimeError("pyserial is required. Run this from `nix develop` or install requirements.txt.") from exc

    with serial.Serial(port=port, baudrate=baud, timeout=timeout) as uart:
        if delay > 0:
            time.sleep(delay)
        uart.write(BOOTLOADER_PACKET)
        uart.flush()


def main() -> int:
    parser = argparse.ArgumentParser(description="Send the ES_BLT UART magic packet to reset app into bootloader")
    parser.add_argument("--port", required=True, help="Serial device (ex: /dev/cu.usbserial-310)")
    parser.add_argument("--baud", default=115200, type=int, help="UART baud rate (default: 115200)")
    parser.add_argument("--timeout", default=1.0, type=float, help="Serial timeout in seconds")
    parser.add_argument("--delay", default=0.1, type=float, help="Delay after opening port before sending")
    args = parser.parse_args()

    try:
        send_packet(args.port, args.baud, args.timeout, args.delay)
    except Exception as exc:
        print(f"Failed to send bootloader packet: {exc}", file=sys.stderr)
        return 1

    print(f"Sent bootloader packet to {args.port} at {args.baud} baud")
    return 0


if __name__ == "__main__":
    sys.exit(main())
