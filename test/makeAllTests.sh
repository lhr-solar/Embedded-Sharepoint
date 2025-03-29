#!/bin/bash

# Find all .cfg files in ../stm and extract their base names
port_list=()
while IFS= read -r -d '' cfg_file; do
    port_list+=("$(basename "${cfg_file%.cfg}")")
done < <(find ../stm -type f -name "*.cfg" -print0)

echo "Compiling all tests for the following ports:"
for port in "${port_list[@]}"; do
    echo "$port"
done
echo "----------------------------------------"

# Compile tests for each port
for port in "${port_list[@]}"; do
    make clean
    for tests_dir in tests/*; do
        test_name="${tests_dir#tests/}"
        test_name="${test_name%.c}"

        echo "Compiling the test - $test_name for $port"
        if ! make TEST="$test_name" PROJECT_TARGET="$port" -j; then
            echo "Errors occurred while compiling $test_name.c using $port"
            exit 1
        fi
    done
done

echo "Jolly Good! All tests compiled successfully"