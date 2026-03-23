#pragma once

#include <stdint.h>

#ifndef MOVING_AVERAGE_WINDOW_SIZE
#define MOVING_AVERAGE_WINDOW_SIZE
#endif /*MOVING_AVERAGE_WINDOW_SIZE*/


#define DEFINE_MOVING_AVG(NAME, TYPE, SUM_TYPE)             \
typedef struct {                                            \
    TYPE buffer[WINDOW_SIZE];                               \
    SUM_TYPE sum;                                           \
    uint8_t index;                                          \
    uint8_t count;                                          \
} NAME;                                                     \
                                                            \
void NAME##_init(NAME *ma) {                                \
    ma->sum = 0;                                            \
    ma->index = 0;                                          \
    ma->count = 0;                                          \
    for (int i = 0; i < WINDOW_SIZE; i++) {                 \
        ma->buffer[i] = 0;                                  \
    }                                                       \
}                                                           \
                                                            \
TYPE NAME##_update(NAME *ma, TYPE sample) {                 \
    ma->sum -= ma->buffer[ma->index];                       \
    ma->buffer[ma->index] = sample;                         \
    ma->sum += sample;                                      \
    ma->index = (ma->index + 1) % WINDOW_SIZE;              \
                                                            \
    if (ma->count < WINDOW_SIZE) {                          \
        ma->count++;                                        \
    }                                                       \
                                                            \
    return (TYPE)(ma->sum / ma->count);                     \
}