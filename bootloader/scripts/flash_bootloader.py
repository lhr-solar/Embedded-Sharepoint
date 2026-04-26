#!/usr/bin/env python3
import argparse
import glob
import os
import subprocess
import sys


BOOTLOADER_ADDRESS = "0x08000000"
DEFAULT_BOOTLOADER_BIN = "build/bootloader/stm32g473xx.bin"


def default_stm32prog() -> str:
    if sys.platform == "darwin":
        return (
            "/Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer/"
            "STM32CubeProgrammer.app/Contents/Resources/bin/STM32_Programmer_CLI"
        )
    return os.path.expanduser("~/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI")


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


def run_stm32prog(stm32prog: str, args: list[str]) -> None:
    cmd = [stm32prog] + args
    print("+ " + " ".join(cmd))
    subprocess.run(cmd, check=True)


def main() -> int:
    parser = argparse.ArgumentParser(description="Flash the resident ES_BLT bootloader over STM32 ROM UART")
    parser.add_argument("--port", help="Serial device. If omitted, common CP210x paths are autodetected.")
    parser.add_argument("--baud", default=115200, type=int, help="UART baud rate")
    parser.add_argument("--bin", default=DEFAULT_BOOTLOADER_BIN, help=f"Bootloader binary (default: {DEFAULT_BOOTLOADER_BIN})")
    parser.add_argument("--address", default=BOOTLOADER_ADDRESS, help=f"Flash address (default: {BOOTLOADER_ADDRESS})")
    parser.add_argument("--stm32prog", default=default_stm32prog(), help="Path to STM32_Programmer_CLI")
    args = parser.parse_args()

    port = args.port or detect_port()
    if port is None:
        print("No serial port found. Pass --port /dev/cu... or /dev/ttyUSB...", file=sys.stderr)
        return 1
    if not os.path.isfile(args.bin):
        print(f"Binary does not exist: {args.bin}", file=sys.stderr)
        return 1
    if not os.path.isfile(args.stm32prog):
        print(f"STM32_Programmer_CLI does not exist: {args.stm32prog}", file=sys.stderr)
        return 1
    if not os.access(args.stm32prog, os.X_OK):
        print(f"STM32_Programmer_CLI is not executable: {args.stm32prog}", file=sys.stderr)
        return 1

    conn = [f"port={port}", f"br={args.baud}"]
    try:
        run_stm32prog(args.stm32prog, ["-c", *conn, "-w", args.bin, args.address, "-v"])
    except subprocess.CalledProcessError as exc:
        return exc.returncode

    return 0


if __name__ == "__main__":
    sys.exit(main())
