

# Struct ws2812b\_handle\_t



[**ClassList**](annotated.md) **>** [**ws2812b\_handle\_t**](structws2812b__handle__t.md)





* `#include <WS2812B.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  uint32\_t | [**channel**](#variable-channel)  <br> |
|  volatile uint8\_t | [**dmaActive**](#variable-dmaactive)  <br> |
|  StaticSemaphore\_t | [**framePendingBuf**](#variable-framependingbuf)  <br> |
|  SemaphoreHandle\_t | [**framePendingSem**](#variable-framependingsem)  <br> |
|  uint8\_t(\* | [**ledData**](#variable-leddata)  <br> |
|  SemaphoreHandle\_t | [**mutex**](#variable-mutex)  <br> |
|  StaticSemaphore\_t | [**mutexBuf**](#variable-mutexbuf)  <br> |
|  uint8\_t | [**numberLeds**](#variable-numberleds)  <br> |
|  uint16\_t \* | [**pwmBuffer**](#variable-pwmbuffer)  <br> |
|  TIM\_HandleTypeDef \* | [**timerHandle**](#variable-timerhandle)  <br> |












































## Public Attributes Documentation




### variable channel 

```C++
uint32_t ws2812b_handle_t::channel;
```




<hr>



### variable dmaActive 

```C++
volatile uint8_t ws2812b_handle_t::dmaActive;
```




<hr>



### variable framePendingBuf 

```C++
StaticSemaphore_t ws2812b_handle_t::framePendingBuf;
```




<hr>



### variable framePendingSem 

```C++
SemaphoreHandle_t ws2812b_handle_t::framePendingSem;
```




<hr>



### variable ledData 

```C++
uint8_t(* ws2812b_handle_t::ledData)[NUMBER_PWM_DATA_ELEMENTS];
```




<hr>



### variable mutex 

```C++
SemaphoreHandle_t ws2812b_handle_t::mutex;
```




<hr>



### variable mutexBuf 

```C++
StaticSemaphore_t ws2812b_handle_t::mutexBuf;
```




<hr>



### variable numberLeds 

```C++
uint8_t ws2812b_handle_t::numberLeds;
```




<hr>



### variable pwmBuffer 

```C++
uint16_t* ws2812b_handle_t::pwmBuffer;
```




<hr>



### variable timerHandle 

```C++
TIM_HandleTypeDef* ws2812b_handle_t::timerHandle;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `driver/Inc/WS2812B.h`

