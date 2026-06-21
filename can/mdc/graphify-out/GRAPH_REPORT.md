# Graph Report - Embedded-Sharepoint/can/mdc  (2026-06-19)

## Corpus Check
- 30 files · ~16,561 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 978 nodes · 1150 edges · 87 communities
- Extraction: 100% EXTRACTED · 0% INFERRED · 0% AMBIGUOUS
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `77e2fae3`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- [[_COMMUNITY_Community 0|Community 0]]
- [[_COMMUNITY_Community 1|Community 1]]
- [[_COMMUNITY_Community 2|Community 2]]
- [[_COMMUNITY_Community 3|Community 3]]
- [[_COMMUNITY_Community 4|Community 4]]
- [[_COMMUNITY_Community 5|Community 5]]
- [[_COMMUNITY_Community 6|Community 6]]
- [[_COMMUNITY_Community 7|Community 7]]
- [[_COMMUNITY_Community 8|Community 8]]
- [[_COMMUNITY_Community 9|Community 9]]
- [[_COMMUNITY_Community 10|Community 10]]
- [[_COMMUNITY_Community 11|Community 11]]
- [[_COMMUNITY_Community 12|Community 12]]
- [[_COMMUNITY_Community 13|Community 13]]
- [[_COMMUNITY_Community 14|Community 14]]
- [[_COMMUNITY_Community 15|Community 15]]
- [[_COMMUNITY_Community 16|Community 16]]
- [[_COMMUNITY_Community 17|Community 17]]
- [[_COMMUNITY_Community 18|Community 18]]
- [[_COMMUNITY_Community 19|Community 19]]
- [[_COMMUNITY_Community 20|Community 20]]
- [[_COMMUNITY_Community 21|Community 21]]
- [[_COMMUNITY_Community 22|Community 22]]
- [[_COMMUNITY_Community 23|Community 23]]
- [[_COMMUNITY_Community 24|Community 24]]
- [[_COMMUNITY_Community 25|Community 25]]
- [[_COMMUNITY_Community 26|Community 26]]
- [[_COMMUNITY_Community 27|Community 27]]
- [[_COMMUNITY_Community 28|Community 28]]
- [[_COMMUNITY_Community 29|Community 29]]
- [[_COMMUNITY_Community 30|Community 30]]
- [[_COMMUNITY_Community 31|Community 31]]
- [[_COMMUNITY_Community 32|Community 32]]
- [[_COMMUNITY_Community 33|Community 33]]
- [[_COMMUNITY_Community 34|Community 34]]
- [[_COMMUNITY_Community 35|Community 35]]
- [[_COMMUNITY_Community 36|Community 36]]
- [[_COMMUNITY_Community 37|Community 37]]
- [[_COMMUNITY_Community 38|Community 38]]
- [[_COMMUNITY_Community 39|Community 39]]
- [[_COMMUNITY_Community 40|Community 40]]
- [[_COMMUNITY_Community 41|Community 41]]
- [[_COMMUNITY_Community 42|Community 42]]
- [[_COMMUNITY_Community 43|Community 43]]
- [[_COMMUNITY_Community 44|Community 44]]
- [[_COMMUNITY_Community 45|Community 45]]
- [[_COMMUNITY_Community 46|Community 46]]
- [[_COMMUNITY_Community 47|Community 47]]
- [[_COMMUNITY_Community 48|Community 48]]
- [[_COMMUNITY_Community 49|Community 49]]
- [[_COMMUNITY_Community 50|Community 50]]
- [[_COMMUNITY_Community 51|Community 51]]
- [[_COMMUNITY_Community 52|Community 52]]
- [[_COMMUNITY_Community 53|Community 53]]
- [[_COMMUNITY_Community 54|Community 54]]
- [[_COMMUNITY_Community 55|Community 55]]
- [[_COMMUNITY_Community 56|Community 56]]
- [[_COMMUNITY_Community 57|Community 57]]
- [[_COMMUNITY_Community 58|Community 58]]
- [[_COMMUNITY_Community 59|Community 59]]
- [[_COMMUNITY_Community 60|Community 60]]
- [[_COMMUNITY_Community 61|Community 61]]
- [[_COMMUNITY_Community 62|Community 62]]
- [[_COMMUNITY_Community 63|Community 63]]
- [[_COMMUNITY_Community 64|Community 64]]
- [[_COMMUNITY_Community 65|Community 65]]
- [[_COMMUNITY_Community 66|Community 66]]
- [[_COMMUNITY_Community 67|Community 67]]
- [[_COMMUNITY_Community 68|Community 68]]
- [[_COMMUNITY_Community 69|Community 69]]
- [[_COMMUNITY_Community 70|Community 70]]
- [[_COMMUNITY_Community 71|Community 71]]
- [[_COMMUNITY_Community 72|Community 72]]
- [[_COMMUNITY_Community 73|Community 73]]
- [[_COMMUNITY_Community 74|Community 74]]
- [[_COMMUNITY_Community 75|Community 75]]
- [[_COMMUNITY_Community 76|Community 76]]
- [[_COMMUNITY_Community 77|Community 77]]
- [[_COMMUNITY_Community 78|Community 78]]
- [[_COMMUNITY_Community 79|Community 79]]
- [[_COMMUNITY_Community 80|Community 80]]
- [[_COMMUNITY_Community 81|Community 81]]
- [[_COMMUNITY_Community 82|Community 82]]
- [[_COMMUNITY_Community 83|Community 83]]
- [[_COMMUNITY_Community 84|Community 84]]
- [[_COMMUNITY_Community 85|Community 85]]
- [[_COMMUNITY_Community 86|Community 86]]

