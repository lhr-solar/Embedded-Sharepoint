#!/usr/bin/env node
/**
 * mdc-v2-to-v3.mjs — migrate MDC v2 (vehicles[] root) to v3 (flattened per-vehicle).
 *
 * Usage:
 *   node tools/mdc-v2-to-v3.mjs <input.mdc.json> [output.mdc.json]
 *   node tools/mdc-v2-to-v3.mjs --all <input.mdc.json> <output-dir/>
 *   node tools/mdc-v2-to-v3.mjs --vehicle <id> <input.mdc.json> [output.mdc.json]
 *
 * With one output path and multiple vehicles, migrates vehicles[0] only (warns).
 * --all writes <output-dir>/<vehicle-id>.mdc.json per vehicle.
 * --vehicle picks a vehicle by id.
 */
import { readFile, writeFile, mkdir } from "node:fs/promises";
import { dirname, join } from "node:path";

const SCHEMA = "https://lhrsolar.org/lhrs-mdc/3.0.0/mdc.schema.json";
const SEND_TYPE_MAP = { cyclic: "Cyclic", triggered: "Triggered" };

function kindToFlags(kind) {
  switch (kind) {
    case "signed":
      return { is_signed: true, is_float: false };
    case "float":
      return { is_signed: false, is_float: true };
    default:
      return { is_signed: false, is_float: false };
  }
}

function migrateConversion(conversion) {
  if (!conversion) return {};
  const { kind, factor, offset, numerator, denominator } = conversion;
  if (kind === "identity") return {};
  if (kind === "linear") {
    return {
      scale: factor ?? 1,
      offset: offset ?? 0,
    };
  }
  if (kind === "rational") {
    return {
      conversion: { kind: "rational", numerator, denominator, offset: offset ?? 0 },
    };
  }
  if (kind === "table") {
    return { conversion: { kind: "table" } };
  }
  return {};
}

function migrateMultiplexer(mux) {
  if (!mux || mux.role === "none") return {};
  if (mux.role === "multiplexor") return { is_multiplexer: true };
  if (mux.role === "multiplexed") {
    const out = { multiplexer_ids: mux.ids ?? [] };
    if (mux.multiplexorName) out.multiplexer_signal = mux.multiplexorName;
    return out;
  }
  return {};
}

function migrateSignal(sig) {
  const out = {};
  for (const [k, v] of Object.entries(sig)) {
    if (k === "description") out.comment = v;
    else if (k === "startBit") out.start = v;
    else if (k === "lengthBits") out.length = v;
    else if (k === "byteOrder") out.byte_order = v;
    else if (k === "kind") Object.assign(out, kindToFlags(v));
    else if (k === "min") out.minimum = v;
    else if (k === "max") out.maximum = v;
    else if (k === "conversion") Object.assign(out, migrateConversion(v));
    else if (k === "multiplexer") Object.assign(out, migrateMultiplexer(v));
    else if (k === "attributes") out.attributes = { ...v };
    else if (!["description", "startBit", "lengthBits", "byteOrder", "kind", "min", "max", "conversion", "multiplexer"].includes(k)) {
      out[k] = v;
    }
  }

  // SPN promotion: attributes.SPN → native spn
  if (out.attributes?.SPN != null) {
    if (out.spn == null) out.spn = out.attributes.SPN;
    const { SPN, ...rest } = out.attributes;
    out.attributes = Object.keys(rest).length ? rest : undefined;
    if (!out.attributes) delete out.attributes;
  }

  return out;
}

function migrateComputedSignal(cs) {
  const out = {};
  for (const [k, v] of Object.entries(cs)) {
    if (k === "description") out.comment = v;
    else if (k === "kind") Object.assign(out, kindToFlags(v));
    else if (k === "min") out.minimum = v;
    else if (k === "max") out.maximum = v;
    else out[k] = v;
  }
  if (out.is_float === undefined && out.is_signed === undefined) {
    out.is_float = true;
    out.is_signed = false;
  }
  return out;
}

function migrateMessage(msg) {
  const out = {};
  for (const [k, v] of Object.entries(msg)) {
    if (k === "description") out.comment = v;
    else if (k === "id") out.frame_id = v;
    else if (k === "isExtended") out.is_extended_frame = v;
    else if (k === "isFd") out.is_fd = v;
    else if (k === "cycleTimeMs") out.cycle_time = v;
    else if (k === "transmissionType") out.send_type = SEND_TYPE_MAP[v] ?? v;
    else if (k === "signals") out.signals = v.map(migrateSignal);
    else if (k === "computedSignals") out.computedSignals = v.map(migrateComputedSignal);
    else if (k === "contained_messages") out.contained_messages = v.map(migrateContainedMessage);
    else if (!["description", "id", "isExtended", "isFd", "cycleTimeMs", "transmissionType"].includes(k)) {
      out[k] = v;
    }
  }
  if (out.send_type === undefined && out.cycle_time != null) out.send_type = "Cyclic";
  return out;
}

