# Integrating CAN messages into your code

## Generating CAN message headers

Use the MDC-driven header generator at `can/mdc/tools/generate_can_headers.mjs`. It reads a vehicle `*.mdc.json` and emits CAN ID macros, DLC macros, value-table enums, and per-message structs.

```sh
node can/mdc/tools/generate_can_headers.mjs can/vehicles/HighNoon/HighNoon.mdc.json \
  -o can/vehicles/HighNoon/HighNoon_can_msgs.h
```

If you only have DBC files, import them first:

```sh
python3 can/mdc/tools/dbc2mdc.py can/vehicles/HighNoon/ --vehicle-id HighNoon \
  -o can/vehicles/HighNoon/HighNoon.mdc.json
```

## What's included in the header

CAN ID macros:

```c
#define CAN_ID_VCU_STATUS 0x10
```

DLC macros:

```c
#define CAN_DLC_VCU_STATUS 3
```

Value-table enums (from MDC `choices` / value tables):

```c
typedef enum {
    VCU_STATUS_VCU_FAULT_NO_FAULT = 0,
    ...
} vcu_status_vcu_fault_e;
```

Per-message structs:

```c
typedef struct {
    uint8_t VCU_Fault;
    ...
} vcu_status_t;
```

## Packing / unpacking

Write small pack/unpack helpers that memcpy or bit-pack struct fields into the CAN payload byte array — same pattern as before, but types come from the MDC-generated header.

## DBC interchange

DBC is supported as an **import path** (`dbc2mdc.py`) and **export path** (`mdc2dbc.py`). MDC holds extensions (computed signals, alarms, display hints, rational conversion) that DBC cannot express; `mdc2dbc` logs warnings when those are dropped.
