#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Build OpenBLT bootloader demo images (.srec/.elf/.bin) for STM32 G4/L4.

Usage:
  build_openblt_bootloader.sh [options]

Options:
  --openblt-dir <path>   Use an existing OpenBLT checkout.
  --clone-dir <path>     Clone OpenBLT here if --openblt-dir is not given.
  --target <name>        Build one target: g473 | g474 | g431 | l476 | l496 | all (default: all)
  --clean                Run make clean before build.
  -h, --help             Show this help message.

Notes:
  - Upstream does not ship a GCC G473 demo. `g473` is auto-generated from the G474 demo.
  - L431/L432 are not shipped as GCC demos in upstream OpenBLT.
EOF
}

OPENBLT_DIR=""
CLONE_DIR="${HOME}/openblt"
TARGET="all"
DO_CLEAN=0

while [[ $# -gt 0 ]]; do
  case "$1" in
    --openblt-dir)
      OPENBLT_DIR="${2:-}"
      shift 2
      ;;
    --clone-dir)
      CLONE_DIR="${2:-}"
      shift 2
      ;;
    --target)
      TARGET="${2:-}"
      shift 2
      ;;
    --clean)
      DO_CLEAN=1
      shift
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

if ! command -v arm-none-eabi-gcc >/dev/null 2>&1; then
  echo "arm-none-eabi-gcc not found in PATH" >&2
  exit 1
fi

if ! command -v arm-none-eabi-objcopy >/dev/null 2>&1; then
  echo "arm-none-eabi-objcopy not found in PATH" >&2
  exit 1
fi

if [[ -z "${OPENBLT_DIR}" ]]; then
  if [[ ! -d "${CLONE_DIR}/.git" ]]; then
    echo "Cloning OpenBLT into: ${CLONE_DIR}"
    git clone --depth 1 https://github.com/feaser/openblt.git "${CLONE_DIR}"
  fi
  OPENBLT_DIR="${CLONE_DIR}"
fi

if [[ ! -f "${OPENBLT_DIR}/README.md" ]]; then
  echo "Invalid OpenBLT directory: ${OPENBLT_DIR}" >&2
  exit 1
fi

declare -A DEMO_DIRS=(
  [g473]="Target/Demo/ARMCM4_STM32G4_Custom_G473_GCC/Boot"
  [g474]="Target/Demo/ARMCM4_STM32G4_Nucleo_G474RE_GCC/Boot"
  [g431]="Target/Demo/ARMCM4_STM32G4_Nucleo_G431RB_GCC/Boot"
  [l476]="Target/Demo/ARMCM4_STM32L4_Nucleo_L476RG_GCC/Boot"
  [l496]="Target/Demo/ARMCM4_STM32L4_Nucleo_L496ZG_GCC/Boot"
)

declare -A PROJ_NAMES=(
  [g473]="openblt_stm32g473"
  [g474]="openblt_stm32g474"
  [g431]="openblt_stm32g431"
  [l476]="openblt_stm32l476"
  [l496]="openblt_stm32l496"
)

prepare_g473_demo() {
  local src_demo="${OPENBLT_DIR}/Target/Demo/ARMCM4_STM32G4_Nucleo_G474RE_GCC"
  local dst_demo="${OPENBLT_DIR}/Target/Demo/ARMCM4_STM32G4_Custom_G473_GCC"
  local src_boot="${src_demo}/Boot"
  local dst_boot="${dst_demo}/Boot"
  local src_ld="${dst_boot}/STM32G474RETx_FLASH.ld"
  local dst_ld="${dst_boot}/STM32G473XXx_FLASH.ld"
  local mf="${dst_boot}/makefile"

  if [[ ! -d "${src_boot}" ]]; then
    echo "Cannot prepare g473 demo, missing source demo: ${src_boot}" >&2
    exit 1
  fi

  if [[ ! -d "${dst_demo}" ]]; then
    echo "Creating local G473 demo from G474 template: ${dst_demo}"
    cp -R "${src_demo}" "${dst_demo}"
  fi

  if [[ -f "${src_ld}" && ! -f "${dst_ld}" ]]; then
    cp "${src_ld}" "${dst_ld}"
  fi

  if [[ ! -f "${mf}" ]]; then
    echo "G473 makefile missing: ${mf}" >&2
    exit 1
  fi

  python3 - "${mf}" <<'PY'
import pathlib
import sys

path = pathlib.Path(sys.argv[1])
text = path.read_text()
text = text.replace("PROJ_NAME=openblt_stm32g474", "PROJ_NAME=openblt_stm32g473")
# Keep STM32G474xx device define because upstream demo CMSIS bundle does not
# ship stm32g473xx.h in this target pack.
text = text.replace('STM32G474RETx_FLASH.ld', 'STM32G473XXx_FLASH.ld')
path.write_text(text)
PY
}

build_one() {
  local key="$1"
  local demo_rel="${DEMO_DIRS[$key]}"
  local proj="${PROJ_NAMES[$key]}"
  local demo_abs="${OPENBLT_DIR}/${demo_rel}"
  local elf_path="${demo_abs}/bin/${proj}.elf"
  local bin_path="${demo_abs}/bin/${proj}.bin"

  if [[ ! -f "${demo_abs}/makefile" ]]; then
    echo "Demo not found for ${key}: ${demo_abs}" >&2
    exit 1
  fi

  echo ""
  echo "=== Building ${key} (${proj}) ==="
  mkdir -p "${demo_abs}/obj" "${demo_abs}/bin"
  if [[ "${DO_CLEAN}" -eq 1 ]]; then
    make -C "${demo_abs}" clean
    mkdir -p "${demo_abs}/obj" "${demo_abs}/bin"
  fi
  make -C "${demo_abs}" all

  if [[ ! -f "${elf_path}" ]]; then
    echo "Build did not produce ELF: ${elf_path}" >&2
    exit 1
  fi

  arm-none-eabi-objcopy -O binary "${elf_path}" "${bin_path}"
  echo "ELF : ${elf_path}"
  echo "SREC: ${demo_abs}/bin/${proj}.srec"
  echo "BIN : ${bin_path}"
}

if [[ "${TARGET}" == "all" ]]; then
  prepare_g473_demo
  build_one g473
  build_one g474
  build_one g431
  build_one l476
  build_one l496
else
  case "${TARGET}" in
    g473)
      prepare_g473_demo
      build_one g473
      ;;
    g474|g431|l476|l496) build_one "${TARGET}" ;;
    *)
      echo "Invalid --target value: ${TARGET}" >&2
      echo "Allowed: g473, g474, g431, l476, l496, all" >&2
      exit 1
      ;;
  esac
fi

echo ""
echo "Done."
echo "L431/L432 still need porting from l476/l496."