function migrateContainedMessage(cm) {
  const out = {};
  for (const [k, v] of Object.entries(cm)) {
    if (k === "description") out.comment = v;
    else if (k === "signals") out.signals = v.map(migrateSignal);
    else out[k] = v;
  }
  return out;
}

function migrateNode(node) {
  const out = { ...node };
  if ("description" in out) {
    out.comment = out.description;
    delete out.description;
  }
  return out;
}

function migrateNetwork(net) {
  const out = {};
  for (const [k, v] of Object.entries(net)) {
    if (k === "description") out.comment = v;
    else if (k === "bitrate") out.baudrate = v;
    else if (k === "dataBitrate") out.fd_baudrate = v;
    else if (k === "protocol") {
      // ponytail: drop protocol enum; fd_baudrate presence implies CAN FD
      if (v === "canfd" && out.fd_baudrate == null && net.dataBitrate == null) {
        // no dataBitrate in source — leave fd_baudrate absent
      }
    } else if (k === "nodes") out.nodes = v.map(migrateNode);
    else if (k === "messages") out.messages = v.map(migrateMessage);
    else if (k === "computedSignals") out.computedSignals = v.map(migrateComputedSignal);
    else if (!["description", "bitrate", "dataBitrate", "protocol"].includes(k)) {
      out[k] = v;
    }
  }
  return out;
}

function migrateVehicle(v2, vehicle) {
  const out = {
    $schema: SCHEMA,
    schemaVersion: "3.0.0",
    id: vehicle.id,
    networks: (vehicle.networks ?? []).map(migrateNetwork),
  };
  if (vehicle.name != null) out.name = vehicle.name;
  if (vehicle.description != null) out.description = vehicle.description;
  if (vehicle.attributes != null) out.attributes = vehicle.attributes;

  if (v2.metadata != null) out.metadata = v2.metadata;
  if (v2.attributeDefinitions != null) out.attributeDefinitions = v2.attributeDefinitions;
  if (v2.valueTables != null) out.valueTables = v2.valueTables;

  return out;
}

function parseArgs(argv) {
  let all = false;
  let vehicleId = null;
  const rest = [];
  for (let i = 0; i < argv.length; i++) {
    if (argv[i] === "--all") all = true;
    else if (argv[i] === "--vehicle") vehicleId = argv[++i];
    else rest.push(argv[i]);
  }
  return { all, vehicleId, rest };
}

async function main() {
  const { all, vehicleId, rest } = parseArgs(process.argv.slice(2));
  const [inputPath, outputPath] = rest;
  if (!inputPath) {
    console.error(
      "Usage: node tools/mdc-v2-to-v3.mjs <input.mdc.json> [output.mdc.json]\n" +
        "       node tools/mdc-v2-to-v3.mjs --all <input.mdc.json> <output-dir/>\n" +
        "       node tools/mdc-v2-to-v3.mjs --vehicle <id> <input.mdc.json> [output.mdc.json]",
    );
    process.exit(2);
  }

  const v2 = JSON.parse(await readFile(inputPath, "utf8"));
  const vehicles = v2.vehicles ?? [];
  if (!vehicles.length) {
    console.error(`${inputPath}: no vehicles[] — already v3 or invalid v2`);
    process.exit(1);
  }

  if (all) {
    if (!outputPath) {
      console.error("--all requires <output-dir/>");
      process.exit(2);
    }
    await mkdir(outputPath, { recursive: true });
    for (const vehicle of vehicles) {
      const out = migrateVehicle(v2, vehicle);
      const file = join(outputPath, `${vehicle.id}.mdc.json`);
      await writeFile(file, JSON.stringify(out, null, 2) + "\n");
      console.log(`Wrote ${file}`);
    }
    return;
  }

  let vehicle;
  if (vehicleId) {
    vehicle = vehicles.find((v) => v.id === vehicleId);
    if (!vehicle) {
      console.error(`Vehicle id "${vehicleId}" not found in ${inputPath}`);
      process.exit(1);
    }
  } else {
    vehicle = vehicles[0];
    if (vehicles.length > 1) {
      console.warn(`Warning: ${vehicles.length} vehicles; migrating vehicles[0] (${vehicle.id}). Use --vehicle or --all.`);
    }
  }

  const out = migrateVehicle(v2, vehicle);
  const dest = outputPath ?? inputPath.replace(/\.mdc\.json$/, `.v3.mdc.json`);
  if (outputPath) await mkdir(dirname(outputPath), { recursive: true }).catch(() => {});
  await writeFile(dest, JSON.stringify(out, null, 2) + "\n");
  console.log(`Wrote ${dest}`);
}

main().catch((err) => {
  console.error(err);
  process.exit(2);
});
