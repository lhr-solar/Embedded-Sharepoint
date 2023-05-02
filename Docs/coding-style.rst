*************
Coding Style
*************

.. note:: 

    | CamelCase - FirstWordSecondWord
    | camelCase - firstWordSecondWord

Macros and Constants
====================

Macros and constants should be written in snakecase with all caps. If a macro utilizes arithmetic, it should be covered in parentheses to prevent the compiler
from assuming priority. Ex

.. code-block:: c

    #define ALL_CAPS_WITH_UNDERSCORE
    #define MATH (4 + x)
    const uint8_t FOUR_KG = 4;
    #ifdef TASKS_H //No leading underscores

Enums
=====

Enum names should be in CamelCase with ``_e`` postfixed and all elements in enums should be in all caps. They should be located in the 
header file unless they are only used within the library. Ex:

.. code-block:: c

    typedef enum {
        ELEMENT_ONE;
        ELEMENT_TWO;
        ELEMENT_THREE;
        } ExampleEnum_e;

Structs
=======

All struct typedef names should have a descriptive name written in CamelCase and an appended ``_t``. The name of the struct should be
in CamelCase and its members should be written in camelCase to make it easier to understand gcc error messages. 
They should be located in the header file. Ex:

.. code-block:: c

    typedef struct StructName {
        uint8_t element;
        uint16_t elementTwo;
    } StructName_t;

Unions
======

Unions should only be used when absolutely necessary (normally for communication protocols). They should be located in the
header file, written in CamelCase, and appended with an ``_t``. Members of the union should be in camelCase.

.. code-block:: c

    typedef union {
	    uint8_t b;
	    uint16_t h;
	    uint32_t w;
	    float f;
    } CANData_t;

Necessary Case: Since we could be sending either 8 bits, or 64 bits, it is necessary to have a union that packs all the different
data types into one memory address and zero pads the rest of the unused spaces. This is better than having 4 different variables 
and using the proper variable at the proper time.

.. code-block:: c

    typedef union {
    	uint8_t b;
    	uint16_t h;
    	uint32_t w;
    	uint64_t d;
    } CANData_t;

Usage:

.. code-block:: c

    CANData_t Data;
    uint8_t byte = 7;
    Data.b = byte;

Unnecessary Case: This union was created so the values of each fault could be accessed either through the bitmap or through the struct
within the union. In one case, the bitmap could be read by checking each bit in the byte sized variable, or you could check each
individual Fault value to see if it is set to 1. This is unncecessary and makes the code more difficult to read and use. 

.. code-block:: c

    typedef union{
        uint8_t bitmap;
        struct{
            State Fault_OS : 1;         // for OS faults
            State Fault_UNREACH : 1;    // for unreachable conditions
            State Fault_TRITIUM : 1;      // for errors sent from the tritium
            State Fault_READBPS : 1;    // for unsuccessfully reading from BPS CAN
            State Fault_DISPLAY : 1;    // for display faults
        };
    } fault_bitmap_t;

Usage:

.. code-block:: c

    fault_bitmap_t FaultBitmap;
    FaultBitmap.bitmap |= 0x02;
    FaultBitmap.Fault_UNREACH = 1;

Libraries
=========

All libraries are separated into source files and header files. The source file consists of the implementation and the header file
holds the interface. 

=============
Library Names
=============

The name of the library depends on the layer that the library is part of. 

Board Support Package (BSP)
---------------------------

BSP library file names should describe the feature of the microcontroller that is being used. They should all be in 
snakecase with the first section being ``BSP_``. If the BSP library is for specific functionality in the system, the name should
describe the functionality. Ex:

.. code-block:: c

    BSP_ADC.c //Microcontroller Feature
    BSP_ADC.h
    BSP_PWM.c //Interface with system
    BSP_PWM.h

Drivers
-------

Driver library file names should be the name of the component the library is for with the appropriate suffix. Ex:

.. code-block:: c

    Partname.c
    Partname.h
    LTC6811.c
    LTC6811.h

