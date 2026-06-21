#!/usr/bin/env node
/** Structural check of staged MDC docs (frame_id/start/length); does not call mapVehicleDoc. */
import fs from "node:fs";
import path from "node:path";
import { fileURLToPath } from "node:url";

const here = path.dirname(fileURLToPath(import.meta.url));
const publicDir = path.join(here, "..", "public");
const indexPath = path.join(publicDir, "mdc-index.json");

if (!fs.existsSync(indexPath)) {
  console.error("smoke-map: run npm run copy-mdc first");
  process.exit(1);
}

const index = JSON.parse(fs.readFileSync(indexPath, "utf8"));
let networks = 0;
let messages = 0;
let signals = 0;

for (const rel of index) {
  const doc = JSON.parse(fs.readFileSync(path.join(publicDir, rel), "utf8"));
  if (!doc.schemaVersion?.startsWith("3.")) {
    throw new Error(`${rel}: expected schemaVersion 3.x, got ${doc.schemaVersion}`);
  }
  if (!doc.id || !Array.isArray(doc.networks)) {
    throw new Error(`${rel}: missing flat v3 root id/networks`);
  }
  for (const net of doc.networks) {
    networks += 1;
    for (const msg of net.messages ?? []) {
      messages += 1;
      if (typeof msg.frame_id !== "number") {
        throw new Error(`${rel}/${net.id}/${msg.name}: missing frame_id`);
      }
      for (const sig of msg.signals ?? []) {
        signals += 1;
        if (typeof sig.start !== "number" || typeof sig.length !== "number") {
          throw new Error(`${rel}/${msg.name}/${sig.name}: missing start/length`);
        }
      }
    }
  }
}

console.log(`smoke-map: ok — ${index.length} vehicle doc(s), ${networks} networks, ${messages} messages, ${signals} signals`);
