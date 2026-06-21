#!/usr/bin/env python3
"""Flash a firmware .bin to an STM32 node over CAN via the resident bootloader.

Implements the host side of the CAN/DFU bootloader protocol. The canonical
wire definition is bootloader/contract/bl_protocol.schema.json; the constants
below are copied from it. Multi-byte fields are little-endian (STM32 native)
and the last write block is padded to 256 bytes with 0xFF (flash erased state).

Transport is pluggable (see "transport adapters" below). The bootloader frames
ride on top of a CanLink, so the same flow flashes through a Peak PCAN dongle,
Linux SocketCAN, an SLCAN serial gateway, or any bespoke way of getting bytes
onto the bus. Pick one with --adapter (run --list-adapters to see them).

`--help`, `--selftest`, `--list-adapters`, and `--dry-run` work with zero extra
dependencies: `import can` (python-can) happens lazily, only when a real bus is
actually opened.

Usage:
    can_flash.py --selftest
    can_flash.py --list-adapters
    can_flash.py --device-id 5 --bin app.bin --slot A --dry-run
    can_flash.py --device-id 5 --bin app.bin --slot A --adapter pcan
    can_flash.py --device-id 5 --bin app.bin --slot A --adapter socketcan --channel can0
"""

from __future__ import annotations

import argparse
import struct
import sys
import time
from pathlib import Path
from typing import NamedTuple, Optional, Tuple

# --- Protocol constants (canonical: bootloader/contract/bl_protocol.schema.json) ---
CMD_ID = 0x010
DATA_ID = 0x011
RESP_BASE = 0x100
STATUS_BASE = 0x180  # async status frames (not used by this linear flow)

DEVICE_ID_MIN = 0
DEVICE_ID_MAX = 119
DEVICE_ID_BROADCAST = 0xFF

BLOCK_SIZE = 256
CAN_FRAME_BYTES = 8  # classic CAN payload

OP_ENTER = 1
OP_ERASE = 2
OP_SET_ADDR = 3
OP_WRITE_BLOCK = 4
OP_VERIFY_CRC32 = 5
OP_COMMIT = 6
OP_RESET = 7
OP_GET_INFO = 8

ACK = 0
NACK = 1
BUSY = 2
ERR = 3
CRC_OK = 16
CRC_FAIL = 17

RESP_NAMES = {ACK: "ACK", NACK: "NACK", BUSY: "BUSY", ERR: "ERR",
              CRC_OK: "CRC_OK", CRC_FAIL: "CRC_FAIL"}

# stm32g491vet dual app slots; use --base-addr for other targets (e.g. g431cbt 0x08002000).
SLOT_BASES = {"A": 0x08004000, "B": 0x08040000}

# Per-step response timeouts (seconds); erase/verify/commit touch flash and run longer.
T_CMD = 2.0
T_WRITE = 2.0
T_ERASE = 15.0
T_VERIFY = 15.0
T_COMMIT = 5.0

DEFAULT_BITRATE = 500000


def crc32_mpeg2(data: bytes) -> int:
    """CRC-32/MPEG-2: poly 0x04C11DB7, init 0xFFFFFFFF, no reflection, no final XOR.

    NOT zlib/Ethernet CRC. crc32_mpeg2(b"123456789") == 0x0376E6E7.
    """
    crc = 0xFFFFFFFF
    for byte in data:
        crc ^= byte << 24
        for _ in range(8):
            if crc & 0x80000000:
                crc = ((crc << 1) ^ 0x04C11DB7) & 0xFFFFFFFF
            else:
                crc = (crc << 1) & 0xFFFFFFFF
    return crc


def _selftest() -> bool:
    got = crc32_mpeg2(b"123456789")
    ok = got == 0x0376E6E7 and got != 0xCBF43926
    print(f"crc32_mpeg2(b'123456789') = 0x{got:08X}  "
          f"(expected 0x0376E6E7; zlib/Ethernet 0xCBF43926 would be WRONG)")
    print("PASS" if ok else "FAIL")
    return ok


