#!/usr/bin/env node
/**
 * Single source of truth for loading and validating MDC projects.
 *
 * Every MDC tool that consumes a project (generate_can_headers, mdc2grafana,
 * mdc-validate) imports from here so parsing/merging/schema-validation logic
 * lives in exactly one place (DRY contract from .cursor/rules/mdc.mdc).
 *
 * A project is either a single `*.mdc.json` (preferred v3 layout) or a folder with
 * `project.mdc.json` plus optional per-network `*.mdc.json` fragments merged into
 * root `networks[]`, keyed by `network.id`.
 */
import { readFile, readdir, stat } from "node:fs/promises";
import { basename, dirname, join } from "node:path";
import { fileURLToPath } from "node:url";
import $RefParser from "@apidevtools/json-schema-ref-parser";
import Ajv2020 from "ajv/dist/2020.js";
import addFormats from "ajv-formats";
import { iterMessages, resolveValueEntries, resolveChoices } from "./model.mjs";

export { iterMessages, resolveValueEntries, resolveChoices };

const __dirname = dirname(fileURLToPath(import.meta.url));
const repoRoot = join(__dirname, "..", "..");
const schemaRoot = join(repoRoot, "mdc/schema");
const SCHEMA_ID_PREFIXES = [
  "https://lhrsolar.org/lhrs-mdc/3.0.0/",
  "https://schemas.lhr.dev/mdc/2025-06/",
];

export const DEFAULT_SCHEMA = join(schemaRoot, "mdc.schema.bundle.json");

/** Map published $id URLs to on-disk paths under mdc/schema/. */
function localPathFromId(url) {
  for (const prefix of SCHEMA_ID_PREFIXES) {
    if (url.startsWith(prefix)) return join(schemaRoot, url.slice(prefix.length));
  }
  return null;
}

const refParserOptions = {
  resolve: {
    http: {
      order: 1,
      canRead(file) {
        return SCHEMA_ID_PREFIXES.some((p) => file.url.startsWith(p));
      },
      async read(file) {
        const path = localPathFromId(file.url);
        if (!path) throw new Error(`Unmapped schema $id: ${file.url}`);
        return readFile(path, "utf8");
      },
    },
  },
};

/** Compile the MDC schema (modular root or bundle) into an Ajv validator. */
export async function compileSchema(schemaPath = DEFAULT_SCHEMA) {
  const ajv = new Ajv2020({ strict: false, allErrors: true });
  addFormats(ajv);
  const schema = await $RefParser.dereference(schemaPath, refParserOptions);
  return ajv.compile(schema);
}

/** Format Ajv errors into short, greppable lines. */
export function formatErrors(errors = []) {
  return errors.map((err) => `  ${err.instancePath || "/"}: ${err.message}`);
}

/**
 * Merge a per-network fragment into the matching network of `project`.
 * A fragment is the network object itself (`{ id, protocol, messages, ... }`)
 * optionally wrapped as `{ network: {...} }`. Matched by `network.id`.
 */
function mergeNetworkFragment(project, fragment, originFile) {
  const net = fragment.network ?? fragment;
  if (!net?.id) {
    throw new Error(`${originFile}: network fragment has no "id"`);
  }
  const networks = project.networks ?? [];
  const idx = networks.findIndex((n) => n.id === net.id);
  if (idx >= 0) {
    networks[idx] = { ...net, filename: basename(originFile) };
    project.networks = networks;
    return;
  }
  throw new Error(
    `${originFile}: no networks[] entry with id "${net.id}" to merge into`,
  );
}

/**
 * Load a project from a folder or a single `project.mdc.json` file and return
 * the merged document. Per-network `*.mdc.json` fragments are merged by id;
 * `project.mdc.json` is excluded from the fragment scan.
 */
export async function loadProject(projectPath) {
  const info = await stat(projectPath);
  const dir = info.isDirectory() ? projectPath : dirname(projectPath);
  const rootFile = info.isDirectory()
    ? join(projectPath, "project.mdc.json")
    : projectPath;

  const project = JSON.parse(await readFile(rootFile, "utf8"));

  if (info.isDirectory()) {
    const fragments = (await readdir(dir))
      .filter((f) => f.endsWith(".mdc.json") && f !== "project.mdc.json")
      .sort();
    for (const f of fragments) {
      const full = join(dir, f);
      mergeNetworkFragment(project, JSON.parse(await readFile(full, "utf8")), full);
    }
  }
  return project;
}

/**
 * Load + schema-validate in one call. Returns `{ project, valid, errors }`.
 * Throws only on I/O / parse failure; schema failures are reported in `errors`.
 */
export async function loadAndValidate(projectPath, schemaPath = DEFAULT_SCHEMA) {
  const project = await loadProject(projectPath);
  const validate = await compileSchema(schemaPath);
  const valid = validate(project);
  return { project, valid, errors: valid ? [] : validate.errors ?? [] };
}

