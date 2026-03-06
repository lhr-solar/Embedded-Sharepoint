

# File CAN\_Common.h



[**FileList**](files.md) **>** [**bsp**](dir_3c5459f7c179b79c90e2565474bb2856.md) **>** [**Inc**](dir_f9758a777e5683291000913d5e959461.md) **>** [**CAN\_Common.h**](CAN__Common_8h.md)

[Go to the source code of this file](CAN__Common_8h_source.md)



* `#include "stm32xx_hal.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**can\_recv\_entry\_t**](structcan__recv__entry__t.md) <br> |
| struct | [**can\_rx\_payload\_t**](structcan__rx__payload__t.md) <br> |
| struct | [**can\_tx\_payload\_t**](structcan__tx__payload__t.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**can\_status\_t**](#enum-can_status_t)  <br> |















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**MAX\_CAN\_DATA\_SIZE**](CAN__Common_8h.md#define-max_can_data_size)  `(8)`<br> |

## Public Types Documentation




### enum can\_status\_t 

```C++
enum can_status_t {
    CAN_ERR,
    CAN_OK,
    CAN_EMPTY
};
```




<hr>
## Macro Definition Documentation





### define MAX\_CAN\_DATA\_SIZE 

```C++
#define MAX_CAN_DATA_SIZE `(8)`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `bsp/Inc/CAN_Common.h`

