# MDC examples

Realistic projects that validate against [`../schema/mdc.schema.bundle.json`](../schema/mdc.schema.bundle.json).

## `lhr-ev1/`

A two-network demo EV:

- **`powertrain`** — CAN FD (500 kbit/s arb, 2 Mbit/s data). Battery + inverter, value table, computed `DrivePower`, alarms, every conversion kind including `rational`.
- **`chassis`** — classic CAN (500 kbit/s). Dynamics plus a multiplexed/ISO-TP `Diagnostics` message.

It covers typed `attributeDefinitions`, shared + network-local `valueTables`, display hints, fault/alarm rules, computed signals, and multiplexing.

## Canonical folder layout

A project is a **folder**. The single source of truth is `project.mdc.json`. A project MAY split each network into its own file for readability and ownership:

```
lhr-ev1/
  project.mdc.json        # required — the full, validatable document
  powertrain.mdc.json     # optional — a single network fragment
  chassis.mdc.json        # optional — a single network fragment
```

When per-network files are present, the loader merges each one into the matching
`vehicles[].networks[]` entry, keyed by `network.id` (and `network.filename`
records the origin for round-tripping). The **merged** document is what the
schema validates and what the engine/app/tools consume.

`lhr-ev1/` here keeps both networks inline in `project.mdc.json` so the example
is a single self-contained, directly-validatable file. The split files above are
the optional layout the loader also accepts.

## Validate

From the repo root (requires `npm install` in `tools/` once):

```bash
# Regenerate bundle after editing mdc/schema/defs/*.json
node tools/bundle-mdc-schema.mjs

# Validate examples against the bundled schema
node tools/validate-mdc.mjs mdc/schema/mdc.schema.bundle.json mdc/examples/lhr-ev1/project.mdc.json
```

The modular root (`mdc/schema/mdc.schema.json` + `defs/`) is for authoring;
`mdc.schema.bundle.json` is what consumers and CI should use.
