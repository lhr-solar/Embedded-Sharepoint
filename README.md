# Embedded-Sharepoint
This repository contains files that can be shared among us, the embedded design teams. 

## Install/Uninstall Script

These scripts install/remove necessary toolchains for embedded development. install.sh will prompt the user for installation of all necessary packages. uninstall.sh will only uninstall the ARM toolchain and the Renode Simulator, as they require special behavior for uninstallation (the rest can be removed with `apt-get`)

Usage: 
- `./install.sh [-Yy] [-Hh]`     
- `./uninstall.sh [-Yy] [-Hh]`

## copyright_updater.py

This script updates all the copyright headers on all the files we have written. When integrating with other repositories, it might be necessary to exclude some directories from being accessed.

## CANGenerate.py

This script looks through the data in CANBUSIDs.xlsx and generates header and library files based on the data. This allows all the embedded teams to easily change the information without having to change the way their code works. It will be necessary for the path of this submodule to be added to the Makefile.