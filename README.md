# Embedded-Sharepoint
This repository contains files that can be shared among us, the embedded design teams. 

## Install Script

This script installs/removes necessary packages for embedded development. install.sh will prompt the user for installation or removal of all necessary packages.

Usage: ./install.sh [-Ii] [-Uu] [-Hh]"
    For each package, choose between (y/n/r/q) to:"
        y: yes, install"
        n: no, don't install"
        r: remove package"
        q: quit"
    [-Ii] install all available packages"
    [-Uu] remove all available packages"
    [-Hh] show this help page"

## copyright_updater.py

This script updates all the copyright headers on all the files we have written. When integrating with other repositories, it might be necessary to exclude some directories from being accessed.

## CANGenerate.py

This script looks through the data in CANBUSIDs.xlsx and generates header and library files based on the data. This allows all the embedded teams to easily change the information without having to change the way their code works. It will be necessary for the path of this submodule to be added to the Makefile.