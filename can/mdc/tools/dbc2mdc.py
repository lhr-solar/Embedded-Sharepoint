#!/usr/bin/env python3
"""dbc2mdc — import DBC files into an MDC v3 project document.

Reuses cantools rather than parsing DBC by hand. One DBC file → one MDC network;
a folder of `<vehicle>/<network>.dbc` → one flat-root document with `networks[]`.

Output validates against mdc/schema/mdc.schema.bundle.json. Validate with:
    node mdc/tools/mdc-validate.mjs --schema-only mdc/schema/mdc.schema.bundle.json <out.mdc.json>

Usage:
    python3 mdc/tools/dbc2mdc.py path/to/MotorCAN.dbc [-o out.mdc.json]
    python3 mdc/tools/dbc2mdc.py path/to/HighNoon/ --vehicle-id high_noon [-o out.mdc.json]
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from datetime import datetime, timezone
from pathlib import Path
from typing import Any

try:
    import cantools
except ImportError:
    print("dbc2mdc: install cantools (pip install cantools)", file=sys.stderr)
    sys.exit(1)

SCHEMA_VERSION = "3.0.0"
SCHEMA_REF = "https://lhrsolar.org/lhrs-mdc/3.0.0/mdc.schema.json"
_FLOAT_WIDTHS = {32, 64}  # ponytail: cantools never emits 16-bit float
_DEFAULT_CYCLE_TIME_MS = 1000

# Consumed into native MDC fields — not stored in per-entity attributes.
_NATIVE_ATTRS = frozenset({"VFrameFormat", "GenMsgSendType", "GenMsgCycleTime"})
_SKIP_ATTRS = _NATIVE_ATTRS | frozenset({"DBName", "SPN"})

_DBC_TYPE_TO_MDC = {
    "INT": "int",
    "FLOAT": "float",
    "STRING": "string",
    "ENUM": "enum",
    "HEX": "hex",
    "BOOL": "bool",
}

_KIND_TO_SCOPES = {
    "SG_": ["signal"],
    "BO_": ["message"],
    "BU_": ["node"],
}

_ENV_TYPE_MAP = {
    0: "integer",
    1: "float",
    2: "string",
    3: "data",
}


def sanitize_id(name: str) -> str:
    """Coerce to the MDC `identifier` pattern: ^[A-Za-z_][A-Za-z0-9_-]*$."""
    s = re.sub(r"[^A-Za-z0-9_-]", "_", str(name))
    if not s:
        s = "_"
    if not re.match(r"[A-Za-z_]", s[0]):
        s = "_" + s
    return s[:128]


def _default_value(defn: Any, raw: Any) -> Any:
    if raw is None or raw == "":
        return None
    if defn.type_name == "ENUM" and isinstance(raw, int) and defn.choices:
        choices = defn.choices
        if isinstance(choices, list) and 0 <= raw < len(choices):
            return choices[raw]
    if defn.type_name in ("INT", "HEX"):
        return int(raw)
    if defn.type_name == "FLOAT":
        return float(raw)
    if defn.type_name == "BOOL":
        return bool(raw)
    return raw


def _resolve_attr_value(attr: Any, definitions: dict[str, Any]) -> Any:
    val = attr.value if hasattr(attr, "value") else attr
    defn = attr.definition if hasattr(attr, "definition") else None
    if defn is None:
        return val
    return _default_value(defn, val)


def _attribute_definitions(db: Any) -> list[dict[str, Any]]:
    if not db.dbc or not db.dbc.attribute_definitions:
        return []
    out: list[dict[str, Any]] = []
    for name, defn in sorted(db.dbc.attribute_definitions.items()):
        if name in _SKIP_ATTRS:
            continue
        entry: dict[str, Any] = {
            "name": name,
            "type": _DBC_TYPE_TO_MDC.get(defn.type_name or "", "string"),
            "scopes": _KIND_TO_SCOPES.get(defn.kind, ["network"]),
        }
        if defn.minimum is not None:
            entry["min"] = defn.minimum
        if defn.maximum is not None:
            entry["max"] = defn.maximum
        if defn.choices:
            choices = defn.choices
            if isinstance(choices, dict):
                choices = list(choices.values())
            entry["enumValues"] = list(dict.fromkeys(list(choices)))
        default = _default_value(defn, defn.default_value)
        if default is not None and default != "":
            entry["default"] = default
        out.append(entry)
    return out


def _serialize_attributes(
    attrs: Any, definitions: dict[str, Any]
) -> dict[str, Any] | None:
    if not attrs:
        return None
    out: dict[str, Any] = {}
    for key, attr in attrs.items():
        if key in _SKIP_ATTRS:
            continue
        out[key] = _resolve_attr_value(attr, definitions)
    return out or None


def _extended_mux_enabled(db: Any) -> bool:
    if not db.dbc or not db.dbc.attributes:
        return False
    me = db.dbc.attributes.get("MultiplexExtEnabled")
    if not me:
        return False
    val = _resolve_attr_value(me, db.dbc.attribute_definitions)
    return val in ("Yes", 1, True)


def _choices(sig: Any) -> list[dict[str, Any]]:
    items = [{"value": int(k), "label": str(v)} for k, v in sig.choices.items()]
    items.sort(key=lambda x: x["value"])
    return items


def _expand_mux_ids(ids: Any) -> list[int]:
    """Expand SG_MUL_VAL_ ranges into a discrete id list (ponytail: cantools usually does this)."""
    out: list[int] = []
    for x in ids:
        if isinstance(x, (list, tuple)) and len(x) == 2:
            lo, hi = int(x[0]), int(x[1])
            out.extend(range(lo, hi + 1))
        else:
            out.append(int(x))
    return out


def _apply_scale_offset(out: dict[str, Any], sig: Any) -> None:
    scale = float(sig.scale)
    offset = float(sig.offset)
    if scale != 1.0:
        out["scale"] = scale
    if offset != 0.0:
        out["offset"] = offset
    if sig.choices:
        out["conversion"] = {"kind": "table"}


def _apply_multiplexer(out: dict[str, Any], sig: Any) -> None:
    if getattr(sig, "is_multiplexer", False):
        out["is_multiplexer"] = True
        return
    ids = getattr(sig, "multiplexer_ids", None)
    if ids:
        out["multiplexer_ids"] = _expand_mux_ids(ids)
        mux_signal = getattr(sig, "multiplexer_signal", None)
        if mux_signal:
            mux_name = mux_signal if isinstance(mux_signal, str) else mux_signal.name
            out["multiplexer_signal"] = sanitize_id(mux_name)


def _signal_to_mdc(sig: Any, attr_definitions: dict[str, Any]) -> dict[str, Any]:
    out: dict[str, Any] = {
        "name": sanitize_id(sig.name),
        "start": int(sig.start),
        "length": int(sig.length),
        "byte_order": str(sig.byte_order),
        "is_signed": bool(sig.is_signed),
        "is_float": bool(sig.is_float),
    }
    if out["is_float"] and out["length"] not in _FLOAT_WIDTHS:
        print(
            f"dbc2mdc: warning: signal {sig.name!r} float width {out['length']} "
            f"not in {_FLOAT_WIDTHS}; downgrading to unsigned",
            file=sys.stderr,
        )
        out["is_float"] = False
        out["is_signed"] = False
    _apply_scale_offset(out, sig)
    if sig.comment:
        out["comment"] = str(sig.comment)
    if sig.unit:
        out["unit"] = str(sig.unit)
    if sig.minimum is not None:
        out["minimum"] = float(sig.minimum)
    if sig.maximum is not None:
        out["maximum"] = float(sig.maximum)
    if sig.choices:
        out["choices"] = _choices(sig)
    if sig.receivers:
        out["receivers"] = list(sig.receivers)
    if getattr(sig, "spn", None) is not None:
        out["spn"] = int(sig.spn)
    if getattr(sig, "raw_initial", None) is not None:
        out["raw_initial"] = float(sig.raw_initial)
    if getattr(sig, "raw_invalid", None) is not None:
        out["raw_invalid"] = float(sig.raw_invalid)
    _apply_multiplexer(out, sig)
    if sig.dbc and getattr(sig.dbc, "attributes", None):
        attrs = _serialize_attributes(sig.dbc.attributes, attr_definitions)
        if attrs:
            out["attributes"] = attrs
    return out


def _message_multiplexing(msg: Any, ext_mux: bool) -> dict[str, Any] | None:
    mux_signal = next((s for s in msg.signals if getattr(s, "is_multiplexer", False)), None)
    if mux_signal is None:
        return None
    mux_block: dict[str, Any] = {
        "multiplexed": True,
        "multiplexorSignal": sanitize_id(mux_signal.name),
    }
    if ext_mux:
        mux_block["extended"] = True
    return mux_block


def _signal_groups(msg: Any) -> list[dict[str, Any]] | None:
    groups = getattr(msg, "signal_groups", None)
    if not groups:
        return None
    out: list[dict[str, Any]] = []
    for g in groups:
        entry: dict[str, Any] = {
            "name": sanitize_id(g.name),
            "signal_names": list(g.signal_names),
        }
        if getattr(g, "repetitions", 1) != 1:
            entry["repetitions"] = int(g.repetitions)
        out.append(entry)
    return out or None


def _contained_message_to_mdc(cm: Any, attr_definitions: dict[str, Any]) -> dict[str, Any]:
    out: dict[str, Any] = {
        "name": sanitize_id(cm.name),
        "header_id": int(cm.header_id),
        "length": int(cm.length),
        "signals": [_signal_to_mdc(s, attr_definitions) for s in sorted(cm.signals, key=lambda s: (s.start, s.name))],
    }
    if cm.comment:
        out["comment"] = str(cm.comment)
    if cm.senders:
        out["senders"] = list(cm.senders)
    if cm.dbc and getattr(cm.dbc, "attributes", None):
        attrs = _serialize_attributes(cm.dbc.attributes, attr_definitions)
        if attrs:
            out["attributes"] = attrs
    return out


def _message_to_mdc(
    msg: Any, attr_definitions: dict[str, Any], ext_mux: bool = False
) -> dict[str, Any]:
    signals = sorted(msg.signals, key=lambda s: (s.start, s.name))
    out: dict[str, Any] = {
        "name": sanitize_id(msg.name),
        "frame_id": int(msg.frame_id),
        "is_extended_frame": bool(msg.is_extended_frame),
        "is_fd": bool(msg.is_fd),
        "length": int(msg.length),
        "signals": [_signal_to_mdc(s, attr_definitions) for s in signals],
    }
    if msg.comment:
        out["comment"] = str(msg.comment)
    send_type = getattr(msg, "send_type", None)
    if send_type:
        out["send_type"] = str(send_type)
    if msg.cycle_time is not None:
        out["cycle_time"] = int(msg.cycle_time)
    elif send_type is None or str(send_type).lower() not in {
        "ifactive", "triggered", "event", "nosig", "nosigsend", "spontaneous",
    }:
        out["cycle_time"] = _DEFAULT_CYCLE_TIME_MS
    if msg.senders:
        out["senders"] = list(msg.senders)
    if msg.dbc and getattr(msg.dbc, "attributes", None):
        attrs = _serialize_attributes(msg.dbc.attributes, attr_definitions)
        if attrs:
            out["attributes"] = attrs
    mux_block = _message_multiplexing(msg, ext_mux)
    if mux_block is not None:
        out["multiplexing"] = mux_block
    protocol = getattr(msg, "protocol", None)
    if protocol == "j1939":
        out["protocol"] = "j1939"
    groups = _signal_groups(msg)
    if groups:
        out["signal_groups"] = groups
    if getattr(msg, "is_container", False):
        contained = getattr(msg, "contained_messages", None) or []
        if contained:
            out["contained_messages"] = [
                _contained_message_to_mdc(cm, attr_definitions) for cm in contained
            ]
        header_id = getattr(msg, "header_id", None)
        if header_id is not None:
            out["header_id"] = int(header_id)
    return out


def _env_var_to_mdc(ev: Any) -> dict[str, Any]:
    out: dict[str, Any] = {"name": sanitize_id(ev.name)}
    env_type = getattr(ev, "env_type", None)
    if env_type is not None:
        out["env_type"] = _ENV_TYPE_MAP.get(int(env_type), "integer")
    if ev.minimum is not None:
        out["minimum"] = float(ev.minimum)
    if ev.maximum is not None:
        out["maximum"] = float(ev.maximum)
    if ev.unit:
        out["unit"] = str(ev.unit)
    if getattr(ev, "initial_value", None) is not None:
        out["initial_value"] = float(ev.initial_value)
    if getattr(ev, "env_id", None) is not None:
        out["env_id"] = int(ev.env_id)
    if getattr(ev, "access_type", None):
        out["access_type"] = str(ev.access_type)
    if getattr(ev, "access_node", None):
        out["access_node"] = list(ev.access_node)
    if ev.comment:
        out["comment"] = str(ev.comment)
    return out


def _bus_bitrates(db: Any, is_fd: bool) -> tuple[int | None, int | None]:
    baudrate: int | None = None
    fd_baudrate: int | None = None
    buses = getattr(db, "buses", None) or []
    if buses:
        bus = buses[0]
        if getattr(bus, "baudrate", None):
            baudrate = int(bus.baudrate)
        if getattr(bus, "fd_baudrate", None):
            fd_baudrate = int(bus.fd_baudrate)
    if is_fd and fd_baudrate is None:
        # ponytail: FD inferred from messages but no bus bitrate in DBC
        pass
    return baudrate, fd_baudrate


def _network_from_dbc(dbc_path: Path) -> tuple[dict[str, Any], list[dict[str, Any]]]:
    db = cantools.database.load_file(str(dbc_path))
    attr_definitions = db.dbc.attribute_definitions if db.dbc else {}
    ext_mux = _extended_mux_enabled(db)
    is_fd = any(m.is_fd for m in db.messages)
    baudrate, fd_baudrate = _bus_bitrates(db, is_fd)
    network: dict[str, Any] = {
        "id": sanitize_id(dbc_path.stem),
        "name": dbc_path.stem,
        "filename": dbc_path.name,
        "messages": sorted(
            (_message_to_mdc(m, attr_definitions, ext_mux) for m in db.messages),
            key=lambda x: x["frame_id"],
        ),
    }
    if baudrate is not None:
        network["baudrate"] = baudrate
    if fd_baudrate is not None:
        network["fd_baudrate"] = fd_baudrate
    elif is_fd:
        # ponytail: FD messages present but no fd_baudrate in DBC — omit
        pass
    if db.dbc and getattr(db.dbc, "attributes", None):
        attrs = _serialize_attributes(db.dbc.attributes, attr_definitions)
        if attrs:
            network["attributes"] = attrs
    if db.nodes:
        nodes: list[dict[str, Any]] = []
        for n in db.nodes:
            node: dict[str, Any] = {"name": n.name}
            if n.comment:
                node["comment"] = str(n.comment)
            if n.dbc and getattr(n.dbc, "attributes", None):
                node_attrs = _serialize_attributes(n.dbc.attributes, attr_definitions)
                if node_attrs:
                    node["attributes"] = node_attrs
            nodes.append(node)
        network["nodes"] = nodes
    env_vars = getattr(db, "environment_variables", None)
    if env_vars:
        network["environment_variables"] = [_env_var_to_mdc(ev) for ev in env_vars]
    return network, _attribute_definitions(db)


def _merge_attr_definitions(def_lists: list[list[dict[str, Any]]]) -> list[dict[str, Any]]:
    seen: dict[str, dict[str, Any]] = {}
    for defs in def_lists:
        for d in defs:
            seen.setdefault(d["name"], d)
    return [seen[k] for k in sorted(seen)]


def build_project(
    networks: list[dict[str, Any]],
    vehicle_id: str,
    name: str,
    attribute_definitions: list[dict[str, Any]] | None = None,
) -> dict[str, Any]:
    now = datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")
    project: dict[str, Any] = {
        "$schema": SCHEMA_REF,
        "schemaVersion": SCHEMA_VERSION,
        "id": sanitize_id(vehicle_id),
        "name": name,
        "metadata": {
            "name": name,
            "source": "dbc2mdc",
            "createdAt": now,
            "modifiedAt": now,
        },
        "networks": networks,
    }
    if attribute_definitions:
        project["attributeDefinitions"] = attribute_definitions
    return project


def main() -> None:
    parser = argparse.ArgumentParser(description="Import DBC file(s) into an MDC v3 project.")
    parser.add_argument("input", type=Path, help="A .dbc file or a folder of .dbc files")
    parser.add_argument("-o", "--output", type=Path, help="Output MDC JSON (default: stdout)")
    parser.add_argument("--vehicle-id", help="Root document id (default: derived from input name)")
    args = parser.parse_args()

    if not args.input.exists():
        parser.error(f"input not found: {args.input}")

    display_name = args.input.name if args.input.is_dir() else args.input.stem
    all_attr_defs: list[list[dict[str, Any]]] = []
    if args.input.is_dir():
        dbcs = sorted(args.input.glob("*.dbc"), key=lambda p: p.name.lower())
        if not dbcs:
            parser.error(f"no .dbc files in {args.input}")
        networks = []
        for p in dbcs:
            net, defs = _network_from_dbc(p)
            networks.append(net)
            all_attr_defs.append(defs)
        vehicle_id = args.vehicle_id or display_name
    else:
        net, defs = _network_from_dbc(args.input)
        networks = [net]
        all_attr_defs = [defs]
        vehicle_id = args.vehicle_id or display_name

    attr_defs = _merge_attr_definitions(all_attr_defs)
    project = build_project(
        networks, vehicle_id, name=display_name, attribute_definitions=attr_defs
    )
    text = json.dumps(project, indent=2) + "\n"
    if args.output:
        args.output.write_text(text, encoding="utf-8")
        print(f"Wrote {args.output}", file=sys.stderr)
    else:
        sys.stdout.write(text)


if __name__ == "__main__":
    main()
