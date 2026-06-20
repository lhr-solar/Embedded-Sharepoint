# MDC — Modular Description for CAN

MDC is the canonical JSON contract describing CAN/CAN-FD networks for the LHR
telemetry platform. It is a **superset of DBC**: it carries everything a DBC
carries plus first-class metadata DBC cannot express. It is validated by
[`../schema/mdc.schema.bundle.json`](../schema/mdc.schema.bundle.json) (JSON
Schema draft 2020-12) and consumed by the C++ engine (decode), the app
(`mdc-editor`), and the `tools/` (`dbc2mdc`, `mdc2cheaders`, `mdc2grafana`,
`mdc-validate`).

The schema is a stable **contract**. Changes are additive within a major
version; `schemaVersion` (semver) gates compatibility. A consumer must reject a
document whose major version it does not understand.

## Schema layout (authoring)

The schema is split for editability; semantics are unchanged from the monolithic
contract:

```
mdc/schema/
  mdc.schema.json           # thin root — project-level properties only
  defs/                     # domain-scoped $defs (edit here)
    primitives.json         # semver, identifier, byteOrder, transport, …
    annotations.json        # typed attributes (DBC BA_/BA_DEF_)
    project-meta.json       # metadata, attributeDefinitions
    enumerations.json       # choices, valueTables
    conversion.json         # raw→physical conversion kinds
    presentation.json       # display hints, alarms
    multiplexing.json       # mux roles
    signals.json            # signal, computedSignal
    hierarchy.json          # node, message, network, vehicle
  mdc.schema.bundle.json    # generated single-file artifact (for consumers)
```

**Edit** `mdc.schema.json` and the relevant `defs/*.json` file. **Regenerate**
the bundle before committing or validating in CI:

```bash
node tools/bundle-mdc-schema.mjs
node tools/validate-mdc.mjs mdc/schema/mdc.schema.bundle.json mdc/examples/lhr-ev1/project.mdc.json
```

Downstream consumers (engine, editor, codegen) should load
`mdc.schema.bundle.json`. The modular root is for authoring only.

## Hierarchy

```
Project                 (schemaVersion, metadata,
  │                      attributeDefinitions[], shared valueTables[])
  └─ Vehicle[]          (id, name, attributes)
       └─ Network/Bus[] (id, protocol can|canfd, bitrate, dataBitrate,
            │            samplePoint, nodes[], valueTables[], attributes)
            ├─ Message[] (name, id, isExtended, isFd, length, transport,
            │   │         transmissionType, cycleTimeMs, senders[],
            │   │         multiplexing, array, attributes)
            │   ├─ Signal[]          (bit-packed, decoded from frame bytes)
            │   └─ computedSignal[]  (virtual, derived from this message)
            └─ computedSignal[]      (virtual, cross-message on this network)
```

One document can hold **multiple vehicles and multiple networks**. Identifiers
namespace downward: a signal is addressed `vehicle.network.message.signal`, and
cross-references (e.g. computed-signal `expr`/`dependsOn`) use the
`message.signal` form within a network.

On disk a project is a **folder**: `project.mdc.json` plus optional per-network
`*.mdc.json` files merged into `vehicles[].networks[]` by `network.id`. See
[`../examples/README.md`](../examples/README.md).

## What MDC adds over DBC

All modeled as first-class schema, not free-text:

