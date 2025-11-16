# Installation Instructions
Development for Embedded-Sharepoint requires a Linux environment. Running Linux natively yields the best results, but there are workarounds through Nix on Mac OS or Windows Subsystem for Linux (WSL) on Windows.  

## Prerequisites
Please ensure you've done the following before moving on:  

- Install [Visual Studio Code](https://code.visualstudio.com/)
- Set up your GitHub account and [SSH key](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent).
    - You need to both generate an ssh key and [add](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account) it to your github account
- Been added to the lhr-solar organization as a member. Ask one of your leads to do so.
- Install [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)

Any line that starts with `#` is a comment and you don't need to run it. 
```sh
# Clone the embedded sharepoint repository from the internet to your local computer
git clone git@github.com:lhr-solar/Embedded-Sharepoint.git --recursive
# cd (change directory) to enter the Embedded Sharepoint's directory 
cd Embedded-Sharepoint
```

## Linux
Run the steps in [Nix](#nix) next

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
From now on, you'll be writing all of your solar code in WSL. We primary use VSCode as our code IDE, but there are other IDEs/code editors you can use like Vim or Nano.

1. Open Visual Studio Code
2. Go into extensions menu on the left side of VSCode, select the WSL extension from Microsoft
3. Press **CTRL+SHIFT+P** and press connect to WSL
4. Now your VSCode is running WSL and you can run it like a Linux environment. For more info see the [WSL guide](https://learn.microsoft.com/en-us/windows/wsl/tutorials/wsl-vscode)
5. Do the instructions for generating the github ssh key again while you're in WSL 

Run the **Ubuntu** application, and now you'll be in your WSL/Linux environment.

Run the steps in [Nix](#nix) next

## Mac
Run the steps in [Nix](#nix)

## Nix
Nix is a package manger that lets you write out all the packages you want to install in a human readable format. This simplifies development since I can write out a list of dependencies needed to compile.  

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
Now that you've successfully compiled a test, we want to flash our code to the microcontroller. The term "flashing" in an embedded system refers to putting your code onto the microcontroller. If you're using WSL there are some extra setup steps you need to do to connect to the microcontroller, that can be found [here](./FlashAndTheBug.md) in the `Attaching USB devices in WSL` section.  

After that run ``lsusb`` and make sure you see an ST-Link Debug device is seen by your terminal. Then run the following command to flash to your microcontroller
```sh
make flash
```

# Common Errors
## Could not open USB device
if you're getting:
```libusb couldn't open USB device /dev/bus/usb/001/003, errno=13```
This means your user doesn't have permissions to use a USB port.  To fix this run
```sh
sudo chmod -R 777 /dev/bus/usb/
```

## Can't flash after changing PROJECT_TARGET
If you build your code for one PROJECT_TARGET and then switch to another, you’ll usually need to run `make clean` before flashing to remove all old build files. Otherwise, leftover files from the previous build may remain in your build folder, causing `make flash` to get confused about which `.bin` file to use.