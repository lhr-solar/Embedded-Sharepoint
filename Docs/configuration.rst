*************************************
Configuring the Embedded Sharepoint
*************************************

.. note::
    The files BSP/Src/BSP.c and /Util/Inc/bsp_config.h contain useful
    comments and information regarding the configuration functionality.
    Please read them if you get the chance.

    The terms "AF" (alternate function) and "peripheral" (similar to alternate function)
    are used interchangably in this document.

################################
Alternate Function Configuration
################################

Configuring the alternate functions each pin maps to is relatively simple. The ``bsp_config.h`` 
configuration file is what the end user should be modifying.

The way the STM32 HAL library works with alternate functions is that each pin has (up to) 15
alternate function "columns" it can support. The HAL library knows which pin does what for each peripheral
type, but we still have to define the peripheral. HAL defines each alternate function type according
to the AF column the specified peripheral exists in. As an example, suppose that PA3 supports
the following peripherals on the "AF5" column:

+--------+-------------------------------+
| Port A |              AF5              |
+========+===============================+
| Pin 3  | UART3_TX, SPI2_MOSI, I2C1_SDL |
+--------+-------------------------------+

The HAL library defines ALL of those peripheral mappings as the integer value "5". This makes it 
impossible for us to determine specifically which AF we are talking about when we configure
PA3. As such, this configuration layer for the BSP implements a few levels of indirection to
support easy user configuration for alternate function and configuration structs for each peripheral
type.

The alternate function config is specified in ``bsp_config.h``. Any overrides for alternate functions
for each pin (Ports A-D, pin numbers varying; see the file for all of them) is performed by:

    #define P<Z><NUM>_ALTERNATE_FUNCTION (AF enum variant)

where the list of enum variants is:

+--------------+--------------------+
| Port A       | Alternate Function |
+==============+====================+
| GPIO (No AF) | BSP_GPIO           |
+--------------+--------------------+
| Timer 1      | BSP_TIM1           |
+--------------+--------------------+
| Timer 2      | BSP_TIM2           |
+--------------+--------------------+
| Timer 3      | BSP_TIM3           |
+--------------+--------------------+
| Timer 9      | BSP_TIM9           |
+--------------+--------------------+
| I2C1         | BSP_I2C1           |
+--------------+--------------------+
| I2C3         | BSP_I2C3           |
+--------------+--------------------+
| SPI2         | BSP_SPI2           |
+--------------+--------------------+
| SPI3         | BSP_SPI3           |
+--------------+--------------------+
| CAN2         | BSP_CAN2           |
+--------------+--------------------+
| CAN3         | BSP_CAN3           |
+--------------+--------------------+
| UART4        | BSP_UART4          |
+--------------+--------------------+
| UART5        | BSP_UART5          |
+--------------+--------------------+

For example, if I wanted to assign (hypothetically) PC10 to support SPI2 functionality,
I would write the following in ``bsp_config.h`` ABOVE the ``#ifndef`` s:

``#define PC10_ALTERNATE_FUNCTION BSP_SPI2``

The preprocessor takes care of the rest.

#########################################
Default Peripheral Initialization Structs
#########################################

The configuration file also has definitions for each peripheral's default init struct data.
HAL uses several different structs for config and init, namely ``XYZ_HandleTypeDef`` and ``XYZ_InitTypeDef``.

Each peripheral has a predefined ``InitTypeDef`` struct located at the bottom of the ``bsp_config.h`` file.
These structs are assigned to **GLOBAL** ``HandleTypeDef`` handles to each peripheral in the ``BSP.c`` file's
``BSP_Init()`` function. This function **must** be called before you call a BSP peripheral initialization function.

The structure of the macros themselves should not be modified, but the struct parameters should be configured as
needed by the user.