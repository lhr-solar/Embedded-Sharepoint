#!/usr/bin/env python3
"""dbc2mdc — import DBC files into an MDC project document.

Reuses cantools (same loader as Embedded-Sharepoint/can/.../export_canspec_json.py)
rather than parsing DBC by hand. One DBC file → one MDC network; a folder of
`<vehicle>/<network>.dbc` → one vehicle with several networks.

Output validates against mdc/schema/mdc.schema.json. Validate the result with:
    node mdc/tools/mdc-validate.mjs --schema-only mdc/schema/mdc.schema.bundle.json <out.mdc.json>

Usage:
    # one DBC -> a single-network project on stdout (or -o file)
    python3 mdc/tools/dbc2mdc.py path/to/MotorCAN.dbc [-o out.mdc.json]
    # a vehicle folder of DBCs -> one vehicle with multiple networks
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

SCHEMA_VERSION = "2.0.0"
_FLOAT_WIDTHS = {32, 64}  # ponytail: cantools never emits 16-bit float
_DEFAULT_CYCLE_TIME_MS = 1000

# DBC GenMsgSendType → MDC transmissionType. Periodic variants count toward bus
# load; event/on-change variants are excluded. Unknown values default to cyclic.
_SEND_TYPE_TRIGGERED = {"ifactive", "triggered", "event", "nosig", "nosigsend", "spontaneous"}

# Consumed into native MDC fields — not stored in per-entity attributes.
_NATIVE_ATTRS = frozenset({"VFrameFormat", "GenMsgSendType", "GenMsgCycleTime"})
_SKIP_ATTRS = _NATIVE_ATTRS | frozenset({"DBName"})

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


def sanitize_id(name: str) -> str:
    """Coerce to the MDC `identifier` pattern: ^[A-Za-z_][A-Za-z0-9_-]*$.

    Intentionally diverges from mdc2cheaders.mjs `sanitize` (C identifiers, no `-`, no 128-cap).
    """
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
    # cantools Attribute objects always carry .definition; the definitions.get
    # fallback only handles raw scalars passed without a definition.
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


def _signal_kind(sig: Any) -> str:
    if sig.is_float:
        return "float"
    return "signed" if sig.is_signed else "unsigned"


def _conversion(sig: Any) -> dict[str, Any]:
    """Map cantools scale/offset/choices to an MDC conversion object.

    Table conversions intentionally omit scale/offset — only discrete value labels survive.
    """
    if sig.choices:
        return {"kind": "table"}
    scale = float(sig.scale)
    offset = float(sig.offset)
    if scale == 1.0 and offset == 0.0:
        return {"kind": "identity"}
    return {"kind": "linear", "factor": scale, "offset": offset}


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


def _multiplexer(sig: Any) -> dict[str, Any] | None:
    if getattr(sig, "is_multiplexer", False):
        return {"role": "multiplexor"}
    ids = getattr(sig, "multiplexer_ids", None)
    if ids:
        role: dict[str, Any] = {"role": "multiplexed", "ids": _expand_mux_ids(ids)}
        mux_signal = getattr(sig, "multiplexer_signal", None)
        if mux_signal:
            mux_name = mux_signal if isinstance(mux_signal, str) else mux_signal.name
            role["multiplexorName"] = sanitize_id(mux_name)
        return role
    return None


def _signal_to_mdc(sig: Any, attr_definitions: dict[str, Any]) -> dict[str, Any]:
    out: dict[str, Any] = {
        "name": sanitize_id(sig.name),
        "startBit": int(sig.start),
        "lengthBits": int(sig.length),
        "byteOrder": str(sig.byte_order),
        "kind": _signal_kind(sig),
        "conversion": _conversion(sig),
    }
    if out["kind"] == "float" and out["lengthBits"] not in _FLOAT_WIDTHS:
        print(
            f"dbc2mdc: warning: signal {sig.name!r} float width {out['lengthBits']} "
            f"not in {_FLOAT_WIDTHS}; downgrading to unsigned identity",
            file=sys.stderr,
        )
        out["kind"] = "unsigned"
        out["conversion"] = {"kind": "identity"}
    if sig.comment:
        out["description"] = str(sig.comment)
    if sig.unit:
        out["unit"] = str(sig.unit)
    if sig.minimum is not None:
        out["min"] = float(sig.minimum)
    if sig.maximum is not None:
        out["max"] = float(sig.maximum)
    if sig.choices:
        out["choices"] = _choices(sig)
    if sig.receivers:
        out["receivers"] = list(sig.receivers)
    mux = _multiplexer(sig)
    if mux:
        out["multiplexer"] = mux
    if sig.dbc and getattr(sig.dbc, "attributes", None):
        attrs = _serialize_attributes(sig.dbc.attributes, attr_definitions)
        if attrs:
            out["attributes"] = attrs
    return out


def _transmission_type(msg: Any) -> str:
    """Map DBC GenMsgSendType to MDC transmissionType (default cyclic)."""
    send_type = getattr(msg, "send_type", None)
    if send_type and str(send_type).lower() in _SEND_TYPE_TRIGGERED:
        return "triggered"
    return "cyclic"


def _message_multiplexing(msg: Any, ext_mux: bool) -> dict[str, Any] | None:
    """Build message.multiplexing when a multiplexor signal is present."""
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


def _message_to_mdc(
    msg: Any, attr_definitions: dict[str, Any], ext_mux: bool = False
) -> dict[str, Any]:
    signals = sorted(msg.signals, key=lambda s: (s.start, s.name))
    transmission_type = _transmission_type(msg)
    out: dict[str, Any] = {
        "name": sanitize_id(msg.name),
        "id": int(msg.frame_id),
        "isExtended": bool(msg.is_extended_frame),
        "isFd": bool(msg.is_fd),
        "length": int(msg.length),
        "transmissionType": transmission_type,
        "signals": [_signal_to_mdc(s, attr_definitions) for s in signals],
    }
    if msg.comment:
        out["description"] = str(msg.comment)
    if msg.cycle_time is not None:
        out["cycleTimeMs"] = int(msg.cycle_time)
    elif transmission_type == "cyclic":
        out["cycleTimeMs"] = _DEFAULT_CYCLE_TIME_MS
    if msg.senders:
        out["senders"] = list(msg.senders)
    if msg.dbc and getattr(msg.dbc, "attributes", None):
        attrs = _serialize_attributes(msg.dbc.attributes, attr_definitions)
        if attrs:
            out["attributes"] = attrs
    mux_block = _message_multiplexing(msg, ext_mux)
    if mux_block is not None:
        out["multiplexing"] = mux_block
    return out


def _network_from_dbc(dbc_path: Path) -> tuple[dict[str, Any], list[dict[str, Any]]]:
    db = cantools.database.load_file(str(dbc_path))
    attr_definitions = db.dbc.attribute_definitions if db.dbc else {}
    ext_mux = _extended_mux_enabled(db)
    is_fd = any(m.is_fd for m in db.messages)
    network: dict[str, Any] = {
        "id": sanitize_id(dbc_path.stem),
        "name": dbc_path.stem,
        "filename": dbc_path.name,
        "protocol": "canfd" if is_fd else "can",
        "messages": sorted(
            (_message_to_mdc(m, attr_definitions, ext_mux) for m in db.messages),
            key=lambda x: x["id"],
        ),
    }
    if db.dbc and getattr(db.dbc, "attributes", None):
        attrs = _serialize_attributes(db.dbc.attributes, attr_definitions)
        if attrs:
            network["attributes"] = attrs
    if db.nodes:
        nodes: list[dict[str, Any]] = []
        for n in db.nodes:
            node: dict[str, Any] = {"name": n.name}
            if n.dbc and getattr(n.dbc, "attributes", None):
                node_attrs = _serialize_attributes(n.dbc.attributes, attr_definitions)
                if node_attrs:
                    node["attributes"] = node_attrs
            nodes.append(node)
        network["nodes"] = nodes
    return network, _attribute_definitions(db)


def _merge_attr_definitions(def_lists: list[list[dict[str, Any]]]) -> list[dict[str, Any]]:
    # ponytail: first network wins on name collision; upgrade path: warn or merge scopes.
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
        "schemaVersion": SCHEMA_VERSION,
        "metadata": {
            "name": name,
            "source": "dbc2mdc",
            "createdAt": now,
            "modifiedAt": now,
        },
        "vehicles": [
            {"id": sanitize_id(vehicle_id), "name": name, "networks": networks}
        ],
    }
    if attribute_definitions:
        project["attributeDefinitions"] = attribute_definitions
    return project


def main() -> None:
    parser = argparse.ArgumentParser(description="Import DBC file(s) into an MDC project.")
    parser.add_argument("input", type=Path, help="A .dbc file or a folder of .dbc files")
    parser.add_argument("-o", "--output", type=Path, help="Output MDC JSON (default: stdout)")
    parser.add_argument("--vehicle-id", help="Vehicle id (default: derived from input name)")
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
