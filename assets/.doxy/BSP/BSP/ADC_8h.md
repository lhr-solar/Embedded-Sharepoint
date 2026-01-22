

# File ADC.h



[**FileList**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Inc**](dir_f9758a777e5683291000913d5e959461.md) **>** [**ADC.h**](ADC_8h.md)

[Go to the source code of this file](ADC_8h_source.md)

_Provides functions for initializing, reading, and managing ADC peripherals._ [More...](#detailed-description)

* `#include "stm32xx_hal.h"`
* `#include "FreeRTOS.h"`
* `#include "queue.h"`
* `#include "semphr.h"`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**adc\_status\_t**](#enum-adc_status_t)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**adc\_status\_t**](ADC_8h.md#enum-adc_status_t) | [**adc\_deinit**](#function-adc_deinit) (ADC\_HandleTypeDef \* h) <br>_Deinitializes the ADC peripheral._  |
|  [**adc\_status\_t**](ADC_8h.md#enum-adc_status_t) | [**adc\_init**](#function-adc_init) (ADC\_InitTypeDef \* init, ADC\_HandleTypeDef \* hadc) <br>_Initializes the ADC peripheral with the specified configuration._  |
|  [**adc\_status\_t**](ADC_8h.md#enum-adc_status_t) | [**adc\_read**](#function-adc_read) (uint32\_t channel, uint32\_t samplingTime, ADC\_HandleTypeDef \* h, QueueHandle\_t \* q) <br>_Reads a value from the specified ADC channel._  |




























## Detailed Description


This file contains the API for working with ADC (Analog-to-Digital Converter) peripherals in an embedded system, including initialization, reading channels, and queue-based data handling (e.g., with FreeRTOS). The functions support flexible configuration of ADC instances, sampling times, and data storage mechanisms.


**Initialization**:



* Call `adc_init()` to configure and initialize a specific ADC instance.
* Provide the ADC configuration structure and handle for the target peripheral.
* Returns a status code indicating success or failure.




**Reading ADC Values**:



* Call `adc_read()` to sample a channel on the ADC.
* Provide the channel number, sampling time, ADC handle, and storage/queue reference.
* Returns a status code indicating success or failure.




**Data Retrieval**:



* If using queues, use standard FreeRTOS queue functions (e.g., `xQueueReceive()`) to retrieve ADC readings.
* Supports both blocking and non-blocking behavior depending on the chosen timeout.






**Note:**

This driver is designed to work with multiple ADC instances and supports integration with an RTOS for asynchronous data handling. 





    
## Public Types Documentation




### enum adc\_status\_t 

```C++
enum adc_status_t {
    ADC_OK,
    ADC_INIT_FAIL,
    ADC_DEINIT_FAIL,
    ADC_CHANNEL_CONFIG_FAIL,
    ADC_INTERRUPT_BUSY,
    ADC_INTERRUPT_TIMEOUT,
    ADC_INTERRUPT_ERROR,
    ADC_QUEUE_FULL
};
```




<hr>
## Public Functions Documentation




### function adc\_deinit 

_Deinitializes the ADC peripheral._ 
```C++
adc_status_t adc_deinit (
    ADC_HandleTypeDef * h
) 
```



This function releases resources used by the ADC and resets it to its default uninitialized state.




**Parameters:**


* `h` Pointer to the ADC handle structure.



**Returns:**

adc\_status\_t Returns ADC\_OK on success or an appropriate error code. 





        

<hr>



### function adc\_init 

_Initializes the ADC peripheral with the specified configuration._ 
```C++
adc_status_t adc_init (
    ADC_InitTypeDef * init,
    ADC_HandleTypeDef * hadc
) 
```



This function sets up the ADC hardware according to the parameters in the initialization structure and prepares it for subsequent conversions.




**Parameters:**


* `init` ADC initialization structure containing configuration parameters. 
* `hadc` Pointer to the ADC handle structure.



**Returns:**

adc\_status\_t Returns ADC\_OK on success or an appropriate error code. 





        

<hr>



### function adc\_read 

_Reads a value from the specified ADC channel._ 
```C++
adc_status_t adc_read (
    uint32_t channel,
    uint32_t samplingTime,
    ADC_HandleTypeDef * h,
    QueueHandle_t * q
) 
```



This function triggers a conversion on the given channel and stores the converted result in the provided queue. It supports both blocking and non-blocking operation depending on configuration.




**Parameters:**


* `channel` ADC channel to read from. 
* `samplingTime` ADC sampling time (in ADC clock cycles). 
* `h` Pointer to the ADC handle structure. 
* `q` Pointer to the user-provided queue handle for result storage.



**Returns:**

adc\_status\_t Returns ADC\_OK on success or an appropriate error code. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `bsp/Inc/ADC.h`

