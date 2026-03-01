#pragma once

#include <stdint.h>

typedef enum
{
    EMA_U16,
    EMA_I16,
    EMA_U32,
    EMA_I32
} ema_type_t;

typedef struct
{
    ema_type_t type;
    uint16_t k;
    uint8_t shift;

    union
    {
        uint16_t u16;
        int16_t  i16;
        uint32_t u32;
        int32_t  i32;
    } y;

} ema_filter_t;

void ema_init(ema_filter_t *f, ema_type_t type, uint16_t k, uint8_t shift, int32_t initial);
void ema_update(ema_filter_t *f, int32_t input);