| Capability | Where | Notes |
|---|---|---|
| Typed attributes | `attributeDefinitions[]` + per-entity `attributes{}` | DBC `BA_DEF_`/`BA_` superset. See [Typed attribute library](#typed-attribute-library). |
| Display / viz hints | `signal.display` | precision, format, chartType, color, dashboardGroup, unitOverride, scale bounds, visible. Advisory — never affects decode. |
| Fault / alarm rules | `signal.alarms[]` | op (lt/le/gt/ge/eq/ne/between/outside/change), threshold(s), severity, message, hysteresis. |
| Computed / virtual signals | `computedSignals[]` (message + network) | `expr` over other signals' physical values; no bit layout. |
| Multi-network / multi-vehicle | one document | with namespaced ids + cross-refs. |

## Timing, transmission type & bus loading

Two message-level fields describe transmit timing; one network field gives the
bus its speed. Together they drive the bus-load budget.

### Transmission type & cycle time

- **`transmissionType`** — `cyclic` (default) or `triggered`.
  - `cyclic` = periodic; the frame goes out every `cycleTimeMs` and **counts
    toward bus load**.
  - `triggered` = event / on-change; sent only when something changes and is
    **excluded from bus-load** (you cannot budget unpredictable traffic).
- **`cycleTimeMs`** — nominal transmit period in milliseconds.
  - Schema default is `1000` for `cyclic` messages (used by the bus-load formula).
  - For `triggered` messages it is optional and ignored by bus-load; use `null`
    for purely event-driven frames.

DBC import (`dbc2mdc`) maps `GenMsgSendType` → `transmissionType`
(`Cyclic`/`CyclicAndEvent`/`CyclicAndEventNoRepetition` → `cyclic`;
`Event`/`Spontaneous`/`None` → `triggered`) and `GenMsgCycleTime` →
`cycleTimeMs`. When the DBC omits both, the **importer** (not the schema)
defaults to `cyclic` with `cycleTimeMs: 1000`.
`VFrameFormat` sets native `isExtended`/`isFd` via cantools and is not stored
in `attributes`.

## Typed attribute library

MDC v2 replaces free-form tags with a **typed attribute system** mirroring DBC
`BA_DEF_` / `BA_` / `BA_DEF_DEF_`.

### Project-level declarations (`attributeDefinitions[]`)

Each entry declares one attribute key and its value type:

```json
{
  "name": "SPN",
  "type": "int",
  "description": "J1939 Suspect Parameter Number.",
  "scopes": ["signal"],
  "min": 0,
  "max": 524287,
  "default": 0
}
```

- **`name`** (required) — key used in per-entity `attributes` maps.
- **`type`** (required) — one of `enum`, `int`, `float`, `string`, `bool`, `hex`.
  `hex` is an integer stored as a JSON number, displayed in hexadecimal (CAN id /
  SPN / mask). `min`/`max` apply like `int`.
- **`scopes`** (optional) — levels where the attribute is allowed:
  `project`, `vehicle`, `network`, `node`, `message`, `signal`. Omit to allow all.
- **`default`**, **`min`/`max`** (optional), **`enumValues`** (for `type: enum`,
  `uniqueItems: true`).

### Per-entity values (`attributes{}`)

On any entity (vehicle, network, node, message, signal):

```json
"attributes": { "BusType": "CAN", "SPN": 1234 }
```

Keys match `attributeDefinitions[].name`; values are scalars (string, number,
boolean).

### Native-field mapping (not stored in `attributes`)

Some DBC attributes set **native MDC fields** the engine decodes from:

| DBC attribute | MDC native field(s) |
|---|---|
| `VFrameFormat` | `message.isExtended`, `message.isFd` |
| `GenMsgSendType` | `message.transmissionType` (`cyclic` \| `triggered`) |
| `GenMsgCycleTime` | `message.cycleTimeMs` |
| `DBName` | skipped (redundant with `network.id`/`name`) |

All other `BA_DEF_` / `BA_` pairs import as `attributeDefinitions[]` +
per-entity `attributes`. `dbc2mdc` deduplicates repeated `enumValues` entries
(e.g. DBC `VFrameFormat` lists `"reserved"` many times) to satisfy
`uniqueItems: true`.

### Network bitrate

`network.bitrate` is an **integer in bits per second** — intentionally *not* an
enum, because odd/non-standard rates exist in the wild. The standard set is:

| Tier | Bitrates (bps) |
|---|---|
| Common | `125000`, `250000`, `500000`, `1000000` |
| Also common | `33333`, `50000`, `800000` |

`mdc-validate` **warns** (does not error) when a `bitrate` is outside this set,
and when a `cyclic` message has `cycleTimeMs <= 0`.

### Bus-load formula

For each network, bus load is the fraction of bus time consumed by **cyclic**
traffic:

```
loadPercent = Σ over cyclic messages of
    (1000 / cycleTimeMs) * bitsPerFrame / bitrate * 100
```

`triggered` messages are excluded. `bitsPerFrame` is the worst-case on-wire bit
count:

- **Classic CAN, 11-bit id:** `47 + 8·DLC` base + stuffing `ceil((34 + 8·DLC)/4)`.
- **Classic CAN, 29-bit id:** `67 + 8·DLC` base + stuffing `ceil((39 + 8·DLC)/4)`.
- **CAN FD:** no bit-stuffing in the data phase; fixed overhead + `8·DLC` + a
  CRC sized by DLC (CRC17 for DLC ≤ 16, CRC21 above) + fixed stuff bits.

**Worst-case caveat:** the classic-CAN model assumes maximum bit-stuffing on
every stuffable field, so reported load is an upper bound (conservative for
headroom budgeting). Real load is lower because stuffing is data-dependent. The
per-frame model lives in one documented function (`bitsPerFrame` in
`tools/mdc-busload.mjs`) with a `// ponytail:` note and an upgrade path to an
average-case estimate.

### `mdc-busload` output (app contract)

`tools/mdc-busload.mjs <project>` prints a per-network table and exports
`computeBusLoad(project)`, returning the structured result the app consumes —
load computed at **all** standard speeds (plus the configured one) for each
network, with the configured speed marked and sorted first:

```jsonc
{
  "networks": [
    {
      "vehicle": "lhr_ev1",
      "network": "powertrain",
      "protocol": "canfd",
      "bitrate": 500000,
      "cyclicMessageCount": 3,
      "speeds": [
        { "speed": 500000,  "loadPercent": 4.96, "isConfigured": true },  // configured first
        { "speed": 33333,   "loadPercent": 74.37, "isConfigured": false }, // then ascending
        { "speed": 50000,   "loadPercent": 49.58, "isConfigured": false }
        // … 125000, 250000, 800000, 1000000
      ]
    }
  ]
}
```

## byteOrder convention

`byteOrder` is one of **`little_endian`** (Intel) or **`big_endian`**
(Motorola / "sawtooth"). These names are chosen to match cantools' output and
the v0 `canspec-ui` exporter, so DBC import is loss-free.

`startBit` follows the **cantools/DBC bit-numbering convention** (the same one
`export_canspec_json.py` emits via `sig.start`):

- **little_endian (Intel):** `startBit` is the LSB of the signal. The signal
  grows toward higher bit numbers. Bit numbering is standard: byte `n` occupies
  bits `8n … 8n+7`, with bit `8n` the LSB of that byte.
- **big_endian (Motorola):** `startBit` is the MSB of the signal in DBC
  sawtooth numbering. The signal grows toward lower bit numbers within the
  cantools start-bit scheme.

`startBit` max is 511 (bit 7 of byte 63) to support 64-byte CAN FD frames.
Importers and the engine share one bit-extraction routine; do not re-derive it
per tool.

## Conversion math

`signal.conversion.kind` selects the raw→physical mapping. `raw` is the integer
(or IEEE-754 float, when `kind` = `float`) extracted from the bits.

- **identity** — `physical = raw`.
- **linear** — `physical = factor * raw + offset` (the DBC factor/offset).
- **rational** — `physical = N(raw) / D(raw) + offset`, where `N` and `D` are
  polynomials given as coefficient arrays **highest power first**. Example:
  `numerator = [1, 0]`, `denominator = [10]` ⇒ `(1·raw + 0) / 10 = raw / 10`.
  `D(raw)` must be non-zero over the signal's range.
- **table** — the raw value is categorical; render it via the signal's
  `choices[]` or `valueTableRef`. The numeric physical value equals `raw`.

`signal.kind` (`unsigned` | `signed` | `float`) governs how the bit-field is
interpreted before conversion: unsigned integer, two's-complement signed
integer, or IEEE-754 float (16/32/64-bit).

### Value tables

`choices[]` is an inline enumeration; `valueTableRef` names a reusable table.
Resolution is **network-local `valueTables` first, then project shared
`valueTables`**. `choices` and `valueTableRef` are mutually exclusive on a
signal.

## Computed (virtual) signals

A `computedSignal` has no bit layout. `expr` is an arithmetic/logical expression
over other signals' **physical** values, referenced by name (or
`message.signal` for cross-message on a network). `dependsOn[]` lists the inputs
for ordering and validation; tools may also infer it from `expr`. The exact
expression grammar is owned by the engine evaluator (the schema only stores the
string).