## God Nodes (most connected - your core abstractions)
1. `sanitize_id()` - 11 edges
2. `_network_from_dbc()` - 11 edges
3. `MDC — Multi CAN Database (v3)` - 11 edges
4. `_signal_to_mdc()` - 9 edges
5. `_message_to_mdc()` - 9 edges
6. `metadata` - 8 edges
7. `$defs` - 8 edges
8. `$ref` - 8 edges
9. `required` - 8 edges
10. `containedMessage` - 7 edges

## Surprising Connections (you probably didn't know these)
- `selfCheck()` --calls--> `bitsPerFrame()`  [EXTRACTED]
  tools/mdc-busload.mjs → lib/busload.mjs
- `selfCheck()` --calls--> `loadPercentAt()`  [EXTRACTED]
  tools/mdc-busload.mjs → lib/busload.mjs
- `validateBitOverlaps()` --calls--> `occupiedBits()`  [EXTRACTED]
  lib/mdc-validate-shared.mjs → lib/mdc-bits.mjs

## Import Cycles
- None detected.

## Communities (87 total, 0 thin omitted)

### Community 0 - "Community 0"
Cohesion: 0.05
Nodes (46): bitsPerFrame(), busLoadForNetwork(), computeBusLoad(), loadPercentAt(), messageBitsPerSecond(), networkProtocolLabel(), networkSpeeds(), SEND_TYPE_TRIGGERED (+38 more)

### Community 1 - "Community 1"
Cohesion: 0.05
Nodes (47): description, minimum, type, description, items, type, items, type (+39 more)

### Community 2 - "Community 2"
Cohesion: 0.05
Nodes (42): additionalProperties, description, items, type, description, $ref, description, description (+34 more)

### Community 3 - "Community 3"
Cohesion: 0.06
Nodes (35): additionalProperties, description, properties, required, type, description, items, type (+27 more)

### Community 4 - "Community 4"
Cohesion: 0.06
Nodes (33): description, enum, type, description, $ref, description, type, properties (+25 more)

### Community 5 - "Community 5"
Cohesion: 0.06
Nodes (32): properties, description, minimum, type, description, $ref, type, description (+24 more)

### Community 6 - "Community 6"
Cohesion: 0.06
Nodes (31): description, items, type, description, $ref, $ref, default, description (+23 more)

### Community 7 - "Community 7"
Cohesion: 0.07
Nodes (30): items, type, signalGroup, description, items, minItems, type, uniqueItems (+22 more)

### Community 8 - "Community 8"
Cohesion: 0.07
Nodes (28): description, enum, type, description, pattern, type, $defs, byteOrder (+20 more)

