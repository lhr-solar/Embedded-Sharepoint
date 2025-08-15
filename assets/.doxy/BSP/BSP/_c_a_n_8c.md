

# File CAN.c



[**FileList**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Src**](dir_160128b01eb7e6b2a554f83ab5d45f5a.md) **>** [**CAN.c**](_c_a_n_8c.md)

[Go to the source code of this file](_c_a_n_8c_source.md)



* `#include "CAN.h"`
* `#include "queue_ex.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**recv\_entry\_t**](structrecv__entry__t.md) <br> |
| struct | [**rx\_payload\_t**](structrx__payload__t.md) <br> |
| struct | [**tx\_payload\_t**](structtx__payload__t.md) <br> |






















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**HAL\_CAN\_MspDeInit**](#function-hal_can_mspdeinit) (CAN\_HandleTypeDef \* hcan) <br> |
|  void | [**HAL\_CAN\_MspInit**](#function-hal_can_mspinit) (CAN\_HandleTypeDef \* hcan) <br> |
|  void | [**HAL\_CAN\_RxFifo0MsgPendingCallback**](#function-hal_can_rxfifo0msgpendingcallback) (CAN\_HandleTypeDef \* hcan) <br> |
|  void | [**HAL\_CAN\_TxMailbox0CompleteCallback**](#function-hal_can_txmailbox0completecallback) (CAN\_HandleTypeDef \* hcan) <br> |
|  void | [**HAL\_CAN\_TxMailbox1CompleteCallback**](#function-hal_can_txmailbox1completecallback) (CAN\_HandleTypeDef \* hcan) <br> |
|  void | [**HAL\_CAN\_TxMailbox2CompleteCallback**](#function-hal_can_txmailbox2completecallback) (CAN\_HandleTypeDef \* hcan) <br> |
|  can\_status\_t | [**can\_deinit**](#function-can_deinit) (CAN\_HandleTypeDef \* handle) <br> |
|  can\_status\_t | [**can\_init**](#function-can_init) (CAN\_HandleTypeDef \* handle, CAN\_FilterTypeDef \* filter) <br> |
|  can\_status\_t | [**can\_recv**](#function-can_recv) (CAN\_HandleTypeDef \* handle, uint16\_t id, CAN\_RxHeaderTypeDef \* header, uint8\_t data, TickType\_t delay\_ticks) <br> |
|  can\_status\_t | [**can\_send**](#function-can_send) (CAN\_HandleTypeDef \* handle, const CAN\_TxHeaderTypeDef \* header, const uint8\_t data, TickType\_t delay\_ticks) <br> |
|  can\_status\_t | [**can\_start**](#function-can_start) (CAN\_HandleTypeDef \* handle) <br> |
|  can\_status\_t | [**can\_stop**](#function-can_stop) (CAN\_HandleTypeDef \* handle) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**HAL\_CAN\_MspF4Init**](#function-hal_can_mspf4init) (CAN\_HandleTypeDef \* hcan) <br> |
|  void | [**HAL\_CAN\_MspL4Init**](#function-hal_can_mspl4init) (CAN\_HandleTypeDef \* hcan) <br> |
|  void | [**transmit**](#function-transmit) (CAN\_HandleTypeDef \* handle) <br> |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**DATA\_SIZE**](_c_a_n_8c.md#define-data_size)  `(8)`<br> |

## Public Functions Documentation




### function HAL\_CAN\_MspDeInit 

```C++
void HAL_CAN_MspDeInit (
    CAN_HandleTypeDef * hcan
) 
```




<hr>



### function HAL\_CAN\_MspInit 

```C++
void HAL_CAN_MspInit (
    CAN_HandleTypeDef * hcan
) 
```




<hr>



### function HAL\_CAN\_RxFifo0MsgPendingCallback 

```C++
void HAL_CAN_RxFifo0MsgPendingCallback (
    CAN_HandleTypeDef * hcan
) 
```




<hr>



### function HAL\_CAN\_TxMailbox0CompleteCallback 

```C++
void HAL_CAN_TxMailbox0CompleteCallback (
    CAN_HandleTypeDef * hcan
) 
```




<hr>



### function HAL\_CAN\_TxMailbox1CompleteCallback 

```C++
void HAL_CAN_TxMailbox1CompleteCallback (
    CAN_HandleTypeDef * hcan
) 
```




<hr>



### function HAL\_CAN\_TxMailbox2CompleteCallback 

```C++
void HAL_CAN_TxMailbox2CompleteCallback (
    CAN_HandleTypeDef * hcan
) 
```




<hr>



### function can\_deinit 

```C++
can_status_t can_deinit (
    CAN_HandleTypeDef * handle
) 
```




<hr>



### function can\_init 

```C++
can_status_t can_init (
    CAN_HandleTypeDef * handle,
    CAN_FilterTypeDef * filter
) 
```




<hr>



### function can\_recv 

```C++
can_status_t can_recv (
    CAN_HandleTypeDef * handle,
    uint16_t id,
    CAN_RxHeaderTypeDef * header,
    uint8_t data,
    TickType_t delay_ticks
) 
```




<hr>



### function can\_send 

```C++
can_status_t can_send (
    CAN_HandleTypeDef * handle,
    const CAN_TxHeaderTypeDef * header,
    const uint8_t data,
    TickType_t delay_ticks
) 
```




<hr>



### function can\_start 

```C++
can_status_t can_start (
    CAN_HandleTypeDef * handle
) 
```




<hr>



### function can\_stop 

```C++
can_status_t can_stop (
    CAN_HandleTypeDef * handle
) 
```




<hr>
## Public Static Functions Documentation




### function HAL\_CAN\_MspF4Init 

```C++
static inline void HAL_CAN_MspF4Init (
    CAN_HandleTypeDef * hcan
) 
```




<hr>



### function HAL\_CAN\_MspL4Init 

```C++
static inline void HAL_CAN_MspL4Init (
    CAN_HandleTypeDef * hcan
) 
```




<hr>



### function transmit 

```C++
static void transmit (
    CAN_HandleTypeDef * handle
) 
```




<hr>
## Macro Definition Documentation





### define DATA\_SIZE 

```C++
#define DATA_SIZE `(8)`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `bsp/Src/CAN.c`

