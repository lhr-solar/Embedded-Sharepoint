# Embedded-Sharepoint
This repository contains files that can be shared among us, the embedded design teams. 

## Building

Call ```make test [testName]``` to compile a BSP specific test. ```[testName]``` is the file name of the test excluding the file extension.

Add the ```chip``` variable specify which MCU you are building for

Add the ```BSPConfig``` variable to specify whether or not you are using bsp_config to configure pins

Call ```make help``` for more information.
 

## Install Script

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

## copyright_updater.py

This script updates all the copyright headers on all the files we have written. When integrating with other repositories, it might be necessary to exclude some directories from being accessed.

## CANGenerate.py

This script looks through the data in CANBUSIDs.xlsx and generates header and library files based on the data. This allows all the embedded teams to easily change the information without having to change the way their code works. It will be necessary for the path of this submodule to be added to the Makefile.

## Supported chips
To add supported chips, please follow this [Confluence article](https://wikis.utexas.edu/display/LHRSOLAR/Autogenerating+STM+Code) 
on how to generate the relevant HAL and startup scripts  
Our current supported chips are:
- ```STM32F413```
- ```STM32L431```  