class FlashError(Exception):
    """A flashing step failed (bad response code or timeout)."""


def _split_blocks(image: bytes) -> "list[bytes]":
    blocks = []
    for i in range(0, len(image), BLOCK_SIZE):
        chunk = image[i:i + BLOCK_SIZE]
        if len(chunk) < BLOCK_SIZE:
            chunk = chunk + bytes([0xFF]) * (BLOCK_SIZE - len(chunk))
        blocks.append(chunk)
    return blocks


# ============================== transport adapters ==============================
#
# The bootloader protocol only needs to put one classic-CAN frame on the bus and
# read one back. That seam is CanLink. Every "way to get bin data onto the bus"
# is a CanLink: a Peak PCAN dongle, Linux SocketCAN, an SLCAN serial gateway, or
# a custom relay node. Two ways to add one:
#
#   1. python-can already speaks it -> add an entry to ADAPTERS (one line). This
#      covers pcan, socketcan, slcan, kvaser, vector, gs_usb, virtual, ...
#   2. it's bespoke (not python-can) -> subclass CanLink and return it from
#      open_link(). The protocol layer and flash() stay untouched either way.


class CanLink:
    """Minimal CAN transport the bootloader protocol rides on.

    Frames are classic CAN: 11-bit standard id and up to 8 data bytes.
    """

    def send_frame(self, can_id: int, data: bytes) -> None:
        raise NotImplementedError

    def recv_frame(self, timeout: float) -> Optional[Tuple[int, bytes]]:
        """Return (arbitration_id, data) for the next frame, or None on timeout."""
        raise NotImplementedError

    def close(self) -> None:
        pass


class PythonCanLink(CanLink):
    """One CanLink for every python-can interface (pcan, socketcan, slcan, ...).

    python-can is imported lazily so --help/--selftest/--list-adapters/--dry-run
    stay dependency-free.
    """

    def __init__(self, interface: str, channel, bitrate: int):
        try:
            import can  # lazy: keeps the no-bus paths dependency-free
        except ImportError:
            sys.exit("error: python-can is not installed. Enter 'nix develop' "
                     "(provides python-can), or run: pip install python-can")
        self._can = can
        kwargs = {"interface": interface, "bitrate": bitrate}
        if channel is not None:
            kwargs["channel"] = channel
        try:
            self.bus = can.Bus(**kwargs)
        except Exception as exc:  # backend-specific; surface an actionable hint
            sys.exit(_open_error(interface, channel, exc))

    def send_frame(self, can_id: int, data: bytes) -> None:
        self.bus.send(self._can.Message(arbitration_id=can_id, data=bytes(data),
                                        is_extended_id=False))

    def recv_frame(self, timeout: float) -> Optional[Tuple[int, bytes]]:
        msg = self.bus.recv(timeout=timeout)
        if msg is None:
            return None
        return msg.arbitration_id, bytes(msg.data)

    def close(self) -> None:
        try:
            self.bus.shutdown()
        except Exception:
            pass


class Adapter(NamedTuple):
    interface: str                 # python-can interface name
    default_channel: Optional[str]  # used when --channel is omitted
    help: str


# Named transports. Most are thin python-can presets; add a bespoke one by
# subclassing CanLink and wiring it into open_link().
ADAPTERS = {
    "socketcan": Adapter("socketcan", None,
                         "Linux SocketCAN; pass --channel (e.g. can0)"),
    "pcan": Adapter("pcan", "PCAN_USBBUS1",
                    "Peak PCAN-USB: PCAN-Basic (Windows/Linux) or PCBUSB (macOS)"),
    "slcan": Adapter("slcan", None,
                     "serial CAN dongle; pass --channel (e.g. /dev/ttyACM0)"),
    "virtual": Adapter("virtual", "vcan-test",
                       "python-can in-process loopback (no hardware; for testing)"),
}
DEFAULT_ADAPTER = "socketcan"


