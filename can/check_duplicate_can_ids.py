#!/usr/bin/env python3

import sys
import os
import cantools
from collections import defaultdict
from pathlib import Path


def find_dbc_files(root_dir):
    """
    Recursively find all .dbc files under root_dir.
    """
    return sorted(Path(root_dir).rglob("*.dbc"))


def load_dbc_messages(dbc_path):
    """
    Load a DBC file and return a list of (can_id, message_name).
    """
    try:
        db = cantools.database.load_file(str(dbc_path))
    except Exception as e:
        print(f"ERROR: Failed to load {dbc_path}: {e}")
        sys.exit(1)

    return [(msg.frame_id, msg.name) for msg in db.messages]


def main(dbc_root):
    dbc_files = find_dbc_files(dbc_root)

    if not dbc_files:
        print(f"ERROR: No .dbc files found in {dbc_root}")
        sys.exit(1)

    print(f"Found {len(dbc_files)} DBC file(s):")
    for f in dbc_files:
        print(f"  - {f}")

    # Maps CAN ID -> list of (dbc_file, message_name)
    can_id_map = defaultdict(list)
    duplicates_found = False

    for dbc in dbc_files:
        messages = load_dbc_messages(dbc)

        # Check duplicates within the same file
        seen_in_file = {}
        for can_id, msg_name in messages:
            if can_id in seen_in_file:
                print("\n[DUPLICATE IN FILE]")
                print(f"  File: {dbc}")
                print(f"  CAN ID: 0x{can_id:X}")
                print(f"  Messages: {seen_in_file[can_id]}, {msg_name}")
                duplicates_found = True
            else:
                seen_in_file[can_id] = msg_name

            can_id_map[can_id].append((dbc, msg_name))

    # Check duplicates across files
    for can_id, entries in can_id_map.items():
        dbc_set = {dbc for dbc, _ in entries}
        if len(dbc_set) > 1:
            print("\n[DUPLICATE ACROSS FILES]")
            print(f"  CAN ID: 0x{can_id:X}")
            for dbc, msg_name in entries:
                print(f"    {dbc}: {msg_name}")
            duplicates_found = True

    if duplicates_found:
        print("\nERROR: Duplicate CAN IDs detected.")
        sys.exit(1)

    print("\nSUCCESS: No duplicate CAN IDs found.")
    sys.exit(0)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: check_duplicate_can_ids.py <dbc_directory>")
        sys.exit(1)

    main(sys.argv[1])
