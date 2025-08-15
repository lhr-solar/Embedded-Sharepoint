# Installation Instructions
Development for Embedded-Sharepoint requires a linux-based environment. Running linux natively yields the best results, but there are workarounds through UTM virtual machine on Mac OS or Windows Subsystem for Linux (WSL) on Windows.  

Please ensure you've done the following before moving on:
1. Follow the instructions here to create your github SSH key
2. Install [Visual Studio Code](https://code.visualstudio.com/)

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
```
winget install usbipd
```

## Mac
To develop on a Mac we will use UTM, which is an apple application to emulate other operating systems. The following instructions have been tested on an M2 Macbook.
### UTM
#### Download
Download the UTM app [here](https://mac.getutm.app/). 
Download Ubuntu 24 [here](https://ubuntu.com/download/server). 
#### Configuring UTM
Next we will use the Ubuntu iso image we just downloaded to create a new Ubuntu VM

1. Open UTM and click on Create a New Virtual Machine.
2. Select Virtualize and then choose Linux.
3. Press browse and select the downloaded ARM-based ISO file.
4. Adjust hardware settings:
    * Memory: Default is 4000 MB (adjust based on Mac capacity).
    * Processors: Set according to your Mac's cores available (up to 8).
    * Storage: Default is 64 GB (adjust as necessary).
5. No shared directory necessary so press Continue here
6. Name the VM (e.g., Ubuntu 24.04 LTS).
7. Click Save to create the VM.

#### Configuring SSH
The Ubuntu VM we created is pretty slow and can't run graphical applications easily, so it'll be hard to develop code here. What we will do instead is SSH into the VM we created from regular MacOS so we can develop our linux code while also being in MacOS.  

Open the Ubuntu VM and run ``` hostname -I``` This will output your Ubuntu VM's IP address to SSH into.  

Open Visual Studio Code in MacOS. 
Install the folowing extensions from the extensions marketplace:  

1. In VSCode press ```command + shift + p```
2. Type Remote-SSH: Open SSH Configuration File
3. Select the one that's in /.ssh/config
4. This file contains all your possible SSH targets, we will add the Ubuntu VM as an SSH target. Type this into your ssh config and the IP address of your VM you got earlier.
```
Host [Name of SSH, can be anything]
HostName [IP Address]
User [Your user name]
``` 
For example mine could be: 
``` 
Host Ubuntu-UTM-VM
HostName 192.169.42.0
User laksh 
```
5. Press ```command + shift + p``` and type Remote-SSH: Connect to Host
6. Select the hostname you just added
7. Congrats! You are now SSH'd into your Ubuntu VM. 

Run the steps in [Nix](#nix) next

## Nix
[todo, sowey]
