

# File UART.h



[**FileList**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Inc**](dir_f9758a777e5683291000913d5e959461.md) **>** [**UART.h**](UART_8h.md)

[Go to the source code of this file](UART_8h_source.md)

_Provides functions for initializing, sending, receiving, and managing UART peripherals._ [More...](#detailed-description)

* `#include "stm32xx_hal.h"`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**uart\_status\_t**](#enum-uart_status_t)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**uart\_status\_t**](UART_8h.md#enum-uart_status_t) | [**uart\_deinit**](#function-uart_deinit) (UART\_HandleTypeDef \* handle) <br>_Deinitializes the specified UART peripheral._  |
|  [**uart\_status\_t**](UART_8h.md#enum-uart_status_t) | [**uart\_init**](#function-uart_init) (UART\_HandleTypeDef \* handle) <br>_Initializes the specified UART peripheral._  |
|  [**uart\_status\_t**](UART_8h.md#enum-uart_status_t) | [**uart\_recv**](#function-uart_recv) (UART\_HandleTypeDef \* handle, uint8\_t \* data, uint8\_t length, TickType\_t delay\_ticks) <br>_Receives data from UART RX queue._  |
|  [**uart\_status\_t**](UART_8h.md#enum-uart_status_t) | [**uart\_send**](#function-uart_send) (UART\_HandleTypeDef \* handle, const uint8\_t \* data, uint8\_t length, TickType\_t delay\_ticks) <br>_Transmits data over UART. If a transmission is in progress, data will be queued._  |




























## Detailed Description


This file contains the API for working with UART (Universal Asynchronous Receiver/Transmitter) peripherals in an embedded system, including initialization, data transmission, reception, and queue-based handling (e.g., with FreeRTOS). The functions support flexible configuration of UART instances and integration with user-provided GPIO initialization.


**Initialization**:



* Call `uart_init()` to configure and initialize a specific UART peripheral.
* Provide the UART configuration structure and handle for the target peripheral.
* Returns a status code indicating success or failure.




**Data Transmission**:



* Call `uart_send()` to queue data for transmission over the UART.
* The function will transmit when the peripheral is available.




**Data Reception**:



* Call `uart_recv()` to read received data from the RX queue.
* Specify the number of bytes to read.




**Deinitialization**:



* Call `uart_deinit()` to reset the UART peripheral and release resources.






**Note:**





* The functions `HAL_UART_MspGPIOInit()` and `HAL_UART_MspGPIODeInit()` must be overridden by the user to provide GPIO initialization/deinitialization for the selected UART, including enabling clocks and calling `HAL_GPIO_Init()` with the correct `GPIO_InitTypeDef`.
* Each UART peripheral is currently intended for use by a single task since only one receive queue exists. 




    
## Public Types Documentation




### enum uart\_status\_t 

```C++
enum uart_status_t {
    UART_ERR,
    UART_OK,
    UART_SENT,
    UART_RECV,
    UART_EMPTY
};
```




<hr>
## Public Functions Documentation




### function uart\_deinit 

_Deinitializes the specified UART peripheral._ 
```C++
uart_status_t uart_deinit (
    UART_HandleTypeDef * handle
) 
```





**Parameters:**


* `handle` Pointer to the UART handle. 



**Returns:**

uart\_status\_t Returns UART\_OK on success, UART\_ERR on failure. 





        

<hr>



### function uart\_init 

_Initializes the specified UART peripheral._ 
```C++
uart_status_t uart_init (
    UART_HandleTypeDef * handle
) 
```





**Parameters:**


* `handle` Pointer to the UART handle. 



**Returns:**

uart\_status\_t Returns UART\_OK on success, UART\_ERR on failure. 





        

<hr>



### function uart\_recv 

_Receives data from UART RX queue._ 
```C++
uart_status_t uart_recv (
    UART_HandleTypeDef * handle,
    uint8_t * data,
    uint8_t length,
    TickType_t delay_ticks
) 
```





**Parameters:**


* `handle` Pointer to the UART handle. 
* `data` Pointer to buffer where received data will be stored. 
* `length` Number of bytes to read. 
* `delay_ticks` Ticks to wait for data (0 = non-blocking, portMAX\_DELAY = block until available). 



**Returns:**

uart\_status\_t Returns UART\_RECV on success, UART\_EMPTY if RX queue empty, UART\_ERR on failure. 





        

<hr>



### function uart\_send 

_Transmits data over UART. If a transmission is in progress, data will be queued._ 
```C++
uart_status_t uart_send (
    UART_HandleTypeDef * handle,
    const uint8_t * data,
    uint8_t length,
    TickType_t delay_ticks
) 
```





**Parameters:**


* `handle` Pointer to the UART handle. 
* `data` Pointer to the buffer containing data to send. 
* `length` Number of bytes to transmit. 
* `delay_ticks` Ticks to wait if TX queue is full (0 = no wait, portMAX\_DELAY = wait indefinitely). 



**Returns:**

uart\_status\_t Returns UART\_SENT on queued/sent, UART\_ERR on failure. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `bsp/Inc/UART.h`

