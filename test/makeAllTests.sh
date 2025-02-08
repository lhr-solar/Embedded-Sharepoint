#!/bin/bash

port_list=("stm32f401re" "stm32f413rht" "stm32f429zit" "stm32f446ret" "stm32l431cbt" "stm32l476rgt")

# Boards that should run UART tests since they support UART4 & UART5
uart_enabled_ports=("stm32f446ret" "stm32l476rgt")

for port in "${port_list[@]}"; do
    make clean
    
    # Check if this is a UART-enabled board
    is_uart_enabled=false
    for uart_port in "${uart_enabled_ports[@]}"; do
        if [[ "$port" == "$uart_port" ]]; then
            is_uart_enabled=true
            break
        fi
    done
    
    for tests_dir in tests/*; do
        test_name="${tests_dir#tests/}"
        test_name="${test_name%.c}"
        
        # Skip CAN tests for f401
        if [[ "$port" =~ ^stm32f401.*[ec]$ ]] && ([[ "$test_name" == "can" ]] || [[ "$test_name" == "can_mt" ]]); then
            echo "Skipping the CAN test for $port because it does not support CAN"
            continue
        fi
        
        # Skip UART tests for non-UART boards
        if [[ "$test_name" == "uart" || "$test_name" == "uart_mt" ]]; then
            if ! $is_uart_enabled; then
                echo "Skipping the UART test for $port because it does not support UART4 & UART5"
                continue
            fi
        fi
        
        echo "Compiling the test - $test_name for $port"
        
        if ! make TEST="$test_name" PROJECT_TARGET="$port" -j; then
            echo "Errors occurred while compiling $test_name.c using $port"
            exit 1
        fi
    done
done

echo "Jolly Good! All tests compiled successfully"