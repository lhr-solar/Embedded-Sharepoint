# MDC examples

Validator test fixtures that exercise [`../schema/mdc.schema.bundle.json`](../schema/mdc.schema.bundle.json).

## `broken/` and `warn/`

Intentionally invalid or warning-producing MDC documents for `mdc-validate` regression tests. Not vehicle projects.

## Vehicle projects

Real vehicle MDC specs live under [`../../vehicles/`](../../vehicles/) (e.g. `../../vehicles/lhr-ev1/project.mdc.json`).

## Validate

From `can/mdc` (requires `npm install` once):

```bash
# Regenerate bundle after editing schema/defs/*.json
node tools/bundle-mdc-schema.mjs

# Validate the lhr-ev1 vehicle project
node tools/mdc-validate.mjs --schema-only schema/mdc.schema.bundle.json ../vehicles/lhr-ev1/project.mdc.json
```

The modular root (`schema/mdc.schema.json` + `defs/`) is for authoring;
`mdc.schema.bundle.json` is what consumers and CI should use.
