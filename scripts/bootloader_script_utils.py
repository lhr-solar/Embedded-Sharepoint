import glob
import os
import platform
import shutil
import subprocess
import sys
from pathlib import Path


def is_wsl() -> bool:
    if sys.platform != "linux":
        return False
    try:
        version = Path("/proc/version").read_text(encoding="utf-8").lower()
    except OSError:
        return False
    return "microsoft" in version or "wsl" in version


def first_existing_path(candidates: list[str]) -> str | None:
    for candidate in candidates:
        expanded = os.path.expandvars(os.path.expanduser(candidate))
        if os.path.isfile(expanded):
            return expanded
    return None


def default_stm32prog() -> str:
    env_path = os.environ.get("STM32_PROGRAMMER_CLI")
    if env_path:
        return env_path

    for exe in ("STM32_Programmer_CLI", "STM32_Programmer_CLI.exe"):
        path_match = shutil.which(exe)
        if path_match:
            return path_match

    candidates: list[str] = []
    if sys.platform == "darwin":
        candidates.append(
            "/Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer/"
            "STM32CubeProgrammer.app/Contents/Resources/bin/STM32_Programmer_CLI"
        )
    elif sys.platform == "win32":
        candidates.extend(
            [
                r"%ProgramFiles%\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe",
                r"%ProgramFiles(x86)%\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe",
            ]
        )
    else:
        candidates.extend(
            [
                "~/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI",
                "/opt/stm32cubeprogrammer/bin/STM32_Programmer_CLI",
                "/opt/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI",
            ]
        )
    existing = first_existing_path(candidates)
    if existing is not None:
        return existing
    return os.path.expandvars(os.path.expanduser(candidates[0])) if candidates else "STM32_Programmer_CLI"


def default_binary(pattern: str) -> str:
    matches = sorted(glob.glob(pattern))
    return matches[0] if matches else pattern


def serial_port_sort_key(port) -> tuple[int, str]:
    text = " ".join(
        str(value or "")
        for value in (getattr(port, "device", ""), getattr(port, "description", ""), getattr(port, "hwid", ""))
    ).lower()
    preferred_tokens = ("stm", "stlink", "st-link", "usbserial", "cp210", "silab", "usb")
    score = 0 if any(token in text for token in preferred_tokens) else 1
    return (score, getattr(port, "device", ""))


def detect_port_with_pyserial() -> str | None:
    try:
        from serial.tools import list_ports
    except ImportError:
        return None

    ports = sorted(list_ports.comports(), key=serial_port_sort_key)
    return ports[0].device if ports else None


def detect_port_with_glob() -> str | None:
    patterns: tuple[str, ...]
    if sys.platform == "darwin":
        patterns = (
            "/dev/cu.usbmodem*",
            "/dev/cu.usbserial*",
            "/dev/cu.SLAB_USBtoUART*",
        )
    elif sys.platform == "win32":
        return None
    else:
        patterns = ("/dev/ttyACM*", "/dev/ttyUSB*", "/dev/serial/by-id/*")

    for pattern in patterns:
        matches = sorted(glob.glob(pattern))
        if matches:
            return matches[0]
    return None


def detect_port() -> str | None:
    return detect_port_with_pyserial() or detect_port_with_glob()


def normalize_port_for_stm32prog(port: str, stm32prog: str) -> str:
    return port


def normalize_port_for_pyserial(port: str) -> str:
    return port


def port_help() -> str:
    system = platform.system()
    if is_wsl():
        return "No serial port found. Attach the USB-UART into WSL with usbipd, then pass --port /dev/ttyACM<N> or /dev/ttyUSB<N>."
    if system == "Windows":
        return "No serial port found. Pass --port COM<N>."
    if system == "Darwin":
        return "No serial port found. Pass --port /dev/cu.usbmodem..., /dev/cu.usbserial..., or /dev/cu.SLAB_USBtoUART..."
    return "No serial port found. Pass --port /dev/ttyACM<N>, /dev/ttyUSB<N>, or /dev/serial/by-id/..."


def validate_file(path: str, description: str, executable: bool = False) -> bool:
    if not os.path.isfile(path):
        print(f"{description} does not exist: {path}", file=sys.stderr)
        return False
    if executable and sys.platform != "win32" and not os.access(path, os.X_OK):
        print(f"{description} is not executable: {path}", file=sys.stderr)
        return False
    return True


def run_stm32prog(stm32prog: str, args: list[str]) -> None:
    cmd = [stm32prog] + args
    print("+ " + " ".join(f'"{part}"' if " " in part else part for part in cmd))
    subprocess.run(cmd, check=True)
