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

Ensure that the link you're using to add this submodule is the HTTPs link and not the SSH link (needed to for automatic github workflows to trigger).

## 3. Writing a makefile for your project
A **Makefile** is a file used to outline how you want your code to compile. There is a [Makefile](https://github.com/lhr-solar/Embedded-Sharepoint/blob/main/Makefile) that compiles all needed files for the STM32 (our microcontroller) in the top level of the Embedded Sharepoint. You need to write another Makefile to compile your project files and call the Embedded-Sharepoint Makefile.   
There is a minimal template Makefile in the template/ folder in Embedded-Sharepoint. Lines with `#❗ CHANGE THIS ❗` define where you should make changes to your Makefile. Move the template Makefile to the same directory as your firmware, and rename it to just be called `Makefile`. 

For a more in depth tutorial, I suggest visiting this article on [Makefiles](https://makefiletutorial.com/)

### Basic Makefile syntax

This section explains common Makefile syntax used in the template Makefile.

#### Comments
Any line becomes a comment by adding `#` When adding comments, it's suggested to make them on a separate line from actual Makefile code, i.e avoid
```make
PROJECT_TARGET ?= stm32g473xx # this line contains the project target
``` 
and instead do
```make
# this line contaisn the project target
PROJECT_TARGET ?= stm32g473xx 
``` 
This is because Makefiles can sometimes compile the comments into the command and break directories. 

#### Conditional Assignments (`?=`)
The `?=` operator assigns a value **only if the variable is not already set**.  
This allows variables to be overridden from the command line.

```make
PROJECT_TARGET ?= stm32g473xx
```
This by default sets the `PROJECT_TARGET` to be `stm32g473xx`, but by running
```sh
make PROJECT_TARGET=stm32l432cbt
```
The `PROJECT_TARGET` will be overriden to `stm32l432cbt`

#### Variable Expansion 
Variables are referenced using `$(VAR)` or `${VAR}` (both are valid).
```make
$(MAKE) -C $(BUILD_MAKEFILE_DIR) all
@echo "Building ${PROJECT_TARGET}"
```

#### Wildcards
Makefiles commonly store multiple files or directories as space-separated lists.
```make
PROJECT_C_INCLUDES = core/Inc drivers/Inc
PROJECT_C_SOURCES  = core/Src/main.c drivers/Src/can.c
```
Instead of setting these manually, you can do a `wildcard` search and add all files that meet certain criteria.

```make
PROJECT_C_SOURCES = $(wildcard */Src/*.c)
```
This looks through all the directories, and adds all c files in any `Src` folder

### Set Project Variables
Below are several variables that the Embedded-Sharepoint Makefile uses to compile your code, these variables are used in the template Makefile, and you can update them.

* `PROJECT_TARGET`: The STM32 part number (e.g., `stm32f446ret`)
* `PROJECT_C_SOURCES`: List of your C source files
* `PROJECT_C_INCLUDES`: List of your include directories
* `PROJECT_BUILD_DIR`: Where to place build outputs
* `BEAR_ENABLE` to make VSCode not mad at you (the red error squiggles). Bear is by default enabled, but you can set it to 0 to turn it off

### Creating a test folder
It's good to be able to test independent portions of your code instead of your whole code base at once. For example, if I have a driver just for controlling lights I should have a test file that just runs some code for my lights driver instead of the whole codebase which could be running a lot of things other than lights.

C expects a `main()` function as the starting point of your code, so the way we generally implement this is filtering out the main.c file where the `main()` function is implemented, and compiling your test file instead with it's own `main()` function.  

Create a `tests/` folder to store your tests, then via your Makefile you can compile those tests into your project, by default they will not be compiled. It's also good practice to add a test prefix or suffix to the name of the test file, e.g. `test_can.c` to show that it's a test file, and to avoid issues on case-insensitive filesystems like on macOS.

### Copy the .gitignore
A lot of junk files are generated during compilation, many of those files should not be pushed so we tell git to ignore these files. A `.gitignore` file tells which files or directories to ignore, so they are not pushed to the remote repository. There is a template .gitignore file in the template/ directory, which you should place in your firmware/ directory. You are also free to add what's in the template to your already existing .gitignore file. 

The following are files and directories you should not be pushing:

* `.DS_Store` these are mac dump files that are generated in every directory
* `build/` this is where compiled c files are placed by default, if you change the directory in the Makefile this needs to be updated
* `.venv/` this is where python environment files are stored, and are usually user and OS dependent


## 4. Add necessary functions
### Create a main.c
Your code begins from the `main()` function, and if there is no `main()` function defined the code will not compile. The `main()` function does not necessarily have to be located in a file named `main.c` but it is good practice. If you want to use any stm32 related header files, please include the `stm32xx_hal.h` header file.

### Define SystemClock_Config
The `SystemClock_Config` function configures the clock that your STM32 runs on (things like clock frequency, clock source, etc). If the clock is not setup properly then you risk unexepected behavior since a lot of low level embedded code runs using the system clock. We normally run the system clock at **80mhz**.

We define the SystemClock_Config function as "weak", which means that the function can be overridden by a different implementation of the function. We have a default implementation in the stm32xx_hal_init.c file, which by default initializes the internal oscillator of some specific Nucleo's clock, but since we operate with a variety of Nucleos and processors you should redefine the function for your specific usecase.

Generate a new SystemClock_Config in STM32CubeMX and add it to one of your files in your repository (suggested is your main.c). This should serve as a redefinition of the function and will override the default behavior. Make sure you are generating the code with the correct microcontroller part number in the software, and you are using an external oscillator (if generating code for one of our SOM PCBs). Instructions on how to use CubeMX and generate a SystemClock_Config can be found [here](./CubeMX.md).


## 5. Compiling your repository
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

## 6. Create a README
A README.md file is very important for onboarding instructions, and to outline workflows. A README should explain how to go from cloning a repository to how to contribute code. The more descriptive the better!


## Examples of projects that use Embedded Sharepoint
* [BPS-Leader](https://github.com/lhr-solar/BPS-Leader)
* [VCU](https://github.com/lhr-solar/PS-VehicleControlUnit)
