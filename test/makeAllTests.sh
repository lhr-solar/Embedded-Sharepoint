#!/bin/bash

# Define color codes
RED=$'\033[0;31m'
GREEN=$'\033[0;32m'
YELLOW=$'\033[0;33m'
BLUE=$'\033[0;34m'
NC=$'\033[0m' # No Color

script_dir=$(cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

export script_dir RED GREEN YELLOW BLUE NC

# Find all .cfg files in ../stm and extract their base names
port_list=()
while IFS= read -r -d '' cfg_file; do
    port_list+=("$(basename "${cfg_file%.cfg}")")
done < <(find ../stm -type f -name "*.cfg" -print0)

echo -e "${BLUE}[INFO] Compiling all tests for the following ports:${NC}"
for port in "${port_list[@]}"; do
    echo "$port"
done
echo -e "${BLUE}----------------------------------------${NC}"

# Collect all test names
test_list=()
for test_file in tests/*.c; do
    test_name=$(basename "$test_file" .c)
    test_list+=("$test_name")
done

# Function to compile a single test for a port with tagged output
compile_test() {
    local port=$1
    local test_name=$2
    # local script_dir=$3
    echo -e "${BLUE}[INFO] Compiling the test - $test_name for $port${NC}"

    project_build_dir=$script_dir/../build/$port/$test_name
    output=$(make TEST="$test_name" PROJECT_TARGET="$port" BEAR_ENABLE=0 PROJECT_BUILD_DIR=$project_build_dir -s 2>&1)
    error_code=$?
    if [ $error_code -ne 0 ]; then
        printf "${RED}[%s:%s] %s${NC}\n" "$port" "$test_name" "$output"
        if [[ "$output" == *"[CONFIG]"* ]]; then
            echo -e "${YELLOW}[WARNING] CONFIG issue. Continuing...${NC}"
            return 0
        fi
        echo -e "${RED}[ERROR] Errors occurred while compiling $test_name.c using $port${NC}"
        return 1
    fi
    echo -e "${GREEN}[INFO] Successfully compiled $test_name.c : $port${NC}"
    return 0
}

# Export function to make it available to GNU Parallel
export -f compile_test

# Use GNU Parallel to run compilations in parallel
if ! parallel --halt now,fail=1 -j $(nproc) compile_test \
    ::: "${port_list[@]}" \
    ::: "${test_list[@]}"; then
    echo -e "${RED}[ERROR] Error: Some builds failed${NC}"
    exit 1
fi

echo -e "${GREEN}[INFO] Jolly Good! All tests compiled successfully${NC}"