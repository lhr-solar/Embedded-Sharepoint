#include "emaFilter.h"

void ema_init(ema_filter_t *f, ema_type_t type, uint16_t k, uint8_t shift, int32_t initial)
{
    f->type = type;
    f->k = k;
    f->shift = shift;

    switch(type)
    {
        case EMA_U16: f->y.u16 = (uint16_t)initial; break;
        case EMA_I16: f->y.i16 = (int16_t)initial;  break;
        case EMA_U32: f->y.u32 = (uint32_t)initial; break;
        case EMA_I32: f->y.i32 = (int32_t)initial;  break;
        default:
            // For safety, initialize to 0 for unknown types.
            // An assert could also be appropriate here.
            f->y.u32 = 0;
            break;
    }
}

void ema_update(ema_filter_t *f, int32_t input)
{
    int64_t error;
    int64_t delta;

    switch(f->type)
    {
        case EMA_U16:
            error = (int64_t)(uint16_t)input - (int64_t)f->y.u16;
            delta = (error * f->k) >> f->shift;
            f->y.u16 += (uint16_t)delta;
            break;

        case EMA_I16:
            error = (int64_t)(int16_t)input - (int64_t)f->y.i16;
            delta = (error * f->k) >> f->shift;
            f->y.i16 += (int16_t)delta;
            break;

        case EMA_U32:
            error = (int64_t)(uint32_t)input - (int64_t)f->y.u32;
            delta = (error * f->k) >> f->shift;
            f->y.u32 += (uint32_t)delta;
            break;

        case EMA_I32:
        default:
            error = (int64_t)(int32_t)input - (int64_t)f->y.i32;
            delta = (error * f->k) >> f->shift;
            f->y.i32 += (int32_t)delta;
            break;
    }
}