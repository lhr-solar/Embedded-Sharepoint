#!/usr/bin/env python3
"""Check for duplicate CAN frame IDs across MDC vehicle databases."""

import json
import sys
from pathlib import Path


def find_mdc_files(root_dir):
    return sorted(Path(root_dir).rglob("*.mdc.json"))


def load_mdc_messages(mdc_path: Path):
    try:
        doc = json.loads(mdc_path.read_text(encoding="utf-8"))
    except Exception as e:
        print(f"ERROR: Failed to load {mdc_path}: {e}")
        sys.exit(1)

    messages = []
    for network in doc.get("networks", []):
        net_id = network.get("id", "?")
        for msg in network.get("messages", []):
            messages.append((net_id, int(msg["frame_id"]), msg.get("name", "?")))
    return messages


def main(mdc_root):
    # Cross-file duplicates are not checked; each file is a separate vehicle and
    # cross-vehicle ID collision is allowed.
    mdc_files = find_mdc_files(mdc_root)
    if not mdc_files:
        print(f"ERROR: No .mdc.json files found in {mdc_root}")
        sys.exit(1)

    print(f"Found {len(mdc_files)} MDC file(s):")
    for f in mdc_files:
        print(f"  - {f}")

    duplicates_found = False

    for mdc in mdc_files:
        messages = load_mdc_messages(mdc)
        seen_in_file = {}
        for net_id, can_id, msg_name in messages:
            key = f"{net_id}:{can_id}"
            if key in seen_in_file:
                print("\n[DUPLICATE IN FILE]")
                print(f"  File: {mdc}")
                print(f"  Network/CAN ID: {net_id} / 0x{can_id:X}")
                print(f"  Messages: {seen_in_file[key]}, {msg_name}")
                duplicates_found = True
            else:
                seen_in_file[key] = msg_name

    if duplicates_found:
        print("\nERROR: Duplicate CAN IDs detected.")
        sys.exit(1)

    print("\nSUCCESS: No duplicate CAN IDs found.")
    sys.exit(0)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: check_duplicate_can_ids.py <vehicles_directory>")
        sys.exit(1)
    main(sys.argv[1])