### Community 9 - "Community 9"
Cohesion: 0.24
Nodes (25): _apply_multiplexer(), _apply_scale_offset(), _attribute_definitions(), build_project(), _bus_bitrates(), _choices(), _contained_message_to_mdc(), _default_value() (+17 more)

### Community 10 - "Community 10"
Cohesion: 0.10
Nodes (21): type, environmentVariable, type, enum, type, additionalProperties, description, properties (+13 more)

### Community 11 - "Community 11"
Cohesion: 0.10
Nodes (20): Bus-load formula, byte_order convention, cantools-shaped field names, Conversion math (v3), Hierarchy (v3 — flattened root), How each consumer uses MDC, MDC — Multi CAN Database (v3), Migration v2 → v3 (+12 more)

### Community 12 - "Community 12"
Cohesion: 0.10
Nodes (19): $defs, multiplexing, default, description, type, $id, default, description (+11 more)

### Community 13 - "Community 13"
Cohesion: 0.11
Nodes (19): additionalProperties, description, properties, required, type, arraySpec, description, type (+11 more)

### Community 14 - "Community 14"
Cohesion: 0.11
Nodes (18): additionalProperties, allOf, description, required, type, $defs, alarm, displayHints (+10 more)

### Community 15 - "Community 15"
Cohesion: 0.11
Nodes (18): type, description, $ref, description, type, description, type, properties (+10 more)

### Community 16 - "Community 16"
Cohesion: 0.12
Nodes (16): attributeDefinitions, description, id, metadata, author, createdAt, description, modifiedAt (+8 more)

### Community 17 - "Community 17"
Cohesion: 0.12
Nodes (15): allOf, description, required, type, unevaluatedProperties, $defs, computedSignal, signal (+7 more)

### Community 18 - "Community 18"
Cohesion: 0.13
Nodes (14): $ref, description, properties, type, additionalProperties, description, $ref, type (+6 more)

### Community 19 - "Community 19"
Cohesion: 0.13
Nodes (15): $ref, default, description, minimum, type, $ref, properties, $ref (+7 more)

### Community 20 - "Community 20"
Cohesion: 0.35
Nodes (15): items, items, items, items, additionalProperties, allOf, description, $ref (+7 more)

### Community 21 - "Community 21"
Cohesion: 0.32
Nodes (13): Database, Message, Signal, _build_database(), export_network(), load_project(), main(), _mdc_message_to_cantools() (+5 more)

### Community 22 - "Community 22"
Cohesion: 0.14
Nodes (14): properties, description, minLength, type, default, type, default, type (+6 more)

### Community 23 - "Community 23"
Cohesion: 0.14
Nodes (13): dependencies, ajv, ajv-formats, @apidevtools/json-schema-ref-parser, description, name, private, scripts (+5 more)

### Community 24 - "Community 24"
Cohesion: 0.21
Nodes (9): kindToFlags(), main(), migrateComputedSignal(), migrateConversion(), migrateMultiplexer(), migrateSignal(), migrateVehicle(), parseArgs() (+1 more)

### Community 25 - "Community 25"
Cohesion: 0.15
Nodes (13): properties, minimum, type, description, maximum, minimum, type, header_id (+5 more)

### Community 26 - "Community 26"
Cohesion: 0.15
Nodes (13): description, type, description, type, properties, author, description, revision (+5 more)

### Community 27 - "Community 27"
Cohesion: 0.18
Nodes (7): TempPathFactory, elcon(), _import_dbc(), dbc2mdc Wave 3 tests (BA_DEF_/BA_ → MDC v3).  Run from can/mdc: pytest tools/tes, synthetic(), test_output_validates_and_generates_headers(), Path

### Community 28 - "Community 28"
Cohesion: 0.17
Nodes (11): description, enum, type, $defs, attributeType, scope, $id, $schema (+3 more)

### Community 29 - "Community 29"
Cohesion: 0.17
Nodes (12): properties, description, $ref, description, type, description, type, default (+4 more)

### Community 30 - "Community 30"
Cohesion: 0.17
Nodes (12): $ref, description, type, description, type, byte_order, initial, multiplexer_signal (+4 more)

