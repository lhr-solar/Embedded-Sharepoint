#pragma once

#include <stdint.h>

#define DEFINE_MOVING_AVG(NAME, TYPE, SUM_TYPE, MOVING_AVERAGE_WINDOW_SIZE) \
_Static_assert(MOVING_AVERAGE_WINDOW_SIZE > 0 && MOVING_AVERAGE_WINDOW_SIZE < UINT16_MAX, "MOVING_AVERAGE_WINDOW_SIZE must be between 1 and 65534"); \
typedef struct {                                            \
    TYPE buffer[MOVING_AVERAGE_WINDOW_SIZE];                \
    SUM_TYPE sum;                                           \
    uint8_t index;                                          \
    uint8_t count;                                          \
} NAME;                                                     \
                                                            \
static inline void NAME##_init(NAME *ma) {                  \
    ma->sum = 0;                                            \
    ma->index = 0;                                          \
    ma->count = 0;                                          \
    for (uint16_t i = 0; i < MOVING_AVERAGE_WINDOW_SIZE; i++) { \
        ma->buffer[i] = 0;                                  \
    }                                                       \
}                                                           \
                                                            \
static inline TYPE NAME##_update(NAME *ma, TYPE sample) {   \
    ma->sum -= ma->buffer[ma->index];                       \
    ma->buffer[ma->index] = sample;                         \
    ma->sum += sample;                                      \
    ma->index = (ma->index + 1) % (MOVING_AVERAGE_WINDOW_SIZE); \
    if (ma->count < MOVING_AVERAGE_WINDOW_SIZE) {           \
        ma->count++;                                        \
    }                                                       \
                                                            \
    if(ma->count == 0){                                     \
        return 0;                                           \
    }                                                       \
    return (TYPE)(ma->sum / ma->count);                     \
}
