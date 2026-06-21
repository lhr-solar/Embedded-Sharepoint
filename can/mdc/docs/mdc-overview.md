# MDC — Multi CAN Database (v3)

MDC is the canonical JSON contract describing CAN/CAN-FD networks for the LHR
telemetry platform. It is a **superset of DBC**: it carries everything a DBC
carries plus first-class metadata DBC cannot express. It is validated by
[`../schema/mdc.schema.bundle.json`](../schema/mdc.schema.bundle.json) (JSON
Schema draft 2020-12) and consumed by the C++ engine (decode), the app
(`mdc-editor`), and the `tools/` (`dbc2mdc`, `generate_can_headers`, `mdc2grafana`,
`mdc-validate`).

**v3** adopts cantools field names, flattens the document root (one vehicle per
file), and bumps `schemaVersion` to `3.0.0`. Schema `$id`:
`https://lhrsolar.org/lhrs-mdc/3.0.0/mdc.schema.json`. Title: **MDC - Multi CAN
Database**.

The schema is a stable **contract**. Changes are additive within a major
version; `schemaVersion` (semver) gates compatibility. A consumer must reject a
document whose major version it does not understand.

## Schema layout (authoring)

The schema is split for editability; semantics are unchanged from the monolithic
contract:

```
mdc/schema/
  mdc.schema.json           # thin root — vehicle-level properties
  defs/                     # domain-scoped $defs (edit here)
    primitives.json         # semver, identifier, byteOrder, transport, …
    annotations.json        # typed attributes (DBC BA_/BA_DEF_)
    project-meta.json       # metadata, attributeDefinitions
    enumerations.json       # choices, valueTables
    conversion.json         # optional rational/table conversion
    presentation.json       # display hints, alarms, is_signed/is_float
    multiplexing.json       # message-level mux summary
    signals.json            # signal, computedSignal
    hierarchy.json          # node, message, network (+ signalGroup, EV, containers)
  mdc.schema.bundle.json    # generated single-file artifact (for consumers)
```

**Edit** `mdc.schema.json` and the relevant `defs/*.json` file. **Regenerate**
the bundle before committing or validating in CI:

```bash
node tools/bundle-mdc-schema.mjs
node tools/mdc-validate.mjs --schema-only schema/mdc.schema.bundle.json examples/lhr-ev1/project.mdc.json
```

Downstream consumers (engine, editor, codegen) should load
`mdc.schema.bundle.json`. The modular root is for authoring only.

## Hierarchy (v3 — flattened root)

**One MDC document = one vehicle.** The root *is* the vehicle; `vehicles[]` is
gone.

```
Vehicle (root)          ($schema, schemaVersion, id, name, description,
  │                      metadata?, attributeDefinitions[], valueTables[],
  │                      attributes?, networks[] — required: schemaVersion, id, networks)
  └─ Network/Bus[]      (id, baudrate, fd_baudrate?, samplePoint, nodes[],
       │                 valueTables[], environment_variables[], attributes)
       ├─ Message[]      (name, frame_id, is_extended_frame, is_fd, length,
       │   │              transport, send_type, cycle_time, senders[],
       │   │              multiplexing, array, signal_groups[], header_id?,
       │   │              contained_messages[], protocol?, attributes)
       │   ├─ Signal[]          (bit-packed, decoded from frame bytes)
       │   └─ computedSignal[]  (virtual, derived from this message)
       └─ computedSignal[]      (virtual, cross-message on this network)
```

Identifiers namespace downward: a signal is addressed
`vehicle.network.message.signal`, and cross-references (e.g. computed-signal
`expr`/`dependsOn`) use the `message.signal` form within a network.

On disk a vehicle project is typically a single `*.mdc.json` (or a folder with
`project.mdc.json` plus optional per-network fragments). See
[`../examples/README.md`](../examples/README.md).

### cantools-shaped field names

v3 renames wire/layout fields to match cantools / DBC round-trip:

