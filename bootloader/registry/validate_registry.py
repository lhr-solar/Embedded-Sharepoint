#!/usr/bin/env python3
"""Validate a bootloader node registry YAML file.

Checks device_id uniqueness (per bus), range (0..119), board_type enum, optional
96-bit UID format, and optional `bus`. Does not require jsonschema; uses plain
Python only plus PyYAML for parsing.

Optionally cross-checks the multi-FDCAN bridge filter coverage: pass one or more
`--recv BUS=FILE` mappings and every node whose `bus` equals BUS must have a
RESP + STATUS receive filter (a `BL_RECV_ENTRIES_CHILD(<device_id>)`, or the
equivalent explicit CAN_RECV_ENTRY pair) in that bus's recv-entries header. This
catches a child node that was added to the registry but never given a hardware
filter on the gateway, so its replies would be dropped before forwarding.

Usage:
    validate_registry.py [registry.yaml] [--recv BUS=FILE ...]

Default registry path: bootloader/registry/example.yaml (relative to repo root
when invoked from repo root).
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

import yaml

ALLOWED_BOARD_TYPES = frozenset({"stm32g491vet", "stm32g431cbt"})
DEVICE_ID_MIN = 0
DEVICE_ID_MAX = 119
UID_PATTERN = re.compile(r"^[0-9A-Fa-f]{24}$")

DEFAULT_REGISTRY = Path(__file__).resolve().parent / "example.yaml"

# Child filter coverage in a recv-entries header. BL_RECV_ENTRIES_CHILD(dev)
# expands to both a RESP and a STATUS filter; the explicit forms cover one each.
_CHILD_MACRO_RE = re.compile(r"BL_RECV_ENTRIES_CHILD\s*\(\s*(\d+)\s*\)")
_RESP_RE = re.compile(r"BL_CAN_ID_RESP_BASE\s*\+\s*\(?\s*(\d+)")
_STATUS_RE = re.compile(r"BL_CAN_ID_STATUS_BASE\s*\+\s*\(?\s*(\d+)")
_COMMENT_RE = re.compile(r"//.*?$|/\*.*?\*/", re.MULTILINE | re.DOTALL)


def _fail(messages: list[str]) -> int:
    for msg in messages:
        print(f"FAIL: {msg}")
    return 1


def _extract_nodes(data: object) -> list[object]:
    if isinstance(data, list):
        return data
    if isinstance(data, dict) and "nodes" in data:
        nodes = data["nodes"]
        if isinstance(nodes, list):
            return nodes
        return []
    return []


def parse_recv_entries(path: Path) -> tuple[set[int], set[int]]:
    """Return (resp_dev_ids, status_dev_ids) covered by a recv-entries header."""
    text = _COMMENT_RE.sub("", path.read_text(encoding="utf-8"))
    resp = {int(m) for m in _RESP_RE.findall(text)}
    status = {int(m) for m in _STATUS_RE.findall(text)}
    for dev in (int(m) for m in _CHILD_MACRO_RE.findall(text)):
        resp.add(dev)
        status.add(dev)
    return resp, status


def validate_registry(
    path: Path, recv_map: dict[str, Path] | None = None
) -> tuple[bool, list[str]]:
    errors: list[str] = []
    recv_map = recv_map or {}

    if not path.is_file():
        return False, [f"file not found: {path}"]

    try:
        with path.open(encoding="utf-8") as f:
            data = yaml.safe_load(f)
    except yaml.YAMLError as exc:
        return False, [f"YAML parse error: {exc}"]

    if data is None:
        return False, ["registry is empty"]

    nodes = _extract_nodes(data)
    if not nodes:
        return False, ["no nodes found (expected top-level list or object with 'nodes' array)"]

    # (bus, device_id) -> first index, so device_ids are unique per bus.
    seen_ids: dict[tuple[str | None, int], int] = {}
    # (index, device_id, bus) for nodes that pass basic checks and name a bus.
    bus_nodes: list[tuple[int, int, str]] = []

    for index, node in enumerate(nodes):
        prefix = f"nodes[{index}]"

        if not isinstance(node, dict):
            errors.append(f"{prefix}: expected object, got {type(node).__name__}")
            continue

        if "device_id" not in node:
            errors.append(f"{prefix}: missing required field 'device_id'")
            continue
        if "board_type" not in node:
            errors.append(f"{prefix}: missing required field 'board_type'")
            continue

        device_id = node["device_id"]
        if not isinstance(device_id, int) or isinstance(device_id, bool):
            errors.append(f"{prefix}: device_id must be integer, got {device_id!r}")
            continue

        if not (DEVICE_ID_MIN <= device_id <= DEVICE_ID_MAX):
            errors.append(
                f"{prefix}: device_id {device_id} out of range "
                f"[{DEVICE_ID_MIN}..{DEVICE_ID_MAX}]"
            )

        board_type = node["board_type"]
        if not isinstance(board_type, str) or board_type not in ALLOWED_BOARD_TYPES:
            errors.append(
                f"{prefix}: board_type {board_type!r} not in "
                f"{sorted(ALLOWED_BOARD_TYPES)}"
            )

        if "uid" in node and node["uid"] is not None:
            uid = node["uid"]
            if not isinstance(uid, str) or not UID_PATTERN.match(uid):
                errors.append(
                    f"{prefix}: uid must be 24 hex characters, got {uid!r}"
                )

        bus = None
        if "bus" in node and node["bus"] is not None:
            bus = node["bus"]
            if not isinstance(bus, str):
                errors.append(f"{prefix}: bus must be a string, got {bus!r}")
                bus = None

        if isinstance(device_id, int) and not isinstance(device_id, bool):
            key = (bus, device_id)
            if key in seen_ids:
                prev = seen_ids[key]
                where = f"bus {bus}" if bus is not None else "no bus"
                errors.append(
                    f"duplicate device_id {device_id} ({where}) at "
                    f"nodes[{prev}] and nodes[{index}]"
                )
            else:
                seen_ids[key] = index
            if bus is not None:
                bus_nodes.append((index, device_id, bus))

    errors.extend(_check_recv_coverage(bus_nodes, recv_map))

    return len(errors) == 0, errors


def _check_recv_coverage(
    bus_nodes: list[tuple[int, int, str]], recv_map: dict[str, Path]
) -> list[str]:
    """For each --recv BUS=FILE, every registry node on BUS must have RESP+STATUS."""
    errors: list[str] = []
    coverage: dict[str, tuple[set[int], set[int]]] = {}

    for bus, file_path in recv_map.items():
        if not file_path.is_file():
            errors.append(f"--recv {bus}: file not found: {file_path}")
            continue
        coverage[bus] = parse_recv_entries(file_path)

    for index, device_id, bus in bus_nodes:
        if bus not in coverage:
            continue  # no recv-entries file supplied for this bus -> not checked
        resp_ids, status_ids = coverage[bus]
        missing = []
        if device_id not in resp_ids:
            missing.append("RESP")
        if device_id not in status_ids:
            missing.append("STATUS")
        if missing:
            errors.append(
                f"nodes[{index}]: device_id {device_id} on bus {bus} is missing "
                f"{'/'.join(missing)} filter(s) in {recv_map[bus]} "
                f"(add BL_RECV_ENTRIES_CHILD({device_id}))"
            )

    return errors


def _parse_recv_arg(value: str) -> tuple[str, Path]:
    if "=" not in value:
        raise argparse.ArgumentTypeError(f"expected BUS=FILE, got {value!r}")
    bus, _, file_str = value.partition("=")
    bus = bus.strip()
    file_str = file_str.strip()
    if not bus or not file_str:
        raise argparse.ArgumentTypeError(f"expected BUS=FILE, got {value!r}")
    return bus, Path(file_str)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Validate a bootloader node registry.")
    parser.add_argument(
        "registry",
        nargs="?",
        type=Path,
        default=DEFAULT_REGISTRY,
        help="registry YAML (default: bootloader/registry/example.yaml)",
    )
    parser.add_argument(
        "--recv",
        metavar="BUS=FILE",
        action="append",
        default=[],
        type=_parse_recv_arg,
        help="cross-check that nodes on BUS have RESP/STATUS filters in recv-entries FILE",
    )
    args = parser.parse_args(argv)

    recv_map = dict(args.recv)

    ok, errors = validate_registry(args.registry, recv_map)
    if ok:
        print(f"PASS: {args.registry}")
        return 0
    return _fail(errors)


if __name__ == "__main__":
    sys.exit(main())
