

# File slcanFormat.h



[**FileList**](files.md) **>** [**driver**](dir_1284b95147afa48b330f502c8bbc0529.md) **>** [**Inc**](dir_63ffe06a04b97e03324a2ad2f10a5002.md) **>** [**slcanFormat.h**](slcanFormat_8h.md)

[Go to the source code of this file](slcanFormat_8h_source.md)



* `#include <stdint.h>`
* `#include <stddef.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  int | [**can\_to\_slcan**](#function-can_to_slcan) (uint16\_t id, const uint8\_t \* data, uint8\_t dlc, char \* out, uint8\_t out\_size) <br>_Convert CAN frame to SLCAN string._  |




























## Public Functions Documentation




### function can\_to\_slcan 

_Convert CAN frame to SLCAN string._ 
```C++
int can_to_slcan (
    uint16_t id,
    const uint8_t * data,
    uint8_t dlc,
    char * out,
    uint8_t out_size
) 
```



Activates the FDCAN peripheral so it can transmit and receive messages.




**Parameters:**


* `id` 11-bit CAN ID 
* `data` pointer to payload 
* `dlc` payload length (0-8) 
* `out` output buffer 
* `out_size` size of output buffer



**Returns:**

int Returns number of bytes written, or -1 on error 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `driver/Inc/slcanFormat.h`

