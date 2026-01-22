

# Struct EMC2305\_I2C\_Message



[**ClassList**](annotated.md) **>** [**EMC2305\_I2C\_Message**](structEMC2305__I2C__Message.md)





* `#include <EMC2305.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**EMC2305\_HandleTypeDef**](structEMC2305__HandleTypeDef.md) \* | [**chip**](#variable-chip)  <br> |
|  [**EMC2305\_I2C\_OP**](EMC2305_8h.md#enum-emc2305_i2c_op) | [**operation**](#variable-operation)  <br> |
|  uint8\_t \* | [**read\_data**](#variable-read_data)  <br> |
|  uint8\_t | [**reg\_addr**](#variable-reg_addr)  <br> |
|  uint8\_t | [**semaphore\_index**](#variable-semaphore_index)  <br> |
|  uint8\_t | [**write\_data**](#variable-write_data)  <br> |












































## Public Attributes Documentation




### variable chip 

```C++
EMC2305_HandleTypeDef* EMC2305_I2C_Message::chip;
```




<hr>



### variable operation 

```C++
EMC2305_I2C_OP EMC2305_I2C_Message::operation;
```




<hr>



### variable read\_data 

```C++
uint8_t* EMC2305_I2C_Message::read_data;
```




<hr>



### variable reg\_addr 

```C++
uint8_t EMC2305_I2C_Message::reg_addr;
```




<hr>



### variable semaphore\_index 

```C++
uint8_t EMC2305_I2C_Message::semaphore_index;
```




<hr>



### variable write\_data 

```C++
uint8_t EMC2305_I2C_Message::write_data;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `driver/Inc/EMC2305.h`

