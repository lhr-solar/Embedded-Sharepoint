#!/usr/bin/env node
/**
 * mdc-busload — compute per-network CAN bus load from an MDC project.
 *
 * Reuses mdc/lib/busload.mjs (canonical formula) and tools/lib/mdc-load.mjs.
 * Usage: node tools/mdc-busload.mjs <project-dir|project.mdc.json> [--self-check]
 */
import { loadProject } from "./lib/mdc-load.mjs";
import {
  STANDARD_SPEEDS,
  bitsPerFrame,
  loadPercentAt,
  computeBusLoad,
} from "../mdc/lib/busload.mjs";

export { STANDARD_SPEEDS, bitsPerFrame, loadPercentAt, computeBusLoad };

function printTable(result) {
  for (const net of result.networks) {
    const cfg = net.bitrate != null ? `${net.bitrate} bps` : "unset";
    console.log(
      `\n${net.vehicle}/${net.network} (${net.protocol}, configured ${cfg}, ${net.cyclicMessageCount} cyclic msgs)`,
    );
    console.log("  speed (bps)   load %    configured");
    for (const { speed, loadPercent, isConfigured } of net.speeds) {
      const mark = isConfigured ? "  <— configured" : "";
      console.log(`  ${String(speed).padStart(10)}  ${loadPercent.toFixed(2).padStart(7)}${mark}`);
    }
  }
}

function assertClose(actual, expected, tol, label) {
  if (Math.abs(actual - expected) > tol) {
    throw new Error(`self-check failed: ${label} = ${actual}, expected ${expected} (±${tol})`);
  }
}

function selfCheck() {
  const bits = bitsPerFrame({ length: 8, isExtended: false, isFd: false });
  assertClose(bits, 136, 0, "bitsPerFrame(8B std)");
  const load = loadPercentAt([{ length: 8, cycleTimeMs: 100 }], 500000);
  assertClose(load, 0.272, 1e-6, "loadPercentAt(8B@100ms, 500k)");
  const triggered = loadPercentAt(
    [{ length: 8, cycleTimeMs: 100, transmissionType: "triggered" }],
    500000,
  );
  assertClose(triggered, 0, 1e-9, "triggered excluded");
  console.log("mdc-busload self-check: OK");
}

const isMain = process.argv[1] && import.meta.url === `file://${process.argv[1]}`;
if (isMain) {
  const arg = process.argv[2];
  if (arg === "--self-check") {
    selfCheck();
    process.exit(0);
  }
  if (!arg) {
    console.error("Usage: node tools/mdc-busload.mjs <project-dir|project.mdc.json> [--self-check]");
    process.exit(2);
  }
  const project = await loadProject(arg);
  printTable(computeBusLoad(project));
}
