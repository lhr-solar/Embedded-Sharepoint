#!/usr/bin/env python3
import argparse
import os
import sys
import subprocess


APP_BASE = "0x08010000"


def default_stm32prog() -> str:
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


def main() -> int:
    parser = argparse.ArgumentParser(description="Flash app via ES_BLT AN3155-compatible UART bootloader")
    parser.add_argument("--port", required=True, help="Serial device (ex: /dev/tty.usbserial-310)")
    parser.add_argument("--baud", default=115200, type=int, help="UART baud rate")
    parser.add_argument("--bin", required=True, help="App binary to flash")
    parser.add_argument("--address", default=APP_BASE, help=f"Flash address (default: {APP_BASE})")
    parser.add_argument("--boot", action="store_true", help="Jump to app after flashing")
    parser.add_argument("--stm32prog", default=default_stm32prog(), help="Path to STM32_Programmer_CLI")
    args = parser.parse_args()

    if not os.path.isfile(args.bin):
        print(f"Binary does not exist: {args.bin}")
        return 1
    if not os.path.isfile(args.stm32prog):
        print(f"STM32_Programmer_CLI does not exist: {args.stm32prog}")
        return 1
    if not os.access(args.stm32prog, os.X_OK):
        print(f"STM32_Programmer_CLI is not executable: {args.stm32prog}")
        return 1

    conn = [f"port={args.port}", f"br={args.baud}"]
    try:
        run_stm32prog(args.stm32prog, ["-c", *conn, "-w", args.bin, args.address, "-v"])
        if args.boot:
            run_stm32prog(args.stm32prog, ["-c", *conn, "-g", args.address])
    except subprocess.CalledProcessError as exc:
        return exc.returncode

    return 0


if __name__ == "__main__":
    sys.exit(main())
