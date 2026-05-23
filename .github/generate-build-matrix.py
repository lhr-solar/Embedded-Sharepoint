#!/usr/bin/env python3
"""
Generate GitHub Actions build matrix from discovered STM32 ports.
Outputs JSON array of port objects suitable for GitHub Actions strategy.matrix.include
"""

import json
import sys
from pathlib import Path

def find_ports(stm_dir: Path) -> list[str]:
    """
    Discover all available STM32 ports by searching for linker script files.
    
    Args:
        stm_dir: Path to the stm directory containing port definitions
        
    Returns:
        Sorted list of port names (e.g., ['stm32f401re', 'stm32f413rht', ...])
    """
    if not stm_dir.is_dir():
        print(f"Error: STM directory not found at {stm_dir}", file=sys.stderr)
        sys.exit(1)

    ports = []
    for ld in stm_dir.rglob("*.ld"):
        name = ld.stem
        # Remove common suffixes and clean up the name
        name = name.replace("_FLASH", "")
        name = name.replace("x", "")
        name = name.replace("_", "")
        ports.append(name.lower())

    if not ports:
        print(f"Error: No linker scripts found in {stm_dir}", file=sys.stderr)
        sys.exit(1)

    return sorted(set(ports))  # Remove duplicates and sort

def generate_matrix(ports: list[str]) -> dict:
    """
    Generate GitHub Actions matrix format.
    
    Args:
        ports: List of port names
        
    Returns:
        Dictionary with 'include' key containing list of port objects
    """
    return {
        "include": [{"port": port} for port in ports]
    }

def main():
    # Find the repository root (script is in .github/)
    script_dir = Path(__file__).resolve().parent
    repo_root = script_dir.parent
    stm_dir = repo_root / "stm"
    
    ports = find_ports(stm_dir)
    matrix = generate_matrix(ports)
    
    # Output as JSON
    print(json.dumps(matrix))

if __name__ == "__main__":
    main()
