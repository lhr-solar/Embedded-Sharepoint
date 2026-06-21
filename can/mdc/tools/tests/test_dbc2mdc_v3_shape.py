"""dbc2mdc v3 emit shape (flat root, cantools field names).  Run from can/mdc: pytest tools/tests"""
from __future__ import annotations

import importlib.util
import json
import subprocess
from pathlib import Path

import cantools
import pytest

REPO = Path(__file__).resolve().parents[2]
TOOLS = REPO / "tools"
FIXTURES = Path(__file__).resolve().parent / "fixtures"
SCHEMA = REPO / "schema" / "mdc.schema.bundle.json"
SYNTHETIC_DBC = FIXTURES / "attr_import.dbc"

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


@pytest.fixture(scope="module")
def synthetic() -> dict:
    return _import_dbc(FIXTURES / "attr_import.dbc")


def test_flat_root_and_schema_version(synthetic: dict) -> None:
    assert synthetic["schemaVersion"] == "3.0.0"
    assert synthetic["$schema"] == dbc2mdc.SCHEMA_REF
    assert "vehicles" not in synthetic
    assert synthetic["id"] == "attr_import"
    assert isinstance(synthetic["networks"], list) and len(synthetic["networks"]) == 1


def test_cantools_field_names_on_message_and_signal(synthetic: dict) -> None:
    cantools_msg = cantools.database.load_file(str(SYNTHETIC_DBC)).get_message_by_name("Test_Msg")
    net = synthetic["networks"][0]
    msg = net["messages"][0]
    sig = next(s for s in msg["signals"] if s["name"] == "Test_Sig")

    # v2 keys must not appear
    for stale in ("id", "isExtended", "isFd", "cycleTimeMs", "transmissionType", "description"):
        assert stale not in msg
    for stale in ("startBit", "lengthBits", "byteOrder", "kind", "min", "max"):
        assert stale not in sig

    # v3 cantools-shaped keys (values follow cantools parse, not raw BO_ literal)
    assert msg["frame_id"] == cantools_msg.frame_id
    assert msg["is_extended_frame"] is cantools_msg.is_extended_frame
    assert msg.get("is_fd") is cantools_msg.is_fd
    assert msg["send_type"] == cantools_msg.send_type
    assert msg["cycle_time"] == cantools_msg.cycle_time
    assert sig["start"] == 0
    assert sig["length"] == 8
    assert sig["byte_order"] == "little_endian"
    assert sig["is_signed"] is False
    assert sig["is_float"] is False
    assert sig.get("spn") == 42
    assert "SPN" not in (sig.get("attributes") or {})

    assert "protocol" not in net
    assert "baudrate" not in net or isinstance(net.get("baudrate"), int)


def test_output_validates_against_bundle(synthetic: dict, tmp_path: Path) -> None:
    out = tmp_path / "synthetic.mdc.json"
    out.write_text(
        json.dumps(synthetic, indent=2) + "\n",
        encoding="utf-8",
    )
    result = subprocess.run(
        ["node", str(TOOLS / "mdc-validate.mjs"), "--schema-only", str(SCHEMA), str(out)],
        cwd=REPO,
        capture_output=True,
        text=True,
    )
    assert result.returncode == 0, result.stderr or result.stdout
