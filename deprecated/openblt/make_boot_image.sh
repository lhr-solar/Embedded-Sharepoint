#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Generate an OpenBLT-friendly S-record image from an ELF.

Usage:
  make_boot_image.sh --elf <input.elf> [--out <output.srec>]

Examples:
  ./scripts/openblt/make_boot_image.sh --elf test/build/stm32g473xx.elf
  ./scripts/openblt/make_boot_image.sh --elf test/build/stm32l431cbt.elf --out test/build/app.srec
EOF
}

ELF_PATH=""
OUT_PATH=""

while [[ $# -gt 0 ]]; do
  case "$1" in
    --elf)
      ELF_PATH="${2:-}"
      shift 2
      ;;
    --out)
      OUT_PATH="${2:-}"
      shift 2
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    *)
      echo "Unknown argument: $1" >&2
      usage
      exit 1
      ;;
  esac
done

if [[ -z "$ELF_PATH" ]]; then
  echo "--elf is required" >&2
  usage
  exit 1
fi

if [[ ! -f "$ELF_PATH" ]]; then
  echo "ELF file does not exist: $ELF_PATH" >&2
  exit 1
fi

if ! command -v arm-none-eabi-objcopy >/dev/null 2>&1; then
  echo "arm-none-eabi-objcopy not found in PATH" >&2
  exit 1
fi

if [[ -z "$OUT_PATH" ]]; then
  OUT_PATH="${ELF_PATH%.elf}.srec"
fi

mkdir -p "$(dirname "$OUT_PATH")"

arm-none-eabi-objcopy -O srec "$ELF_PATH" "$OUT_PATH"

echo "Generated S-record: $OUT_PATH"
echo "Next: flash this .srec using OpenBLT BootCommander over USB or CAN."
