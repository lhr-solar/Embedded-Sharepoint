

# File slcanFormat.h

[**File List**](files.md) **>** [**driver**](dir_1284b95147afa48b330f502c8bbc0529.md) **>** [**Inc**](dir_63ffe06a04b97e03324a2ad2f10a5002.md) **>** [**slcanFormat.h**](slcanFormat_8h.md)

[Go to the documentation of this file](slcanFormat_8h.md)


```C++
#pragma once

#include <stdint.h>
#include <stddef.h>

/*
 * Convert CAN frame to SLCAN string.
 *
 * id        : 11-bit CAN ID
 * data      : pointer to payload
 * dlc       : payload length (0-8)
 * out       : output buffer
 * out_size  : size of output buffer
 *
 * returns number of bytes written, or -1 on error
 */

int can_to_slcan(uint16_t id, const uint8_t *data, uint8_t dlc, char *out, uint8_t out_size);
```


