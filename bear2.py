#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import jq
import sys
import json
import os

if __name__ == "__main__":
    # Read input directory from command-line
    if len(sys.argv) < 3:
        print("Usage: bear.py <build_dir> <output_file>")
        sys.exit(1)

    build_dir = sys.argv[1]
    output_file = sys.argv[2]

    if not os.path.isdir(build_dir):
        print(f"Error: {build_dir} is not a valid directory.")
        sys.exit(1)

    # Collect all files that start with 'cc_' in the given build directory
    input_lines = []
    try:
        for fname in os.listdir(build_dir):
            if fname.startswith("cc_"):
                full_path = os.path.join(build_dir, fname)
                with open(full_path, "r") as file:
                    input_lines.extend(file.readlines())
    except Exception as e:
        print(f"Error reading from directory: {e}")
        sys.exit(1)

    # jq filter to convert command line to Bear-style JSON object
    jq_filter = r'''
    . as $line
    | $line
    | split(" ")  
    | map(select(length > 0))
    | {
        arguments: [.[]],
        output: (
            (index("-o") as $o_idx | .[$o_idx + 1])
        ),
        file: (
            (
                map(select(test("\\.(c|s|S|asm)$"))) | .[-1]
            )
            | if type == "string" and startswith("/") then . 
              elif type == "string" then "/workdir/" + . 
              else null 
              end
        ),
        directory: "/workdir"
    }
    '''

    # Apply filter to each input line
    results = []
    for input_string in input_lines:
        input_string = input_string.strip()
        if not input_string:
            continue

        # Filter out irrelevant lines
        if "arm-none-eabi-gcc" not in input_string or " -c " not in input_string:
            continue

        try:
            result = jq.compile(jq_filter).input(input_string).first()
            results.append(result)
        except Exception as e:
            print(f"Error processing line: {e}\n  -> {input_string}")

    # Write to output
    try:
        with open(output_file, "w") as file:
            json.dump(results, file, indent=2)
        print(f"Results written to {output_file}")
    except Exception as e:
        print(f"Error writing to file: {e}")
        sys.exit(1)
