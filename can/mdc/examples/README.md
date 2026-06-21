# MDC examples

Validator test fixtures that exercise [`../schema/mdc.schema.bundle.json`](../schema/mdc.schema.bundle.json).

## `broken/` and `warn/`

Intentionally invalid or warning-producing MDC documents for `mdc-validate` regression tests. Not vehicle projects.

## Vehicle projects

Canonical v3 example: [`lhr-ev1/project.mdc.json`](lhr-ev1/project.mdc.json) (migrated from [`../../vehicles/lhr-ev1/project.mdc.json`](../../vehicles/lhr-ev1/project.mdc.json)).

Other vehicle MDC specs may live under [`../../vehicles/`](../../vehicles/).

## Validate

From `can/mdc` (requires `npm install` once):

```bash
# Regenerate bundle after editing schema/defs/*.json
node tools/bundle-mdc-schema.mjs

# Validate the v3 lhr-ev1 example
node tools/mdc-validate.mjs --schema-only schema/mdc.schema.bundle.json examples/lhr-ev1/project.mdc.json
```

The modular root (`schema/mdc.schema.json` + `defs/`) is for authoring;
`mdc.schema.bundle.json` is what consumers and CI should use.
