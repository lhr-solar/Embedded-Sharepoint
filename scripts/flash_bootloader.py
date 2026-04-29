#!/usr/bin/env python3
import argparse
import sys

from bootloader_script_utils import (
    default_binary,
    default_stm32prog,
    detect_port,
    normalize_port_for_stm32prog,
    port_help,
    run_stm32prog,
    validate_file,
)

BOOTLOADER_ADDRESS = "0x08000000"
DEFAULT_BOOTLOADER_BIN_GLOB = "build/bootloader/stm*.bin"


def default_bootloader_bin() -> str:
    return default_binary(DEFAULT_BOOTLOADER_BIN_GLOB)


def main() -> int:
    parser = argparse.ArgumentParser(description="Flash the resident ES_BLT bootloader over STM32 ROM UART")
    parser.add_argument("--port", help="Serial device. If omitted, common CP210x paths are autodetected.")
    parser.add_argument("--baud", default=115200, type=int, help="UART baud rate")
    parser.add_argument("--bin", default=default_bootloader_bin(), help="Bootloader binary")
    parser.add_argument("--address", default=BOOTLOADER_ADDRESS, help=f"Flash address (default: {BOOTLOADER_ADDRESS})")
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
        run_stm32prog(args.stm32prog, ["-c", *conn, "-w", args.bin, args.address, "-v"])
    except Exception as exc:
        returncode = getattr(exc, "returncode", None)
        if isinstance(returncode, int):
            return returncode
        print(f"STM32_Programmer_CLI failed: {exc}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