### Community 31 - "Community 31"
Cohesion: 0.18
Nodes (11): description, items, type, uniqueItems, type, dependsOn, receivers, description (+3 more)

### Community 32 - "Community 32"
Cohesion: 0.18
Nodes (11): description, type, description, format, type, properties, author, createdAt (+3 more)

### Community 33 - "Community 33"
Cohesion: 0.20
Nodes (9): $defs, network, $id, allOf, description, required, type, unevaluatedProperties (+1 more)

### Community 34 - "Community 34"
Cohesion: 0.20
Nodes (10): description, type, description, $ref, description, type, properties, attributeDefinitions (+2 more)

### Community 35 - "Community 35"
Cohesion: 0.22
Nodes (8): id, metadata, description, name, source, networks, $schema, schemaVersion

### Community 36 - "Community 36"
Cohesion: 0.22
Nodes (9): description, type, description, minLength, $ref, type, properties, comment (+1 more)

### Community 37 - "Community 37"
Cohesion: 0.25
Nodes (7): __dirname, input, localPathFromId(), output, read(), repoRoot, schemaRoot

### Community 38 - "Community 38"
Cohesion: 0.22
Nodes (8): id, metadata, description, name, source, networks, $schema, schemaVersion

### Community 39 - "Community 39"
Cohesion: 0.25
Nodes (7): additionalProperties, description, $id, required, $schema, title, type

### Community 40 - "Community 40"
Cohesion: 0.29
Nodes (6): description, oneOf, $defs, conversion, $id, $schema

### Community 41 - "Community 41"
Cohesion: 0.33
Nodes (6): allOf, description, required, type, unevaluatedProperties, containedMessage

### Community 42 - "Community 42"
Cohesion: 0.33
Nodes (6): message, allOf, description, required, type, unevaluatedProperties

### Community 43 - "Community 43"
Cohesion: 0.33
Nodes (6): node, allOf, description, required, type, unevaluatedProperties

### Community 44 - "Community 44"
Cohesion: 0.33
Nodes (6): description, items, type, uniqueItems, type, enumValues

### Community 45 - "Community 45"
Cohesion: 0.33
Nodes (6): $ref, scopes, description, items, type, uniqueItems

### Community 46 - "Community 46"
Cohesion: 0.33
Nodes (6): enum, scopes, description, items, type, uniqueItems

### Community 47 - "Community 47"
Cohesion: 0.40
Nodes (5): description, maximum, minimum, type, frame_id

### Community 48 - "Community 48"
Cohesion: 0.40
Nodes (5): additionalProperties, description, required, type, attributeDefinition

### Community 49 - "Community 49"
Cohesion: 0.40
Nodes (5): minimum, description, items, type, multiplexer_ids

### Community 50 - "Community 50"
Cohesion: 0.40
Nodes (5): description, maximum, minimum, type, length

### Community 51 - "Community 51"
Cohesion: 0.40
Nodes (5): start, description, maximum, minimum, type

### Community 52 - "Community 52"
Cohesion: 0.40
Nodes (4): `broken/` and `warn/`, MDC examples, Validate, Vehicle projects

### Community 53 - "Community 53"
Cohesion: 0.40
Nodes (5): description, items, type, uniqueItems, enumValues

### Community 54 - "Community 54"
Cohesion: 0.40
Nodes (5): description, minLength, $ref, type, name

### Community 55 - "Community 55"
Cohesion: 0.40
Nodes (5): samplePoint, description, maximum, minimum, type

### Community 56 - "Community 56"
Cohesion: 0.50
Nodes (4): default, description, type, is_extended_frame

### Community 57 - "Community 57"
Cohesion: 0.50
Nodes (4): default, description, type, is_fd

### Community 58 - "Community 58"
Cohesion: 0.50
Nodes (4): description, format, type, createdAt

### Community 59 - "Community 59"
Cohesion: 0.50
Nodes (4): metadata, additionalProperties, description, type

### Community 60 - "Community 60"
Cohesion: 0.50
Nodes (4): description, format, type, modifiedAt

### Community 61 - "Community 61"
Cohesion: 0.50
Nodes (4): description, minLength, type, name

