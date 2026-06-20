#!/usr/bin/env node
/**
 * Bundle the modular MDC schema into a single dereferenced file.
 *
 * Authoring: edit mdc/schema/mdc.schema.json + mdc/schema/defs/*.json
 * Consumption: use mdc/schema/mdc.schema.bundle.json (CI, codegen, editors)
 */
import { readFile, writeFile } from "node:fs/promises";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";
import $RefParser from "@apidevtools/json-schema-ref-parser";

const __dirname = dirname(fileURLToPath(import.meta.url));
const repoRoot = join(__dirname, "..");
const schemaRoot = join(repoRoot, "mdc/schema");
const input = join(schemaRoot, "mdc.schema.json");
const output = join(schemaRoot, "mdc.schema.bundle.json");

const SCHEMA_ID_PREFIX = "https://schemas.lhr.dev/mdc/2025-06/";

/** Map published $id URLs to on-disk paths under mdc/schema/. */
function localPathFromId(url) {
  if (!url.startsWith(SCHEMA_ID_PREFIX)) return null;
  return join(schemaRoot, url.slice(SCHEMA_ID_PREFIX.length));
}

const bundled = await $RefParser.bundle(input, {
  resolve: {
    http: {
      order: 1,
      canRead(file) {
        return file.url.startsWith(SCHEMA_ID_PREFIX);
      },
      async read(file) {
        const path = localPathFromId(file.url);
        if (!path) throw new Error(`Unmapped schema $id: ${file.url}`);
        return readFile(path, "utf8");
      },
    },
  },
});

await writeFile(output, JSON.stringify(bundled, null, 2) + "\n");
console.log(`Bundled ${input} → ${output}`);
