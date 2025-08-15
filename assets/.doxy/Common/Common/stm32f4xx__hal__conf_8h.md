

# File stm32f4xx\_hal\_conf.h



[**FileList**](files.md) **>** [**common**](dir_bdd9a5d540de89e9fe90efdfc6973a4f.md) **>** [**Inc**](dir_4152f073bf32bf79124b676c9fe2adc1.md) **>** [**stm32f4xx\_hal\_conf.h**](stm32f4xx__hal__conf_8h.md)

[Go to the source code of this file](stm32f4xx__hal__conf_8h_source.md)



* `#include "stm32f4xx_hal_rcc.h"`
* `#include "stm32f4xx_hal_gpio.h"`
* `#include "stm32f4xx_hal_exti.h"`
* `#include "stm32f4xx_hal_dma.h"`
* `#include "stm32f4xx_hal_cortex.h"`
* `#include "stm32f4xx_hal_adc.h"`
* `#include "stm32f4xx_hal_can.h"`
* `#include "stm32f4xx_hal_dac.h"`
* `#include "stm32f4xx_hal_flash.h"`
* `#include "stm32f4xx_hal_i2c.h"`
* `#include "stm32f4xx_hal_i2s.h"`
* `#include "stm32f4xx_hal_iwdg.h"`
* `#include "stm32f4xx_hal_pwr.h"`
* `#include "stm32f4xx_hal_rng.h"`
* `#include "stm32f4xx_hal_rtc.h"`
* `#include "stm32f4xx_hal_sd.h"`
* `#include "stm32f4xx_hal_spi.h"`
* `#include "stm32f4xx_hal_tim.h"`
* `#include "stm32f4xx_hal_uart.h"`
* `#include "stm32f4xx_hal_usart.h"`
* `#include "stm32f4xx_hal_wwdg.h"`
































































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**DATA\_CACHE\_ENABLE**](stm32f4xx__hal__conf_8h.md#define-data_cache_enable)  `1U`<br> |
| define  | [**DP83848\_PHY\_ADDRESS**](stm32f4xx__hal__conf_8h.md#define-dp83848_phy_address)  <br> |
| define  | [**ETH\_RXBUFNB**](stm32f4xx__hal__conf_8h.md#define-eth_rxbufnb)  `4U       /\* 4 Rx buffers of size ETH\_RX\_BUF\_SIZE  \*/`<br> |
| define  | [**ETH\_RX\_BUF\_SIZE**](stm32f4xx__hal__conf_8h.md#define-eth_rx_buf_size)  `ETH\_MAX\_PACKET\_SIZE /\* buffer size for receive               \*/`<br> |
| define  | [**ETH\_TXBUFNB**](stm32f4xx__hal__conf_8h.md#define-eth_txbufnb)  `4U       /\* 4 Tx buffers of size ETH\_TX\_BUF\_SIZE  \*/`<br> |
| define  | [**ETH\_TX\_BUF\_SIZE**](stm32f4xx__hal__conf_8h.md#define-eth_tx_buf_size)  `ETH\_MAX\_PACKET\_SIZE /\* buffer size for transmit              \*/`<br> |
| define  | [**EXTERNAL\_CLOCK\_VALUE**](stm32f4xx__hal__conf_8h.md#define-external_clock_value)  `12288000U`<br>_External clock source for I2S peripheral This value is used by the I2S HAL module to compute the I2S clock source frequency, this source is inserted directly through I2S\_CKIN pad._  |
| define  | [**HAL\_ADC\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_adc_module_enabled)  <br> |
| define  | [**HAL\_CAN\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_can_module_enabled)  <br> |
| define  | [**HAL\_CORTEX\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_cortex_module_enabled)  <br> |
| define  | [**HAL\_DAC\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_dac_module_enabled)  <br> |
| define  | [**HAL\_DMA\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_dma_module_enabled)  <br> |
| define  | [**HAL\_EXTI\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_exti_module_enabled)  <br> |
| define  | [**HAL\_FLASH\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_flash_module_enabled)  <br> |
| define  | [**HAL\_GPIO\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_gpio_module_enabled)  <br> |
| define  | [**HAL\_I2C\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_i2c_module_enabled)  <br> |
| define  | [**HAL\_I2S\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_i2s_module_enabled)  <br> |
| define  | [**HAL\_IWDG\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_iwdg_module_enabled)  <br> |
| define  | [**HAL\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_module_enabled)  <br>_This is the list of modules to be used in the HAL driver._  |
| define  | [**HAL\_PWR\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_pwr_module_enabled)  <br> |
| define  | [**HAL\_RCC\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_rcc_module_enabled)  <br> |
| define  | [**HAL\_RNG\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_rng_module_enabled)  <br> |
| define  | [**HAL\_RTC\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_rtc_module_enabled)  <br> |
| define  | [**HAL\_SD\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_sd_module_enabled)  <br> |
| define  | [**HAL\_SPI\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_spi_module_enabled)  <br> |
| define  | [**HAL\_TIM\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_tim_module_enabled)  <br> |
| define  | [**HAL\_UART\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_uart_module_enabled)  <br> |
| define  | [**HAL\_USART\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_usart_module_enabled)  <br> |
| define  | [**HAL\_WWDG\_MODULE\_ENABLED**](stm32f4xx__hal__conf_8h.md#define-hal_wwdg_module_enabled)  <br> |
| define  | [**HSE\_STARTUP\_TIMEOUT**](stm32f4xx__hal__conf_8h.md#define-hse_startup_timeout)  `100U`<br> |
| define  | [**HSE\_VALUE**](stm32f4xx__hal__conf_8h.md#define-hse_value)  `25000000U`<br>_Adjust the value of External High Speed oscillator (HSE) used in your application. This value is used by the RCC HAL module to compute the system frequency (when HSE is used as system clock source, directly or through the PLL)._  |
| define  | [**HSI\_VALUE**](stm32f4xx__hal__conf_8h.md#define-hsi_value)  `((uint32\_t)16000000U)`<br>_Internal High Speed oscillator (HSI) value. This value is used by the RCC HAL module to compute the system frequency (when HSI is used as system clock source, directly or through the PLL)._  |
| define  | [**INSTRUCTION\_CACHE\_ENABLE**](stm32f4xx__hal__conf_8h.md#define-instruction_cache_enable)  `1U`<br> |
| define  | [**LSE\_STARTUP\_TIMEOUT**](stm32f4xx__hal__conf_8h.md#define-lse_startup_timeout)  `5000U`<br> |
| define  | [**LSE\_VALUE**](stm32f4xx__hal__conf_8h.md#define-lse_value)  `32768U`<br>_External Low Speed oscillator (LSE) value._  |
| define  | [**LSI\_VALUE**](stm32f4xx__hal__conf_8h.md#define-lsi_value)  `32000U`<br>_Internal Low Speed oscillator (LSI) value._  |
| define  | [**MAC\_ADDR0**](stm32f4xx__hal__conf_8h.md#define-mac_addr0)  `2U`<br>_Uncomment the line below to expanse the "assert\_param" macro in the HAL drivers code._  |
| define  | [**MAC\_ADDR1**](stm32f4xx__hal__conf_8h.md#define-mac_addr1)  `0U`<br> |
| define  | [**MAC\_ADDR2**](stm32f4xx__hal__conf_8h.md#define-mac_addr2)  `0U`<br> |
| define  | [**MAC\_ADDR3**](stm32f4xx__hal__conf_8h.md#define-mac_addr3)  `0U`<br> |
| define  | [**MAC\_ADDR4**](stm32f4xx__hal__conf_8h.md#define-mac_addr4)  `0U`<br> |
| define  | [**MAC\_ADDR5**](stm32f4xx__hal__conf_8h.md#define-mac_addr5)  `0U`<br> |
| define  | [**PHY\_AUTONEGOTIATION**](stm32f4xx__hal__conf_8h.md#define-phy_autonegotiation)  `((uint16\_t)0x1000U)`<br> |
| define  | [**PHY\_AUTONEGO\_COMPLETE**](stm32f4xx__hal__conf_8h.md#define-phy_autonego_complete)  `((uint16\_t)0x0020U)`<br> |
| define  | [**PHY\_BCR**](stm32f4xx__hal__conf_8h.md#define-phy_bcr)  `((uint16\_t)0x0000U)`<br> |
| define  | [**PHY\_BSR**](stm32f4xx__hal__conf_8h.md#define-phy_bsr)  `((uint16\_t)0x0001U)`<br> |
| define  | [**PHY\_CONFIG\_DELAY**](stm32f4xx__hal__conf_8h.md#define-phy_config_delay)  `0x00000FFFU`<br> |
| define  | [**PHY\_DUPLEX\_STATUS**](stm32f4xx__hal__conf_8h.md#define-phy_duplex_status)  `((uint16\_t))`<br> |
| define  | [**PHY\_FULLDUPLEX\_100M**](stm32f4xx__hal__conf_8h.md#define-phy_fullduplex_100m)  `((uint16\_t)0x2100U)`<br> |
| define  | [**PHY\_FULLDUPLEX\_10M**](stm32f4xx__hal__conf_8h.md#define-phy_fullduplex_10m)  `((uint16\_t)0x0100U)`<br> |
| define  | [**PHY\_HALFDUPLEX\_100M**](stm32f4xx__hal__conf_8h.md#define-phy_halfduplex_100m)  `((uint16\_t)0x2000U)`<br> |
| define  | [**PHY\_HALFDUPLEX\_10M**](stm32f4xx__hal__conf_8h.md#define-phy_halfduplex_10m)  `((uint16\_t)0x0000U)`<br> |
| define  | [**PHY\_ISOLATE**](stm32f4xx__hal__conf_8h.md#define-phy_isolate)  `((uint16\_t)0x0400U)`<br> |
| define  | [**PHY\_JABBER\_DETECTION**](stm32f4xx__hal__conf_8h.md#define-phy_jabber_detection)  `((uint16\_t)0x0002U)`<br> |
| define  | [**PHY\_LINKED\_STATUS**](stm32f4xx__hal__conf_8h.md#define-phy_linked_status)  `((uint16\_t)0x0004U)`<br> |
| define  | [**PHY\_LOOPBACK**](stm32f4xx__hal__conf_8h.md#define-phy_loopback)  `((uint16\_t)0x4000U)`<br> |
| define  | [**PHY\_POWERDOWN**](stm32f4xx__hal__conf_8h.md#define-phy_powerdown)  `((uint16\_t)0x0800U)`<br> |
| define  | [**PHY\_READ\_TO**](stm32f4xx__hal__conf_8h.md#define-phy_read_to)  `0x0000FFFFU`<br> |
| define  | [**PHY\_RESET**](stm32f4xx__hal__conf_8h.md#define-phy_reset)  `((uint16\_t)0x8000U)`<br> |
| define  | [**PHY\_RESET\_DELAY**](stm32f4xx__hal__conf_8h.md#define-phy_reset_delay)  `0x000000FFU`<br> |
| define  | [**PHY\_RESTART\_AUTONEGOTIATION**](stm32f4xx__hal__conf_8h.md#define-phy_restart_autonegotiation)  `((uint16\_t)0x0200U)`<br> |
| define  | [**PHY\_SPEED\_STATUS**](stm32f4xx__hal__conf_8h.md#define-phy_speed_status)  `((uint16\_t))`<br> |
| define  | [**PHY\_SR**](stm32f4xx__hal__conf_8h.md#define-phy_sr)  `((uint16\_t))`<br> |
| define  | [**PHY\_WRITE\_TO**](stm32f4xx__hal__conf_8h.md#define-phy_write_to)  `0x0000FFFFU`<br> |
| define  | [**PREFETCH\_ENABLE**](stm32f4xx__hal__conf_8h.md#define-prefetch_enable)  `1U`<br> |
| define  | [**TICK\_INT\_PRIORITY**](stm32f4xx__hal__conf_8h.md#define-tick_int_priority)  `15U`<br> |
| define  | [**USE\_HAL\_ADC\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_adc_register_callbacks)  `0U /\* ADC register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_CAN\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_can_register_callbacks)  `0U /\* CAN register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_CEC\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_cec_register_callbacks)  `0U /\* CEC register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_CRYP\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_cryp_register_callbacks)  `0U /\* CRYP register callback disabled      \*/`<br> |
| define  | [**USE\_HAL\_DAC\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_dac_register_callbacks)  `0U /\* DAC register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_DCMI\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_dcmi_register_callbacks)  `0U /\* DCMI register callback disabled      \*/`<br> |
| define  | [**USE\_HAL\_DFSDM\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_dfsdm_register_callbacks)  `0U /\* DFSDM register callback disabled     \*/`<br> |
| define  | [**USE\_HAL\_DMA2D\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_dma2d_register_callbacks)  `0U /\* DMA2D register callback disabled     \*/`<br> |
| define  | [**USE\_HAL\_DSI\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_dsi_register_callbacks)  `0U /\* DSI register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_ETH\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_eth_register_callbacks)  `0U /\* ETH register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_FMPI2C\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_fmpi2c_register_callbacks)  `0U /\* FMPI2C register callback disabled    \*/`<br> |
| define  | [**USE\_HAL\_FMPSMBUS\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_fmpsmbus_register_callbacks)  `0U /\* FMPSMBUS register callback disabled  \*/`<br> |
| define  | [**USE\_HAL\_HASH\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_hash_register_callbacks)  `0U /\* HASH register callback disabled      \*/`<br> |
| define  | [**USE\_HAL\_HCD\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_hcd_register_callbacks)  `0U /\* HCD register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_I2C\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_i2c_register_callbacks)  `0U /\* I2C register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_I2S\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_i2s_register_callbacks)  `0U /\* I2S register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_IRDA\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_irda_register_callbacks)  `0U /\* IRDA register callback disabled      \*/`<br> |
| define  | [**USE\_HAL\_LPTIM\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_lptim_register_callbacks)  `0U /\* LPTIM register callback disabled     \*/`<br> |
| define  | [**USE\_HAL\_LTDC\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_ltdc_register_callbacks)  `0U /\* LTDC register callback disabled      \*/`<br> |
| define  | [**USE\_HAL\_MMC\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_mmc_register_callbacks)  `0U /\* MMC register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_NAND\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_nand_register_callbacks)  `0U /\* NAND register callback disabled      \*/`<br> |
| define  | [**USE\_HAL\_NOR\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_nor_register_callbacks)  `0U /\* NOR register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_PCCARD\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_pccard_register_callbacks)  `0U /\* PCCARD register callback disabled    \*/`<br> |
| define  | [**USE\_HAL\_PCD\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_pcd_register_callbacks)  `0U /\* PCD register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_QSPI\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_qspi_register_callbacks)  `0U /\* QSPI register callback disabled      \*/`<br> |
| define  | [**USE\_HAL\_RNG\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_rng_register_callbacks)  `0U /\* RNG register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_RTC\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_rtc_register_callbacks)  `0U /\* RTC register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_SAI\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_sai_register_callbacks)  `0U /\* SAI register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_SDRAM\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_sdram_register_callbacks)  `0U /\* SDRAM register callback disabled     \*/`<br> |
| define  | [**USE\_HAL\_SD\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_sd_register_callbacks)  `0U /\* SD register callback disabled        \*/`<br> |
| define  | [**USE\_HAL\_SMARTCARD\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_smartcard_register_callbacks)  `0U /\* SMARTCARD register callback disabled \*/`<br> |
| define  | [**USE\_HAL\_SMBUS\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_smbus_register_callbacks)  `0U /\* SMBUS register callback disabled     \*/`<br> |
| define  | [**USE\_HAL\_SPDIFRX\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_spdifrx_register_callbacks)  `0U /\* SPDIFRX register callback disabled   \*/`<br> |
| define  | [**USE\_HAL\_SPI\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_spi_register_callbacks)  `0U /\* SPI register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_SRAM\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_sram_register_callbacks)  `0U /\* SRAM register callback disabled      \*/`<br> |
| define  | [**USE\_HAL\_TIM\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_tim_register_callbacks)  `0U /\* TIM register callback disabled       \*/`<br> |
| define  | [**USE\_HAL\_UART\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_uart_register_callbacks)  `0U /\* UART register callback disabled      \*/`<br> |
| define  | [**USE\_HAL\_USART\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_usart_register_callbacks)  `0U /\* USART register callback disabled     \*/`<br> |
| define  | [**USE\_HAL\_WWDG\_REGISTER\_CALLBACKS**](stm32f4xx__hal__conf_8h.md#define-use_hal_wwdg_register_callbacks)  `0U /\* WWDG register callback disabled      \*/`<br> |
| define  | [**USE\_RTOS**](stm32f4xx__hal__conf_8h.md#define-use_rtos)  `0U`<br> |
| define  | [**USE\_SPI\_CRC**](stm32f4xx__hal__conf_8h.md#define-use_spi_crc)  `0U`<br> |
| define  | [**VDD\_VALUE**](stm32f4xx__hal__conf_8h.md#define-vdd_value)  `3300U`<br>_This is the HAL system configuration section._  |
| define  | [**assert\_param**](stm32f4xx__hal__conf_8h.md#define-assert_param) (expr) `((void)0U)`<br>_Include module's header file._  |

## Macro Definition Documentation





### define DATA\_CACHE\_ENABLE 

```C++
#define DATA_CACHE_ENABLE `1U`
```




<hr>



### define DP83848\_PHY\_ADDRESS 

```C++
#define DP83848_PHY_ADDRESS 
```




<hr>



### define ETH\_RXBUFNB 

```C++
#define ETH_RXBUFNB `4U       /* 4 Rx buffers of size ETH_RX_BUF_SIZE  */`
```




<hr>



### define ETH\_RX\_BUF\_SIZE 

```C++
#define ETH_RX_BUF_SIZE `ETH_MAX_PACKET_SIZE /* buffer size for receive               */`
```




<hr>



### define ETH\_TXBUFNB 

```C++
#define ETH_TXBUFNB `4U       /* 4 Tx buffers of size ETH_TX_BUF_SIZE  */`
```




<hr>



### define ETH\_TX\_BUF\_SIZE 

```C++
#define ETH_TX_BUF_SIZE `ETH_MAX_PACKET_SIZE /* buffer size for transmit              */`
```




<hr>



### define EXTERNAL\_CLOCK\_VALUE 

_External clock source for I2S peripheral This value is used by the I2S HAL module to compute the I2S clock source frequency, this source is inserted directly through I2S\_CKIN pad._ 
```C++
#define EXTERNAL_CLOCK_VALUE `12288000U`
```



Value of the External audio frequency in Hz 


        

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
#define HSE_STARTUP_TIMEOUT `100U`
```



Time out for HSE start up, in ms 


        

<hr>



### define HSE\_VALUE 

_Adjust the value of External High Speed oscillator (HSE) used in your application. This value is used by the RCC HAL module to compute the system frequency (when HSE is used as system clock source, directly or through the PLL)._ 
```C++
#define HSE_VALUE `25000000U`
```



Value of the External oscillator in Hz 


        

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

_External Low Speed oscillator (LSE) value._ 
```C++
#define LSE_VALUE `32768U`
```



&lt; Value of the Internal Low Speed oscillator in Hz The real value may vary depending on the variations in voltage and temperature. Value of the External Low Speed oscillator in Hz 


        

<hr>



### define LSI\_VALUE 

_Internal Low Speed oscillator (LSI) value._ 
```C++
#define LSI_VALUE `32000U`
```



LSI Typical Value in Hz 


        

<hr>



### define MAC\_ADDR0 

_Uncomment the line below to expanse the "assert\_param" macro in the HAL drivers code._ 
```C++
#define MAC_ADDR0 `2U`
```




<hr>



### define MAC\_ADDR1 

```C++
#define MAC_ADDR1 `0U`
```




<hr>



### define MAC\_ADDR2 

```C++
#define MAC_ADDR2 `0U`
```




<hr>



### define MAC\_ADDR3 

```C++
#define MAC_ADDR3 `0U`
```




<hr>



### define MAC\_ADDR4 

```C++
#define MAC_ADDR4 `0U`
```




<hr>



### define MAC\_ADDR5 

```C++
#define MAC_ADDR5 `0U`
```




<hr>



### define PHY\_AUTONEGOTIATION 

```C++
#define PHY_AUTONEGOTIATION `((uint16_t)0x1000U)`
```



Enable auto-negotiation function 


        

<hr>



### define PHY\_AUTONEGO\_COMPLETE 

```C++
#define PHY_AUTONEGO_COMPLETE `((uint16_t)0x0020U)`
```



Auto-Negotiation process completed 


        

<hr>



### define PHY\_BCR 

```C++
#define PHY_BCR `((uint16_t)0x0000U)`
```



Transceiver Basic Control Register 


        

<hr>



### define PHY\_BSR 

```C++
#define PHY_BSR `((uint16_t)0x0001U)`
```



Transceiver Basic Status Register 


        

<hr>



### define PHY\_CONFIG\_DELAY 

```C++
#define PHY_CONFIG_DELAY `0x00000FFFU`
```




<hr>



### define PHY\_DUPLEX\_STATUS 

```C++
#define PHY_DUPLEX_STATUS `((uint16_t))`
```



PHY Duplex mask 


        

<hr>



### define PHY\_FULLDUPLEX\_100M 

```C++
#define PHY_FULLDUPLEX_100M `((uint16_t)0x2100U)`
```



Set the full-duplex mode at 100 Mb/s 


        

<hr>



### define PHY\_FULLDUPLEX\_10M 

```C++
#define PHY_FULLDUPLEX_10M `((uint16_t)0x0100U)`
```



Set the full-duplex mode at 10 Mb/s 


        

<hr>



### define PHY\_HALFDUPLEX\_100M 

```C++
#define PHY_HALFDUPLEX_100M `((uint16_t)0x2000U)`
```



Set the half-duplex mode at 100 Mb/s 


        

<hr>



### define PHY\_HALFDUPLEX\_10M 

```C++
#define PHY_HALFDUPLEX_10M `((uint16_t)0x0000U)`
```



Set the half-duplex mode at 10 Mb/s 


        

<hr>



### define PHY\_ISOLATE 

```C++
#define PHY_ISOLATE `((uint16_t)0x0400U)`
```



Isolate PHY from MII 


        

<hr>



### define PHY\_JABBER\_DETECTION 

```C++
#define PHY_JABBER_DETECTION `((uint16_t)0x0002U)`
```



Jabber condition detected 


        

<hr>



### define PHY\_LINKED\_STATUS 

```C++
#define PHY_LINKED_STATUS `((uint16_t)0x0004U)`
```



Valid link established 


        

<hr>



### define PHY\_LOOPBACK 

```C++
#define PHY_LOOPBACK `((uint16_t)0x4000U)`
```



Select loop-back mode 


        

<hr>



### define PHY\_POWERDOWN 

```C++
#define PHY_POWERDOWN `((uint16_t)0x0800U)`
```



Select the power down mode 


        

<hr>



### define PHY\_READ\_TO 

```C++
#define PHY_READ_TO `0x0000FFFFU`
```




<hr>



### define PHY\_RESET 

```C++
#define PHY_RESET `((uint16_t)0x8000U)`
```



PHY Reset 


        

<hr>



### define PHY\_RESET\_DELAY 

```C++
#define PHY_RESET_DELAY `0x000000FFU`
```




<hr>



### define PHY\_RESTART\_AUTONEGOTIATION 

```C++
#define PHY_RESTART_AUTONEGOTIATION `((uint16_t)0x0200U)`
```



Restart auto-negotiation function 


        

<hr>



### define PHY\_SPEED\_STATUS 

```C++
#define PHY_SPEED_STATUS `((uint16_t))`
```



PHY Speed mask 


        

<hr>



### define PHY\_SR 

```C++
#define PHY_SR `((uint16_t))`
```



PHY status register Offset 


        

<hr>



### define PHY\_WRITE\_TO 

```C++
#define PHY_WRITE_TO `0x0000FFFFU`
```




<hr>



### define PREFETCH\_ENABLE 

```C++
#define PREFETCH_ENABLE `1U`
```




<hr>



### define TICK\_INT\_PRIORITY 

```C++
#define TICK_INT_PRIORITY `15U`
```



tick interrupt priority 


        

<hr>



### define USE\_HAL\_ADC\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_ADC_REGISTER_CALLBACKS `0U /* ADC register callback disabled       */`
```




<hr>



### define USE\_HAL\_CAN\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_CAN_REGISTER_CALLBACKS `0U /* CAN register callback disabled       */`
```




<hr>



### define USE\_HAL\_CEC\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_CEC_REGISTER_CALLBACKS `0U /* CEC register callback disabled       */`
```




<hr>



### define USE\_HAL\_CRYP\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_CRYP_REGISTER_CALLBACKS `0U /* CRYP register callback disabled      */`
```




<hr>



### define USE\_HAL\_DAC\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_DAC_REGISTER_CALLBACKS `0U /* DAC register callback disabled       */`
```




<hr>



### define USE\_HAL\_DCMI\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_DCMI_REGISTER_CALLBACKS `0U /* DCMI register callback disabled      */`
```




<hr>



### define USE\_HAL\_DFSDM\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_DFSDM_REGISTER_CALLBACKS `0U /* DFSDM register callback disabled     */`
```




<hr>



### define USE\_HAL\_DMA2D\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_DMA2D_REGISTER_CALLBACKS `0U /* DMA2D register callback disabled     */`
```




<hr>



### define USE\_HAL\_DSI\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_DSI_REGISTER_CALLBACKS `0U /* DSI register callback disabled       */`
```




<hr>



### define USE\_HAL\_ETH\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_ETH_REGISTER_CALLBACKS `0U /* ETH register callback disabled       */`
```




<hr>



### define USE\_HAL\_FMPI2C\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_FMPI2C_REGISTER_CALLBACKS `0U /* FMPI2C register callback disabled    */`
```




<hr>



### define USE\_HAL\_FMPSMBUS\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_FMPSMBUS_REGISTER_CALLBACKS `0U /* FMPSMBUS register callback disabled  */`
```




<hr>



### define USE\_HAL\_HASH\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_HASH_REGISTER_CALLBACKS `0U /* HASH register callback disabled      */`
```




<hr>



### define USE\_HAL\_HCD\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_HCD_REGISTER_CALLBACKS `0U /* HCD register callback disabled       */`
```




<hr>



### define USE\_HAL\_I2C\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_I2C_REGISTER_CALLBACKS `0U /* I2C register callback disabled       */`
```




<hr>



### define USE\_HAL\_I2S\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_I2S_REGISTER_CALLBACKS `0U /* I2S register callback disabled       */`
```




<hr>



### define USE\_HAL\_IRDA\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_IRDA_REGISTER_CALLBACKS `0U /* IRDA register callback disabled      */`
```




<hr>



### define USE\_HAL\_LPTIM\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_LPTIM_REGISTER_CALLBACKS `0U /* LPTIM register callback disabled     */`
```




<hr>



### define USE\_HAL\_LTDC\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_LTDC_REGISTER_CALLBACKS `0U /* LTDC register callback disabled      */`
```




<hr>



### define USE\_HAL\_MMC\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_MMC_REGISTER_CALLBACKS `0U /* MMC register callback disabled       */`
```




<hr>



### define USE\_HAL\_NAND\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_NAND_REGISTER_CALLBACKS `0U /* NAND register callback disabled      */`
```




<hr>



### define USE\_HAL\_NOR\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_NOR_REGISTER_CALLBACKS `0U /* NOR register callback disabled       */`
```




<hr>



### define USE\_HAL\_PCCARD\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_PCCARD_REGISTER_CALLBACKS `0U /* PCCARD register callback disabled    */`
```




<hr>



### define USE\_HAL\_PCD\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_PCD_REGISTER_CALLBACKS `0U /* PCD register callback disabled       */`
```




<hr>



### define USE\_HAL\_QSPI\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_QSPI_REGISTER_CALLBACKS `0U /* QSPI register callback disabled      */`
```




<hr>



### define USE\_HAL\_RNG\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_RNG_REGISTER_CALLBACKS `0U /* RNG register callback disabled       */`
```




<hr>



### define USE\_HAL\_RTC\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_RTC_REGISTER_CALLBACKS `0U /* RTC register callback disabled       */`
```




<hr>



### define USE\_HAL\_SAI\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_SAI_REGISTER_CALLBACKS `0U /* SAI register callback disabled       */`
```




<hr>



### define USE\_HAL\_SDRAM\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_SDRAM_REGISTER_CALLBACKS `0U /* SDRAM register callback disabled     */`
```




<hr>



### define USE\_HAL\_SD\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_SD_REGISTER_CALLBACKS `0U /* SD register callback disabled        */`
```




<hr>



### define USE\_HAL\_SMARTCARD\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_SMARTCARD_REGISTER_CALLBACKS `0U /* SMARTCARD register callback disabled */`
```




<hr>



### define USE\_HAL\_SMBUS\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_SMBUS_REGISTER_CALLBACKS `0U /* SMBUS register callback disabled     */`
```




<hr>



### define USE\_HAL\_SPDIFRX\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_SPDIFRX_REGISTER_CALLBACKS `0U /* SPDIFRX register callback disabled   */`
```




<hr>



### define USE\_HAL\_SPI\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_SPI_REGISTER_CALLBACKS `0U /* SPI register callback disabled       */`
```




<hr>



### define USE\_HAL\_SRAM\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_SRAM_REGISTER_CALLBACKS `0U /* SRAM register callback disabled      */`
```




<hr>



### define USE\_HAL\_TIM\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_TIM_REGISTER_CALLBACKS `0U /* TIM register callback disabled       */`
```




<hr>



### define USE\_HAL\_UART\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_UART_REGISTER_CALLBACKS `0U /* UART register callback disabled      */`
```




<hr>



### define USE\_HAL\_USART\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_USART_REGISTER_CALLBACKS `0U /* USART register callback disabled     */`
```




<hr>



### define USE\_HAL\_WWDG\_REGISTER\_CALLBACKS 

```C++
#define USE_HAL_WWDG_REGISTER_CALLBACKS `0U /* WWDG register callback disabled      */`
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
The documentation for this class was generated from the following file `common/Inc/stm32f4xx_hal_conf.h`

