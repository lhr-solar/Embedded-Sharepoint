#!/usr/bin/env python3
import argparse
import os
import sys
import time


APP_BASE = 0x08010000
DEFAULT_APP_BIN = "build/app/stm32l431cbt.bin"

BL_ACK = 0x79
BL_NACK = 0x1F

BL_CMD_GET = 0x00
BL_CMD_READ_MEMORY = 0x11
BL_CMD_GO = 0x21
BL_CMD_WRITE_MEMORY = 0x31
BL_CMD_ERASE = 0x43

BOOTLOADER_CAN_SYNC_ID = 0x079
BOOTLOADER_CAN_COMMAND_ID = 0x000


class BootloaderError(RuntimeError):
    pass


def parse_int(value: str) -> int:
    return int(value, 0)


def load_can_module():
    try:
        import can
    except ImportError as exc:
        raise BootloaderError("python-can is required. Install requirements.txt or run inside nix develop.") from exc
    return can


class CanBootloader:
    def __init__(self, bus, timeout: float) -> None:
        self.bus = bus
        self.timeout = timeout

    def send_frame(self, frame_id: int, data: bytes = b"") -> None:
        can = load_can_module()
        msg = can.Message(arbitration_id=frame_id, data=data, is_extended_id=False)
        self.bus.send(msg, timeout=self.timeout)

    def recv_frame(self, expected_id: int | None = None, timeout: float | None = None):
        deadline = time.monotonic() + (self.timeout if timeout is None else timeout)
        while time.monotonic() < deadline:
            msg = self.bus.recv(timeout=max(0.0, deadline - time.monotonic()))
            if msg is None:
                continue
            if msg.is_extended_id or getattr(msg, "is_remote_frame", False):
                continue
            if expected_id is None or msg.arbitration_id == expected_id:
                return msg
        expected = "any frame" if expected_id is None else f"frame 0x{expected_id:03X}"
        raise BootloaderError(f"Timed out waiting for {expected}")

    def wait_ack(self, frame_id: int) -> None:
        msg = self.recv_frame(frame_id)
        if not msg.data:
            raise BootloaderError(f"Empty response on 0x{frame_id:03X}")
        code = msg.data[0]
        if code == BL_ACK:
            return
        if code == BL_NACK:
            raise BootloaderError(f"NACK on 0x{frame_id:03X}")
        raise BootloaderError(f"Unexpected response on 0x{frame_id:03X}: 0x{code:02X}")

    def sync(self, retries: int) -> None:
        last_error: Exception | None = None
        for _ in range(retries):
            try:
                self.send_frame(BOOTLOADER_CAN_SYNC_ID)
                self.wait_ack(BOOTLOADER_CAN_SYNC_ID)
                return
            except Exception as exc:
                last_error = exc
                time.sleep(0.1)
        raise BootloaderError(f"CAN bootloader sync failed: {last_error}")

    def get(self) -> list[int]:
        self.send_frame(BL_CMD_GET)
        self.wait_ack(BL_CMD_GET)

        count = bytes(self.recv_frame(BL_CMD_GET).data)[0]
        version = bytes(self.recv_frame(BL_CMD_GET).data)[0]
        commands = [bytes(self.recv_frame(BL_CMD_GET).data)[0] for _ in range(count)]
        self.wait_ack(BL_CMD_GET)

        print(f"Bootloader version 0x{version:02X}, commands: " + ", ".join(f"0x{cmd:02X}" for cmd in commands))
        return commands

    def erase_app(self) -> None:
        self.send_frame(BL_CMD_ERASE, b"\xFF")
        self.wait_ack(BL_CMD_ERASE)

    def write_memory(self, address: int, data: bytes) -> None:
        if not 1 <= len(data) <= 256:
            raise ValueError("write length must be 1..256 bytes")

        request = address.to_bytes(4, "big") + bytes((len(data) - 1,))
        self.send_frame(BL_CMD_WRITE_MEMORY, request)
        self.wait_ack(BL_CMD_WRITE_MEMORY)

        for offset in range(0, len(data), 8):
            self.send_frame(BL_CMD_WRITE_MEMORY, data[offset:offset + 8])
            self.wait_ack(BL_CMD_WRITE_MEMORY)

    def read_memory(self, address: int, length: int) -> bytes:
        if not 1 <= length <= 256:
            raise ValueError("read length must be 1..256 bytes")

        request = address.to_bytes(4, "big") + bytes((length - 1,))
        self.send_frame(BL_CMD_READ_MEMORY, request)
        self.wait_ack(BL_CMD_READ_MEMORY)

        out = bytearray()
        while len(out) < length:
            msg = self.recv_frame(BL_CMD_READ_MEMORY)
            out.extend(bytes(msg.data))
        self.wait_ack(BL_CMD_READ_MEMORY)
        return bytes(out[:length])

    def go(self, address: int) -> None:
        self.send_frame(BL_CMD_GO, address.to_bytes(4, "big"))
        self.wait_ack(BL_CMD_GO)


