# MDC — Multi CAN Database

MDC is the **source of truth** for vehicle CAN databases in this repo. DBC files remain as an import/export interchange format.

## Toolchain

| Tool | Purpose |
|------|---------|
| `can/mdc/tools/dbc2mdc.py` | Import `.dbc` → MDC v3 JSON (cantools-backed) |
| `can/mdc/tools/mdc2dbc.py` | Export MDC → `.dbc` (best-effort; MDC-only extras dropped with warnings) |
| `can/mdc/tools/generate_can_headers.mjs` | MDC → C header (IDs, DLC macros, enums, structs) |
| `can/mdc/tools/mdc-validate.mjs` | JSON Schema + semantic lint |
| `can/mdc/tools/mdc-busload.mjs` | Per-network bus-load estimate |

## Layout

- **Vehicle docs:** `can/vehicles/{HighNoon,Daybreak}/*.mdc.json` (one document per vehicle, flat root with `networks[]`)
- **DBC sources:** `can/vehicles/{HighNoon,Daybreak}/*.dbc` (import only; kept for round-trip and legacy editors)
- **Schema:** `can/mdc/schema/mdc.schema.bundle.json`

## Typical workflow

```sh
# Import a vehicle folder of DBCs
python3 can/mdc/tools/dbc2mdc.py can/vehicles/HighNoon/ --vehicle-id HighNoon \
  -o can/vehicles/HighNoon/HighNoon.mdc.json

# Validate
node can/mdc/tools/mdc-validate.mjs --schema-only \
  can/mdc/schema/mdc.schema.bundle.json can/vehicles/HighNoon/HighNoon.mdc.json

# Generate embedded C headers
node can/mdc/tools/generate_can_headers.mjs can/vehicles/HighNoon/HighNoon.mdc.json \
  -o can/vehicles/HighNoon/HighNoon_can_msgs.h

# Round-trip smoke (optional)
python3 can/mdc/tools/mdc2dbc.py can/vehicles/HighNoon/HighNoon.mdc.json -o /tmp/out.dbc
```

## CI

`.github/workflows/check-dbc.yml` validates MDC duplicate frame IDs and C header generation for `can/vehicles/HighNoon/`.

## DBC reorder helper

`can/vehicles/reorder_dbc.py` still operates on `.dbc` files (e.g. after CANdb++ export) for stable message/signal ordering before import.
