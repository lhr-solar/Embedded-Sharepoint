#!/usr/bin/env python3
import argparse
import os
import sys
import time

from bootloader_script_utils import detect_port, normalize_port_for_pyserial, port_help


BOOTLOADER_PACKET = b"ESBLT_BOOT\n"
BOOTLOADER_BYTE_DELAY_SECONDS = 0.005


def send_packet_termios(port: str, baud: int, delay: float, byte_delay: float) -> None:
    import termios

    baud_attr = getattr(termios, f"B{baud}", None)
    if baud_attr is None:
        raise RuntimeError(f"Unsupported baud rate for termios fallback: {baud}")

    fd = os.open(port, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
    try:
        attrs = termios.tcgetattr(fd)
        attrs[0] = 0
        attrs[1] = 0
        attrs[2] = termios.CLOCAL | termios.CREAD | termios.CS8
        attrs[3] = 0
        attrs[4] = baud_attr
        attrs[5] = baud_attr
        attrs[6][termios.VMIN] = 0
        attrs[6][termios.VTIME] = 10

        termios.tcsetattr(fd, termios.TCSANOW, attrs)
        if delay > 0:
            time.sleep(delay)
        for byte in BOOTLOADER_PACKET:
            os.write(fd, bytes((byte,)))
            time.sleep(byte_delay)
        termios.tcdrain(fd)
    finally:
        os.close(fd)


def send_packet(port: str, baud: int, timeout: float, delay: float, byte_delay: float) -> None:
    try:
        import serial
    except ImportError:
        if sys.platform == "win32":
            raise RuntimeError("pyserial is required to send the bootloader packet on Windows")
        send_packet_termios(port, baud, delay, byte_delay)
        return

    with serial.Serial(port=port, baudrate=baud, timeout=timeout) as uart:
        if delay > 0:
            time.sleep(delay)
        for byte in BOOTLOADER_PACKET:
            uart.write(bytes((byte,)))
            uart.flush()
            time.sleep(byte_delay)
        uart.flush()


def main() -> int:
    parser = argparse.ArgumentParser(description="Send the ES_BLT UART magic packet to reset app into bootloader")
    parser.add_argument("--port", help="Serial device. If omitted, common CP210x paths are autodetected.")
    parser.add_argument("--baud", default=115200, type=int, help="UART baud rate (default: 115200)")
    parser.add_argument("--timeout", default=1.0, type=float, help="Serial timeout in seconds")
    parser.add_argument("--delay", default=0.1, type=float, help="Delay after opening port before sending")
    parser.add_argument(
        "--byte-delay",
        default=BOOTLOADER_BYTE_DELAY_SECONDS,
        type=float,
        help=f"Delay between ES_BLT command bytes in seconds (default: {BOOTLOADER_BYTE_DELAY_SECONDS})",
    )
    args = parser.parse_args()

    port = args.port or detect_port()
    if port is None:
        print(port_help(), file=sys.stderr)
        return 1

    try:
        serial_port = normalize_port_for_pyserial(port)
        send_packet(serial_port, args.baud, args.timeout, args.delay, args.byte_delay)
    except Exception as exc:
        print(f"Failed to send bootloader packet: {exc}", file=sys.stderr)
        return 1

    print(f"Sent bootloader packet to {port} at {args.baud} baud")
    return 0


if __name__ == "__main__":
    sys.exit(main())
