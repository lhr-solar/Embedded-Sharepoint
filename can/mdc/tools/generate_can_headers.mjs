#!/usr/bin/env node
/**
 * generate_can_headers — generate a C header from an MDC v3 project.
 *
 * Emits CAN id + DLC macros, value-table enums, and per-message structs.
 *
 * Usage: node tools/generate_can_headers.mjs <project-dir|project.mdc.json> [-o out.h]
 */
import { writeFile } from "node:fs/promises";
import { loadProject, iterMessages, resolveChoices } from "../lib/mdc-load.mjs";

const args = process.argv.slice(2);
const projectPath = args[0];
const outIdx = args.findIndex((a) => a === "-o" || a === "--output");
const outFile = outIdx >= 0 ? args[outIdx + 1] : null;
if (!projectPath) {
  console.error("Usage: node tools/generate_can_headers.mjs <project-dir|project.mdc.json> [-o out.h]");
  process.exit(2);
}

const sanitize = (name) => {
  let s = String(name).replace(/\W+/g, "_");
  if (/^[0-9]/.test(s)) s = "_" + s;
  return s;
};

function cType(sig) {
  if (sig.is_float) return sig.length > 32 ? "double" : "float";
  const signed = sig.is_signed;
  const w = sig.length;
  const base = w <= 8 ? "8" : w <= 16 ? "16" : w <= 32 ? "32" : "64";
  return `${signed ? "int" : "uint"}${base}_t`;
}

const project = await loadProject(projectPath);

const idMacros = [];
const lenMacros = [];
const enums = [];
const structs = [];

const messages = [...iterMessages(project)].sort((a, b) => a.message.frame_id - b.message.frame_id);
for (const { network, message } of messages) {
  const macro = sanitize(message.name.toUpperCase());
  idMacros.push(`#define CAN_ID_${macro} 0x${message.frame_id.toString(16).toUpperCase()}`);
  lenMacros.push(`#define CAN_DLC_${macro} ${message.length}`);

  const structName = sanitize(message.name.toLowerCase() + "_t");
  const members = [];
  for (const sig of message.signals ?? []) {
    members.push(`    ${cType(sig)} ${sanitize(sig.name)};`);
    const choices = resolveChoices(sig, network, project);
    if (choices?.length) {
      const enumName = sanitize(`${message.name}_${sig.name}_e`.toLowerCase());
      const lines = ["typedef enum {"];
      for (const c of choices) {
        const member = sanitize(`${message.name}_${sig.name}_${c.label}`.toUpperCase());
        lines.push(`    ${member} = ${c.value},`);
      }
      lines.push(`} ${enumName};`);
      enums.push(lines.join("\n"));
    }
  }
  structs.push(["typedef struct {", ...members, `} ${structName};`].join("\n"));
}

const out = [];
out.push("#pragma once");
out.push("");
out.push("#include <stdint.h>");
out.push("");
out.push("/* ================= CAN ID Macros ================= */");
out.push("");
out.push(...idMacros);
out.push("");
out.push("/* ================= CAN Length Macros ================= */");
out.push("");
out.push(...lenMacros);
out.push("");
if (enums.length) {
  out.push("/* ================= Value Table Enums ================= */");
  out.push("");
  out.push(enums.join("\n\n"));
  out.push("");
}
out.push("/* ================= Message Structs ================= */");
out.push("");
out.push(structs.join("\n\n"));
out.push("");

const text = out.join("\n");
if (outFile) {
  await writeFile(outFile, text);
  console.error(`Generated: ${outFile}`);
} else {
  process.stdout.write(text);
}
