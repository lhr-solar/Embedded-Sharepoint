#!/usr/bin/env python3
"""
Check CAN message ID uniqueness in a DBC directory.

Duplicate IDs within a single DBC file are always errors.

Duplicate IDs across different DBC files are errors for all buses except DAqCAN.
Overlaps involving DAqCAN are warnings only, since telem/DAQ runs on a separate bus
with its own Photon ID scheme (+0x800).

Message IDs are parsed directly from BO_ lines so this check does not depend on
cantools being able to load every DBC.
"""

import argparse
import re
import sys
from collections import defaultdict
from itertools import combinations
from pathlib import Path

BO_PATTERN = re.compile(r"^BO_\s+(\d+)\s+(\S+)")
DAQ_CAN_DBC = "DAqCAN.dbc"


def find_dbc_files(root_dir: Path) -> list[Path]:
    return sorted(root_dir.rglob("*.dbc"))


def parse_dbc_messages(dbc_path: Path) -> list[tuple[int, str]]:
    messages: list[tuple[int, str]] = []

    try:
        content = dbc_path.read_text(encoding="utf-8", errors="replace")
    except OSError as exc:
        print(f"ERROR: Failed to read {dbc_path}: {exc}")
        sys.exit(1)

    for line in content.splitlines():
        match = BO_PATTERN.match(line.strip())
        if match:
            can_id = int(match.group(1))
            msg_name = match.group(2).rstrip(":")
            messages.append((can_id, msg_name))

    if not messages:
        print(f"ERROR: No BO_ message definitions found in {dbc_path}")
        sys.exit(1)

    return messages


def format_id(can_id: int) -> str:
    return f"0x{can_id:X} ({can_id})"


def involves_daq_can(dbc_files: set[Path]) -> bool:
    return any(dbc_file.name == DAQ_CAN_DBC for dbc_file in dbc_files)


def report_overlap(
    can_id: int,
    entries: list[tuple[Path, str]],
    *,
    warning: bool,
) -> None:
    label = "WARNING" if warning else "ERROR"
    print(f"\n[{label}: DUPLICATE ACROSS BUSES]")
    print(f"  CAN ID: {format_id(can_id)}")
    if warning:
        print(f"  Overlap involves {DAQ_CAN_DBC}; allowed on separate telem bus.")
    else:
        print("  Duplicate IDs are not allowed across non-telem CAN buses.")
    for dbc_file, msg_name in entries:
        print(f"    {dbc_file.name}: {msg_name}")


def main(dbc_root: Path, strict_daq: bool) -> None:
    dbc_files = find_dbc_files(dbc_root)

    if not dbc_files:
        print(f"ERROR: No .dbc files found in {dbc_root}")
        sys.exit(1)

    print(f"Found {len(dbc_files)} DBC file(s):")
    for dbc_file in dbc_files:
        print(f"  - {dbc_file}")

    can_id_map: dict[int, list[tuple[Path, str]]] = defaultdict(list)
    within_file_errors = False

    for dbc_file in dbc_files:
        messages = parse_dbc_messages(dbc_file)
        seen_in_file: dict[int, str] = {}

        for can_id, msg_name in messages:
            if can_id in seen_in_file:
                print("\n[ERROR: DUPLICATE IN FILE]")
                print(f"  File: {dbc_file}")
                print(f"  CAN ID: {format_id(can_id)}")
                print(f"  Messages: {seen_in_file[can_id]}, {msg_name}")
                within_file_errors = True
            else:
                seen_in_file[can_id] = msg_name

            can_id_map[can_id].append((dbc_file, msg_name))

    cross_bus_errors = False
    cross_bus_warnings = False
    reported_pairs: set[tuple[int, frozenset[Path]]] = set()

    for can_id, entries in sorted(can_id_map.items()):
        by_file: dict[Path, str] = {}
        for dbc_file, msg_name in entries:
            by_file[dbc_file] = msg_name

        if len(by_file) <= 1:
            continue

        for dbc_a, dbc_b in combinations(sorted(by_file, key=lambda p: p.name), 2):
            pair_key = (can_id, frozenset((dbc_a, dbc_b)))
            if pair_key in reported_pairs:
                continue
            reported_pairs.add(pair_key)

            pair_entries = [
                (dbc_a, by_file[dbc_a]),
                (dbc_b, by_file[dbc_b]),
            ]
            is_daq_overlap = involves_daq_can({dbc_a, dbc_b})

            if is_daq_overlap and not strict_daq:
                cross_bus_warnings = True
                report_overlap(can_id, pair_entries, warning=True)
            else:
                cross_bus_errors = True
                report_overlap(can_id, pair_entries, warning=False)

    if within_file_errors or cross_bus_errors:
        print("\nERROR: Duplicate CAN IDs detected.")
        sys.exit(1)

    if cross_bus_warnings:
        print("\nSUCCESS: No blocking duplicate CAN IDs found.")
        print(f"NOTE: Cross-bus overlaps involving {DAQ_CAN_DBC} were warnings only.")
        sys.exit(0)

    print("\nSUCCESS: No duplicate CAN IDs found.")
    sys.exit(0)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Check CAN ID uniqueness in DBC files."
    )
    parser.add_argument("dbc_directory", help="Root directory to scan for .dbc files")
    parser.add_argument(
        "--strict-daq",
        action="store_true",
        help=f"Also fail on duplicate IDs involving {DAQ_CAN_DBC}",
    )
    args = parser.parse_args()

    main(Path(args.dbc_directory), args.strict_daq)
