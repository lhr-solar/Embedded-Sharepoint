

# File UART.h



[**FileList**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Inc**](dir_f9758a777e5683291000913d5e959461.md) **>** [**UART.h**](_u_a_r_t_8h.md)

[Go to the source code of this file](_u_a_r_t_8h_source.md)



* `#include "stm32xx_hal.h"`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**uart\_status\_t**](#enum-uart_status_t)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  uart\_status\_t | [**uart\_deinit**](#function-uart_deinit) (UART\_HandleTypeDef \* handle) <br>_Deinitializes the UART peripheral._  |
|  uart\_status\_t | [**uart\_init**](#function-uart_init) (UART\_HandleTypeDef \* handle) <br>_Initializes the UART peripheral._  |
|  uart\_status\_t | [**uart\_recv**](#function-uart_recv) (UART\_HandleTypeDef \* handle, uint8\_t \* data, uint8\_t length, TickType\_t delay\_ticks) <br>_Reads data from the user-provided RX queue._  |
|  uart\_status\_t | [**uart\_send**](#function-uart_send) (UART\_HandleTypeDef \* handle, const uint8\_t \* data, uint8\_t length, TickType\_t delay\_ticks) <br>_Transmits data over UART. If transmission is in progress, data will be queued in internal TX queue._  |




























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

_Deinitializes the UART peripheral._ 
```C++
uart_status_t uart_deinit (
    UART_HandleTypeDef * handle
) 
```





**Parameters:**


* `handle` pointer to the UART handle 



**Returns:**

uart\_status\_t 





        

<hr>



### function uart\_init 

_Initializes the UART peripheral._ 
```C++
uart_status_t uart_init (
    UART_HandleTypeDef * handle
) 
```





**Parameters:**


* `handle` pointer to the UART handle 



**Returns:**

uart\_status\_t 





        

<hr>



### function uart\_recv 

_Reads data from the user-provided RX queue._ 
```C++
uart_status_t uart_recv (
    UART_HandleTypeDef * handle,
    uint8_t * data,
    uint8_t length,
    TickType_t delay_ticks
) 
```





**Parameters:**


* `huart` pointer to the UART handle 
* `data` pointer to the data buffer that will be read into 
* `length` of the buffer that will be read 
* `delay_ticks` number of ticks to wait for data to be received 



**Returns:**

uart\_status\_t 





        

<hr>



### function uart\_send 

_Transmits data over UART. If transmission is in progress, data will be queued in internal TX queue._ 
```C++
uart_status_t uart_send (
    UART_HandleTypeDef * handle,
    const uint8_t * data,
    uint8_t length,
    TickType_t delay_ticks
) 
```





**Parameters:**


* `data` pointer to the data buffer that will be written/transmitted 
* `length` of the buffer that will be written/transmitted 
* `delay_ticks` number of ticks to wait for data to be transmitted 



**Returns:**

uart\_status\_t 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `bsp/Inc/UART.h`

