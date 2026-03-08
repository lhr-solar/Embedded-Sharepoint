

# File CAN\_FD.h



[**FileList**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Inc**](dir_f9758a777e5683291000913d5e959461.md) **>** [**CAN\_FD.h**](CAN__FD_8h.md)

[Go to the source code of this file](CAN__FD_8h_source.md)



* `#include "stm32xx_hal.h"`
* `#include "CAN_Common.h"`
* `#include "queue_ex.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**can\_status\_t**](CAN__Common_8h.md#enum-can_status_t) | [**can\_fd\_deinit**](#function-can_fd_deinit) (FDCAN\_HandleTypeDef \* handle) <br>_Deinitializes the FDCAN peripheral._  |
|  [**can\_status\_t**](CAN__Common_8h.md#enum-can_status_t) | [**can\_fd\_init**](#function-can_fd_init) (FDCAN\_HandleTypeDef \* handle, FDCAN\_FilterTypeDef \* filter) <br>_Initializes the FDCAN peripheral._  |
|  [**can\_status\_t**](CAN__Common_8h.md#enum-can_status_t) | [**can\_fd\_recv**](#function-can_fd_recv) (FDCAN\_HandleTypeDef \* handle, uint16\_t id, FDCAN\_RxHeaderTypeDef \* header, uint8\_t data, TickType\_t delay\_ticks) <br>_Receives a FDCAN message._  |
|  void | [**can\_fd\_rx\_callback\_hook**](#function-can_fd_rx_callback_hook) (FDCAN\_HandleTypeDef \* hfdcan, uint32\_t RxFifo0ITs, [**can\_rx\_payload\_t**](structcan__rx__payload__t.md) recv\_payload) <br> |
|  [**can\_status\_t**](CAN__Common_8h.md#enum-can_status_t) | [**can\_fd\_send**](#function-can_fd_send) (FDCAN\_HandleTypeDef \* handle, FDCAN\_TxHeaderTypeDef \* header, uint8\_t data, TickType\_t delay\_ticks) <br>_Sends a FDCAN message._  |
|  [**can\_status\_t**](CAN__Common_8h.md#enum-can_status_t) | [**can\_fd\_start**](#function-can_fd_start) (FDCAN\_HandleTypeDef \* handle) <br>_Starts the FDCAN peripheral._  |
|  void | [**can\_fd\_tx\_callback\_hook**](#function-can_fd_tx_callback_hook) (FDCAN\_HandleTypeDef \* hfdcan, const [**can\_tx\_payload\_t**](structcan__tx__payload__t.md) \* payload) <br> |




























## Public Functions Documentation




### function can\_fd\_deinit 

_Deinitializes the FDCAN peripheral._ 
```C++
can_status_t can_fd_deinit (
    FDCAN_HandleTypeDef * handle
) 
```



This function deinitializes the HAL FDCAN driver and disables FDCAN interrupts.




**Parameters:**


* `handle` Pointer to the FDCAN handle structure.



**Returns:**

can\_status\_t Returns CAN\_OK on success, CAN\_ERR on failure. 





        

<hr>



### function can\_fd\_init 

_Initializes the FDCAN peripheral._ 
```C++
can_status_t can_fd_init (
    FDCAN_HandleTypeDef * handle,
    FDCAN_FilterTypeDef * filter
) 
```



This function initializes the FDCAN peripheral, sets up send/receive queues, configures the HAL FDCAN driver, applies the filter configuration, and enables FDCAN interrupts.




**Parameters:**


* `handle` Pointer to the FDCAN handle structure. 
* `filter` Pointer to the FDCAN filter configuration structure.



**Returns:**

can\_status\_t Returns CAN\_OK on success, CAN\_ERR on failure. 





        

<hr>



### function can\_fd\_recv 

_Receives a FDCAN message._ 
```C++
can_status_t can_fd_recv (
    FDCAN_HandleTypeDef * handle,
    uint16_t id,
    FDCAN_RxHeaderTypeDef * header,
    uint8_t data,
    TickType_t delay_ticks
) 
```



Reads a message from the receive queue corresponding to the specified ID. Supports blocking or non-blocking behavior depending on delay\_ticks.




**Parameters:**


* `handle` Pointer to the FDCAN handle structure. 
* `id` CAN identifier of the message to receive. 
* `header` Pointer to a FDCAN\_RxHeaderTypeDef structure to store the received header. 
* `data` Array to store the received data. 
* `delay_ticks` Maximum delay to wait if queue is empty (FreeRTOS ticks).



**Returns:**

can\_status\_t Returns CAN\_OK if a message was received, CAN\_EMPTY if the queue was empty, CAN\_ERR on failure or invalid ID. 





        

<hr>



### function can\_fd\_rx\_callback\_hook 

```C++
void can_fd_rx_callback_hook (
    FDCAN_HandleTypeDef * hfdcan,
    uint32_t RxFifo0ITs,
    can_rx_payload_t recv_payload
) 
```




<hr>



### function can\_fd\_send 

_Sends a FDCAN message._ 
```C++
can_status_t can_fd_send (
    FDCAN_HandleTypeDef * handle,
    FDCAN_TxHeaderTypeDef * header,
    uint8_t data,
    TickType_t delay_ticks
) 
```



Places a FDCAN message into the transmit mailbox if available, otherwise queues it in the send queue for later transmission.




**Parameters:**


* `handle` Pointer to the FDCAN handle structure. 
* `header` Pointer to the FDCAN transmit header structure. 
* `data` Array containing the data to send. 
* `delay_ticks` Maximum delay to wait if queue is full (FreeRTOS ticks).



**Returns:**

can\_status\_t Returns CAN\_OK if message was successfully sent or queued, CAN\_ERR on failure. 





        

<hr>



### function can\_fd\_start 

_Starts the FDCAN peripheral._ 
```C++
can_status_t can_fd_start (
    FDCAN_HandleTypeDef * handle
) 
```



Activates the FDCAN peripheral so it can transmit and receive messages.




**Parameters:**


* `handle` Pointer to the FDCAN handle structure.



**Returns:**

can\_status\_t Returns CAN\_OK on success, CAN\_ERR on failure. 





        

<hr>



### function can\_fd\_tx\_callback\_hook 

```C++
void can_fd_tx_callback_hook (
    FDCAN_HandleTypeDef * hfdcan,
    const can_tx_payload_t * payload
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `bsp/Inc/CAN_FD.h`

