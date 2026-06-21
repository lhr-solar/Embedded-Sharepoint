#!/usr/bin/env python3
"""mdc2dbc — best-effort export of an MDC v3 document to DBC via cantools.

Lossy MDC-only features are dropped with a warning per contract §8.2.

Usage:
    python3 mdc/tools/mdc2dbc.py path/to/project.mdc.json [-o out.dbc]
    python3 mdc/tools/mdc2dbc.py path/to/vehicle_dir/ [-o out_dir/]
"""
from __future__ import annotations

import argparse
import json
import sys
from collections import OrderedDict
from pathlib import Path
from typing import Any

try:
    import cantools
    from cantools.database.can import Bus, Database, Message, Node, Signal
    from cantools.database.can.environment_variable import EnvironmentVariable
    from cantools.database.can.signal_group import SignalGroup
    from cantools.database.can.formats.dbc_specifics import DbcSpecifics
    from cantools.database.conversion import LinearConversion, NamedSignalConversion
except ImportError:
    print("mdc2dbc: install cantools (pip install cantools)", file=sys.stderr)
    sys.exit(1)


def warn(msg: str) -> None:
    print(f"mdc2dbc: warning: {msg}", file=sys.stderr)


def _resolve_choices(sig: dict[str, Any], project: dict[str, Any], network: dict[str, Any]) -> dict[int, str] | None:
    if sig.get("choices"):
        return {int(c["value"]): str(c["label"]) for c in sig["choices"]}
    ref = sig.get("valueTableRef")
    if not ref:
        return None
    for table in (network.get("valueTables") or []) + (project.get("valueTables") or []):
        if table.get("name") == ref:
            return {int(e["value"]): str(e["label"]) for e in table.get("entries", [])}
    warn(f"valueTableRef {ref!r} on signal {sig.get('name')!r} unresolved — choices omitted")
    return None


def _mdc_signal_to_cantools(
    sig: dict[str, Any],
    project: dict[str, Any],
    network: dict[str, Any],
    *,
    path: str,
) -> Signal:
    if sig.get("conversion", {}).get("kind") == "rational":
        warn(f"{path}: rational conversion dropped (not DBC-expressible); exporting scale/offset only")
    if sig.get("display"):
        warn(f"{path}: display dropped (not DBC-expressible)")
    if sig.get("alarms"):
        warn(f"{path}: alarms dropped (not DBC-expressible)")
    if sig.get("raw_initial") is not None or sig.get("raw_invalid") is not None:
        warn(f"{path}: raw_initial/raw_invalid dropped (limited DBC round-trip)")
    if sig.get("invalid") is not None:
        warn(f"{path}: invalid dropped (not DBC-expressible)")

    scale = float(sig.get("scale", 1))
    offset = float(sig.get("offset", 0))
    is_float = bool(sig.get("is_float", False))
    choices = _resolve_choices(sig, project, network)
    if choices:
        conversion = NamedSignalConversion(scale=scale, offset=offset, choices=choices, is_float=is_float)
    else:
        conversion = LinearConversion(scale=scale, offset=offset, is_float=is_float)

    kwargs: dict[str, Any] = {
        "name": sig["name"],
        "start": int(sig["start"]),
        "length": int(sig["length"]),
        "byte_order": sig.get("byte_order", "little_endian"),
        "is_signed": bool(sig.get("is_signed", False)),
        "conversion": conversion,
        "minimum": sig.get("minimum"),
        "maximum": sig.get("maximum"),
        "unit": sig.get("unit") or None,
        "comment": sig.get("comment"),
        "receivers": sig.get("receivers") or [],
        "spn": sig.get("spn"),
    }
    if sig.get("is_multiplexer"):
        kwargs["is_multiplexer"] = True
    if sig.get("multiplexer_signal"):
        kwargs["multiplexer_signal"] = sig["multiplexer_signal"]
    if sig.get("multiplexer_ids"):
        kwargs["multiplexer_ids"] = list(sig["multiplexer_ids"])
    if sig.get("initial") is not None:
        kwargs["initial"] = sig["initial"]
    return Signal(**{k: v for k, v in kwargs.items() if v is not None})


def _mdc_message_to_cantools(
    msg: dict[str, Any],
    project: dict[str, Any],
    network: dict[str, Any],
    *,
    path: str,
) -> Message:
    if msg.get("computedSignals"):
        warn(f"{path}: computedSignals dropped (not DBC-expressible)")
    if msg.get("display"):
        warn(f"{path}: display dropped (not DBC-expressible)")
    if msg.get("array"):
        warn(f"{path}: array dropped (not DBC-expressible)")
    if msg.get("transport"):
        warn(f"{path}: transport dropped (not DBC-expressible)")
    if msg.get("header_id") is not None or msg.get("contained_messages"):
        warn(f"{path}: header_id/contained_messages dropped (not DBC-expressible)")
    if msg.get("multiplexing"):
        warn(f"{path}: message.multiplexing extension dropped (cantools infers from signals)")

    signals = [
        _mdc_signal_to_cantools(s, project, network, path=f"{path}/signals/{s['name']}")
        for s in msg.get("signals", [])
    ]
    signal_groups = None
    if msg.get("signal_groups"):
        signal_groups = [
            SignalGroup(
                name=g["name"],
                repetitions=int(g.get("repetitions", 1)),
                signal_names=list(g["signal_names"]),
            )
            for g in msg["signal_groups"]
        ]

    kwargs: dict[str, Any] = {
        "frame_id": int(msg["frame_id"]),
        "name": msg["name"],
        "length": int(msg["length"]),
        "is_extended_frame": bool(msg.get("is_extended_frame", False)),
        "is_fd": bool(msg.get("is_fd", False)),
        "signals": signals,
        "senders": msg.get("senders") or [],
        "comment": msg.get("comment"),
        "send_type": msg.get("send_type"),
        "cycle_time": msg.get("cycle_time"),
        "signal_groups": signal_groups,
    }
    if msg.get("protocol") == "j1939":
        kwargs["protocol"] = "j1939"
    return Message(**{k: v for k, v in kwargs.items() if v is not None})


