# Welcome to the Embedded Sharepoint Documentation

Welcome to the Embedded Sharepoint! This documentation is designed to assist LHR - Solar teams in developing projects using STM32 devices.

## Key Features

- **Simplified Development:** Provides a unified interface for working with various STM32 devices.
- **Modular Design:** Easily integrate with different components and libraries.
- **Comprehensive Documentation:** Detailed guides and API references for every module.

<!-- ## Quick Links -->

<!-- TODO: Add confluence or add documentation -->
<!-- - [Getting Started](getting-started.md) - Learn how to set up and begin using the library.
- [API Reference](api-reference.md) - Explore detailed documentation of all available functions and modules.
- [Examples & Tutorials](examples.md) - Follow step-by-step guides to implement common use cases.
- [FAQ](faq.md) - Find answers to frequently asked questions. -->

## Scripts

### Install Script

Note: Install tools for development on Ubuntu 22.04. Newer versions may break when installing Python packages.

This script installs/removes necessary packages for embedded development. Running install.sh will prompt the user for installation or removal of all necessary packages.

Usage: 
    
    ./install.sh [-Ii] [-Uu] [-Hh]
    For each package, choose between (y/n/r/q) to:
        y: yes, install
        n: no, don't install
        r: remove package
        q: quit
    [-Ii] install all available packages
    [-Uu] remove all available packages
    [-Hh] show this help page
