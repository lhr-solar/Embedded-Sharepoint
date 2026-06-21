#include "bl_crc32.h"

#define BL_CRC32_POLY 0x04C11DB7U

uint32_t bl_crc32_buf(const uint8_t *data, uint32_t len)
{
    uint32_t crc = 0xFFFFFFFFU;

    for (uint32_t i = 0; i < len; i++) {
        crc ^= ((uint32_t)data[i] << 24);
        for (int bit = 0; bit < 8; bit++) {
            if ((crc & 0x80000000U) != 0U) {
                crc = (crc << 1) ^ BL_CRC32_POLY;
            } else {
                crc <<= 1;
            }
            crc &= 0xFFFFFFFFU;
        }
    }

    return crc;
}
