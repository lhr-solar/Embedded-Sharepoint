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
A good example of this a Makefile you'd make is the sharepoint's test [Makefile](https://github.com/lhr-solar/Embedded-Sharepoint/blob/main/test/Makefile).  


### Set Project Variables
* `PROJECT_TARGET`: The STM32 part number (e.g., `stm32f446ret`)
* `PROJECT_C_SOURCES`: List of your C source files
* `PROJECT_C_INCLUDES`: List of your include directories
* `PROJECT_BUILD_DIR`: Where to place build outputs
* `BEAR_ENABLE` to make VSCode not mad at you (the red error squiggles)

### Creating a test directive
It's good to be able to test independent portions of your code instead of your whole code base at once. For example, if I have a driver just for controlling lights I should have a test file that just runs some code for my lights driver instead of the whole codebase. 


## Examples of projects that use Embedded Sharepoint
* [BPS-Leader](https://github.com/lhr-solar/BPS-Leader)
* [Active-Precharge Board](https://github.com/lhr-solar/Active-Precharge-Board#)