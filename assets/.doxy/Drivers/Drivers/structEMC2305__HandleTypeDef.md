

# Struct EMC2305\_HandleTypeDef



[**ClassList**](annotated.md) **>** [**EMC2305\_HandleTypeDef**](structEMC2305__HandleTypeDef.md)





* `#include <EMC2305.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  uint16\_t | [**dev\_addr**](#variable-dev_addr)  <br> |
|  I2C\_HandleTypeDef \* | [**hi2c**](#variable-hi2c)  <br> |
|  SemaphoreHandle\_t | [**i2c\_complete**](#variable-i2c_complete)  <br> |












































## Public Attributes Documentation




### variable dev\_addr 

```C++
uint16_t EMC2305_HandleTypeDef::dev_addr;
```




<hr>



### variable hi2c 

```C++
I2C_HandleTypeDef* EMC2305_HandleTypeDef::hi2c;
```




<hr>



### variable i2c\_complete 

```C++
SemaphoreHandle_t EMC2305_HandleTypeDef::i2c_complete;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `driver/Inc/EMC2305.h`