| Entity | Key renames (v2 → v3) |
|---|---|
| Signal | `startBit`→`start`, `lengthBits`→`length`, `byteOrder`→`byte_order`, `description`→`comment`, `kind`→`is_signed`+`is_float`, `min`/`max`→`minimum`/`maximum`, `multiplexer{}`→`is_multiplexer`/`multiplexer_signal`/`multiplexer_ids` |
| Message | `id`→`frame_id`, `isExtended`→`is_extended_frame`, `isFd`→`is_fd`, `transmissionType`→`send_type`, `cycleTimeMs`→`cycle_time`, `description`→`comment` |
| Network | `bitrate`→`baudrate`, `dataBitrate`→`fd_baudrate`, `description`→`comment`; `protocol` enum **removed** (CAN FD implied by `fd_baudrate`) |
| Node | `description`→`comment` |

Root keeps `description` (doc-level); entities use `comment` (DBC convention).

### New v3 objects

- **`signal_groups[]`** on message — DBC `SIG_GROUP_` metadata (`name`,
  `repetitions`, `signal_names`).
- **`environment_variables[]`** on network — DBC `EV_` metadata (`name`,
  `env_type`, `minimum`/`maximum`, `initial_value`, `unit`, `env_id`,
  `access_type`, `access_node`, `comment`).
- **`contained_messages[]`** + **`header_id`** on message — ISO-TP / AUTOSAR PDU
  container sub-PDUs (decode-load-bearing; coexists with `transport`).

## What MDC adds over DBC

All modeled as first-class schema, not free-text:

