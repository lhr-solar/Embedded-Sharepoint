#!/usr/bin/env node
/** Copy vehicle *.mdc.json docs into public/ for static gh-pages fetch. */
import fs from "node:fs";
import path from "node:path";
import { fileURLToPath } from "node:url";

const here = path.dirname(fileURLToPath(import.meta.url));
const uiRoot = path.join(here, "..");
const canRoot = path.join(uiRoot, "..");
const publicDir = path.join(uiRoot, "public");
const mdcDir = path.join(publicDir, "mdc");

const index = [];
const stagedVehicleIds = new Set();

function isV3Doc(srcPath) {
  try {
    const doc = JSON.parse(fs.readFileSync(srcPath, "utf8"));
    return typeof doc.schemaVersion === "string" && doc.schemaVersion.startsWith("3.");
  } catch {
    return false;
  }
}

function copyDoc(srcPath, relDest) {
  const dest = path.join(publicDir, relDest);
  fs.mkdirSync(path.dirname(dest), { recursive: true });
  fs.copyFileSync(srcPath, dest);
  index.push(relDest.replaceAll("\\", "/"));
  const vehicleId = relDest.split("/")[1];
  if (vehicleId) stagedVehicleIds.add(vehicleId);
}

function scanDir(vehicleDir, vehicleId) {
  if (!fs.existsSync(vehicleDir) || !fs.statSync(vehicleDir).isDirectory()) return;
  for (const file of fs.readdirSync(vehicleDir).sort((a, b) => a.localeCompare(b))) {
    if (!file.endsWith(".mdc.json")) continue;
    const src = path.join(vehicleDir, file);
    if (!isV3Doc(src)) continue;
    copyDoc(src, `mdc/${vehicleId}/${file}`);
  }
}

const vehiclesRoot = path.join(canRoot, "vehicles");
if (fs.existsSync(vehiclesRoot)) {
  for (const vehicleId of fs.readdirSync(vehiclesRoot).sort((a, b) => a.localeCompare(b))) {
    scanDir(path.join(vehiclesRoot, vehicleId), vehicleId);
  }
}

// ponytail: until tools track lands v3 vehicle docs, fill gaps from can/mdc/examples/.
const examplesRoot = path.join(canRoot, "mdc/examples");
if (fs.existsSync(examplesRoot)) {
  for (const vehicleId of fs.readdirSync(examplesRoot).sort((a, b) => a.localeCompare(b))) {
    if (stagedVehicleIds.has(vehicleId)) continue;
    scanDir(path.join(examplesRoot, vehicleId), vehicleId);
  }
}

if (index.length === 0) {
  const example = path.join(canRoot, "mdc/examples/lhr-ev1/project.mdc.json");
  if (fs.existsSync(example)) {
    copyDoc(example, "mdc/lhr-ev1/project.mdc.json");
  }
}

if (index.length === 0) {
  console.error("copy-mdc: no *.mdc.json found under can/vehicles/ or can/mdc/examples/");
  process.exit(1);
}

fs.mkdirSync(mdcDir, { recursive: true });
fs.writeFileSync(path.join(publicDir, "mdc-index.json"), `${JSON.stringify(index, null, 2)}\n`);
console.log(`copy-mdc: staged ${index.length} doc(s) → public/mdc-index.json`);
