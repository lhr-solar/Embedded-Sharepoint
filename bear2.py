#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import jq
import sys
import json
import os

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

    jq_filter = r'''
    .
    | .workdir as $workdir
    | .command as $line
    | $line
    | split(" ")
    | map(select(length > 0))
    | {
        arguments: [.[]],
        output: (
          (index("-o") as $o_idx | .[$o_idx + 1])
          | if startswith("/") then . else $workdir + "/" + . end
        ),
        file: (
          (
            map(select(test("\\.(c|s|S|asm)$"))) | .[-1]
          )
          | if type == "string" and startswith("/") then .
            elif type == "string" then $workdir + "/" + .
            else null
          end
        ),
        directory: $workdir
      }
    '''

    compiled = jq.compile(jq_filter)

    results = []
    for workdir, command in input_pairs:
        if " -c " not in command:
            continue
        try:
            res = compiled.input({"command": command, "workdir": workdir}).first()
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
