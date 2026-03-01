#include "fixed_ema.h"

void ema_init(ema_filter_t *filt, uint16_t k, uint8_t shift, int32_t initial_value)
{
    filt->k = k;
    filt->shift = shift;
    filt->y = initial_value;
}

int32_t ema_update(ema_filter_t *filt, int32_t input)
{
    int32_t error = input - filt->y;
    filt->y += (error * filt->k) >> filt->shift;
    return filt->y;
}