Application
-----------

Application-level library names should describe the functionality of the application. If the application library is for a task,
it should be prefixed with the ``Task_`` prefix. the task's name should be in CamelCase. Ex:

.. code-block:: c 

    EEPROM.c //interface with EEPROM
    Temperature.c

Tasks
-----

Task level library names should be prefixed by ``Task_`` and then the name of the task. The name of the task should be in CamelCase and
accurately describe what the task is doing. This is the highest level and care should be taken to make sure that most logic is done in the
lower levels.

.. code-block:: c 

    Task_BatteryBalance.c //interface with EEPROM
    Task_VoltTempMonitor.c

Tests
-----

Files in the test library are used to test libraries of all layers. These should all be source files that are prefixed with ``Test_``. The 
following name should be the library that it is testing. If there are multiple things to be tested for each library, additional tests can 
be created for them with the specific test appended in snakecase. For example,

.. code-block:: c

    Test_EEPROM_Read.c //For Driver EEPROM
    Test_EEPROM_Write.c
    Test_Temperature.c //For Application Temperature
    Test_BatteryBalance.c //For Application Battery Balancing
    Test_Task_BatteryBalance.c //For Task Battery Balancing

============
Source Files
============

Source files should be formatted in the following method.

    1. Trademark/Description of file
    2. Includes
    3. Defines
    4. Globals
    5. Static Functions
    6. Public Functions
    7. Newline

The trademark/description should adhere to the following format:

.. code-block:: c

    /* Copyright (c) <YEAR> UT Longhorn Racing Solar */
    /** DRIVER.c
    * DESCRIPTION
    */

Descriptions of Test Files should adhere to the following format:

.. code-block:: c

    /* Copyright (c) <YEAR> UT Longhorn Racing Solar */
    /** DRIVER.c
    * Steps to performing tests
    * What results to expect and how to determine if library functions
    */

Included files should only be what is necessary for the source code to run. Defines should follow Macro coding style.
If there is debugging functionality in the source file (that is not essential for the system to run), ``#ifdef DEBUG`` and ``endif``
should surround all code necessary for that functionality. This consists of included files, ``printf`` statements, and other code
inside functions that run at regular runtime. Debugging functions do not have to be surrounded. Static functions must be declared
before all public functions. It is the prerogative of the programmer to have the static function definition at the start of the 
source file or the end. More information can be found in the :ref:`Private Functions`.

============
Header Files
============

Header files should be formatted in the following method.

    1. Trademark/Description of file
    2. Defines
    3. Includes
    4. Typedefs
    5. Function Declarations
    6. Endifs
    7. Newline

All header files should be surrounded with header guards. For example::
    
    #ifndef HEADER_H
    #define HEADER_H
    
    // your code here
    
    #endif

Header files should only have the number of includes necessary for the header file to work. For example, if a function returns an 
``int32_t``, then the header file should ``#include <stdint.h>``. However, if the source file calls ``memcpy`` in a function, it is not
necessary for the header file to ``#include <stdlib.h>``.

Functions
=========

============
Descriptions
============

All function declarations and definitions should have a comment paragraph description that follows the specified format provided by 
`Doxygen <https://www.doxygen.nl/>`__. ::

    /**
     * NOTE: Include anything important someone else will need to know
     * @brief Give Description of Function
     * @param variable Describe input parameters
     * @return What function returns
     */

================
Public Functions
================

The first section should be separated by snakecase and have the library the function is a part of. The second section should
also be separated by snakecase and describe the function's purpose. If there are multiple words in
the second section they should be written in CamelCase. Ex

.. code-block:: c

    Library_Function();
    Contactor_GetState();

Descriptions for public functions should be included above the function definition in the source file and above the declaration 
in the header file.

=================
Private Functions
=================

Code duplication should be avoided by using private functions instead. Their format is the same as public functions but their description 
should be included in the source file, not the header file. All private functions should be ``static`` and can be implemented in one of two ways.

