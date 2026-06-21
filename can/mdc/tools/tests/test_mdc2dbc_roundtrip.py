"""dbc2mdc → mdc2dbc best-effort round-trip.  Run from can/mdc: pytest tools/tests"""
from __future__ import annotations

import copy
import importlib.util
import io
from contextlib import redirect_stderr
from pathlib import Path

import cantools
import pytest

REPO = Path(__file__).resolve().parents[2]
CAN = REPO.parent
TOOLS = REPO / "tools"
FIXTURES = Path(__file__).resolve().parent / "fixtures"
ELCON_DBC = CAN / "vehicles" / "HighNoon" / "ElconCAN.dbc"
SYNTHETIC_DBC = FIXTURES / "attr_import.dbc"


def _load_module(name: str, path: Path):
    spec = importlib.util.spec_from_file_location(name, path)
    mod = importlib.util.module_from_spec(spec)
    assert spec.loader is not None
    spec.loader.exec_module(mod)
    return mod


dbc2mdc = _load_module("dbc2mdc", TOOLS / "dbc2mdc.py")
mdc2dbc = _load_module("mdc2dbc", TOOLS / "mdc2dbc.py")


def _import_dbc(path: Path) -> dict:
    network, attr_defs = dbc2mdc._network_from_dbc(path)
    return dbc2mdc.build_project(
        [network],
        vehicle_id=path.stem,
        name=path.stem,
        attribute_definitions=attr_defs,
    )


def _dbc_counts(path: Path) -> tuple[int, int, int]:
    db = cantools.database.load_file(str(path))
    return (
        len(db.messages),
        sum(len(m.signals) for m in db.messages),
        len(db.nodes),
    )


@pytest.fixture(scope="module")
def elcon() -> dict:
    if not ELCON_DBC.is_file():
        pytest.skip(f"missing fixture DBC: {ELCON_DBC}")
    return _import_dbc(ELCON_DBC)


@pytest.fixture(scope="module")
def synthetic() -> dict:
    return _import_dbc(SYNTHETIC_DBC)


@pytest.mark.parametrize("dbc_path", [ELCON_DBC, SYNTHETIC_DBC], ids=["elcon", "synthetic"])
def test_roundtrip_preserves_core_counts(
    dbc_path: Path, tmp_path_factory: pytest.TempPathFactory
) -> None:
    if not dbc_path.is_file():
        pytest.skip(f"missing fixture DBC: {dbc_path}")
    project = _import_dbc(dbc_path)
    before = _dbc_counts(dbc_path)
    out_dbc = tmp_path_factory.mktemp("roundtrip") / f"{dbc_path.stem}.dbc"
    mdc2dbc.export_network(project, project["networks"][0], out_dbc)
    assert _dbc_counts(out_dbc) == before


def test_roundtrip_warns_on_multiplexing_extension(elcon: dict, tmp_path: Path) -> None:
    buf = io.StringIO()
    with redirect_stderr(buf):
        mdc2dbc.export_network(elcon, elcon["networks"][0], tmp_path / "elcon.dbc")
    warnings = buf.getvalue()
    assert "multiplexing extension dropped" in warnings


def test_mdc2dbc_warns_on_lossy_mdc_only_fields(synthetic: dict, tmp_path: Path) -> None:
    project = copy.deepcopy(synthetic)
    net = project["networks"][0]
    net["samplePoint"] = 75.0
    msg = net["messages"][0]
    msg["computedSignals"] = [
        {"name": "derived", "expr": "Test_Sig * 2", "is_float": True, "is_signed": False}
    ]
    msg["display"] = {"chartType": "line"}
    msg["signals"][0]["alarms"] = [{"name": "high", "condition": ">", "threshold": 200}]
    msg["signals"][0]["conversion"] = {
        "kind": "rational",
        "numerator": [1, 0, 0],
        "denominator": [1, 0, 0],
    }

    buf = io.StringIO()
    with redirect_stderr(buf):
        mdc2dbc.export_network(project, net, tmp_path / "lossy.dbc")
    warnings = buf.getvalue()
    for fragment in (
        "computedSignals dropped",
        "display dropped",
        "alarms dropped",
        "rational conversion dropped",
        "samplePoint dropped",
    ):
        assert fragment in warnings