def _open_error(interface: str, channel, exc: Exception) -> str:
    chan = f" channel '{channel}'" if channel else ""
    hints = {
        "socketcan": "hint: bring the bus up and pass --channel, e.g. "
                     "'sudo ip link set can0 up type can bitrate 500000' "
                     "then --channel can0.",
        "pcan": "hint: plug in the Peak PCAN-USB and install its driver "
                "(PCAN-Basic on Windows/Linux; the PCBUSB/libPCBUSB library on "
                "macOS). Default channel is PCAN_USBBUS1; use --channel "
                "PCAN_USBBUS2 for a second device.",
        "slcan": "hint: pass the serial device as --channel (e.g. /dev/ttyACM0) "
                 "and set --bitrate to match the dongle.",
    }
    hint = hints.get(interface,
                     "hint: check the device is connected and that --interface / "
                     "--channel match your hardware (see --list-adapters).")
    return (f"error: could not open CAN interface '{interface}'{chan}: {exc}\n"
            f"{hint}")


def resolve_transport(adapter_name: str, interface, channel):
    """Resolve (interface, channel) from the chosen adapter + explicit overrides."""
    adapter = ADAPTERS[adapter_name]
    iface = interface or adapter.interface
    chan = channel if channel is not None else adapter.default_channel
    return iface, chan


def open_link(adapter_name: str, interface, channel, bitrate: int) -> CanLink:
    iface, chan = resolve_transport(adapter_name, interface, channel)
    return PythonCanLink(iface, chan, bitrate)


def _list_adapters() -> None:
    print("CAN transport adapters (--adapter NAME):\n")
    for name, a in ADAPTERS.items():
        chan = a.default_channel if a.default_channel else "(required: --channel)"
        default = "  [default]" if name == DEFAULT_ADAPTER else ""
        print(f"  {name:<10} interface={a.interface:<10} "
              f"default-channel={chan}{default}")
        print(f"             {a.help}")
    print("\nAdvanced: --interface / --channel override an adapter's python-can")
    print("settings. Add a bespoke (non-python-can) transport by subclassing")
    print("CanLink in this file and returning it from open_link().")


# ================================ protocol layer ================================
#
# Bootloader request/response on top of any CanLink. Transport-agnostic: the
# same code runs whether frames ride a Peak dongle, SocketCAN, or an SLCAN gateway.


class BlSession:
    """Bootloader command/response session over a CanLink."""

    def __init__(self, link: CanLink, device_id: int, verbose: bool):
        self.link = link
        self.device_id = device_id
        self.resp_id = RESP_BASE + device_id
        self.verbose = verbose

    def _log(self, msg: str) -> None:
        if self.verbose:
            print(msg)

    def send(self, can_id: int, data: bytes, label: str) -> None:
        self.link.send_frame(can_id, data)
        self._log(f"  TX id=0x{can_id:03X} [{len(data)}] {bytes(data).hex()}  # {label}")

    def send_data(self, payload: bytes, label: str) -> None:
        n = 0
        for i in range(0, len(payload), CAN_FRAME_BYTES):
            self.link.send_frame(DATA_ID, payload[i:i + CAN_FRAME_BYTES])
            n += 1
        self._log(f"  TX id=0x{DATA_ID:03X} x{n} ({len(payload)} bytes)  # {label}")

    def recv_response(self, label: str, timeout: float, ok_code: int) -> Optional[int]:
        # ok_code only matters for the dry-run twin; the real bus returns what it gets.
        deadline = time.monotonic() + timeout
        while True:
            remaining = deadline - time.monotonic()
            if remaining <= 0:
                raise FlashError(f"{label}: timed out waiting for a response on "
                                 f"id 0x{self.resp_id:03X}")
            frame = self.link.recv_frame(remaining)
            if frame is None:
                continue
            can_id, data = frame
            if can_id != self.resp_id:
                continue  # ignore other nodes / DATA echoes / status frames
            code = data[0] if data else None
            name = RESP_NAMES.get(code, f"0x{code:02X}" if code is not None else "<empty>")
            self._log(f"  RX id=0x{can_id:03X} code={name}  # {label}")
            return code

    def close(self) -> None:
        self.link.close()


