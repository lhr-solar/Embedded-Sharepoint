#!/usr/bin/env python3
"""Validate MDC → C header generation for vehicle databases."""

import json
import subprocess
import sys
import os
from pathlib import Path


def test_compilation(header_path: Path):
    test_c_file = header_path.with_suffix(".test_compile.c")
    with open(test_c_file, "w", encoding="utf-8") as f:
        f.write(f'#include "{header_path.name}"\n\n')
        f.write("int main(void) { return 0; }\n")

    try:
        result = subprocess.run(
            ["gcc", "-fsyntax-only", "-std=c11", "-Wall", "-Werror", "-I", str(header_path.parent), str(test_c_file)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        if result.returncode != 0:
            print(f"  [!] Compilation Error in {header_path.name}:")
            print(result.stderr)
            return False
        return True
    except FileNotFoundError:
        print("  [!] Error: 'gcc' not found. Please install a C compiler.")
        return False
    finally:
        if test_c_file.exists():
            os.remove(test_c_file)


def run_generator_and_test(generator_script: Path, mdc_file: Path):
    try:
        stem = mdc_file.name.removesuffix(".mdc.json").removesuffix(".json")
        header_path = mdc_file.with_name(f"{stem}.h")
        gen = generator_script.resolve()
        result = subprocess.run(
            ["node", str(gen), str(mdc_file.resolve()), "-o", str(header_path.resolve())],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        if result.returncode != 0:
            print(f"FAILED (Generator): {mdc_file.name}")
            print(result.stderr)
            return False

        if not header_path.exists():
            print(f"FAILED (Missing Output): Expected {header_path.name} to be created.")
            return False

        if not test_compilation(header_path):
            print(f"FAILED (C Syntax): {mdc_file.name}")
            return False

        print(f"PASSED: {mdc_file.name}")
        return True
    except Exception as e:
        print(f"EXCEPTION while processing {mdc_file.name}: {e}")
        return False


def main():
    if len(sys.argv) != 3:
        print("Usage:")
        print("  python validate_dbc_generation.py <vehicles_directory> <generate_can_headers.mjs>")
        sys.exit(1)

    vehicles_dir = Path(sys.argv[1])
    generator_script = Path(sys.argv[2])

    if not vehicles_dir.is_dir():
        print(f"Error: {vehicles_dir} is not a directory")
        sys.exit(1)

    if not generator_script.is_file():
        print(f"Error: {generator_script} not found")
        sys.exit(1)

    mdc_files = sorted(vehicles_dir.rglob("*.mdc.json"))
    if not mdc_files:
        print("No MDC files found.")
        sys.exit(0)

    print(f"Found {len(mdc_files)} MDC file(s). Validating generation and syntax...\n")

    failures = []
    for mdc_file in mdc_files:
        if not run_generator_and_test(generator_script, mdc_file):
            failures.append(mdc_file)

    print("\n" + "=" * 30)
    if failures:
        print(f"{len(failures)} test(s) FAILED.")
        for f in failures:
            print(f"  - {f}")
        sys.exit(1)
    print("Success! All MDC docs generated valid C headers.")
    sys.exit(0)


if __name__ == "__main__":
    main()
