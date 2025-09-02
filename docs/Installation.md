# Installation Instructions
Development for Embedded-Sharepoint requires a Linux environment. Running Linux natively yields the best results, but there are workarounds through Nix on Mac OS or Windows Subsystem for Linux (WSL) on Windows.  

Please ensure you've done the following before moving on:  
- Install [Visual Studio Code](https://code.visualstudio.com/)
- Set up your GitHub account and [SSH key](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent).
- Been added to the lhr-solar organization as a member. Ask one of your leads to do so.
- Cloned [Embedded-Sharepoint](https://github.com/lhr-solar/Embedded-Sharepoint/) onto your computer. If you've never used git before, some great tutorials exist [here](https://learngitbranching.js.org/?locale=en_US) and [here](https://www.atlassian.com/git/tutorials/what-is-version-control).
    - You can also check out our internal documentation on [How To Git](https://cloud.wikis.utexas.edu/wiki/spaces/LHRSOLAR/pages/28285528/How+to+Git)

## Linux
Thanks for making our lives easy :)
Run the steps in [Nix](#nix) next

## Windows
### Windows Subsystem for Linux (WSL)
Windows Subsystem for Linux allows you to run a linux terminal on your windows machine. We specifically use WSL2 since it allows USB passthrough for flashing our microcontrollers.  
To install WSL follow this [guide](https://learn.microsoft.com/en-us/windows/wsl/install). Make sure to install WSL2 and not WSL1.  
It will take time to fully install it, but once installed you can open WSL and it will ask you to create a username and password.
### USBIPD
In order to see a USB device in WSL, we need to pass access of a USB device from windows to WSL. You can do this through USBIPD.  
In an administrator Powershell terminal run:
```
winget install usbipd
```

Run the steps in [Nix](#nix) next

## Mac
Run the steps in [Nix](#nix)

## Nix
Nix is a package manager that lets you write out all the packages you want to install in a human readable format. This simplifies development since I can write out a list of dependencies needed to compile.  
Open Embedded-Sharepoint in your Unix/Linux (WSL for windows and MacOS for mac) environment, and run the following commands in the terminal: 

Make the nix_install script runnable
```
chmod +x ./nix_install.sh
```

Run the script to install the needed nix dependencies.
```
sudo ./nix_install.sh
```

Enter the nix shell and download dependencies for Embedded-Sharepoint
```
nix develop
```

In this nix shell all the dependencies needed for Embedded-Sharepoint should be installed.  Anytime you open a new terminal and want to run compile/flash code you need to run nix develop to open the shell with Sharepoint's dependencies.  

Test if installation is succesful by compiling a test.
```
cd test
make TEST=blinky
```