class _DryRunSession:
    """Prints the planned frames and synthesizes ACKs; opens no bus and needs no
    python-can. Mirrors BlSession's surface so flash() runs identically."""

    def __init__(self, device_id: int, verbose: bool, transport_desc: str):
        self.device_id = device_id
        self.verbose = verbose
        print(f"  (dry-run; would transmit via {transport_desc})")

    def send(self, can_id: int, data: bytes, label: str) -> None:
        print(f"  TX id=0x{can_id:03X} [{len(data)}] {bytes(data).hex()}  # {label}")

    def send_data(self, payload: bytes, label: str) -> None:
        n = (len(payload) + CAN_FRAME_BYTES - 1) // CAN_FRAME_BYTES
        print(f"  TX id=0x{DATA_ID:03X} x{n} ({len(payload)} bytes)  # {label}")
        if self.verbose:
            for i in range(0, len(payload), CAN_FRAME_BYTES):
                print(f"       {payload[i:i + CAN_FRAME_BYTES].hex()}")

    def recv_response(self, label: str, timeout: float, ok_code: int) -> int:
        print(f"  RX id=0x{RESP_BASE + self.device_id:03X} "
              f"<expect {RESP_NAMES[ok_code]}>  # {label}")
        return ok_code

    def close(self) -> None:
        pass


def _require(code, expected: int, name: str) -> None:
    if code == expected:
        return
    got = RESP_NAMES.get(code, f"0x{code:02X}" if code is not None else "<no data>")
    raise FlashError(f"{name} failed: device replied {got}, expected {RESP_NAMES[expected]}")


def _cmd(device_id: int, opcode: int, params: bytes = b"") -> bytes:
    return bytes([device_id, opcode]) + params


def _step(session, device_id: int, name: str, opcode: int, params: bytes = b"",
          timeout: float = T_CMD, ok_code: int = ACK) -> None:
    session.send(CMD_ID, _cmd(device_id, opcode, params), name)
    _require(session.recv_response(name, timeout, ok_code), ok_code, name)


def flash(session, device_id: int, base_addr: int, image: bytes) -> None:
    blocks = _split_blocks(image)
    crc = crc32_mpeg2(image)
    print(f"image: {len(image)} bytes -> {len(blocks)} x {BLOCK_SIZE}B blocks  "
          f"crc32-mpeg2=0x{crc:08X}")
    print(f"flashing device-id {device_id} at base 0x{base_addr:08X}")

    _step(session, device_id, "GET_INFO", OP_GET_INFO)
    _step(session, device_id, "ENTER", OP_ENTER)
    _step(session, device_id, "ERASE", OP_ERASE, timeout=T_ERASE)
    _step(session, device_id, "SET_ADDR", OP_SET_ADDR, struct.pack("<I", base_addr))

    for i, block in enumerate(blocks):
        name = f"WRITE_BLOCK[{i}]"
        session.send(CMD_ID, _cmd(device_id, OP_WRITE_BLOCK, struct.pack("<H", i)), name)
        session.send_data(block, name)
        _require(session.recv_response(name, T_WRITE, ACK), ACK, name)

    # VERIFY: device is authoritative. Trigger, then send image length + host CRC on DATA.
    session.send(CMD_ID, _cmd(device_id, OP_VERIFY_CRC32), "VERIFY_CRC32")
    session.send_data(struct.pack("<II", len(image), crc), "VERIFY len+crc")
    _require(session.recv_response("VERIFY_CRC32", T_VERIFY, CRC_OK), CRC_OK, "VERIFY_CRC32")

    _step(session, device_id, "COMMIT", OP_COMMIT, timeout=T_COMMIT)

    # ponytail: RESET reboots the node and may not ACK, so we fire and don't await.
    session.send(CMD_ID, _cmd(device_id, OP_RESET), "RESET")


