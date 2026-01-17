# Adding Sharepoint To Your Project

## 1. Suggested directories
It is good practice to organize files into folders based on their abstraction from the STM32 hardware. Embedded-Sharepoint contains a folder for the STM32 HAL (Hardware Abstraction Library), which holds low-level calls to the STM32. Above this are drivers that interact with the HAL, and then applications that use the drivers. This application layer is where your RTOS tasks typically run. You should also have a `tests/` directory. If your repository contains more than just firmware, it's recommended to create a `firmware/` directory for all your code, including Embedded-Sharepoint.

The suggested (minimal) structure is as follows:


```text
Firmware/
├── Embedded-Sharepoint/  #  Don't create this folder manually, it'll be created in the next step
├── Makefile              # The makefile that you write
├── core/                 # Stores tasks and main application logic
├── drivers/              # Drivers
├── middleware/           # External libraries stored in the repository
└── tests/                # Unit and integration tests
```

These directories should all have Src/ and Inc/ folders to separate the c header and source files.

## 2. Add Embedded-Sharepoint as a Submodule
A **Git submodule** is a way to include one Git repository inside another. 
Embedded-Sharepoint contains common files so it should be stored as a submodule inside your repository.
For more information about submodules: [git submodules documentation](https://github.blog/open-source/git/working-with-submodules/).

Wherever you want to add Embedded-Sharepoint, run: 
```sh
git submodule add https://github.com/lhr-solar/Embedded-Sharepoint.git
git submodule update --init --recursive
```

## 3. Writing a makefile for your project
A **Makefile** is a file used to outline how you want your code to compile. There is a [Makefile](https://github.com/lhr-solar/Embedded-Sharepoint/blob/main/Makefile) that compiles all needed files for the STM32 (our microcontroller) in the top level of the Embedded Sharepoint. You need to write another Makefile to compile your project files and call the Embedded-Sharepoint Makefile.   
There is a minimal template Makefile in the template/ folder in Embedded-Sharepoint. Lines with `#❗ CHANGE THIS ❗` define where you should make changes to your Makefile. Move the template Makefile to the same directory as your firmware, and rename it to just be called `Makefile`


### Set Project Variables
Below are several variables that the Embedded-Sharepoint Makefile uses to compile your code, these variables are used in the template Makefile, and you can update them.

* `PROJECT_TARGET`: The STM32 part number (e.g., `stm32f446ret`)
* `PROJECT_C_SOURCES`: List of your C source files
* `PROJECT_C_INCLUDES`: List of your include directories
* `PROJECT_BUILD_DIR`: Where to place build outputs
* `BEAR_ENABLE` to make VSCode not mad at you (the red error squiggles). Bear is by default enabled, but you can set it to 0 to turn it off

### Create a main.c
Your code begins from the `main()` function, and if there is no `main()` function defined the code will not compile. The `main()` function does not necessarily have to be located in a file named `main.c` but it is good practice. If you want to use any stm32 related header files, please include the `stm32xx_hal.h` header file.


### Creating a test folder
It's good to be able to test independent portions of your code instead of your whole code base at once. For example, if I have a driver just for controlling lights I should have a test file that just runs some code for my lights driver instead of the whole codebase which could be running a lot of things other than lights.

C expects a `main()` function as the starting point of your code, so the way we generally implement this is filtering out the main.c file where the `main()` function is implemented, and compiling your test file instead with it's own `main()` function.  

Create a `tests/` folder to store your tests, then via your Makefile you can compile those tests into your project, by default they will not be compiled. It's also good practice to add a test prefix or suffix to the name of the test file, e.g. `test_can.c` to show that it's a test file, and to avoid issues on case-insensitive filesystems like on macOS.

### Define SystemClock_Config
The `SystemClock_Config` function configures the clock that your STM32 runs on (things like clock frequency, clock source, etc). If the clock is not setup properly then you risk unexepected behavior since a lot of low level embedded code runs using the system clock. We normally run the system clock at **80mhz**.

We define the SystemClock_Config function as "weak", which means that the function can be overriden by a different implementaiton of the function. We have a default implementaiton in the stm32xx_hal_init.c file, which by default initializes the internal oscillator of some specific Nucleo's clock, but since we operate with a variety of Nucleos and processors you should redefine the function for your specific usecase.

Generate a new SystemClock_Config in STM32CubeMX and add it to one of your files in your repository. This should serve as a redefinition of the function and will override the default behavior. Make sure you are generating the code with the correct microcontroller part number in the software, and you are using an external oscillator (if generating code for one of our SOM PCBs). Instructions on how to use CubeMX and generate a SystemClock_Config can be found [here](./CubeMX.md).


## 4. Compiling your repository
Code compilation must be done in a nix shell, in Embedded-Sharepoint you can normally just run
```sh
nix develop
```
since the `flake.nix` file is stored in that directory, but in your repository the flake.nix file is stored in a different directory. To enter the Nix shell when the `flake.nix` file is stored somewhere else, simply run:
``` sh
nix develop [directory to where flake.nix is]
```
For example:
```sh
nix develop ./Embedded-Sharepoint
```
Once you're in the nix shell, run 
```sh 
make
```
in the same directory as your new Makefile.

If you do not want to type that out everytime, you can create a bash script that runs the nix develop command and hardcodes the directory where the nix file is. See this example in [VCU](https://github.com/lhr-solar/PS-VehicleControlUnit/blob/main/Firmware/run_nix.sh)

## 5. Create a README
A README.md file is very important for onboarding instructions, and to outline workflows. A README should explain how to go from cloning a repository to how to contribute code. The more descriptive the better!


## Examples of projects that use Embedded Sharepoint
* [BPS-Leader](https://github.com/lhr-solar/BPS-Leader)
* [VCU](https://github.com/lhr-solar/PS-VehicleControlUnit)
