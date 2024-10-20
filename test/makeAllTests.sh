#!/bin/bash

for tests_dir in tests/*; do
    make clean
    test_name="${tests_dir#tests/}"
    test_name="${test_name%.c}"
    echo "Compiling the test - $test_name"

    if ! make TEST="$test_name" -j; then
        echo "Errors occurred during testing while testing $test_name.c"
        exit 1
    fi
done

echo "Jolly Good! All tests compiled successfully"