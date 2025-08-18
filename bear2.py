#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys
import json
import os
import re

def load_commands(build_dir):
    """Read cc_*.txt files and extract (workdir, command) pairs."""
    pairs = []
    for fname in os.listdir(build_dir):
        if fname.startswith("cc_") and fname.endswith(".txt"):
            full_path = os.path.join(build_dir, fname)
            try:
                with open(full_path, "r") as f:
                    lines = [line.strip() for line in f if line.strip()]
                    if len(lines) >= 2:
                        pairs.append((lines[0], lines[1]))
            except Exception as e:
                print(f"Warning: Failed to read {full_path}: {e}", file=sys.stderr)
    return pairs

def process_command(workdir, command):
    """Convert a command string into a dict similar to your jq filter."""
    if " -c " not in command:
        return None

    # Split command into arguments, ignoring extra spaces
    args = [arg for arg in command.split(" ") if arg]

    # Find output file after "-o" flag
    output = None
    if "-o" in args:
        o_idx = args.index("-o")
        if o_idx + 1 < len(args):
            output = args[o_idx + 1]
            if not output.startswith("/"):
                output = os.path.join(workdir, output)

    # Find source file ending with .c, .s, .S, or .asm
    src_files = [arg for arg in args if re.search(r"\.(c|s|S|asm)$", arg)]
    file = None
    if src_files:
        file = src_files[-1]
        if not file.startswith("/"):
            file = os.path.join(workdir, file)

    return {
        "arguments": args,
        "output": output,
        "file": file,
        "directory": workdir
    }

def main():
    if len(sys.argv) < 3:
        print("Usage: bear.py <build_dir> <output_file>")
        sys.exit(1)

    build_dir = sys.argv[1]
    output_file = sys.argv[2]

    if not os.path.isdir(build_dir):
        print(f"Error: {build_dir} is not a valid directory.")
        sys.exit(1)

    input_pairs = load_commands(build_dir)
    results = []

    for workdir, command in input_pairs:
        try:
            res = process_command(workdir, command)
            if res is not None:
                results.append(res)
        except Exception as e:
            print(f"Warning: Failed to process command in {workdir}: {e}", file=sys.stderr)

    try:
        with open(output_file, "w") as f:
            json.dump(results, f, indent=2)
        print(f"Results written to {output_file}")
    except Exception as e:
        print(f"Error writing to output file: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
