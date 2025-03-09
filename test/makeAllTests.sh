#!/bin/bash

port_list=("stm32f401re" "stm32f413rht" "stm32f429zit" "stm32f446ret" "stm32l431cbt" "stm32l476rgt")

# Boards that should run UART tests since they support UART4 & UART5, will add more boards as they are tested
uart_enabled_ports=("stm32f446ret" "stm32l476rgt")

for port in "${port_list[@]}"; do
    make clean
    for tests_dir in tests/*; do
        test_name="${tests_dir#tests/}"
        test_name="${test_name%.c}"
        
        # Skip the "can" test for stm32f401*e or stm32f401*c
        if [[ "$port" =~ ^stm32f401.*[ec]$ ]] && ([[ "$test_name" == "can" ]] || [[ "$test_name" == "can_mt" ]]); then
            echo "Skipping the CAN test for $port because it does not support CAN"
            continue
        fi
        
        # Skip UART tests for non-UART boards
        if ([[ "$test_name" == "uart" ]] || [[ "$test_name" == "uart_mt" ]]) && [[ ! " ${uart_enabled_ports[@]} " =~ " ${port} " ]]; then
            echo "Skipping the UART test for $port because it does not support UART4 & UART5"
            continue
        fi

        echo "Compiling the test - $test_name for $port"
        if ! make TEST="$test_name" PROJECT_TARGET="$port" -j; then
            echo "Errors occurred while compiling $test_name.c using $port"
            exit 1
        fi
    done
done

echo "Jolly Good! All tests compiled successfully"