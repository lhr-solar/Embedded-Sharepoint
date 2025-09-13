# Flashing and Debugging on an MCU

This page will walk you through how you can flash your C code on an MCU using Embedded Sharepoint!

... and also debug it 🫠

> ℹ️ **Prerequisite :** 
> Make sure you've [added Embedded Sharepoint](SharepointSubmodule.md).

## Attaching USB devices

Assuming you're using WSL, when you plug in your Nucleo it won't automatically get recognized.

You can verify this by running `lsusb`.

To attach a USB device on WSL you need to install [`usbipd-win`](https://github.com/dorssel/usbipd-win/releases).

1. Go to the latest release.

2. Select the `.msi` file, and download it. (You may get a warning asking you to confirm that you trust this download).

3. Run the downloaded `usbipd-win_x.msi` installer file.

Once we have `usbipd` we can bind and attach USB devices.

1. Open Powershell and **"Run as Administrator"**.
2. Run `usbipd list` to get a list of all USB buses.
3. Locate the device called "**ST-Link Debug**" and note the **"BUSID"**.
4. Run `usbipd bind --busid <BUSID>`.
5. Run `usbipd attach --wsl --busid <BUSID>` (**must have an instance of WSL running**).

And you're good!

Confirm all is well with `lsusb`.

## Hardware Interface

**Serial Wire Debug** (SWD) is a two-wire protocol that is an alternative to JTAG. JTAG is the most common interface for debugging/accessing MCU registers, but it requires 4 pins to communicate while SWD only requires 2, so many ARM microcontrollers will use SWD to ease pin requirements.

If you take a look at your STM32 Nucleo you should notice **two** sections on the board:

![Nucleo Section](assets/NucleoDivide.png)

<div style="display: flex; justify-content: space-between;">
    <span><span style="color: #e06666; font-weight: bold;">Red</span> ST-Link for Debugging</span>
    <span><span style="color: #4a90e2; font-weight: bold;">Blue</span> STM32 MCU and Peripherals</span>
</div>

### ST-Link Connector

Your board may have the jumpers ON or OFF on the ST-Link Connector (**CN2**).

**ON** - you are programming the **INTERNAL** STM32 MCU (Nucleo)

**OFF** - you are programming the **EXTERNAL** STM32 MCU (Solar Board)

Decide whether you need jumpers based on which MCU you are trying to program.


> ℹ️ If you're unsure, run `st-info --probe` to see the MCU you're flashing to.


### SWD Connector

When programming an **external** MCU (not on the Nucleo) the 6-pin SWD connector (CN4) on the ST-Link will be used. 

Here's the pinout for reference

| Pin | CN4  | Designation                |
|-----|------|---------------------------|
| 1   | VDD_TARGET | VDD from application     |
| 2   | SWCLK      | SWD clock               |
| 3   | GND        | Ground                  |
| 4   | SWDIO      | SWD data I/O            |
| 5   | NRST       | RESET of target STM32   |
| 6   | SWO        | Reserved                |

On our solar boards we have a SWD interface that looks like this

<div style="display: flex; align-items: center; gap: 20px;">
    <img src="../assets/SWDSolarBoard.png" alt="Solar SWD 1" style="height: 200px; width: 200px; object-fit: contain;">
    <img src="../assets/AnothaOne.png" alt="Solar SWD 2" style="height: 200px; width: 200px; transform: rotate(90deg); object-fit: contain;">
</div>

After connecting the corresponding pins you'll be able to program the MCU on your board.

## Software Tooling

### Overview

The OpenOCD and stlink packages are some software tools we use to flash and debug code on our board. They are two different options for doing the same thing (flashing and debugging via JTAG/SWD).

- [**stlink**](https://github.com/stlink-org/stlink) is a software package developed by STMicroelectronics to interface with an ST-LINK device for programming and debugging purposes.

- [**OpenOCD**](https://pfeerick.github.io/InfiniTime/doc/openOCD.html) runs a GDB server, which allows us to debug remote targets via GDB. It also lets us write to flash with some extra configuration.

We currently use the OpenOCD GDB server for debugging while using `st-flash`, a tool in the stlink package for flashing code.

### Implementation

Embedded Sharepoint contains Makefiles to simplify the process of flashing code on an MCU. These Makefiles directly call the `st-flash` command.

On an STM32 MCU, flash memory starts at `0x8000000`.


![0x8000](assets/0x8000.png)


After the Makefile compiles your source code into a binary (`.bin`), it writes to flash using

<pre> st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000 </pre>

**NOTE**: We're not flashing the `.elf` file because it contains unnecessary debug symbols that we don't need to simply flash.


### Make Executables

You can use the Makefile from the `test/` directory to flash files within the `tests/` directory.

1. Navigate to the `test/` directory.

2. If you've set the environment variables mentioned in "[Adding Embedded Sharepoint](SharepointSubmodule.md)" then you can run `make TEST=<testfile>` where `tests/testfile.c` is your test.

3. After your code has been compiled and linked you should see information regarding the `.elf` file created.

![Will Ferrell](assets/ELF.png)

If you don't see a similar message nor a `.elf` file in the `build/` directory, then look at your console output for a specific error traceback.

### Flash

If you ran the previous section without error then you should have a `.bin` and a `.elf` in your `build/` corresponding to your target MCU.

To flash

1. Navigate to `test/`
2. Run `make flash`

... and you've flashed to the MCU!

### Debug

To debug we'll use **OpenOCD**. 

1. Navigate to the root directory of Embedded Sharepoint.
2. Run `openocd -f openocd-stm32f4x.cfg` if you're programming an F4 MCU or the corresponding `..l4x.cfg` if you're programming an L4.
3. You should see a message that a GDB server was started on port `3333`.

Open a second terminal session to use GDB

1. Run `gdb-multiarch build/(TARGET).elf`. If you aren't in root the filepath will look slightly different.
2. Verify that GDB is using the debug symbols from the `.elf` file.
3. Run `tar extended-remote :3333` to connect to the OpenOCD GDB server.

Step through your code in GDB to analyze execution!
