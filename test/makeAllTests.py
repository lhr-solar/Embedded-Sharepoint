#!/usr/bin/env python3
import argparse
import os
import sys
import subprocess
from pathlib import Path
from multiprocessing import cpu_count
from concurrent.futures import ThreadPoolExecutor, as_completed

# ANSI color codes
RED = "\033[0;31m"
GREEN = "\033[0;32m"
YELLOW = "\033[0;33m"
BLUE = "\033[0;34m"
NC = "\033[0m"

def error(msg, exit_code=1):
    print(f"{RED}[ERROR] {msg}{NC}", file=sys.stderr)
    sys.exit(exit_code)

def info(msg, color=BLUE):
    print(f"{color}[INFO] {msg}{NC}")

def parse_args():
    parser = argparse.ArgumentParser(
        description="Compile all tests for all STM ports",
        add_help=False
    )
    parser.add_argument("-v", "--verbose", action="store_true",
                        help="Enable verbose output (show commands as they're run)")
    parser.add_argument("-h", "--help", action="help",
                        help="Show this help message and exit")
    return parser.parse_args()

def find_ports(stm_dir: Path):
    if not stm_dir.is_dir():
        error("Something is horribly wrong. No stm directory found.")

    ports = []
    for ld in stm_dir.rglob("*.ld"):
        name = ld.stem
        name = name.replace("_FLASH", "")
        name = name.replace("x", "")
        name = name.replace("_", "")
        ports.append(name.lower())

    if not ports:
        error("Something is horribly wrong. No port linker scripts found.")

    return ports

def find_tests(tests_dir: Path):
    if not tests_dir.is_dir():
        error("Something is horribly wrong. No tests directory found.")

    tests = []
    for cfile in tests_dir.glob("*_test.c"):
        tests.append(cfile.stem.replace("_test", ""))

    if not tests:
        error("Something is horribly wrong. No test files found in the tests directory.")

    return tests

def compile_test(script_dir: Path, port: str, test_name: str,
                 make_flags: list[str], verbose: bool):
    info(f"Compiling the test - {test_name} for {port}")

    build_dir = script_dir.parent / "build" / port / test_name

    cmd = [
        "make",
        "-C", str(script_dir),
        f"TEST={test_name}",
        f"PROJECT_TARGET={port}",
        "BEAR_ENABLE=0",
        f"PROJECT_BUILD_DIR={build_dir}",
        *make_flags,
    ]

    try:
        proc = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            check=False,
        )
    except Exception as e:
        return False, f"Failed to invoke make: {e}"

    output = proc.stdout + proc.stderr

    if proc.returncode != 0:
        print(f"{RED}[{port}:{test_name}] {output}{NC}")
        if "[CONFIG]" in output:
            print(f"{YELLOW}[WARNING] CONFIG issue. Continuing...{NC}")
            return True, None
        return False, f"Errors occurred while compiling {test_name}.c using {port}"

    info(f"Successfully compiled {test_name}.c : {port}", GREEN)
    if verbose and output.strip():
        print(f"{GREEN}[INFO] Output:{NC}\n{output}")

    return True, None

def main():
    args = parse_args()
    script_dir = Path(__file__).resolve().parent

    make_flags = ["-B", "-s"]
    if args.verbose:
        make_flags = ["-B"]

    ports = find_ports(script_dir / "../stm")
    tests = find_tests(script_dir / "tests")

    info("Compiling all tests for the following ports:")
    for p in ports:
        print(p)
    print(f"{BLUE}{'-' * 40}{NC}")

    failures = []

    max_workers = cpu_count()
    with ThreadPoolExecutor(max_workers=max_workers) as executor:
        futures = []
        for port in ports:
            for test in tests:
                futures.append(
                    executor.submit(
                        compile_test,
                        script_dir,
                        port,
                        test,
                        make_flags,
                        args.verbose,
                    )
                )

        for fut in as_completed(futures):
            ok, err = fut.result()
            if not ok and err:
                failures.append(err)

    if failures:
        error("Error: Some builds failed")

    info("Jolly Good! All tests compiled successfully", GREEN)

if __name__ == "__main__":
    main()

