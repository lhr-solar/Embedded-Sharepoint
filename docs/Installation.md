# Installation Instructions
Development for Embedded-Sharepoint requires a linux-based environment. Running linux natively yields the best results, but there are workarounds through nix on Mac OS or Windows Subsystem for Linux (WSL) on Windows.  

Please ensure you've done the following before moving on:  
Install [Visual Studio Code](https://code.visualstudio.com/)

## Linux
Thanks for making our lives easy :)    

1. Follow the instructions here to create your github SSH key
2. Run the steps in [Nix](#nix) next

## Windows
### Windows Subsystem for Linux (WSL)
Windows Subsystem for Linux allows you to run a linux terminal on your windows machine. We specifically use WSL2 since it allows USB passthrough for flashing our microcontrollers.  
To install WSL follow this [guide](https://learn.microsoft.com/en-us/windows/wsl/install). Make sure to install WSL2 and not WSL1.  
It will take time to fully install it, but once installed you can open WSL and it will ask you to create a username and password.
### USBIPD
In order to see a USB device in WSL, we need to pass access of a USB device from windows to WSL. You can do this through USBIPD.  
In an administrator Powershell terminal run:
```sh
winget install usbipd
```
### Using WSL
From now on, you'll be writting all of your solar code in WSL. We primary use VSCode as our code IDE, but there are other IDEs/code editors you can use like Vim or Nano.
1. Open Visual Studio Code
2. Go into extensions menu on the left side of VSCode, 
1. Run the **Ubuntu** application, and now you'll be in your WSL/Linux environment.

Run the steps in [Nix](#nix) next

## Mac
Run the steps in [Nix](#nix)

## Nix
Nix is a package manger that lets you write out all the packages you want to install in a human readable format. This simplifies development since I can write out a list of dependencies needed to compile.  
If you have not already, clone Embedded Sharepoint into your unix/linux environment (WSL for windows and MacOS for mac). Any line with # is a comment so you don't need to run those lines
```sh
# Clone the embedded sharepoint repository from the internet to your local computer
git clone git@github.com:lhr-solar/Embedded-Sharepoint.git --recursive
# cd (change directory) to enter the Embedded Sharepoint's directory 
cd Embedded-Sharepoint
```
Enter the Embedded Sharepoint and run the following commands in the terminal: 
Make the nix_install script runnable
```sh
chmod +x ./nix_install.sh
```
Run the script to install the needed nix dependencies.
```sh
sudo ./nix_install.sh
```
Enter the nix shell and download dependencies for Embedded-Sharepoint
```sh
nix develop
```
In this nix shell all the dependencies needed for Embedded-Sharepoint should be installed.  Anytime you open a new terminal and want to run compile/flash code you need to run nix develop to open the shell with Sharepoint's dependencies.  

Test if installation is succesful by compiling a test 
```sh
cd test
make TEST=blinky
```