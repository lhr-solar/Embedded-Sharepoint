#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "bl_crc32.h"

int main(void)
{
    static const uint8_t ref[] = "123456789";
    uint32_t crc = bl_crc32_buf(ref, sizeof(ref) - 1U);

    if (crc != 0x0376E6E7U) {
        printf("FAIL: crc=0x%08X expected 0x0376E6E7\n", crc);
        return 1;
    }
    if (crc == 0xCBF43926U) {
        printf("FAIL: got zlib CRC variant\n");
        return 1;
    }

    printf("PASS: CRC-32/MPEG-2 = 0x%08X\n", crc);
    return 0;
}
