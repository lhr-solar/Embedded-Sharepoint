# Embedded-Sharepoint
This repository contains files that can be shared among us, the embedded design teams. 

## install.sh

This script installs all the necessary toolchains for developing on our embedded systems.

## copyright_updater.py

This script updates all the copyright headers on all the files we have written. When integrating with other repositories, it might be necessary to exclude some directories from being accessed.

## CANGenerate.py

This script looks through the data in CANBUSIDs.xlsx and generates header and library files based on the data. This allows all the embedded teams to easily change the information without having to change the way their code works. It will be necessary for the path of this submodule to be added to the Makefile.