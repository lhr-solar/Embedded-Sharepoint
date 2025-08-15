

# File ADC.h



[**FileList**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Inc**](dir_f9758a777e5683291000913d5e959461.md) **>** [**ADC.h**](_a_d_c_8h.md)

[Go to the source code of this file](_a_d_c_8h_source.md)



* `#include "stm32xx_hal.h"`
* `#include "FreeRTOS.h"`
* `#include "queue.h"`
* `#include "semphr.h"`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**adc\_status\_t**](#enum-adc_status_t)  <br>_File that shows how to initialize and read from ADCs using static queues and FreeRTOS tasks._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  adc\_status\_t | [**adc\_deinit**](#function-adc_deinit) (ADC\_HandleTypeDef \* h) <br> |
|  adc\_status\_t | [**adc\_init**](#function-adc_init) (ADC\_InitTypeDef init, ADC\_HandleTypeDef \* hadc) <br> |
|  adc\_status\_t | [**adc\_read**](#function-adc_read) (uint32\_t channel, uint32\_t samplingTime, ADC\_HandleTypeDef \* h, QueueHandle\_t \* q) <br> |




























## Public Types Documentation




### enum adc\_status\_t 

_File that shows how to initialize and read from ADCs using static queues and FreeRTOS tasks._ 
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

```C++
adc_status_t adc_deinit (
    ADC_HandleTypeDef * h
) 
```



channel uint32\_t channel to read samplingTime uint32\_t ADC sampling time blocking bool whether the process is blocking 


        

<hr>



### function adc\_init 

```C++
adc_status_t adc_init (
    ADC_InitTypeDef init,
    ADC_HandleTypeDef * hadc
) 
```




<hr>



### function adc\_read 

```C++
adc_status_t adc_read (
    uint32_t channel,
    uint32_t samplingTime,
    ADC_HandleTypeDef * h,
    QueueHandle_t * q
) 
```



init ADC\_InitTypeDef bitNum uint8\_t number of bits in ADC Vcc double rxQueue QueueHandle\_t pointer to user-provided Queue handle 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `bsp/Inc/ADC.h`

