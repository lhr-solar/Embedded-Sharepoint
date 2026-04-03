#!/usr/bin/env python3

import subprocess
import sys
import os
from pathlib import Path

def test_compilation(header_path: Path):
    """
    Creates a temporary C file to verify the header is syntactically valid.
    """
    test_c_file = header_path.with_suffix(".test_compile.c")
    
    # Create a dummy C file that includes the generated header
    with open(test_c_file, "w") as f:
        f.write(f'#include "{header_path.name}"\n\n')
        f.write("int main(void) { return 0; }\n")

    try:
        # -fsyntax-only checks for errors without needing to link or output a binary
        # -I. ensures the compiler looks in the current directory for the header
        result = subprocess.run(
            ["gcc", "-fsyntax-only", "-I", str(header_path.parent), str(test_c_file)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
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
        # Clean up the dummy C file
        if test_c_file.exists():
            os.remove(test_c_file)


def run_generator_and_test(generator_script, dbc_file):
    """
    Runs the header generator and then attempts to compile the output.
    """
    try:
        # Run the Generator
        result = subprocess.run(
            ["python3", str(generator_script), str(dbc_file)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        if result.returncode != 0:
            print(f"FAILED (Generator): {dbc_file.name}")
            print(result.stderr)
            return False

        # Identify the generated header (assuming standard naming from your previous script)
        header_path = dbc_file.with_name(f"{dbc_file.stem}_can_msgs.h")
        
        if not header_path.exists():
            print(f"FAILED (Missing Output): Expected {header_path.name} to be created.")
            return False

        # 3. Test Compilation
        if not test_compilation(header_path):
            print(f"FAILED (C Syntax): {dbc_file.name}")
            return False

        print(f"PASSED: {dbc_file.name}")
        return True

    except Exception as e:
        print(f"EXCEPTION while processing {dbc_file.name}: {e}")
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

    print(f"Found {len(dbc_files)} DBC files. Validating generation and syntax...\n")

    failures = []

    for dbc_file in dbc_files:
        success = run_generator_and_test(generator_script, dbc_file)
        if not success:
            failures.append(dbc_file)

    print("\n" + "="*30)

    if failures:
        print(f"{len(failures)} test(s) FAILED.")
        for f in failures:
            print(f"  - {f}")
        sys.exit(1)
    else:
        print("Success! All DBCs generated valid C headers.")
        sys.exit(0)


if __name__ == "__main__":
    main()