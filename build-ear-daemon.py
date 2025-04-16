#!/usr/bin/env python3
import os
import json
import shutil
import sys
import threading

from numba import njit, jit
from concurrent.futures import ThreadPoolExecutor
PIPE_PATH = "/tmp/compile_command_pipe"

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
VSCODE_DIR = os.path.join(BASE_DIR, ".vscode")
os.makedirs(VSCODE_DIR, exist_ok=True)

COMPILE_COMMANDS_PATH = os.path.join(VSCODE_DIR, "compile_commands.json")

running = True

def ensure_pipe():
    if not os.path.exists(PIPE_PATH):
        os.mkfifo(PIPE_PATH)

def save_compile_commands(compile_commands):
    with open(COMPILE_COMMANDS_PATH, "w") as f:
        json.dump(compile_commands, f, indent=2)

def parse_compile_command(compile_command):
    compile_args = compile_command.split()
    try:
        output_index = compile_args.index("-o") + 1
        source_index = output_index - 2
        output_file = compile_args[output_index]
        source_file = compile_args[source_index]
    except ValueError:
        print("Error: '-o' flag not found in the compile command.", file=sys.stdout)
        sys.exit(1)

    compile_args[0] = shutil.which(compile_args[0])

    new_entry = {
        "arguments": compile_args,
        "directory": BASE_DIR,
        "file": os.path.abspath(os.path.join(BASE_DIR, source_file)),
        "output": os.path.abspath(os.path.join(BASE_DIR, output_file))
    }

    return new_entry

compile_commands = []
executor = ThreadPoolExecutor(max_workers=os.cpu_count())

def process_compile_command(line):
    global compile_commands
    if not line or "gcc" not in line:
        return

    print(f"[Daemon] Processing compile command: {line}", file=sys.stdout)
    new_entry = parse_compile_command(line)
    if new_entry:
        compile_commands.append(new_entry)

def daemon_loop():
    ensure_pipe()

    global running
    while running:
        with open(PIPE_PATH, "r") as pipe:
            for line in pipe:
                line = line.strip()

                if "close-build-ear" in line:
                    print("[Daemon] Received close-build-ear command. Exiting...", file=sys.stdout)
                    running = False
                    break

                executor.submit(process_compile_command, line)

    executor.shutdown(wait=True)
    save_compile_commands(compile_commands)

def save_loop():
    global compile_commands
    global running
    while running:
        if compile_commands:  # Ensure the list is not empty
            print("[Daemon] Saving compile commands...", file=sys.stdout)
            save_compile_commands(compile_commands)
        threading.Event().wait(1)  # Use threading.Event().wait instead of os.sleep

def main():
    os.makedirs(os.path.dirname(COMPILE_COMMANDS_PATH), exist_ok=True)
    dthread = threading.Thread(target=daemon_loop)
    # sthread = threading.Thread(target=save_loop)

    dthread.start()
    # sthread.start()
    
    # Wait for daemon loop to finish
    dthread.join()
    # sthread.join()
    os.remove(PIPE_PATH)
    
    print("[Daemon] Daemon stopped.", file=sys.stdout)

if __name__ == "__main__":
    main()
