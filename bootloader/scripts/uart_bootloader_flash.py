#!/usr/bin/env python3
import argparse
import glob
import os
import shutil
import sys
import subprocess
import time


APP_BASE = "0x08010000"
DEFAULT_APP_BIN = "build/app/stm32g473xx.bin"
BOOTLOADER_PACKET = b"ESBLT_BOOT\n"
BOOTLOADER_BYTE_DELAY_SECONDS = 0.005


def default_stm32prog() -> str:
    env_path = os.environ.get("STM32_PROGRAMMER_CLI")
    if env_path:
        return env_path

    path_match = shutil.which("STM32_Programmer_CLI")
    if path_match:
        return path_match

    if sys.platform == "darwin":
        return (
            "/Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer/"
            "STM32CubeProgrammer.app/Contents/Resources/bin/STM32_Programmer_CLI"
        )
    return os.path.expanduser("~/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI")


def run_stm32prog(stm32prog: str, args: list[str]) -> None:
    cmd = [stm32prog] + args
    print("+ " + " ".join(cmd))
    subprocess.run(cmd, check=True)


def detect_port() -> str | None:
    patterns = (
        "/dev/cu.usbserial*",
        "/dev/cu.SLAB_USBtoUART*",
        "/dev/ttyUSB*",
    )
    for pattern in patterns:
        ports = sorted(glob.glob(pattern))
        if ports:
            return ports[0]
    return None


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
    parser.add_argument("--bin", default=DEFAULT_APP_BIN, help=f"App binary to flash (default: {DEFAULT_APP_BIN})")
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
        print("No serial port found. Pass --port /dev/cu... or /dev/ttyUSB...", file=sys.stderr)
        return 1
    if not os.path.isfile(args.bin):
        print(f"Binary does not exist: {args.bin}")
        return 1
    if not os.path.isfile(args.stm32prog):
        print(f"STM32_Programmer_CLI does not exist: {args.stm32prog}")
        return 1
    if not os.access(args.stm32prog, os.X_OK):
        print(f"STM32_Programmer_CLI is not executable: {args.stm32prog}")
        return 1

    conn = [f"port={port}", f"br={args.baud}"]
    try:
        if args.enter:
            request_bootloader(port, args.baud, args.byte_delay)
            time.sleep(0.5)
        run_stm32prog(args.stm32prog, ["-c", *conn, "-w", args.bin, args.address, "-v"])
        if args.boot:
            run_stm32prog(args.stm32prog, ["-c", *conn, "-g", args.address])
    except subprocess.CalledProcessError as exc:
        return exc.returncode

    return 0


if __name__ == "__main__":
    sys.exit(main())
