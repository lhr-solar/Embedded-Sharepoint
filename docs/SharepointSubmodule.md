# Adding Sharepoint To Your Project

### 1. Add Embedded-Sharepoint as a Submodule
A **Git submodule** is a way to include one Git repository inside another. 
Embedded-Sharepoint contains common files so it should be stored as a submodule inside your repository.
For more information about submodules: [git submodules documentaiton](https://github.blog/open-source/git/working-with-submodules/)

Whever you want to add the Embedded-Sharepoint, run: 
```sh
git submodule add https://github.com/lhr-solar/Embedded-Sharepoint.git
git submodule update --init --recursive
```

## 2. Writting a makefile for your project
A **Makefile** is a file used to outline how you want your code to compile. There is a [Makefile](https://github.com/lhr-solar/Embedded-Sharepoint/blob/main/Makefile) that compiles all needed files for the STM32 (our microcontroller). You need to write a Makefile to compile your project files and call the Embedded-Sharepoint Makefile. A good example of this a Makefile you'd make is the sharepoint's test [Makefile](https://github.com/lhr-solar/Embedded-Sharepoint/blob/main/test/Makefile). 

### How the Embedded Sharepoint Makefile works:
You need to specify certain variables in your Makefile that describe your project:

## Examples of projects that use Embedded Sharepoint
* [BPS-Leader](https://github.com/lhr-solar/BPS-Leader)
* [Active-Precharge Board](https://github.com/lhr-solar/Active-Precharge-Board#)