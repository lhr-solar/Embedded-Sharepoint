

# File printf.h



[**FileList**](files.md) **>** [**driver**](dir_1284b95147afa48b330f502c8bbc0529.md) **>** [**Inc**](dir_63ffe06a04b97e03324a2ad2f10a5002.md) **>** [**printf.h**](printf_8h.md)

[Go to the source code of this file](printf_8h_source.md)



* `#include <stdbool.h>`
* `#include "UART.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  int | [**printf**](#function-printf) (const char \* format, ...) <br> |
|  bool | [**printf\_init**](#function-printf_init) (UART\_HandleTypeDef \* huart) <br> |
|  int | [**scanf**](#function-scanf) (const char \* format, ...) <br> |




























## Public Functions Documentation




### function printf 

```C++
int printf (
    const char * format,
    ...
) 
```




<hr>



### function printf\_init 

```C++
bool printf_init (
    UART_HandleTypeDef * huart
) 
```




<hr>



### function scanf 

```C++
int scanf (
    const char * format,
    ...
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `driver/Inc/printf.h`

