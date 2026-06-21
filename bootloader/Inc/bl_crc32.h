#ifndef BL_CRC32_H
#define BL_CRC32_H

#include <stdint.h>

/* CRC-32/MPEG-2: poly 0x04C11DB7, init 0xFFFFFFFF, no reflection, no final XOR */
uint32_t bl_crc32_buf(const uint8_t *data, uint32_t len);

#endif /* BL_CRC32_H */
