#!/usr/bin/env bash
set -e

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <dir1> [dir2 ...]"
    exit 1
fi

for dir in "$@"; do
    if [ -d "$dir" ]; then
        echo "Formatting C/C++ files in $dir..."
        find "$dir" -type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" \) \
            -exec clang-format -i {} +
    else
        echo "Warning: $dir is not a directory, skipping."
    fi
done

echo "Done."
