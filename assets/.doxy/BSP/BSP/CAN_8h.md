

# File CAN.h



[**FileList**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Inc**](dir_f9758a777e5683291000913d5e959461.md) **>** [**CAN.h**](CAN_8h.md)

[Go to the source code of this file](CAN_8h_source.md)

_CAN peripheral driver for LHRS embedded systems._ [More...](#detailed-description)

* `#include "stm32xx_hal.h"`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**can\_status\_t**](#enum-can_status_t)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**can\_status\_t**](CAN_8h.md#enum-can_status_t) | [**can\_deinit**](#function-can_deinit) (CAN\_HandleTypeDef \* handle) <br>_Deinitializes the CAN peripheral._  |
|  [**can\_status\_t**](CAN_8h.md#enum-can_status_t) | [**can\_init**](#function-can_init) (CAN\_HandleTypeDef \* handle, CAN\_FilterTypeDef \* filter) <br>_Initializes the CAN peripheral._  |
|  [**can\_status\_t**](CAN_8h.md#enum-can_status_t) | [**can\_recv**](#function-can_recv) (CAN\_HandleTypeDef \* handle, uint16\_t id, CAN\_RxHeaderTypeDef \* header, uint8\_t data, TickType\_t delay\_ticks) <br>_Receives a CAN message._  |
|  [**can\_status\_t**](CAN_8h.md#enum-can_status_t) | [**can\_send**](#function-can_send) (CAN\_HandleTypeDef \* handle, const CAN\_TxHeaderTypeDef \* header, const uint8\_t data, TickType\_t delay\_ticks) <br>_Sends a CAN message._  |
|  [**can\_status\_t**](CAN_8h.md#enum-can_status_t) | [**can\_start**](#function-can_start) (CAN\_HandleTypeDef \* handle) <br>_Starts the CAN peripheral._  |
|  [**can\_status\_t**](CAN_8h.md#enum-can_status_t) | [**can\_stop**](#function-can_stop) (CAN\_HandleTypeDef \* handle) <br>_Stops the CAN peripheral._  |




























## Detailed Description


This driver provides initialization, deinitialization, start/stop, sending, and receiving functionality for CAN peripherals.


Function Descriptions:



* Init: Initializes the CAN peripheral given the configuration (does not start the CAN peripheral).
* DeInit: Deinitializes the CAN peripheral; resets configuration to default values.
* Start: Starts the CAN peripheral, enabling interrupts and message handling.
* Stop: Stops the CAN peripheral; configuration is unchanged, but interrupts and message handling are disabled.
* Send: Prepares a message to be sent when the CAN peripheral is ready (puts it into a queue).
* Recv: Reads a message from the queue depending on ID.




How to Use This Driver:



* Init to initialize the CAN peripheral.
* Start to activate the CAN peripheral.
* Use Send, Recv, Stop, and Start as needed.
* DeInit to deinitialize the CAN peripheral.




Usage Notes:



* CAN2 is usually a slave CAN, sharing transistors with CAN1. Ensure CAN1 is initialized before CAN2.
* CAN2 often shares filter banks with CAN1; carefully consider filter configurations passed to can\_init.
* CAN3 is usually a master CAN; no sharing considerations are needed.
* This driver uses a macro to define which messages a CAN interface should receive. See can1\_recv\_entries.h for examples. Ensure entries align with filter configurations. 




    
## Public Types Documentation




### enum can\_status\_t 

```C++
enum can_status_t {
    CAN_ERR,
    CAN_OK,
    CAN_SENT,
    CAN_RECV,
    CAN_EMPTY
};
```




<hr>
## Public Functions Documentation




### function can\_deinit 

_Deinitializes the CAN peripheral._ 
```C++
can_status_t can_deinit (
    CAN_HandleTypeDef * handle
) 
```



This function deinitializes the HAL CAN driver and disables CAN interrupts.




**Parameters:**


* `handle` Pointer to the CAN handle structure.



**Returns:**

can\_status\_t Returns CAN\_OK on success, CAN\_ERR on failure. 





        

<hr>



### function can\_init 

_Initializes the CAN peripheral._ 
```C++
can_status_t can_init (
    CAN_HandleTypeDef * handle,
    CAN_FilterTypeDef * filter
) 
```



This function initializes the CAN peripheral, sets up send/receive queues, configures the HAL CAN driver, applies the filter configuration, and enables CAN interrupts.




**Parameters:**


* `handle` Pointer to the CAN handle structure. 
* `filter` Pointer to the CAN filter configuration structure.



**Returns:**

can\_status\_t Returns CAN\_OK on success, CAN\_ERR on failure. 





        

<hr>



### function can\_recv 

_Receives a CAN message._ 
```C++
can_status_t can_recv (
    CAN_HandleTypeDef * handle,
    uint16_t id,
    CAN_RxHeaderTypeDef * header,
    uint8_t data,
    TickType_t delay_ticks
) 
```



Reads a message from the receive queue corresponding to the specified ID. Supports blocking or non-blocking behavior depending on delay\_ticks.




**Parameters:**


* `handle` Pointer to the CAN handle structure. 
* `id` CAN identifier of the message to receive. 
* `header` Pointer to a CAN\_RxHeaderTypeDef structure to store the received header. 
* `data` Array to store the received data. 
* `delay_ticks` Maximum delay to wait if queue is empty (FreeRTOS ticks).



**Returns:**

can\_status\_t Returns CAN\_RECV if a message was received, CAN\_EMPTY if the queue was empty, CAN\_ERR on failure or invalid ID. 





        

<hr>



### function can\_send 

_Sends a CAN message._ 
```C++
can_status_t can_send (
    CAN_HandleTypeDef * handle,
    const CAN_TxHeaderTypeDef * header,
    const uint8_t data,
    TickType_t delay_ticks
) 
```



Places a CAN message into the transmit mailbox if available, otherwise queues it in the send queue for later transmission.




**Parameters:**


* `handle` Pointer to the CAN handle structure. 
* `header` Pointer to the CAN transmit header structure. 
* `data` Array containing the data to send. 
* `delay_ticks` Maximum delay to wait if queue is full (FreeRTOS ticks).



**Returns:**

can\_status\_t Returns CAN\_SENT if message was successfully sent or queued, CAN\_ERR on failure. 





        

<hr>



### function can\_start 

_Starts the CAN peripheral._ 
```C++
can_status_t can_start (
    CAN_HandleTypeDef * handle
) 
```



Activates the CAN peripheral so it can transmit and receive messages.




**Parameters:**


* `handle` Pointer to the CAN handle structure.



**Returns:**

can\_status\_t Returns CAN\_OK on success, CAN\_ERR on failure. 





        

<hr>



### function can\_stop 

_Stops the CAN peripheral._ 
```C++
can_status_t can_stop (
    CAN_HandleTypeDef * handle
) 
```



Stops the CAN peripheral; the configuration is preserved, but the peripheral will not process messages or trigger interrupts.




**Parameters:**


* `handle` Pointer to the CAN handle structure.



**Returns:**

can\_status\_t Returns CAN\_OK on success, CAN\_ERR on failure. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `bsp/Inc/CAN.h`