| Capability | Where | Notes |
|---|---|---|
| Typed attributes | `attributeDefinitions[]` + per-entity `attributes{}` | DBC `BA_DEF_`/`BA_` superset. See [Typed attribute library](#typed-attribute-library). |
| Display / viz hints | `signal.display` | precision, format, chartType, color, dashboardGroup, unitOverride, scale bounds, visible. Advisory — never affects decode. |
| Fault / alarm rules | `signal.alarms[]` | op (lt/le/gt/ge/eq/ne/between/outside/change), threshold(s), severity, message, hysteresis. |
| Computed / virtual signals | `computedSignals[]` (message + network) | `expr` over other signals' physical values; no bit layout. |
| Multi-network per vehicle | one document | with namespaced ids + cross-refs. |

## Timing, send type & bus loading

Two message-level fields describe transmit timing; network `baudrate` gives the
bus its speed. Together they drive the bus-load budget.

### Send type & cycle time

- **`send_type`** — free string preserving raw DBC `GenMsgSendType` (e.g.
  `Cyclic`, `Triggered`, `Event`). Bus-load tooling classifies it: labels in a
  triggered set are **excluded** from bus load; others with a positive
  **`cycle_time`** count as cyclic.
- **`cycle_time`** — nominal transmit period in milliseconds (cantools
  `Message.cycle_time`). Schema default is `1000` when used for cyclic bus-load.

DBC import (`dbc2mdc`) maps `GenMsgSendType` → `send_type` verbatim and
`GenMsgCycleTime` → `cycle_time`. `VFrameFormat` sets native `is_extended_frame` /
`is_fd` via cantools and is not stored in `attributes`.

## Typed attribute library

MDC replaces free-form tags with a **typed attribute system** mirroring DBC
`BA_DEF_` / `BA_` / `BA_DEF_DEF_`.

### Project-level declarations (`attributeDefinitions[]`)

Each entry declares one attribute key and its value type:

```json
{
  "name": "SystemOwner",
  "type": "string",
  "description": "Owning subsystem team.",
  "scopes": ["network", "message"]
}
```

- **`name`** (required) — key used in per-entity `attributes` maps.
- **`type`** (required) — one of `enum`, `int`, `float`, `string`, `bool`, `hex`.
- **`scopes`** (optional) — levels where the attribute is allowed:
  `project`, `vehicle`, `network`, `node`, `message`, `signal`. Omit to allow all.

### Per-entity values (`attributes{}`)

On any entity (vehicle root, network, node, message, signal):

```json
"attributes": { "BusType": "CAN FD" }
```

Keys match `attributeDefinitions[].name`; values are scalars (string, number,
boolean).

### Native-field mapping (not stored in `attributes`)

| DBC attribute | MDC native field(s) |
|---|---|
| `VFrameFormat` | `message.is_extended_frame`, `message.is_fd` |
| `GenMsgSendType` | `message.send_type` |
| `GenMsgCycleTime` | `message.cycle_time` |
| `SPN` | `signal.spn` (v3: native, not an attribute) |
| `DBName` | skipped (redundant with `network.id`/`name`) |

### Network baudrate

`network.baudrate` is an **integer in bits per second**. CAN FD is implied when
`fd_baudrate` is present. `mdc-validate` warns when `baudrate` is outside the
standard set.

### Bus-load formula

For each network, bus load is the fraction of bus time consumed by **cyclic**
traffic (messages whose lowercased `send_type` is not in the triggered set and
have a positive `cycle_time`):

```
loadPercent = Σ over cyclic messages of
    (1000 / cycle_time) * bitsPerFrame / baudrate * 100
```

## byte_order convention

`byte_order` is one of **`little_endian`** (Intel) or **`big_endian`**
(Motorola / "sawtooth"). These names match cantools' output.

`start` follows the **cantools/DBC bit-numbering convention**:

- **little_endian (Intel):** `start` is the LSB of the signal.
- **big_endian (Motorola):** `start` is the MSB in DBC sawtooth numbering.

`start` max is 511 (bit 7 of byte 63) to support 64-byte CAN FD frames.

## Conversion math (v3)

**Native linear (cantools-shaped):**

- `scale` (default `1`) and `offset` (default `0`) on the signal.
- Identity = omit both or `scale: 1, offset: 0`.

**Optional `conversion` extension — non-linear only:**

- **rational** — polynomial ratio + offset.
- **table** — categorical; render via `choices` or `valueTableRef`.

When `conversion` is present it governs raw→physical math; `scale`/`offset` are
ignored.

`is_signed` / `is_float` (booleans) replace v2 `kind`:

| v2 `kind` | `is_signed` | `is_float` |
|---|---|---|
| `unsigned` | `false` | `false` |
| `signed` | `true` | `false` |
| `float` | `false` | `true` |

Computed signals default `is_float: true`, `is_signed: false`.

### Value tables

`choices[]` is an inline enumeration; `valueTableRef` names a reusable table.
Resolution is **network-local `valueTables` first, then root shared
`valueTables`**. `choices` and `valueTableRef` are mutually exclusive on a
signal.

## Multiplexing (v3)

Per-signal mux is three flat fields (cantools-shaped):

- `is_multiplexer: true` — selector signal.
- `multiplexer_signal` + `multiplexer_ids` — gated signals.

Message-level `multiplexing` summary (`multiplexed`, `multiplexorSignal`,
`extended`) is retained as an MDC extension.

## Migration v2 → v3

Use `tools/mdc-v2-to-v3.mjs` to flatten a v2 project (with `vehicles[]`) into
one v3 file per vehicle with renamed fields and conversion mapping:

```bash
node tools/mdc-v2-to-v3.mjs --vehicle lhr_ev1 ../vehicles/lhr-ev1/project.mdc.json examples/lhr-ev1/project.mdc.json
```

## How each consumer uses MDC

- **Engine (decode):** reads networks → messages → signals; uses `frame_id`,
  `is_extended_frame`, `length`, bit layout, `is_signed`/`is_float`, `scale`/
  `offset`/`conversion`, multiplexing to turn frames into physical values;
  evaluates `computedSignals`; raises `alarms`.
- **App (`mdc-editor` + telemetry/analytics):** edits the document; uses
  `display` hints, typed `attributes`, `valueTables`, and `alarms` for
  rendering and validation.
- **Tools:** `dbc2mdc` imports DBC (cantools-backed), `generate_can_headers` generates
  C headers, `mdc2grafana` builds dashboards from `display`/`dashboardGroup`/
  `chartType`, `mdc-validate` runs this schema plus semantic checks.
