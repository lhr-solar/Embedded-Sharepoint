#!/usr/bin/env python3
import os
import sys
from numba import njit, jit

PIPE_PATH = "/tmp/compile_command_pipe"

def send_to_daemon(command):
    try:
        with open(PIPE_PATH, 'w') as pipe:
            pipe.write(command + '\n')
    except Exception as e:
        print(f"Warning: Failed to send command to daemon: {e}")

def main():
    compile_command = " ".join(sys.argv[1:])
    send_to_daemon(compile_command)

if __name__ == "__main__":
    main()