def _device_id(text: str) -> int:
    try:
        value = int(text, 0)
    except ValueError:
        raise argparse.ArgumentTypeError(f"invalid device id: {text!r}")
    if not (DEVICE_ID_MIN <= value <= DEVICE_ID_MAX):
        raise argparse.ArgumentTypeError(
            f"device id must be {DEVICE_ID_MIN}..{DEVICE_ID_MAX}")
    return value


def _hex_addr(text: str) -> int:
    try:
        return int(text, 16)
    except ValueError:
        raise argparse.ArgumentTypeError(f"invalid hex address: {text!r}")


def build_parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(
        prog="can_flash.py",
        description="Flash a firmware .bin to an STM32 node over CAN using the "
                    "resident bootloader protocol "
                    "(bootloader/contract/bl_protocol.schema.json).")
    p.add_argument("--device-id", type=_device_id, metavar="0-119",
                   help="target node id (0..119); required to flash")
    p.add_argument("--bin", metavar="PATH", help="firmware .bin to flash")
    p.add_argument("--slot", choices=["A", "B"],
                   help="app slot for stm32g491vet (A=0x08004000, B=0x08040000)")
    p.add_argument("--base-addr", type=_hex_addr, metavar="HEX",
                   help="flash base address override, e.g. 0x08002000")
    p.add_argument("--adapter", choices=sorted(ADAPTERS), default=DEFAULT_ADAPTER,
                   help=f"CAN transport adapter (default: {DEFAULT_ADAPTER}); "
                        "see --list-adapters")
    p.add_argument("--interface",
                   help="advanced: python-can interface override for the adapter")
    p.add_argument("--channel", help="python-can channel, e.g. can0 or PCAN_USBBUS1")
    p.add_argument("--bitrate", type=int, default=DEFAULT_BITRATE,
                   help=f"CAN bitrate (default: {DEFAULT_BITRATE})")
    p.add_argument("--dry-run", action="store_true",
                   help="print the planned frames; do not open the bus")
    p.add_argument("--verbose", action="store_true", help="log every frame")
    p.add_argument("--list-adapters", action="store_true",
                   help="list the available transport adapters and exit")
    p.add_argument("--selftest", action="store_true",
                   help="run the CRC-32/MPEG-2 self-test and exit")
    return p


def _fail(msg: str) -> int:
    print(f"error: {msg}", file=sys.stderr)
    return 1


def main(argv=None) -> int:
    args = build_parser().parse_args(argv)

    if args.selftest:
        return 0 if _selftest() else 1

    if args.list_adapters:
        _list_adapters()
        return 0

    if args.device_id is None:
        return _fail("--device-id (0..119) is required to flash; or use --selftest")
    if not args.bin:
        return _fail("--bin PATH is required to flash")

    if args.base_addr is not None:
        base = args.base_addr
    elif args.slot is not None:
        base = SLOT_BASES[args.slot]
    else:
        return _fail("need --slot {A,B} or --base-addr HEX to know where to write")

    try:
        image = Path(args.bin).read_bytes()
    except OSError as exc:
        return _fail(f"cannot read --bin: {exc}")
    if not image:
        return _fail(f"--bin is empty: {args.bin}")

    iface, chan = resolve_transport(args.adapter, args.interface, args.channel)
    transport_desc = (f"adapter '{args.adapter}' (interface={iface}, "
                      f"channel={chan or '<none>'}, bitrate={args.bitrate})")

    if args.dry_run:
        session = _DryRunSession(args.device_id, args.verbose, transport_desc)
    else:
        session = BlSession(open_link(args.adapter, args.interface, args.channel,
                                      args.bitrate), args.device_id, args.verbose)
    try:
        flash(session, args.device_id, base, image)
    except FlashError as exc:
        return _fail(str(exc))
    finally:
        session.close()

    print("done." if not args.dry_run else "dry-run complete.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
