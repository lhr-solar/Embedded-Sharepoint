#include "slcanFormat.h"


static inline char hex_char(uint8_t v)
{
    v &= 0xF;
    return (v < 10) ? ('0' + v) : ('A' + (v - 10));
}


int can_to_slcan(uint16_t id,
                 const uint8_t *data,
                 uint8_t dlc,
                 char *out,
                 uint8_t out_size)
{
    if (dlc > 8) return -1;
    if (out == NULL) return -1;
    if ((dlc > 0U) && (data == NULL)) return -1;

    uint8_t needed = 1 + 3 + 1 + (dlc * 2) + 1;
    if (out_size < needed) return -1;

    uint8_t idx = 0;

    /* frame type */
    out[idx++] = 't';

    /* 11-bit ID -> 3 hex chars */
    out[idx++] = hex_char((id >> 8) & 0xF);
    out[idx++] = hex_char((id >> 4) & 0xF);
    out[idx++] = hex_char(id & 0xF);

    /* DLC */
    out[idx++] = hex_char(dlc);

    /* data bytes */
    for (uint8_t i = 0; i < dlc; i++) {
        uint8_t b = data[i];
        out[idx++] = hex_char(b >> 4);
        out[idx++] = hex_char(b);
    }

    /* terminator */
    out[idx++] = '\r';

    return (int)idx;
}