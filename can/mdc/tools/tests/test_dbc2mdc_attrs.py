"""dbc2mdc Wave 1 attribute-import tests (BA_DEF_/BA_ → MDC v2).

Run from can/mdc: pytest tools/tests
"""
from __future__ import annotations

import importlib.util
import json
import subprocess
import sys
from pathlib import Path

import pytest

REPO = Path(__file__).resolve().parents[2]
CAN = REPO.parent
TOOLS = REPO / "tools"
FIXTURES = Path(__file__).resolve().parent / "fixtures"
ELCON_DBC = CAN / "dbc" / "HighNoon" / "ElconCAN.dbc"
SCHEMA = REPO / "schema" / "mdc.schema.bundle.json"

_spec = importlib.util.spec_from_file_location("dbc2mdc", TOOLS / "dbc2mdc.py")
dbc2mdc = importlib.util.module_from_spec(_spec)
assert _spec.loader is not None
_spec.loader.exec_module(dbc2mdc)


def _import_dbc(path: Path) -> dict:
    network, attr_defs = dbc2mdc._network_from_dbc(path)
    return dbc2mdc.build_project(
        [network],
        vehicle_id=path.stem,
        name=path.stem,
        attribute_definitions=attr_defs,
    )


def _def_by_name(project: dict, name: str) -> dict:
    for entry in project.get("attributeDefinitions", []):
        if entry["name"] == name:
            return entry
    raise KeyError(name)


@pytest.fixture(scope="module")
def elcon() -> dict:
    if not ELCON_DBC.is_file():
        pytest.skip(f"missing fixture DBC: {ELCON_DBC}")
    return _import_dbc(ELCON_DBC)


@pytest.fixture(scope="module")
def synthetic() -> dict:
    return _import_dbc(FIXTURES / "attr_import.dbc")


def test_ba_def_maps_to_attribute_definitions(elcon: dict, synthetic: dict) -> None:
    """BA_DEF_ → attributeDefinitions[] with scopes, types, min/max, defaults."""
    elcon_defs = {d["name"]: d for d in elcon["attributeDefinitions"]}
    assert set(elcon_defs) == {"BusType", "MultiplexExtEnabled", "SPN"}

    spn = elcon_defs["SPN"]
    assert spn["type"] == "int"
    assert spn["scopes"] == ["signal"]
    assert spn["min"] == 0
    assert spn["max"] == 524287
    assert spn["default"] == 0

    assert elcon_defs["BusType"]["type"] == "string"
    assert elcon_defs["BusType"]["scopes"] == ["network"]

    mux = elcon_defs["MultiplexExtEnabled"]
    assert mux["type"] == "enum"
    assert mux["scopes"] == ["network"]
    assert mux["enumValues"] == ["No", "Yes"]
    assert mux["default"] == "No"

    native = {"VFrameFormat", "GenMsgSendType", "GenMsgCycleTime"}
    assert native.isdisjoint(elcon_defs)
    assert native.isdisjoint({d["name"] for d in synthetic["attributeDefinitions"]})

    syn_defs = {d["name"]: d for d in synthetic["attributeDefinitions"]}
    assert syn_defs["NodeAttr"] == {"name": "NodeAttr", "type": "string", "scopes": ["node"]}
    custom = syn_defs["CustomEnum"]
    assert custom["type"] == "enum"
    assert custom["scopes"] == ["network"]
    assert custom["enumValues"] == ["A", "B", "C"]
    assert custom["default"] == "A"


def test_native_fields_not_stored_as_attributes(elcon: dict, synthetic: dict) -> None:
    """VFrameFormat / GenMsgSendType / GenMsgCycleTime map to native message fields."""
    elcon_msgs = {m["name"]: m for m in elcon["vehicles"][0]["networks"][0]["messages"]}
    for name in ("Elcon_Control", "Elcon_Control_Mux", "Elcon_CCS_Status"):
        msg = elcon_msgs[name]
        assert msg["isExtended"] is True
        assert msg.get("isFd") is False
        assert "VFrameFormat" not in (msg.get("attributes") or {})
        assert "GenMsgSendType" not in (msg.get("attributes") or {})
        assert "GenMsgCycleTime" not in (msg.get("attributes") or {})

    syn_msg = synthetic["vehicles"][0]["networks"][0]["messages"][0]
    assert syn_msg["name"] == "Test_Msg"
    assert syn_msg["isExtended"] is True
    assert syn_msg["isFd"] is False
    assert syn_msg["transmissionType"] == "triggered"
    assert syn_msg["cycleTimeMs"] == 250
    assert syn_msg.get("attributes") is None


def test_ba_assignments_map_to_entity_attributes(elcon: dict, synthetic: dict) -> None:
    """BA_ → per-entity attributes for non-native attrs."""
    elcon_net = elcon["vehicles"][0]["networks"][0]
    assert elcon_net["attributes"] == {
        "MultiplexExtEnabled": "Yes",
        "BusType": "CAN",
    }
    assert "DBName" not in elcon_net["attributes"]

    syn_net = synthetic["vehicles"][0]["networks"][0]
    assert syn_net["attributes"] == {"CustomEnum": "B"}

    syn_msg = syn_net["messages"][0]
    sig = next(s for s in syn_msg["signals"] if s["name"] == "Test_Sig")
    assert sig["attributes"] == {"SPN": 42}

    node = next(n for n in syn_net["nodes"] if n["name"] == "ECU")
    assert node["attributes"] == {"NodeAttr": "bench-ecu"}


def test_output_validates_and_generates_headers(
    elcon: dict, synthetic: dict, tmp_path_factory: pytest.TempPathFactory
) -> None:
    """Generated MDC validates against the v2 bundle; mdc2cheaders succeeds."""
    tmp = tmp_path_factory.mktemp("mdc")
    for label, project in (("elcon", elcon), ("synthetic", synthetic)):
        out = tmp / f"{label}.mdc.json"
        out.write_text(json.dumps(project, indent=2) + "\n", encoding="utf-8")

        validate = subprocess.run(
            ["node", str(TOOLS / "mdc-validate.mjs"), "--schema-only", str(SCHEMA), str(out)],
            cwd=REPO,
            capture_output=True,
            text=True,
        )
        assert validate.returncode == 0, validate.stderr or validate.stdout

        header = tmp / f"{label}.h"
        headers = subprocess.run(
            ["node", str(TOOLS / "mdc2cheaders.mjs"), str(out), "-o", str(header)],
            cwd=REPO,
            capture_output=True,
            text=True,
        )
        assert headers.returncode == 0, headers.stderr or headers.stdout
        text = header.read_text(encoding="utf-8")
        assert "#pragma once" in text
        assert "typedef struct" in text
