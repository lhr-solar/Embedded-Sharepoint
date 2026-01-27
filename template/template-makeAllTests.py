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

def find_tests(tests_dir: Path):
    print(tests_dir)
    if not tests_dir.is_dir():
        error("Something is horribly wrong. No tests directory found.")

    tests = []
    for cfile in tests_dir.glob("*.c"):
        tests.append(cfile)

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
    git_dir = Path(
        os.popen("git rev-parse --show-toplevel").read().strip()
    )

    make_flags = ["-B", "-s"]
    if args.verbose:
        make_flags = ["-B"]


    # ❗ CHANGE THIS ❗
    # If it's an LSOM then do - stm32g473xx
    # If it's a PSOM then do - stm32l431cbt
    ####
    ports = ["stm32g473xx"]
    ####

    # ❗ CHANGE THIS ❗
    # Change tests_dir to wherever your tests live relative to your top-level git repo directory
    ####
    tests_dir = Path("Software/Tests")
    ####
    tests = find_tests(git_dir / tests_dir)

    # ❗ CHANGE THIS ❗
    # Change makefile_dir to wherever your software Makefile lives relative to your top-level git repo directory
    ####
    makefile_dir = Path("Software")
    ####
    script_dir = git_dir / makefile_dir

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

