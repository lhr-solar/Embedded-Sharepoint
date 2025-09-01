# Adding Sharepoint To Your Project

### 1. Add Embedded-Sharepoint as a Submodule
Embedded-Sharepoint contains common files so it should be stored as a submodule inside your repository.

For more information about submodules: [git submodules documentaiton](https://github.blog/open-source/git/working-with-submodules/)

Whever you want to add the Embedded-Sharepoint, run: 
```sh
git submodule add https://github.com/lhr-solar/Embedded-Sharepoint.git
git submodule update --init --recursive
```

## 2. Writting a makefile for your project


## Examples of projects that use Embedded Sharepoint
* [BPS-Leader](https://github.com/lhr-solar/BPS-Leader)
* [Active-Precharge Board](https://github.com/lhr-solar/Active-Precharge-Board#)