

# File CAN.h



[**FileList**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Inc**](dir_f9758a777e5683291000913d5e959461.md) **>** [**CAN.h**](_c_a_n_8h.md)

[Go to the source code of this file](_c_a_n_8h_source.md)



* `#include "stm32xx_hal.h"`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**can\_status\_t**](#enum-can_status_t)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  can\_status\_t | [**can\_deinit**](#function-can_deinit) (CAN\_HandleTypeDef \* handle) <br> |
|  can\_status\_t | [**can\_init**](#function-can_init) (CAN\_HandleTypeDef \* handle, CAN\_FilterTypeDef \* filter) <br> |
|  can\_status\_t | [**can\_recv**](#function-can_recv) (CAN\_HandleTypeDef \* handle, uint16\_t id, CAN\_RxHeaderTypeDef \* header, uint8\_t data, TickType\_t delay\_ticks) <br> |
|  can\_status\_t | [**can\_send**](#function-can_send) (CAN\_HandleTypeDef \* handle, const CAN\_TxHeaderTypeDef \* header, const uint8\_t data, TickType\_t delay\_ticks) <br> |
|  can\_status\_t | [**can\_start**](#function-can_start) (CAN\_HandleTypeDef \* handle) <br> |
|  can\_status\_t | [**can\_stop**](#function-can_stop) (CAN\_HandleTypeDef \* handle) <br> |




























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

------------------------------
The documentation for this class was generated from the following file `bsp/Inc/CAN.h`

