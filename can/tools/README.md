# MDC tooling

Tools that produce and consume MDC projects. Every tool that **consumes** a
project shares one loader/validator — `lib/mdc-load.mjs` — so parsing, folder
merging, and schema validation are defined exactly once (DRY contract in
`.cursor/rules/mdc.mdc`).

## Toolchain choice

- **Node** for the JSON tools (`validate-mdc`, `mdc-validate`, `mdc2cheaders`,
  `mdc2grafana`) — they read/emit JSON and share `lib/mdc-load.mjs`.
- **Python** for `dbc2mdc` — DBC parsing reuses **cantools** (the same library as
  `Embedded-Sharepoint/can/canspec-ui/export_canspec_json.py`), the laziest
  correct path; writing a DBC parser by hand would be wasteful and error-prone.

Setup: `npm install` once in `tools/`. For `dbc2mdc`: `pip install cantools`.

## Shared loader — `lib/mdc-load.mjs`

The single source of truth for loading + validating a project.

- `loadProject(path)` — accepts a project folder (`project.mdc.json` + optional
  per-network `*.mdc.json` fragments merged by `network.id`) or a single
  `project.mdc.json` file; returns the merged document.
- `compileSchema(schemaPath?)` / `formatErrors(errors)` — Ajv 2020 + `$ref`
  resolution against `mdc/schema/` (defaults to the bundle).
- `loadAndValidate(path, schemaPath?)` — load + schema-validate in one call.
- `iterMessages(project)` — iterate `(vehicle, network, message)` tuples.

## Tools

| Tool | Path | Run |
|------|------|-----|
| schema bundler | `bundle-mdc-schema.mjs` | `node tools/bundle-mdc-schema.mjs` |
| schema validate | `validate-mdc.mjs` | `node tools/validate-mdc.mjs <schema> <data.mdc.json>` |
| semantic linter | `mdc-validate.mjs` | `node tools/mdc-validate.mjs <project-dir\|project.mdc.json>` |
| DBC importer | `dbc2mdc.py` | `python3 tools/dbc2mdc.py <file.dbc\|vehicle-dir> [-o out.mdc.json] [--vehicle-id id]` |
| C header gen | `mdc2cheaders.mjs` | `node tools/mdc2cheaders.mjs <project> [-o out.h]` |
| Grafana gen | `mdc2grafana.mjs` | `node tools/mdc2grafana.mjs <project> [-o dash.json]` |
| bus-load calc | `mdc-busload.mjs` | `node tools/mdc-busload.mjs <project>` (`--self-check`) |

### `mdc-validate` — semantic linter

Schema validation first, then checks the schema cannot express: signal
bit-overlap (multiplex-aware), message id collision, DLC bounds, and
multiplexing integrity. Exits non-zero on any violation.

### `dbc2mdc` — DBC → MDC

One `.dbc` → one network; a `<vehicle>/` folder of `.dbc` files → one vehicle
with several networks. Output validates against `mdc/schema/mdc.schema.json` and
is intended to be enriched afterward (tags, display hints, alarms) in the editor.

## Notes / deferrals

- `dbc2mdc` emits decode-faithful structure; MDC value-adds (display hints,
  alarms, tags, typed attributes, computed signals) are intentionally **not**
  invented from a DBC — they are added later. So a freshly imported project
  yields an empty `mdc2grafana` dashboard (no `display` hints yet); the
  hand-authored `lhr-ev1` example yields a populated one.
- DBC attribute round-tripping (`BA_DEF_`/`BA_`) and value-table extraction into
  shared `valueTables` are deferred; choices are inlined per-signal for now.
