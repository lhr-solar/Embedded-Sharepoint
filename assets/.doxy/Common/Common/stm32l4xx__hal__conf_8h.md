

# File stm32l4xx\_hal\_conf.h



[**FileList**](files.md) **>** [**common**](dir_bdd9a5d540de89e9fe90efdfc6973a4f.md) **>** [**Inc**](dir_4152f073bf32bf79124b676c9fe2adc1.md) **>** [**stm32l4xx\_hal\_conf.h**](stm32l4xx__hal__conf_8h.md)

[Go to the source code of this file](stm32l4xx__hal__conf_8h_source.md)

_HAL configuration template file. This file should be copied to the application folder and renamed to_ [_**stm32l4xx\_hal\_conf.h**_](stm32l4xx__hal__conf_8h.md) _._[More...](#detailed-description)

* `#include "stm32l4xx_hal_rcc.h"`
* `#include "stm32l4xx_hal_gpio.h"`
* `#include "stm32l4xx_hal_dma.h"`
* `#include "stm32l4xx_hal_cortex.h"`
* `#include "stm32l4xx_hal_adc.h"`
* `#include "stm32l4xx_hal_can.h"`
* `#include "stm32l4xx_hal_dac.h"`
* `#include "stm32l4xx_hal_exti.h"`
* `#include "stm32l4xx_hal_flash.h"`
* `#include "stm32l4xx_hal_i2c.h"`
* `#include "stm32l4xx_hal_iwdg.h"`
* `#include "stm32l4xx_hal_pwr.h"`
* `#include "stm32l4xx_hal_rng.h"`
* `#include "stm32l4xx_hal_rtc.h"`
* `#include "stm32l4xx_hal_sd.h"`
* `#include "stm32l4xx_hal_spi.h"`
* `#include "stm32l4xx_hal_tim.h"`
* `#include "stm32l4xx_hal_uart.h"`
* `#include "stm32l4xx_hal_usart.h"`
* `#include "stm32l4xx_hal_wwdg.h"`
































































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**DATA\_CACHE\_ENABLE**](stm32l4xx__hal__conf_8h.md#define-data_cache_enable)  `1U`<br> |
| define  | [**EXTERNAL\_SAI1\_CLOCK\_VALUE**](stm32l4xx__hal__conf_8h.md#define-external_sai1_clock_value)  `2097000U`<br>_External clock source for SAI1 peripheral This value is used by the RCC HAL module to compute the SAI1 & SAI2 clock source frequency._  |
| define  | [**EXTERNAL\_SAI2\_CLOCK\_VALUE**](stm32l4xx__hal__conf_8h.md#define-external_sai2_clock_value)  `2097000U`<br>_External clock source for SAI2 peripheral This value is used by the RCC HAL module to compute the SAI1 & SAI2 clock source frequency._  |
| define  | [**HAL\_ADC\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_adc_module_enabled)  <br> |
| define  | [**HAL\_CAN\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_can_module_enabled)  <br> |
| define  | [**HAL\_CORTEX\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_cortex_module_enabled)  <br> |
| define  | [**HAL\_DAC\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_dac_module_enabled)  <br> |
| define  | [**HAL\_DMA\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_dma_module_enabled)  <br> |
| define  | [**HAL\_EXTI\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_exti_module_enabled)  <br> |
| define  | [**HAL\_EXTI\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_exti_module_enabled)  <br> |
| define  | [**HAL\_FLASH\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_flash_module_enabled)  <br> |
| define  | [**HAL\_GPIO\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_gpio_module_enabled)  <br> |
| define  | [**HAL\_I2C\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_i2c_module_enabled)  <br> |
| define  | [**HAL\_I2S\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_i2s_module_enabled)  <br> |
| define  | [**HAL\_IWDG\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_iwdg_module_enabled)  <br> |
| define  | [**HAL\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_module_enabled)  <br>_This is the list of modules to be used in the HAL driver._  |
| define  | [**HAL\_PWR\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_pwr_module_enabled)  <br> |
| define  | [**HAL\_RCC\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_rcc_module_enabled)  <br> |
| define  | [**HAL\_RNG\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_rng_module_enabled)  <br> |
| define  | [**HAL\_RTC\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_rtc_module_enabled)  <br> |
| define  | [**HAL\_SD\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_sd_module_enabled)  <br> |
| define  | [**HAL\_SPI\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_spi_module_enabled)  <br> |
| define  | [**HAL\_TIM\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_tim_module_enabled)  <br> |
| define  | [**HAL\_UART\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_uart_module_enabled)  <br> |
| define  | [**HAL\_USART\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_usart_module_enabled)  <br> |
| define  | [**HAL\_WWDG\_MODULE\_ENABLED**](stm32l4xx__hal__conf_8h.md#define-hal_wwdg_module_enabled)  <br> |
| define  | [**HSE\_STARTUP\_TIMEOUT**](stm32l4xx__hal__conf_8h.md#define-hse_startup_timeout)  `((uint32\_t)100U)`<br> |
| define  | [**HSE\_VALUE**](stm32l4xx__hal__conf_8h.md#define-hse_value)  `((uint32\_t)8000000U)`<br>_Adjust the value of External High Speed oscillator (HSE) used in your application. This value is used by the RCC HAL module to compute the system frequency (when HSE is used as system clock source, directly or through the PLL)._  |
| define  | [**HSI48\_VALUE**](stm32l4xx__hal__conf_8h.md#define-hsi48_value)  `((uint32\_t)48000000U)`<br>_Internal High Speed oscillator (HSI48) value for USB FS, SDMMC and RNG. This internal oscillator is mainly dedicated to provide a high precision clock to the USB peripheral by means of a special Clock Recovery System (CRS) circuitry. When the CRS is not used, the HSI48 RC oscillator runs on it default frequency which is subject to manufacturing process variations._  |
| define  | [**HSI\_VALUE**](stm32l4xx__hal__conf_8h.md#define-hsi_value)  `((uint32\_t)16000000U)`<br>_Internal High Speed oscillator (HSI) value. This value is used by the RCC HAL module to compute the system frequency (when HSI is used as system clock source, directly or through the PLL)._  |
| define  | [**INSTRUCTION\_CACHE\_ENABLE**](stm32l4xx__hal__conf_8h.md#define-instruction_cache_enable)  `1U`<br> |
| define  | [**LSE\_STARTUP\_TIMEOUT**](stm32l4xx__hal__conf_8h.md#define-lse_startup_timeout)  `5000U`<br> |
| define  | [**LSE\_VALUE**](stm32l4xx__hal__conf_8h.md#define-lse_value)  `32768U`<br>_External Low Speed oscillator (LSE) value. This value is used by the UART, RTC HAL module to compute the system frequency._  |
| define  | [**LSI\_VALUE**](stm32l4xx__hal__conf_8h.md#define-lsi_value)  `32000U`<br>_Internal Low Speed oscillator (LSI) value._  |
| define  | [**MSI\_VALUE**](stm32l4xx__hal__conf_8h.md#define-msi_value)  `((uint32\_t)4000000U)`<br>_Internal Multiple Speed oscillator (MSI) default value. This value is the default MSI range value after Reset._  |
| define  | [**PREFETCH\_ENABLE**](stm32l4xx__hal__conf_8h.md#define-prefetch_enable)  `0U`<br> |
| define  | [**TICK\_INT\_PRIORITY**](stm32l4xx__hal__conf_8h.md#define-tick_int_priority)  `15U`<br> |
| define  | [**USE\_HAL\_ADC\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_adc_register_callbacks)  `0U`<br>_Uncomment the line below to expanse the "assert\_param" macro in the HAL drivers code._  |
| define  | [**USE\_HAL\_CAN\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_can_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_COMP\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_comp_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_CRYP\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_cryp_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_DAC\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_dac_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_DCMI\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_dcmi_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_DFSDM\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_dfsdm_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_DMA2D\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_dma2d_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_DSI\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_dsi_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_GFXMMU\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_gfxmmu_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_HASH\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_hash_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_HCD\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_hcd_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_I2C\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_i2c_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_IRDA\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_irda_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_LPTIM\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_lptim_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_LTDC\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_ltdc_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_MMC\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_mmc_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_OPAMP\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_opamp_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_OSPI\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_ospi_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_PCD\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_pcd_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_QSPI\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_qspi_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_RNG\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_rng_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_RTC\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_rtc_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_SAI\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_sai_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_SD\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_sd_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_SMARTCARD\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_smartcard_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_SMBUS\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_smbus_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_SPI\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_spi_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_SWPMI\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_swpmi_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_TIM\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_tim_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_TSC\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_tsc_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_UART\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_uart_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_USART\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_usart_register_callbacks)  `0U`<br> |
| define  | [**USE\_HAL\_WWDG\_REGISTER\_CALLBACKS**](stm32l4xx__hal__conf_8h.md#define-use_hal_wwdg_register_callbacks)  `0U`<br> |
| define  | [**USE\_RTOS**](stm32l4xx__hal__conf_8h.md#define-use_rtos)  `0U`<br> |
| define  | [**USE\_SPI\_CRC**](stm32l4xx__hal__conf_8h.md#define-use_spi_crc)  `0U`<br> |
| define  | [**VDD\_VALUE**](stm32l4xx__hal__conf_8h.md#define-vdd_value)  `3300U`<br>_This is the HAL system configuration section._  |
| define  | [**assert\_param**](stm32l4xx__hal__conf_8h.md#define-assert_param) (expr) `((void)0U)`<br>_Include module's header file._  |

## Detailed Description




**Author:**

MCD Application Team 




**Attention:**




Copyright (c) 2017 STMicroelectronics. All rights reserved.


This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component. If no LICENSE file comes with this software, it is provided AS-IS. 


    
## Macro Definition Documentation





### define DATA\_CACHE\_ENABLE 

```C++
#define DATA_CACHE_ENABLE `1U`
```




<hr>



### define EXTERNAL\_SAI1\_CLOCK\_VALUE 

_External clock source for SAI1 peripheral This value is used by the RCC HAL module to compute the SAI1 & SAI2 clock source frequency._ 
```C++
#define EXTERNAL_SAI1_CLOCK_VALUE `2097000U`
```



Value of the SAI1 External clock source in Hz 


        

<hr>



### define EXTERNAL\_SAI2\_CLOCK\_VALUE 

_External clock source for SAI2 peripheral This value is used by the RCC HAL module to compute the SAI1 & SAI2 clock source frequency._ 
```C++
#define EXTERNAL_SAI2_CLOCK_VALUE `2097000U`
```



Value of the SAI2 External clock source in Hz 


        

<hr>



### define HAL\_ADC\_MODULE\_ENABLED 

```C++
#define HAL_ADC_MODULE_ENABLED 
```




<hr>



### define HAL\_CAN\_MODULE\_ENABLED 

```C++
#define HAL_CAN_MODULE_ENABLED 
```




<hr>



### define HAL\_CORTEX\_MODULE\_ENABLED 

```C++
#define HAL_CORTEX_MODULE_ENABLED 
```




<hr>



### define HAL\_DAC\_MODULE\_ENABLED 

```C++
#define HAL_DAC_MODULE_ENABLED 
```




<hr>



### define HAL\_DMA\_MODULE\_ENABLED 

```C++
#define HAL_DMA_MODULE_ENABLED 
```




<hr>



### define HAL\_EXTI\_MODULE\_ENABLED 

```C++
#define HAL_EXTI_MODULE_ENABLED 
```




<hr>



### define HAL\_EXTI\_MODULE\_ENABLED 

```C++
#define HAL_EXTI_MODULE_ENABLED 
```




<hr>



### define HAL\_FLASH\_MODULE\_ENABLED 

```C++
#define HAL_FLASH_MODULE_ENABLED 
```




<hr>



### define HAL\_GPIO\_MODULE\_ENABLED 

```C++
#define HAL_GPIO_MODULE_ENABLED 
```




<hr>



### define HAL\_I2C\_MODULE\_ENABLED 

```C++
#define HAL_I2C_MODULE_ENABLED 
```




<hr>



### define HAL\_I2S\_MODULE\_ENABLED 

```C++
#define HAL_I2S_MODULE_ENABLED 
```




<hr>



### define HAL\_IWDG\_MODULE\_ENABLED 

```C++
#define HAL_IWDG_MODULE_ENABLED 
```




<hr>



### define HAL\_MODULE\_ENABLED 

_This is the list of modules to be used in the HAL driver._ 
```C++
#define HAL_MODULE_ENABLED 
```




<hr>



### define HAL\_PWR\_MODULE\_ENABLED 

```C++
#define HAL_PWR_MODULE_ENABLED 
```




<hr>



### define HAL\_RCC\_MODULE\_ENABLED 

```C++
#define HAL_RCC_MODULE_ENABLED 
```




<hr>



### define HAL\_RNG\_MODULE\_ENABLED 

```C++
#define HAL_RNG_MODULE_ENABLED 
```




<hr>



### define HAL\_RTC\_MODULE\_ENABLED 

```C++
#define HAL_RTC_MODULE_ENABLED 
```




<hr>



### define HAL\_SD\_MODULE\_ENABLED 

```C++
#define HAL_SD_MODULE_ENABLED 
```




<hr>



### define HAL\_SPI\_MODULE\_ENABLED 

```C++
#define HAL_SPI_MODULE_ENABLED 
```




<hr>



### define HAL\_TIM\_MODULE\_ENABLED 

```C++
#define HAL_TIM_MODULE_ENABLED 
```




<hr>



### define HAL\_UART\_MODULE\_ENABLED 

```C++
#define HAL_UART_MODULE_ENABLED 
```




<hr>



### define HAL\_USART\_MODULE\_ENABLED 

```C++
#define HAL_USART_MODULE_ENABLED 
```




<hr>



### define HAL\_WWDG\_MODULE\_ENABLED 

```C++
#define HAL_WWDG_MODULE_ENABLED 
```




<hr>



### define HSE\_STARTUP\_TIMEOUT 

```C++
#define HSE_STARTUP_TIMEOUT `((uint32_t)100U)`
```



Time out for HSE start up, in ms 


        

<hr>



### define HSE\_VALUE 

_Adjust the value of External High Speed oscillator (HSE) used in your application. This value is used by the RCC HAL module to compute the system frequency (when HSE is used as system clock source, directly or through the PLL)._ 
```C++
#define HSE_VALUE `((uint32_t)8000000U)`
```



Value of the External oscillator in Hz 


        

<hr>



### define HSI48\_VALUE 

_Internal High Speed oscillator (HSI48) value for USB FS, SDMMC and RNG. This internal oscillator is mainly dedicated to provide a high precision clock to the USB peripheral by means of a special Clock Recovery System (CRS) circuitry. When the CRS is not used, the HSI48 RC oscillator runs on it default frequency which is subject to manufacturing process variations._ 
```C++
#define HSI48_VALUE `((uint32_t)48000000U)`
```



Value of the Internal High Speed oscillator for USB FS/SDMMC/RNG in Hz. The real value my vary depending on manufacturing process variations. 


        

<hr>



### define HSI\_VALUE 

_Internal High Speed oscillator (HSI) value. This value is used by the RCC HAL module to compute the system frequency (when HSI is used as system clock source, directly or through the PLL)._ 
```C++
#define HSI_VALUE `((uint32_t)16000000U)`
```



Value of the Internal oscillator in Hz 


        

<hr>



### define INSTRUCTION\_CACHE\_ENABLE 

```C++
#define INSTRUCTION_CACHE_ENABLE `1U`
```




<hr>



### define LSE\_STARTUP\_TIMEOUT 

```C++
#define LSE_STARTUP_TIMEOUT `5000U`
```



Time out for LSE start up, in ms 


        

<hr>



### define LSE\_VALUE 

_External Low Speed oscillator (LSE) value. This value is used by the UART, RTC HAL module to compute the system frequency._ 
```C++
#define LSE_VALUE `32768U`
```



&lt; Value of the Internal Low Speed oscillator in Hz The real value may vary depending on the variations in voltage and temperature. Value of the External oscillator in Hz 


        

<hr>



### define LSI\_VALUE 

_Internal Low Speed oscillator (LSI) value._ 
```C++
#define LSI_VALUE `32000U`
```



LSI Typical Value in Hz 


        

<hr>



### define MSI\_VALUE 

_Internal Multiple Speed oscillator (MSI) default value. This value is the default MSI range value after Reset._ 
```C++
#define MSI_VALUE `((uint32_t)4000000U)`
```



Value of the Internal oscillator in Hz 


        

<hr>



### define PREFETCH\_ENABLE 

```C++
#define PREFETCH_ENABLE `0U`
```




<hr>



### define TICK\_INT\_PRIORITY 

```C++
#define TICK_INT_PRIORITY `15U`
```



tick interrupt priority 


        

<hr>



### define USE\_HAL\_ADC\_REGISTER\_CALLBACKS 

_Uncomment the line below to expanse the "assert\_param" macro in the HAL drivers code._ 
```C++
#define USE_HAL_ADC_REGISTER_CALLBACKS `0U`
```



Set below the peripheral configuration to "1U" to add the support of HAL callback registration/deregistration feature for the HAL driver(s). This allows user application to provide specific callback functions thanks to HAL\_PPP\_RegisterCallback() rather than overwriting the default weak callback functions (see each stm32l4xx\_hal\_ppp.h file for possible callback identifiers defined in HAL\_PPP\_CallbackIDTypeDef for each PPP peripheral). 


        

<hr>



### define USE\_HAL\_CAN\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_CAN_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_COMP\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_COMP_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_CRYP\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_CRYP_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_DAC\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_DAC_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_DCMI\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_DCMI_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_DFSDM\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_DFSDM_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_DMA2D\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_DMA2D_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_DSI\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_DSI_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_GFXMMU\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_GFXMMU_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_HASH\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_HASH_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_HCD\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_HCD_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_I2C\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_I2C_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_IRDA\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_IRDA_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_LPTIM\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_LPTIM_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_LTDC\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_LTDC_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_MMC\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_MMC_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_OPAMP\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_OPAMP_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_OSPI\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_OSPI_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_PCD\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_PCD_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_QSPI\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_QSPI_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_RNG\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_RNG_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_RTC\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_RTC_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_SAI\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_SAI_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_SD\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_SD_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_SMARTCARD\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_SMARTCARD_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_SMBUS\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_SMBUS_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_SPI\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_SPI_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_SWPMI\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_SWPMI_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_TIM\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_TIM_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_TSC\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_TSC_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_UART\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_UART_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_USART\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_USART_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_HAL\_WWDG\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_WWDG_REGISTER_CALLBACKS `0U`
```




<hr>



### define USE\_RTOS 

```C++
#define USE_RTOS `0U`
```




<hr>



### define USE\_SPI\_CRC 

```C++
#define USE_SPI_CRC `0U`
```




<hr>



### define VDD\_VALUE 

_This is the HAL system configuration section._ 
```C++
#define VDD_VALUE `3300U`
```



Value of VDD in mv 


        

<hr>



### define assert\_param 

_Include module's header file._ 
```C++
#define assert_param (
    expr
) `((void)0U)`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `common/Inc/stm32l4xx_hal_conf.h`

