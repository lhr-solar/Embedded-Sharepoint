

# File UART.c



[**FileList**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Src**](dir_160128b01eb7e6b2a554f83ab5d45f5a.md) **>** [**UART.c**](_u_a_r_t_8c.md)

[Go to the source code of this file](_u_a_r_t_8c_source.md)



* `#include "UART.h"`
* `#include <string.h>`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**rx\_payload\_t**](structrx__payload__t.md) <br> |
| struct | [**tx\_payload\_t**](structtx__payload__t.md) <br> |






















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**HAL\_UART\_MspDeInit**](#function-hal_uart_mspdeinit) (UART\_HandleTypeDef \* huart) <br> |
|  \_\_weak void | [**HAL\_UART\_MspGPIODeInit**](#function-hal_uart_mspgpiodeinit) (UART\_HandleTypeDef \* huart) <br> |
|  \_\_weak void | [**HAL\_UART\_MspGPIOInit**](#function-hal_uart_mspgpioinit) (UART\_HandleTypeDef \* huart) <br> |
|  void | [**HAL\_UART\_MspInit**](#function-hal_uart_mspinit) (UART\_HandleTypeDef \* huart) <br> |
|  void | [**HAL\_UART\_RxCpltCallback**](#function-hal_uart_rxcpltcallback) (UART\_HandleTypeDef \* huart) <br> |
|  void | [**HAL\_UART\_TxCpltCallback**](#function-hal_uart_txcpltcallback) (UART\_HandleTypeDef \* huart) <br> |
|  uart\_status\_t | [**uart\_deinit**](#function-uart_deinit) (UART\_HandleTypeDef \* handle) <br>_Deinitializes the UART peripheral._  |
|  uart\_status\_t | [**uart\_init**](#function-uart_init) (UART\_HandleTypeDef \* handle) <br>_Initializes the UART peripheral._  |
|  uart\_status\_t | [**uart\_recv**](#function-uart_recv) (UART\_HandleTypeDef \* handle, uint8\_t \* data, uint8\_t length, TickType\_t delay\_ticks) <br>_Reads data from the user-provided RX queue._  |
|  uart\_status\_t | [**uart\_send**](#function-uart_send) (UART\_HandleTypeDef \* handle, const uint8\_t \* data, uint8\_t length, TickType\_t delay\_ticks) <br>_Transmits data over UART. If transmission is in progress, data will be queued in internal TX queue._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  bool | [**is\_uart\_initialized**](#function-is_uart_initialized) (UART\_HandleTypeDef \* handle) <br> |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**DATA\_SIZE**](_u_a_r_t_8c.md#define-data_size)  `(1)`<br> |
| define  | [**UART\_NVIC\_PREEMPT\_PRIO**](_u_a_r_t_8c.md#define-uart_nvic_preempt_prio)  `(5)`<br> |

## Public Functions Documentation




### function HAL\_UART\_MspDeInit 

```C++
void HAL_UART_MspDeInit (
    UART_HandleTypeDef * huart
) 
```




<hr>



### function HAL\_UART\_MspGPIODeInit 

```C++
__weak void HAL_UART_MspGPIODeInit (
    UART_HandleTypeDef * huart
) 
```




<hr>



### function HAL\_UART\_MspGPIOInit 

```C++
__weak void HAL_UART_MspGPIOInit (
    UART_HandleTypeDef * huart
) 
```




<hr>



### function HAL\_UART\_MspInit 

```C++
void HAL_UART_MspInit (
    UART_HandleTypeDef * huart
) 
```




<hr>



### function HAL\_UART\_RxCpltCallback 

```C++
void HAL_UART_RxCpltCallback (
    UART_HandleTypeDef * huart
) 
```




<hr>



### function HAL\_UART\_TxCpltCallback 

```C++
void HAL_UART_TxCpltCallback (
    UART_HandleTypeDef * huart
) 
```




<hr>



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
## Public Static Functions Documentation




### function is\_uart\_initialized 

```C++
static bool is_uart_initialized (
    UART_HandleTypeDef * handle
) 
```




<hr>
## Macro Definition Documentation





### define DATA\_SIZE 

```C++
#define DATA_SIZE `(1)`
```




<hr>



### define UART\_NVIC\_PREEMPT\_PRIO 

```C++
#define UART_NVIC_PREEMPT_PRIO `(5)`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `bsp/Src/UART.c`