.. code-block:: c

    /**
     * NOTE: Include anything important someone else will need to know
     * @brief Give Description of Function
     * @param variable Describe input parameters
     * @return What function returns
     */
    static void ADC_InitDMA(void);

Implementation One: Preferred for longer function blocks

.. code-block:: c 

    //Start of Source File
    static void Static_FunctionOne(inone, intwo, inthree);

    //Source code with calls to function

    static void Static_FunctionOne(inone, intwo, inthree){
        //code for static function
    }

Implementation Two: Preferred for shorter function blocks

.. code-block:: c 

    //Start of Source File
    static void Static_FunctionOne(inone, intwo, inthree){
        //code for static function
    }

    //Source code with calls to function

These implementations should not be mixed within the same source file (i.e you cannot have some static functions defined at the start of the file
and some at the end).

Variables
=========

================
Local Variables
================

If variables have the same type, declare them on the same line if possible. 
Meaningful names for variables that aren't used for iterations in a loop should be written in camelCase.
Unnecessary variables should be avoided. i.e

.. code-block:: c
    :linenos:

    //NO BUENO
    int x = function();
    if (x) return;
    //YES BUENO
    if (function()) {
        return;
    }

If the functionality of the code is affected by this (calling the same function multiple times), then
it is fine to have a variable for the return value.

================
Global Variables
================

Variable names should be meaningful and written in CamelCase. Efforts must be made to prevent global variables from
being used in multiple libraries. All global variables must be static variables unless the logic requires global variables
being shared. All global variables should have comments describing their purpose (what they hold). Ex.

.. code-block:: c
    :linenos:

    static OS_MUTEX VoltageMutex;
    static uint16_t Voltages[NUM_BATTERY_MODULES]; //Voltage values gathered
    static int32_t Temperatures[NUM_MINIONS][MAX_TEMP_SENSORS_PER_MINION_BOARD];

Miscellaneous
=============

============
Punctuation
============

**Curly Braces:** Opening braces should be on the same line as the condition or function that contains them. Curly braces should always 
be used in conditionals even if it is one line.

**Parentheses:** Should be used for clarifying the order of precedence.

**Indentation:** Indentation should be used in all conditionals, iteratives, and functions. Indentation should be 4 spaces (NOT TABS).

**Line Endings:** All End Of Line sequences should be LF(line feed). This prevents files from looking like they are changed when
nothing was changed at all.

**End of File:** All source files (.c and .h files) should have newlines at the end of the file.

**Whitespace:** All whitespace should remain consistent in function blocks. Unacceptable:

.. code-block:: c
    :linenos:

    void functionName(int var) {
        int x=3;
        for(i= 0; i < 5; i++) {
            x +=1;
        }
    }

Acceptable:

.. code-block:: c
    :linenos:

    void functionName(int var){ //no space between curly brace and parentheses
        int x = 3; //spaces between everything except semicolon
        for(i = 0; i < 5; i++){ //spaces between comment line and last character of code
            x += 1;
        }
    }

===============
Common Practice
===============

**Pointers:** Members to pointers should be accessed through ``p->member`` operator instead of ``(*p).member``.

**Indentation:** Tabs should be 4 spaces. If a pull request is made and changes are made to files you did not edit, check to see
if your editor is editing whitespace when opening files. If these issues are not fixed, your PR WILL NOT BE MERGED

**Return Path:** It is common practice for most functions to have one return path, making it easier to read the code and understand what 
complicated functions do. For example:

.. code-block:: c
    :linenos:

    //GOOD PRACTICE
    bool return_value;
    if (...) {
       return_value = ...
    }
    else if (...) {
        return_value = ...
    }
    return return_value
    // BAD PRACTICE
    bool return_value;
    if (...) {
       return return_value
    }
    else if (...) {
        return_value
    }

While this is the preferred style, it should not be done at the expense of making the code too complex. Sometimes it is not possible to
have just one return path.