## Arrays / indexed messages

Some ECUs send **one message id repeatedly**, each frame carrying an *index*
signal plus the per-element data for that index — the classic battery-cell
pattern: a single `BMS_CellArray` frame describes cell *N* via `CellVoltage` /
`CellTemp`, and 96 successive frames reconstruct the full array.

The optional message-level **`array`** object captures the post-decode/storage
semantics that the wire bits alone cannot:

```jsonc
"array": {
  "indexSignal": "CellIndex",                 // signal in this message carrying the element index
  "size": 96,                                  // total elements; null for dynamic/unknown
  "elementSignals": ["CellVoltage", "CellTemp"], // per-element data signals (repeat per index)
  "storage": "series_per_index"               // sink/ingest behavior (see below)
}
```

`indexSignal` and every `elementSignals` entry must name a signal **in the same
message**, and the index signal must not also be an element signal. Index and
element roles are **derived from this block** — there is intentionally no
redundant per-signal `arrayRole` field.

`array` is the **storage semantic layered on top of the wire layout**. An array
message will typically *also* use `multiplexing` (the index is the multiplexor;
element signals are multiplexed on it) to describe how the bits are packed; the
two constructs coexist and describe different layers.

`storage` is a typed hint that drives sink/ingest behavior:

| Value | Behavior | Use when |
|---|---|---|
| `series_per_index` (default) | One independent time series per index value (`cell_voltage[0]`, `cell_voltage[1]`, …). | Per-element trending over time. |
| `array_column` | Each full frame's elements stored as one array/vector column (one row per timestamp holding the whole vector). | Snapshots queried as a unit. |
| `snapshot` | Keep only the latest fully-populated array, overwriting prior values. | Current-state view with no history. |

## How each consumer uses MDC

- **Engine (decode):** reads networks → messages → signals; uses `id`,
  `isExtended`, `length`, bit layout, `kind`, `conversion`, multiplexing to turn
  frames into physical values; evaluates `computedSignals`; raises `alarms`.
  Bit-extraction and conversion live in one shared module.
- **App (`mdc-editor` + telemetry/analytics):** edits the document; uses
  `display` hints, typed `attributes`, `valueTables`, and `alarms` for
  rendering and validation.
- **Tools:** `dbc2mdc` imports DBC (reusing `cantools` / `export_canspec_json.py`
  logic), `mdc2cheaders` generates C headers, `mdc2grafana` builds dashboards
  from `display`/`dashboardGroup`/`chartType`, `mdc-validate` runs this schema
  plus semantic checks. All share **one** MDC load/validate module.
