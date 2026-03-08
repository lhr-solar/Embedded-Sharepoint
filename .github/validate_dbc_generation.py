#!/usr/bin/env python3

import subprocess
import sys
from pathlib import Path

def run_generator(generator_script, dbc_file):
    """
    Runs the header generator script for a single DBC file.
    Returns True on success, False on failure.
    """
    try:
        result = subprocess.run(
            ["python3", generator_script, str(dbc_file)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        if result.returncode != 0:
            print(f"\nFAILED: {dbc_file}")
            print("STDERR:")
            print(result.stderr)
            return False

        print(f"OK: {dbc_file}")
        return True

    except Exception as e:
        print(f"\nEXCEPTION while processing {dbc_file}")
        print(str(e))
        return False


def main():
    if len(sys.argv) != 3:
        print("Usage:")
        print("  python validate_dbc_generation.py <dbc_directory> <generator_script>")
        sys.exit(1)

    dbc_directory = Path(sys.argv[1])
    generator_script = Path(sys.argv[2])

    if not dbc_directory.is_dir():
        print(f"Error: {dbc_directory} is not a directory")
        sys.exit(1)

    if not generator_script.is_file():
        print(f"Error: {generator_script} not found")
        sys.exit(1)

    dbc_files = list(dbc_directory.rglob("*.dbc"))

    if not dbc_files:
        print("No DBC files found.")
        sys.exit(0)

    print(f"Found {len(dbc_files)} DBC files.\n")

    failures = []

    for dbc_file in dbc_files:
        success = run_generator(generator_script, dbc_file)
        if not success:
            failures.append(dbc_file)

    print("\n==============================")

    if failures:
        print(f"{len(failures)} DBC files FAILED.")
        for f in failures:
            print(f"  - {f}")
        sys.exit(1)
    else:
        print("All DBC files processed successfully.")
        sys.exit(0)


if __name__ == "__main__":
    main()