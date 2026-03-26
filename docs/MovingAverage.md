# Moving Average Filter Documentation

## Overview
This header-only library provides a macro-based generator for static moving average filters.

## Filter Theory
The filter maintains a circular buffer of the last N samples (where `N` is defined by `MOVING_AVERAGE_WINDOW_SIZE`).

To maintain O(1) time complexity, the implementation tracks a running sum. When a new sample arrives, the oldest sample is subtracted from the sum, the new one is added, and the average is returned.

## Macro Name Generation
The `DEFINE_MOVING_AVG` macro uses C preprocessor token pasting (the ## operator) to dynamically create unique struct names and function identifiers. This allows you to define multiple independent filters (e.g., one for Voltage, one for Current) without naming collisions.

### Internal Logic
The header uses the ## operator to concatenate the user-provided name with specific suffixes. For example, the update function is defined internally as:
```c
static inline TYPE NAME##_update(NAME *ma, TYPE sample) { ... }
```

When the compiler processes DEFINE_MOVING_AVG(CellVoltage, uint16_t, uint32_t, 20), it generates a unique function signature:
```c
static inline uint16_t CellVoltage_update(CellVoltage *ma, uint16_t sample)
```
### Generation Format
When you call the macro as `DEFINE_MOVING_AVG(NAME, TYPE, SUM_TYPE)`, the preprocessor maps your NAME argument to the following identifiers:

| Generated Component | Format | Example (if NAME is Battery) |
| -------- | -------- | -------- |
| Struct Type | NAME | Battery |
| Initialization | NAME##_init | Battery_init |
| Update/Process | NAME##_update | Battery_update |


## Usage Guide

### 1. Define the Filter

Use the `DEFINE_MOVING_AVG` macro to generate a custom struct and its associated functions.
```c
// DEFINE_MOVING_AVG(NAME, TYPE, SUM_TYPE)
DEFINE_MOVING_AVG(VoltageFilter, uint16_t, uint32_t, 20);
```

* NAME: The prefix for the generated struct and functions.
* TYPE: The data type of the input samples (e.g., int16_t).
* SUM_TYPE: A larger type used for the internal accumulator to prevent overflow. For uint16_t samples and a window of 8, a uint32_t is recommended.
* MOVING_AVERAGE_WINDOW_SIZE: The size of that moving average window

### 2. Initialization
Initialize the filter state (clears the buffer and resets the sum) before use.
```c
VoltageFilter myFilter;
VoltageFilter_init(&myFilter);
```

### 3. Updating and Filtering
Pass the handle and the new sample to the `_update` function. It returns the newly calculated average.
```c
uint16_t raw_adc = HAL_ADC_GetValue(&hadc1);
uint16_t filtered_adc = VoltageFilter_update(&myFilter, raw_adc);
```

## Common Pitfalls
### Overflow Prevention
Always ensure `SUM_TYPE` is large enough to hold `TYPE_MAX * MOVING_AVERAGE_WINDOW_SIZE`. For example, if your window is 256 and your type is uint16_t, the sum could exceed 16 million, which fits in a uint32_t but not a uint16_t.

## Examples of moving average filter usage
* [Moving Average Filter Test](https://github.com/lhr-solar/Embedded-Sharepoint/blob/main/test/tests/movingAverage_test.c)
