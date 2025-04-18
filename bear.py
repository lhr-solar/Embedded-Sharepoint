#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import jq
import sys
import json

if __name__ == "__main__":
    # Read input line-by-line from a file specified as the first command-line argument
    if len(sys.argv) < 2:
        print("Usage: bear.py <input_file> <output_file>")
        sys.exit(1)

    input_file = sys.argv[1]

    try:
        with open(input_file, "r") as file:
            input_lines = file.readlines()
    except Exception as e:
        print(f"Error reading file: {e}")
        sys.exit(1)

    # Define the jq filter
    jq_filter = """
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
            (map(select(test("\\\\.c$"))) | .[-1])
            | if startswith("/") then . else "/workdir/" + . end
        ),
        directory: "/workdir"
    }
    """

    # Process each line with the jq filter
    results = []
    for input_string in input_lines:
        input_string = input_string.strip()
        if not input_string:
            continue

        # Filter out non-compilation lines
        if "arm-none-eabi-gcc" not in input_string or " -c " not in input_string:
            continue

        try:
            result = jq.compile(jq_filter).input(input_string).first()
            results.append(result)
        except Exception as e:
            print(f"Error processing line: {e}")

    # Print the results
    print(results)

    # Write results to a file with pretty-printed JSON
    output_file = sys.argv[2]
    try:
        with open(output_file, "w") as file:
            json.dump(results, file, indent=2)
        print(f"Results written to {output_file}")
    except Exception as e:
        print(f"Error writing to file: {e}")
        sys.exit(1)
