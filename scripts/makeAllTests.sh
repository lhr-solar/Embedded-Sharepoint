#!/bin/bash

port_list=("stm32f401re" "stm32f413rht" "stm32f429zit" "stm32f446ret" "stm32l431cbt" "stm32l476rgt")

makefile_dir="Embedded-Sharepoint/test/Makefile"
tests_dir="Embedded-Sharepoint/test/tests"

for port in "${port_list[@]}"; do
    
    make -f "$makefile_dir" clean
    for test_file in "$tests_dir"/*; do
        test_name="${test_file#$tests_dir/}"
        test_name="${test_name%.c}"
        
        # Skip the "can" test for stm32f401*e or stm32f401*c
        if [[ "$port" =~ ^stm32f401.*[ec]$ ]] && ([[ "$test_name" == "can" ]] || [[ "$test_name" == "can_mt" ]]); then
            echo "Skipping the CAN test for $port because it does not support CAN"
            continue
        fi

        echo "Compiling the test - $test_name for $port"

        if ! make -f "$makefile_dir" TEST="$test_name" PROJECT_TARGET="$port" -j; then
            echo "Errors occurred while compiling $test_name.c using $port"
            exit 1
        fi
    done

done

echo "Jolly Good! All tests compiled successfully"