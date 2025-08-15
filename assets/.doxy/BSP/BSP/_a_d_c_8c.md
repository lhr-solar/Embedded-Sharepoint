

# File ADC.c



[**FileList**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Src**](dir_160128b01eb7e6b2a554f83ab5d45f5a.md) **>** [**ADC.c**](_a_d_c_8c.md)

[Go to the source code of this file](_a_d_c_8c_source.md)



* `#include "ADC.h"`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  uint32\_t | [**adc\_err\_code**](#variable-adc_err_code)   = `0`<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**HAL\_ADC\_ConvCpltCallback**](#function-hal_adc_convcpltcallback) (ADC\_HandleTypeDef \* h) <br> |
|  void | [**HAL\_ADC\_ErrorCallback**](#function-hal_adc_errorcallback) (ADC\_HandleTypeDef \* h) <br> |
|  void | [**HAL\_ADC\_MspDeInit**](#function-hal_adc_mspdeinit) (ADC\_HandleTypeDef \* h) <br> |
|  \_\_weak void | [**HAL\_ADC\_MspGPIOInit**](#function-hal_adc_mspgpioinit) () <br> |
|  void | [**HAL\_ADC\_MspInit**](#function-hal_adc_mspinit) (ADC\_HandleTypeDef \* h) <br> |
|  adc\_status\_t | [**adc\_deinit**](#function-adc_deinit) (ADC\_HandleTypeDef \* h) <br> |
|  adc\_status\_t | [**adc\_init**](#function-adc_init) (ADC\_InitTypeDef init, ADC\_HandleTypeDef \* h) <br> |
|  adc\_status\_t | [**adc\_read**](#function-adc_read) (uint32\_t channel, uint32\_t samplingTime, ADC\_HandleTypeDef \* h, QueueHandle\_t \* q) <br> |




























## Public Attributes Documentation




### variable adc\_err\_code 

```C++
uint32_t adc_err_code;
```




<hr>
## Public Functions Documentation




### function HAL\_ADC\_ConvCpltCallback 

```C++
void HAL_ADC_ConvCpltCallback (
    ADC_HandleTypeDef * h
) 
```




<hr>



### function HAL\_ADC\_ErrorCallback 

```C++
void HAL_ADC_ErrorCallback (
    ADC_HandleTypeDef * h
) 
```




<hr>



### function HAL\_ADC\_MspDeInit 

```C++
void HAL_ADC_MspDeInit (
    ADC_HandleTypeDef * h
) 
```




<hr>



### function HAL\_ADC\_MspGPIOInit 

```C++
__weak void HAL_ADC_MspGPIOInit () 
```




<hr>



### function HAL\_ADC\_MspInit 

```C++
void HAL_ADC_MspInit (
    ADC_HandleTypeDef * h
) 
```




<hr>



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
    ADC_HandleTypeDef * h
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
The documentation for this class was generated from the following file `bsp/Src/ADC.c`

