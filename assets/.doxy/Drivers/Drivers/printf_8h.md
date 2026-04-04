

# File printf.h



[**FileList**](files.md) **>** [**driver**](dir_1284b95147afa48b330f502c8bbc0529.md) **>** [**Inc**](dir_63ffe06a04b97e03324a2ad2f10a5002.md) **>** [**printf.h**](printf_8h.md)

[Go to the source code of this file](printf_8h_source.md)



* `#include <stdbool.h>`
* `#include "UART.h"`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  int | [**printf**](#function-printf) (const char \* fmt, ...) <br> |
|  bool | [**printf\_init**](#function-printf_init) (UART\_HandleTypeDef \* huart) <br> |
|  int | [**snprintf**](#function-snprintf) (char \* buffer, size\_t bufsz, char const \* fmt, ...) <br> |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**NANOPRINTF\_USE\_BINARY\_FORMAT\_SPECIFIERS**](printf_8h.md#define-nanoprintf_use_binary_format_specifiers)  `1`<br> |
| define  | [**NANOPRINTF\_USE\_FIELD\_WIDTH\_FORMAT\_SPECIFIERS**](printf_8h.md#define-nanoprintf_use_field_width_format_specifiers)  `1`<br> |
| define  | [**NANOPRINTF\_USE\_FLOAT\_FORMAT\_SPECIFIERS**](printf_8h.md#define-nanoprintf_use_float_format_specifiers)  `1`<br> |
| define  | [**NANOPRINTF\_USE\_LARGE\_FORMAT\_SPECIFIERS**](printf_8h.md#define-nanoprintf_use_large_format_specifiers)  `1`<br> |
| define  | [**NANOPRINTF\_USE\_PRECISION\_FORMAT\_SPECIFIERS**](printf_8h.md#define-nanoprintf_use_precision_format_specifiers)  `1`<br> |
| define  | [**NANOPRINTF\_USE\_SMALL\_FORMAT\_SPECIFIERS**](printf_8h.md#define-nanoprintf_use_small_format_specifiers)  `1`<br> |
| define  | [**NANOPRINTF\_USE\_WRITEBACK\_FORMAT\_SPECIFIERS**](printf_8h.md#define-nanoprintf_use_writeback_format_specifiers)  `0`<br> |

## Public Functions Documentation




### function printf 

```C++
int printf (
    const char * fmt,
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



### function snprintf 

```C++
int snprintf (
    char * buffer,
    size_t bufsz,
    char const * fmt,
    ...
) 
```




<hr>
## Macro Definition Documentation





### define NANOPRINTF\_USE\_BINARY\_FORMAT\_SPECIFIERS 

```C++
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS `1`
```




<hr>



### define NANOPRINTF\_USE\_FIELD\_WIDTH\_FORMAT\_SPECIFIERS 

```C++
#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS `1`
```




<hr>



### define NANOPRINTF\_USE\_FLOAT\_FORMAT\_SPECIFIERS 

```C++
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS `1`
```




<hr>



### define NANOPRINTF\_USE\_LARGE\_FORMAT\_SPECIFIERS 

```C++
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS `1`
```




<hr>



### define NANOPRINTF\_USE\_PRECISION\_FORMAT\_SPECIFIERS 

```C++
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS `1`
```




<hr>



### define NANOPRINTF\_USE\_SMALL\_FORMAT\_SPECIFIERS 

```C++
#define NANOPRINTF_USE_SMALL_FORMAT_SPECIFIERS `1`
```




<hr>



### define NANOPRINTF\_USE\_WRITEBACK\_FORMAT\_SPECIFIERS 

```C++
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS `0`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `driver/Inc/printf.h`