def make_bus(interface: str, channel: str, bitrate: int):
    can = load_can_module()
    return can.Bus(interface=interface, channel=channel, bitrate=bitrate, receive_own_messages=False)


def send_control_frame(interface: str, channel: str, bitrate: int, value: int) -> None:
    with make_bus(interface, channel, bitrate) as bus:
        can = load_can_module()
        msg = can.Message(
            arbitration_id=BOOTLOADER_CAN_COMMAND_ID,
            data=bytes((value & 0xFF,)),
            is_extended_id=False,
        )
        bus.send(msg, timeout=1.0)


def flash(args: argparse.Namespace) -> None:
    if not os.path.isfile(args.bin):
        raise BootloaderError(f"Binary does not exist: {args.bin}")

    with open(args.bin, "rb") as f:
        image = f.read()
    if not image:
        raise BootloaderError(f"Binary is empty: {args.bin}")

    if args.enter_value is not None:
        print(f"Sending CAN boot-control value 0x{args.enter_value & 0xFF:02X}")
        send_control_frame(args.interface, args.channel, args.bitrate, args.enter_value)
        time.sleep(args.enter_delay)

    with make_bus(args.interface, args.channel, args.bitrate) as bus:
        bl = CanBootloader(bus, args.timeout)

        print("Synchronizing with CAN bootloader")
        bl.sync(args.sync_retries)

        if args.get:
            bl.get()

        if not args.no_erase:
            print("Erasing app region")
            bl.erase_app()

        total = len(image)
        print(f"Writing {total} bytes to 0x{args.address:08X}")
        for offset in range(0, total, args.chunk_size):
            chunk = image[offset:offset + args.chunk_size]
            bl.write_memory(args.address + offset, chunk)
            print(f"\rWrote {min(offset + len(chunk), total)}/{total} bytes", end="", flush=True)
        print()

        if args.verify:
            print("Verifying")
            for offset in range(0, total, args.chunk_size):
                expected = image[offset:offset + args.chunk_size]
                actual = bl.read_memory(args.address + offset, len(expected))
                if actual != expected:
                    raise BootloaderError(f"Verify failed at 0x{args.address + offset:08X}")
                print(f"\rVerified {min(offset + len(expected), total)}/{total} bytes", end="", flush=True)
            print()

        if args.boot:
            print(f"Jumping to 0x{args.address:08X}")
            bl.go(args.address)


def main() -> int:
    parser = argparse.ArgumentParser(description="Flash app via ES_BLT classic CAN bootloader using python-can")
    parser.add_argument("--interface", default="pcan", help="python-can interface backend (default: pcan)")
    parser.add_argument("--channel", default="PCAN_USBBUS1", help="CAN channel (default: PCAN_USBBUS1)")
    parser.add_argument("--bitrate", default=125000, type=int, help="CAN bitrate in bit/s (default: 125000)")
    parser.add_argument("--bin", default=DEFAULT_APP_BIN, help=f"App binary to flash (default: {DEFAULT_APP_BIN})")
    parser.add_argument("--address", default=APP_BASE, type=parse_int, help=f"Flash address (default: 0x{APP_BASE:08X})")
    parser.add_argument("--timeout", default=1.0, type=float, help="CAN response timeout in seconds")
    parser.add_argument("--sync-retries", default=10, type=int, help="CAN sync retry count")
    parser.add_argument("--chunk-size", default=256, type=int, choices=range(1, 257), metavar="[1-256]")
    parser.add_argument("--enter-value", type=parse_int, help="Send app boot-control value on CAN ID 0x000 before flashing")
    parser.add_argument("--enter-delay", default=0.75, type=float, help="Delay after --enter-value before sync")
    parser.add_argument("--reset-all", action="store_true", help="Send CAN ID 0x000 byte 0x00 and exit")
    parser.add_argument("--no-erase", action="store_true", help="Skip app erase before writing")
    parser.add_argument("--no-verify", dest="verify", action="store_false", help="Skip readback verification")
    parser.add_argument("--get", action="store_true", help="Print bootloader version and supported commands")
    parser.add_argument("--boot", action="store_true", help="Jump to app after flashing")
    parser.set_defaults(verify=True)
    args = parser.parse_args()

    try:
        if args.reset_all:
            send_control_frame(args.interface, args.channel, args.bitrate, 0)
            print("Sent CAN reset-all control frame")
            return 0
        flash(args)
    except Exception as exc:
        print(f"CAN bootloader flash failed: {exc}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
