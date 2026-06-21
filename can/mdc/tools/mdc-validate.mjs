#!/usr/bin/env node
/**
 * mdc-validate — semantic linter for MDC v3 projects.
 *
 * Runs JSON Schema validation FIRST, then semantic checks via shared modules.
 * Schema-only mode:
 *   node tools/mdc-validate.mjs --schema-only <schema.json> <data.mdc.json>
 *
 * Full lint:
 *   node tools/mdc-validate.mjs <project-dir|project.mdc.json> [schema.json]
 *
 * Exit codes: 0 = clean, 1 = violations found, 2 = usage/IO error.
 */
import { readFile } from "node:fs/promises";
import { loadProject, compileSchema, formatErrors, DEFAULT_SCHEMA } from "../lib/mdc-load.mjs";
import { STANDARD_SPEEDS, SEND_TYPE_TRIGGERED, networkProtocolLabel } from "../lib/busload.mjs";
import {
  validateArrayBlock,
  validateBitOverlaps,
} from "../lib/mdc-validate-shared.mjs";

const argv = process.argv.slice(2);

if (argv[0] === "--schema-only") {
  const [, schemaPath, dataPath] = argv;
  if (!schemaPath || !dataPath) {
    console.error("Usage: node tools/mdc-validate.mjs --schema-only <schema.json> <data.mdc.json>");
    process.exit(2);
  }
  const validate = await compileSchema(schemaPath);
  const data = JSON.parse(await readFile(dataPath, "utf8"));
  if (validate(data)) {
    console.log(`${dataPath}: valid (${schemaPath})`);
    process.exit(0);
  }
  console.error(`${dataPath}: invalid (${schemaPath})`);
  for (const line of formatErrors(validate.errors)) console.error(line);
  process.exit(1);
}

const projectPath = argv[0];
const schemaPath = argv[1] ?? DEFAULT_SCHEMA;
if (!projectPath) {
  console.error(
    "Usage: node tools/mdc-validate.mjs <project-dir|project.mdc.json> [schema.json]\n" +
      "       node tools/mdc-validate.mjs --schema-only <schema.json> <data.mdc.json>",
  );
  process.exit(2);
}

const problems = [];
const warnings = [];
const add = (where, msg) => problems.push(`${where}: ${msg}`);
const warn = (where, msg) => warnings.push(`${where}: ${msg}`);

const project = await loadProject(projectPath);

const validate = await compileSchema(schemaPath);
if (!validate(project)) {
  console.error(`${projectPath}: schema-invalid`);
  for (const line of formatErrors(validate.errors)) console.error(line);
  process.exit(1);
}

const vehicleId = project.id ?? "project";
const seenNetworkIds = new Set();
for (const network of project.networks ?? []) {
  const netRef = `${vehicleId}/${network.id}`;
  if (seenNetworkIds.has(network.id)) add(netRef, `duplicate network id "${network.id}"`);
  seenNetworkIds.add(network.id);

  if (typeof network.baudrate === "number" && !STANDARD_SPEEDS.includes(network.baudrate)) {
    warn(netRef, `baudrate ${network.baudrate} is not a standard CAN speed (${STANDARD_SPEEDS.join(", ")})`);
  }

  const seenMsgIds = new Map();
  const maxDlc = network.fd_baudrate != null ? 64 : 8;

  for (const message of network.messages ?? []) {
    const msgRef = `${netRef}/${message.name}`;
    const idKey = `${message.frame_id}:${message.is_extended_frame ? "ext" : "std"}`;
    if (seenMsgIds.has(idKey)) {
      add(msgRef, `message id 0x${message.frame_id.toString(16)} collides with "${seenMsgIds.get(idKey)}"`);
    }
    seenMsgIds.set(idKey, message.name);

    if (message.length > maxDlc) {
      const proto = networkProtocolLabel(network);
      add(msgRef, `length ${message.length} exceeds ${proto} max DLC ${maxDlc}`);
    }

    const sendType = (message.send_type ?? "").toLowerCase();
    if (!SEND_TYPE_TRIGGERED.has(sendType) && typeof message.cycle_time === "number" && message.cycle_time <= 0) {
      warn(msgRef, `cyclic message has cycle_time ${message.cycle_time} (must be > 0)`);
    }

    validateBitOverlaps(message, (severity, msg) => {
      (severity === "error" ? add : warn)(msgRef, msg);
    });

    validateArrayBlock(message, (severity, msg) => {
      (severity === "error" ? add : warn)(msgRef, msg);
    });
  }
}

if (warnings.length > 0) {
  console.error(`${projectPath}: ${warnings.length} warning(s)`);
  for (const w of warnings) console.error(`  WARN ${w}`);
}

if (problems.length === 0) {
  console.log(`${projectPath}: clean (0 issues, ${warnings.length} warning(s))`);
  process.exit(0);
}

console.error(`${projectPath}: ${problems.length} semantic violation(s)`);
for (const p of problems) console.error(`  ${p}`);
process.exit(1);