### Community 62 - "Community 62"
Cohesion: 0.50
Nodes (4): default, description, type, is_multiplexer

### Community 63 - "Community 63"
Cohesion: 0.50
Nodes (4): default, description, type, offset

### Community 64 - "Community 64"
Cohesion: 0.50
Nodes (4): scale, default, description, type

### Community 65 - "Community 65"
Cohesion: 0.50
Nodes (4): description, minimum, type, baudrate

### Community 66 - "Community 66"
Cohesion: 0.50
Nodes (4): description, minimum, type, fd_baudrate

### Community 67 - "Community 67"
Cohesion: 0.50
Nodes (4): additionalProperties, description, type, metadata

### Community 68 - "Community 68"
Cohesion: 0.50
Nodes (4): description, format, type, modifiedAt

### Community 69 - "Community 69"
Cohesion: 0.50
Nodes (4): schemaVersion, description, pattern, type

### Community 70 - "Community 70"
Cohesion: 0.50
Nodes (4): type, description, enum, type

### Community 71 - "Community 71"
Cohesion: 0.67
Nodes (3): protocol, enum, type

### Community 72 - "Community 72"
Cohesion: 0.67
Nodes (3): transport, default, $ref

### Community 73 - "Community 73"
Cohesion: 0.67
Nodes (3): description, type, comment

### Community 74 - "Community 74"
Cohesion: 0.67
Nodes (3): description, $ref, conversion

### Community 75 - "Community 75"
Cohesion: 0.67
Nodes (3): description, type, invalid

### Community 76 - "Community 76"
Cohesion: 0.67
Nodes (3): raw_initial, description, type

### Community 77 - "Community 77"
Cohesion: 0.67
Nodes (3): raw_invalid, description, type

### Community 78 - "Community 78"
Cohesion: 0.67
Nodes (3): description, type, computedSignals

### Community 79 - "Community 79"
Cohesion: 0.67
Nodes (3): description, type, entries

### Community 80 - "Community 80"
Cohesion: 0.67
Nodes (3): description, $ref, id

### Community 81 - "Community 81"
Cohesion: 0.67
Nodes (3): description, type, min

### Community 82 - "Community 82"
Cohesion: 0.67
Nodes (3): description, type, networks

### Community 83 - "Community 83"
Cohesion: 0.67
Nodes (3): description, type, nodes

### Community 84 - "Community 84"
Cohesion: 0.67
Nodes (3): revision, description, type

### Community 85 - "Community 85"
Cohesion: 0.67
Nodes (3): $schema, description, type

### Community 86 - "Community 86"
Cohesion: 0.67
Nodes (3): valueTables, description, type

## Knowledge Gaps
- **586 isolated node(s):** `$schema`, `schemaVersion`, `id`, `networks`, `name` (+581 more)
  These have ≤1 connection - possible missing edges or undocumented components.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `properties` connect `Community 19` to `Community 1`, `Community 36`, `Community 71`, `Community 7`, `Community 72`, `Community 42`, `Community 47`, `Community 56`, `Community 25`, `Community 57`?**
  _High betweenness centrality (0.019) - this node is a cross-community bridge._
- **Why does `properties` connect `Community 1` to `Community 33`, `Community 36`?**
  _High betweenness centrality (0.014) - this node is a cross-community bridge._
- **Why does `properties` connect `Community 15` to `Community 65`, `Community 34`, `Community 66`, `Community 70`, `Community 78`, `Community 79`, `Community 80`, `Community 81`, `Community 46`, `Community 83`, `Community 20`, `Community 53`, `Community 54`, `Community 55`, `Community 86`?**
  _High betweenness centrality (0.013) - this node is a cross-community bridge._
- **What connects `$schema`, `schemaVersion`, `id` to the rest of the system?**
  _589 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `Community 0` be split into smaller, more focused modules?**
  _Cohesion score 0.05288207297726071 - nodes in this community are weakly interconnected._
- **Should `Community 1` be split into smaller, more focused modules?**
  _Cohesion score 0.04625346901017576 - nodes in this community are weakly interconnected._
- **Should `Community 2` be split into smaller, more focused modules?**
  _Cohesion score 0.048726467331118496 - nodes in this community are weakly interconnected._