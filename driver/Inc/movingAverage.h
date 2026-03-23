#pragma once

#include <stdint.h>

#ifndef MOVING_AVERAGE_WINDOW_SIZE
#define MOVING_AVERAGE_WINDOW_SIZE 8
#endif

#define DEFINE_MOVING_AVG(NAME, TYPE, SUM_TYPE)             \
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
    for (uint8_t i = 0; i < MOVING_AVERAGE_WINDOW_SIZE; i++) { \
        ma->buffer[i] = 0;                                  \
    }                                                       \
}                                                           \
                                                            \
static inline TYPE NAME##_update(NAME *ma, TYPE sample) {   \
    ma->sum -= ma->buffer[ma->index];                       \
    ma->buffer[ma->index] = sample;                         \
    ma->sum += sample;                                      \
    ma->index = (ma->index + 1) % MOVING_AVERAGE_WINDOW_SIZE; \
                                                            \
    if (ma->count < MOVING_AVERAGE_WINDOW_SIZE) {           \
        ma->count++;                                        \
    }                                                       \
                                                            \
    if(ma->count == 0){                                     \
        return 0;                                           \
    }                                                       \
    return (TYPE)(ma->sum / ma->count);                     \
}
