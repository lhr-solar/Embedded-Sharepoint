#pragma once

#include <stdint.h>

#define DEFINE_MOVING_AVG(NAME, TYPE, SUM_TYPE, MOVING_AVERAGE_WINDOW_SIZE)             \
typedef struct {                                            \
    TYPE buffer[MOVING_AVERAGE_WINDOW_SIZE];                \
    SUM_TYPE sum;                                           \
    uint8_t index;                                          \
    uint8_t count;                                          \
    uint32_t WINDOWS_SIZE;                                  \
} NAME;                                                     \
                                                            \
static inline void NAME##_init(NAME *ma) {                  \
    ma->sum = 0;                                            \
    ma->index = 0;                                          \
    ma->count = 0;                                          \
    ma->WINDOWS_SIZE = MOVING_AVERAGE_WINDOW_SIZE;          \
    for (uint8_t i = 0; i < ma->WINDOWS_SIZE; i++) {        \
        ma->buffer[i] = 0;                                  \
    }                                                       \
}                                                           \
                                                            \
static inline TYPE NAME##_update(NAME *ma, TYPE sample) {   \
    ma->sum -= ma->buffer[ma->index];                       \
    ma->buffer[ma->index] = sample;                         \
    ma->sum += sample;                                      \
    ma->index = (ma->index + 1) % (ma->WINDOWS_SIZE);       \
    if (ma->count < (ma->WINDOWS_SIZE)) {                   \
        ma->count++;                                        \
    }                                                       \
                                                            \
    if(ma->count == 0){                                     \
        return 0;                                           \
    }                                                       \
    return (TYPE)(ma->sum / ma->count);                     \
}
