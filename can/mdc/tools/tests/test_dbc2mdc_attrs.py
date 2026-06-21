"""dbc2mdc Wave 3 tests (BA_DEF_/BA_ → MDC v3).  Run from can/mdc: pytest tools/tests"""
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
ELCON_DBC = CAN / "vehicles" / "HighNoon" / "ElconCAN.dbc"
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
    elcon_defs = {d["name"]: d for d in elcon["attributeDefinitions"]}
    # v3: SPN is native on signals — not emitted as attributeDefinition
    assert set(elcon_defs) == {"BusType", "MultiplexExtEnabled"}

    assert elcon_defs["BusType"]["type"] == "string"
    assert elcon_defs["BusType"]["scopes"] == ["network"]

    mux = elcon_defs["MultiplexExtEnabled"]
    assert mux["type"] == "enum"
    assert mux["scopes"] == ["network"]
    assert mux["enumValues"] == ["No", "Yes"]

    native = {"VFrameFormat", "GenMsgSendType", "GenMsgCycleTime"}
    assert native.isdisjoint(elcon_defs)
    assert native.isdisjoint({d["name"] for d in synthetic["attributeDefinitions"]})


def test_native_fields_not_stored_as_attributes(elcon: dict, synthetic: dict) -> None:
    elcon_msgs = {m["name"]: m for m in elcon["networks"][0]["messages"]}
    for name in ("Elcon_Control", "Elcon_Control_Mux", "Elcon_CCS_Status"):
        msg = elcon_msgs[name]
        assert msg["is_extended_frame"] is True
        assert msg.get("is_fd") is False
        assert "VFrameFormat" not in (msg.get("attributes") or {})

    syn_msg = synthetic["networks"][0]["messages"][0]
    assert syn_msg["name"] == "Test_Msg"
    assert syn_msg["is_extended_frame"] is True
    assert syn_msg.get("is_fd") is False
    assert syn_msg.get("send_type", "").lower() in ("triggered", "event", "ifactive") or syn_msg.get("cycle_time") == 250


def test_spn_promoted_not_attribute(elcon: dict, synthetic: dict) -> None:
    syn_msg = synthetic["networks"][0]["messages"][0]
    sig = next(s for s in syn_msg["signals"] if s["name"] == "Test_Sig")
    assert sig.get("spn") == 42
    assert "SPN" not in (sig.get("attributes") or {})


def test_ba_assignments_map_to_entity_attributes(elcon: dict, synthetic: dict) -> None:
    elcon_net = elcon["networks"][0]
    assert elcon_net["attributes"] == {
        "MultiplexExtEnabled": "Yes",
        "BusType": "CAN",
    }

    syn_net = synthetic["networks"][0]
    assert syn_net["attributes"] == {"CustomEnum": "B"}

    node = next(n for n in syn_net["nodes"] if n["name"] == "ECU")
    assert node["attributes"] == {"NodeAttr": "bench-ecu"}


def test_output_validates_and_generates_headers(
    elcon: dict, synthetic: dict, tmp_path_factory: pytest.TempPathFactory
) -> None:
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
            ["node", str(TOOLS / "generate_can_headers.mjs"), str(out), "-o", str(header)],
            cwd=REPO,
            capture_output=True,
            text=True,
        )
        assert headers.returncode == 0, headers.stderr or headers.stdout
        text = header.read_text(encoding="utf-8")
        assert "#pragma once" in text
        assert "typedef struct" in text