def _build_database(project: dict[str, Any], network: dict[str, Any]) -> Database:
    net_path = f"networks/{network['id']}"
    if network.get("computedSignals"):
        warn(f"{net_path}: computedSignals dropped (not DBC-expressible)")
    if network.get("samplePoint") is not None:
        warn(f"{net_path}: samplePoint dropped (not DBC-expressible)")

    messages = [
        _mdc_message_to_cantools(m, project, network, path=f"{net_path}/messages/{m['name']}")
        for m in network.get("messages", [])
    ]
    nodes = [Node(name=n["name"], comment=n.get("comment")) for n in network.get("nodes", [])]

    buses = []
    if network.get("baudrate") or network.get("fd_baudrate"):
        buses.append(
            Bus(
                name=network.get("name", network["id"]),
                comment=network.get("comment"),
                baudrate=network.get("baudrate"),
                fd_baudrate=network.get("fd_baudrate"),
            )
        )

    env_vars = []
    for ev in network.get("environment_variables", []):
        type_map = {"integer": 0, "float": 1, "string": 2, "data": 3}
        env_vars.append(
            EnvironmentVariable(
                name=ev["name"],
                env_type=type_map.get(ev.get("env_type", "integer"), 0),
                minimum=ev.get("minimum"),
                maximum=ev.get("maximum"),
                unit=ev.get("unit"),
                initial_value=ev.get("initial_value"),
                env_id=ev.get("env_id"),
                access_type=ev.get("access_type"),
                access_node=ev.get("access_node"),
                comment=ev.get("comment"),
            )
        )

    dbc_specifics = None
    if env_vars:
        dbc_specifics = DbcSpecifics(
            environment_variables=OrderedDict((ev.name, ev) for ev in env_vars)
        )

    db = Database(
        messages=messages,
        nodes=nodes,
        buses=buses or None,
        dbc_specifics=dbc_specifics,
    )
    return db


def export_network(project: dict[str, Any], network: dict[str, Any], out_path: Path) -> None:
    db = _build_database(project, network)
    cantools.database.dump_file(db, str(out_path))
    print(f"Wrote {out_path}", file=sys.stderr)


def load_project(path: Path) -> dict[str, Any]:
    if path.is_dir():
        candidates = sorted(path.glob("*.mdc.json"))
        if not candidates:
            raise FileNotFoundError(f"no .mdc.json in {path}")
        # ponytail: prefer <dirname>.mdc.json or project.mdc.json
        preferred = path / f"{path.name}.mdc.json"
        if preferred.is_file():
            return json.loads(preferred.read_text(encoding="utf-8"))
        project_file = path / "project.mdc.json"
        if project_file.is_file():
            return json.loads(project_file.read_text(encoding="utf-8"))
        return json.loads(candidates[0].read_text(encoding="utf-8"))
    return json.loads(path.read_text(encoding="utf-8"))


def main() -> None:
    parser = argparse.ArgumentParser(description="Export MDC v3 to DBC (best-effort).")
    parser.add_argument("input", type=Path, help="project.mdc.json or vehicle folder")
    parser.add_argument("-o", "--output", type=Path, help="Output .dbc file or directory")
    args = parser.parse_args()

    if not args.input.exists():
        parser.error(f"input not found: {args.input}")

    project = load_project(args.input)
    networks = project.get("networks") or []
    if not networks:
        parser.error("no networks[] in MDC document")

    if len(networks) == 1 and args.output and args.output.suffix.lower() == ".dbc":
        export_network(project, networks[0], args.output)
        return

    out_dir = args.output or (args.input if args.input.is_dir() else args.input.parent)
    out_dir.mkdir(parents=True, exist_ok=True)
    for net in networks:
        filename = net.get("filename", f"{net['id']}.dbc")
        # filename may be a source name like "powertrain.mdc.json"; strip a known
        # suffix so we don't emit "powertrain.mdc.json.dbc".
        for suffix in (".mdc.json", ".json", ".dbc"):
            if filename.endswith(suffix):
                stem = filename[: -len(suffix)]
                break
        else:
            stem = filename
        out_file = out_dir / f"{stem}.dbc"
        export_network(project, net, out_file)


if __name__ == "__main__":
    main()
