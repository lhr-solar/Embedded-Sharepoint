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
    parser = argparse.ArgumentParser(
        description=(
            "Flash the resident ES_BLT bootloader image at the start of internal flash. "
            "This uses STM32CubeProgrammer over UART and expects the STM32 ROM UART "
            "bootloader (BOOT0 high per application note), not the running resident bootloader."
        )
    )
    parser.add_argument("--port", help="Serial device. If omitted, common CP210x paths are autodetected.")
    parser.add_argument("--baud", default=115200, type=int, help="UART baud rate")
    parser.add_argument("--bin", default=default_bootloader_bin(), help="Bootloader binary")
    parser.add_argument("--address", default=BOOTLOADER_ADDRESS, help=f"Flash address (default: {BOOTLOADER_ADDRESS})")
    parser.add_argument("--stm32prog", default=default_stm32prog(), help="Path to STM32_Programmer_CLI")
    parser.add_argument(
        "--assume-rom-bootloader",
        action="store_true",
        help="Do not print the BOOT0 / resident-bootloader note (use when chip is already in ROM UART boot).",
    )
    args = parser.parse_args()

    port = args.port or detect_port()
    if port is None:
        print(port_help(), file=sys.stderr)
        return 1
    erase_hint_printed = False
    if (not args.assume_rom_bootloader) and args.address:
        try:
            addr_i = int(args.address, 0)
        except ValueError:
            addr_i = -1
        if addr_i == int(BOOTLOADER_ADDRESS, 0):
            print(
                "\n"
                "flash_bootloader.py programs 0x08000000, so STM32CubeProgrammer must erase that flash.\n"
                "• Use the STM32 factory ROM UART bootloader: set BOOT0 for System Memory, reset, then run this\n"
                "  script (do not run uart_bootloader_enter.py first—the resident bootloader executes from that\n"
                "  same flash and erase will fail, e.g. \"failed to erase memory\" / sectors [0 N]).\n"
                "• Or use ST-Link: make -C test flash ... FIRMWARE_TYPE=bootloader\n"
                "Pass --assume-rom-bootloader to hide this message.\n",
                file=sys.stderr,
            )
            erase_hint_printed = True

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
