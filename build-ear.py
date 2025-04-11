#!/usr/bin/env python3
import sys
import os
import json
import shutil
from numba import njit,jit

def parse_compile_command(compile_command):
    compile_args = compile_command.split()
    try:
        output_index = compile_args.index("-o") + 1
        source_index = output_index - 2
        output_file = compile_args[output_index]
        source_file = compile_args[source_index]
    except ValueError:
        print("Error: '-o' flag not found in the compile command.")
        sys.exit(1)

    compile_args[0] = shutil.which(compile_args[0])

    return compile_args, output_file, source_file

@jit
def load_compile_commands(file_path):
    if os.path.exists(file_path) and os.path.getsize(file_path) > 0:
        with open(file_path, "r") as f:
            return json.load(f)
    return []

@jit
def save_compile_commands(file_path, compile_commands):
    with open(file_path, "w") as f:
        json.dump(compile_commands, f, indent=2)

@jit
def update_compile_commands(compile_commands_path, new_entry):
    compile_commands = load_compile_commands(compile_commands_path)
    compile_commands.append(new_entry)
    save_compile_commands(compile_commands_path, compile_commands)

def main():
    if len(sys.argv) < 2:
        print("Error: Missing compile command argument.")
        sys.exit(1)

    compile_command = sys.argv[1].strip()
    if "gcc" not in compile_command:
        sys.exit(0)

    base_dir = os.path.dirname(os.path.abspath(__file__))
    vscode_dir = os.path.join(base_dir, ".vscode")
    os.makedirs(vscode_dir, exist_ok=True)

    compile_args, output_file, source_file = parse_compile_command(compile_command)
    new_entry = {
        "arguments": compile_args,
        "directory": base_dir,
        "file": os.path.abspath(os.path.join(base_dir, source_file)),
        "output": os.path.abspath(os.path.join(base_dir, output_file))
    }

    compile_commands_path = os.path.join(vscode_dir, "compile_commands.json")
    

if __name__ == "__main__":
    main()
