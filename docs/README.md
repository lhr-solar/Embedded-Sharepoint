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

## WSL

### USB PassThrough

#### Install USBIPD (only need to do this the first time)

In Powershell administrator:
    
    winget install usbipd

#### Pass USB Devices

In PowerShell administrator: 

    usbipd list
    usbipd bind --busid <busid>
    usbipd attach --wsl --busid <busid>

## Setup

Note: Docker installs tools for development on Ubuntu 22.04.

Note: DO NOT install packages in the docker session. Changes will be lost. Update the Dockerfile instead!

Note: If USB devices are detected via lsusb but fail to open, try restarting the container with the device plugged in.

### VS Code

Install Dev Containers extension

Open repository folder in VS Code

F1 → "Dev Containers: Open Folder in Container..."

Ensure docker is running
    
    sudo systemctl start docker

### CLI Script

This script sets up a docker container for embedded development.

Usage:

    ./start.sh

Remove Docker Image:

    docker rmi embedded-sharepoint-dev
