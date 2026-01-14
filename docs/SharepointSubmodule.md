# Adding Sharepoint To Your Project

## 1. Add Embedded-Sharepoint as a Submodule
A **Git submodule** is a way to include one Git repository inside another. 
Embedded-Sharepoint contains common files so it should be stored as a submodule inside your repository.
For more information about submodules: [git submodules documentaiton](https://github.blog/open-source/git/working-with-submodules/)

Whever you want to add the Embedded-Sharepoint, run: 
```sh
git submodule add https://github.com/lhr-solar/Embedded-Sharepoint.git
git submodule update --init --recursive
```

## 2. Writting a makefile for your project
A **Makefile** is a file used to outline how you want your code to compile. There is a [Makefile](https://github.com/lhr-solar/Embedded-Sharepoint/blob/main/Makefile) that compiles all needed files for the STM32 (our microcontroller) in the top level of the Embedded Sharepoint. You need to write a Makefile to compile your project files and call the Embedded-Sharepoint Makefile.   
There is a minimal template Makefile in the template/ folder in Embedded-Sharepoint. Lines with `# CHANGES THIS` define where you should make changes to your Makefile.


### Set Project Variables
* `PROJECT_TARGET`: The STM32 part number (e.g., `stm32f446ret`)
* `PROJECT_C_SOURCES`: List of your C source files
* `PROJECT_C_INCLUDES`: List of your include directories
* `PROJECT_BUILD_DIR`: Where to place build outputs
* `BEAR_ENABLE` to make VSCode not mad at you (the red error squiggles)


### Creating a test directive
It's good to be able to test independent portions of your code instead of your whole code base at once. For example, if I have a driver just for controlling lights I should have a test file that just runs some code for my lights driver instead of the whole codebase.

C expects a `main()` function as the starting point of your code, so the way we generally implement this is filtering out the main.c file where the `main()` function is implemented, and compiling your test file instead with it's own `main()` function.


### Define SystemClock_Config
The `SystemClock_Config` function configures the clock that your STM32 runs on (things like clock frequency, clock source, etc). If the clock is not setup properly then you risk unexepected behavior since a lot of low level embedded code runs using the system clock. We normally run the system clock at **80mhz**.

We define the SystemClock_Config function as "weak", which means that the function can be overriden by a different implementaiton of the function. We have a default implementaiton in the stm32xx_hal_init.c file, which by default initializes the internal oscillator of some specific Nucleo's clock, but since we operate with a variety of Nucleos and processors you should redefine the function for your specific usecase.

Generate a new SystemClock_Config in STM32CubeMX and add it to one of your files in your repository. This should serve as a redefinition of the function and will override the default behavior. Make sure you are generating the code with the correct microcontroller part number in the software, and you are using an external oscillator (if generating code for one of our SOM PCBs). Instructions on how to use CubeMX can be found [here](./CubeMX.md).

## Examples of projects that use Embedded Sharepoint
* [BPS-Leader](https://github.com/lhr-solar/BPS-Leader)
* [Active-Precharge Board](https://github.com/lhr-solar/Active-Precharge-Board#)
