#pragma once

#include <stdint.h>

typedef struct
{
    int32_t y;       // Filter state (scaled)
    uint16_t k;      // Alpha numerator
    uint8_t shift;   // Alpha denominator = 2^shift
} ema_filter_t;

void ema_init(ema_filter_t *filt, uint16_t k, uint8_t shift, int32_t initial_value);

int32_t ema_update(ema_filter_t *filt, int32_t input);