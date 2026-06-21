"""mdc-busload send_type classification (contract §7).  Run from can/mdc: pytest tools/tests"""
from __future__ import annotations

import json
import subprocess
from pathlib import Path

import pytest

REPO = Path(__file__).resolve().parents[2]
TOOLS = REPO / "tools"
LIB = REPO / "lib" / "busload.mjs"


def _node_is_cyclic(message: dict) -> dict:
    script = f"""
import {{ isCyclic, loadPercentAt }} from {json.dumps(LIB.as_uri())};
const msg = {json.dumps(message)};
console.log(JSON.stringify({{
  cyclic: isCyclic(msg),
  load: loadPercentAt([msg], 500000),
}}));
"""
    result = subprocess.run(
        ["node", "--input-type=module", "-e", script],
        cwd=REPO,
        capture_output=True,
        text=True,
    )
    assert result.returncode == 0, result.stderr or result.stdout
    return json.loads(result.stdout)


@pytest.mark.parametrize(
    "send_type,expect_cyclic",
    [
        ("Cyclic", True),
        ("cyclic", True),
        ("CyclicAndEvent", True),
        (None, True),
        ("Event", False),
        ("event", False),
        ("Triggered", False),
        ("IfActive", False),
        ("Spontaneous", False),
        ("NoSig", False),
        ("none", False),
        ("NoMsgSendType", False),
    ],
)
def test_send_type_cyclic_classification(send_type: str | None, expect_cyclic: bool) -> None:
    msg = {"length": 8, "cycle_time": 100}
    if send_type is not None:
        msg["send_type"] = send_type
    out = _node_is_cyclic(msg)
    assert out["cyclic"] is expect_cyclic
    if expect_cyclic:
        assert out["load"] > 0
    else:
        assert out["load"] == 0


def test_missing_cycle_time_not_cyclic() -> None:
    out = _node_is_cyclic({"length": 8, "send_type": "Cyclic"})
    assert out["cyclic"] is False


def test_mdc_busload_self_check() -> None:
    result = subprocess.run(
        ["node", str(TOOLS / "mdc-busload.mjs"), "--self-check"],
        cwd=REPO,
        capture_output=True,
        text=True,
    )
    assert result.returncode == 0, result.stderr or result.stdout
    assert "OK" in result.stdout
