#!/usr/bin/env python3
"""Flash a firmware .bin over USB DFU by wrapping the `dfu-util` binary.

Targets the STM32 system/ROM bootloader (VID:PID 0483:df11). The device must
ALREADY be in ROM-DFU before you run this (the app calls EnterRomDfu(),
or use a BOOT0 strap + reset). This tool only performs the transfer; it cannot
put the device into DFU.

Only the standard library is used. `dfu-util` is located lazily via shutil.which
at execution time, so --help and --dry-run work with nothing installed.

Usage:
    dfu_flash.py --list
    dfu_flash.py --bin app.bin --slot A --target stm32g491vet --dry-run
    dfu_flash.py --bin app.bin --address 0x08004000
"""

from __future__ import annotations

import argparse
import shlex
import shutil
import subprocess
import sys
from pathlib import Path

DFU_UTIL = "dfu-util"
STM_DFU_ID = "0483:df11"  # STM32 system bootloader VID:PID

# Default app base per target/slot (canonical: bootloader/memory_map/*.json).
APP_BASES = {
    "stm32g491vet": {"A": 0x08004000, "B": 0x08040000},
    "stm32g431cbt": {"A": 0x08002000},  # single app slot
}


def _hex_addr(text: str) -> int:
    try:
        return int(text, 16)
    except ValueError:
        raise argparse.ArgumentTypeError(f"invalid hex address: {text!r}")


def resolve_address(args):
    """Return (address, error). --address wins; else --slot + --target."""
    if args.address is not None:
        return args.address, None
    if args.slot and args.target:
        slots = APP_BASES[args.target]
        if args.slot not in slots:
            valid = ", ".join(sorted(slots))
            return None, f"{args.target} has no slot {args.slot} (valid: {valid})"
        return slots[args.slot], None
    return None, "need --address HEX, or --slot {A,B} together with --target"


def build_parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(
        prog="dfu_flash.py",
        description="Flash a firmware .bin over USB DFU by wrapping dfu-util, "
                    "targeting the STM32 system bootloader (VID:PID 0483:df11).",
        epilog="The device must ALREADY be in ROM-DFU before running this tool "
               "(the app calls EnterRomDfu(), or use a BOOT0 strap + "
               "reset). This tool only performs the transfer.")
    p.add_argument("--bin", metavar="PATH", help="firmware .bin to download")
    p.add_argument("--address", type=_hex_addr, metavar="HEX",
                   help="flash address, e.g. 0x08004000")
    p.add_argument("--slot", choices=["A", "B"],
                   help="app slot; resolved with --target to a default base")
    p.add_argument("--target", choices=sorted(APP_BASES),
                   help="MCU target used to resolve --slot to a base address")
    p.add_argument("--alt", type=int, default=0,
                   help="DFU alt setting (default: 0)")
    p.add_argument("--serial", help="match a specific device by DFU serial")
    p.add_argument("--list", action="store_true",
                   help="list attached DFU devices (dfu-util -l) and exit")
    p.add_argument("--dry-run", action="store_true",
                   help="print the dfu-util command without running it")
    p.add_argument("--verbose", action="store_true",
                   help="print the resolved address before running")
    return p


def _fail(msg: str) -> int:
    print(f"error: {msg}", file=sys.stderr)
    return 1


def main(argv=None) -> int:
    args = build_parser().parse_args(argv)

    if args.list:
        cmd = [DFU_UTIL, "-l"]
    else:
        if not args.bin:
            return _fail("--bin PATH is required (or use --list)")
        address, err = resolve_address(args)
        if err:
            return _fail(err)
        if args.verbose:
            print(f"resolved address: 0x{address:08X} (alt {args.alt})")
        cmd = ([DFU_UTIL, "-d", STM_DFU_ID]
               + (["-S", args.serial] if args.serial else [])
               + ["-a", str(args.alt), "-s", f"0x{address:08X}:leave", "-D", args.bin])

    print(f"+ {shlex.join(cmd)}")

    if args.dry_run:
        return 0

    if shutil.which(DFU_UTIL) is None:
        return _fail("'dfu-util' not found on PATH. Enter 'nix develop' (provides "
                     "dfu-util), or install it (macOS: brew install dfu-util; "
                     "Debian/Ubuntu: sudo apt install dfu-util).")
    if not args.list and not Path(args.bin).is_file():
        return _fail(f"--bin not found: {args.bin}")

    return subprocess.run(cmd).returncode


if __name__ == "__main__":
    sys.exit(main())
