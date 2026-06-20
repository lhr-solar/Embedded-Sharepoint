#!/usr/bin/env node
/**
 * mdc2grafana — generate a Grafana dashboard JSON from MDC display/viz hints.
 *
 * Each signal with display hints becomes a panel; panels are grouped into rows
 * by `display.dashboardGroup` (falling back to the message name). chartType maps
 * to a Grafana panel type; precision/unit/scale become fieldConfig defaults.
 * The query target is left generic (refId + signal name) — the engine/datasource
 * wiring is out of scope here; this produces the layout + presentation.
 *
 * Usage: node tools/mdc2grafana.mjs <project-dir|project.mdc.json> [-o dash.json]
 */
import { writeFile } from "node:fs/promises";
import { loadProject, iterMessages } from "./lib/mdc-load.mjs";

const args = process.argv.slice(2);
const projectPath = args[0];
const outIdx = args.findIndex((a) => a === "-o" || a === "--output");
const outFile = outIdx >= 0 ? args[outIdx + 1] : null;
if (!projectPath) {
  console.error("Usage: node tools/mdc2grafana.mjs <project-dir|project.mdc.json> [-o dash.json]");
  process.exit(2);
}

const CHART_TO_PANEL = {
  line: "timeseries",
  scatter: "timeseries",
  gauge: "gauge",
  stat: "stat",
  state: "state-timeline",
  bar: "barchart",
  table: "table",
  heatmap: "heatmap",
};

const FORMAT_TO_UNIT = { percent: "percent", hex: "hex", bytes: "bytes", duration: "s" };

function fieldConfig(sig) {
  const d = sig.display ?? {};
  const defaults = {};
  if (d.unitOverride) defaults.unit = d.unitOverride;
  else if (d.format && FORMAT_TO_UNIT[d.format]) defaults.unit = FORMAT_TO_UNIT[d.format];
  else if (sig.unit) defaults.unit = sig.unit;
  if (typeof d.precision === "number") defaults.decimals = d.precision;
  if (typeof d.color === "string") defaults.color = { mode: "fixed", fixedColor: d.color };
  const min = d.scaleMin ?? sig.min;
  const max = d.scaleMax ?? sig.max;
  if (typeof min === "number") defaults.min = min;
  if (typeof max === "number") defaults.max = max;
  return { defaults, overrides: [] };
}

const project = await loadProject(projectPath);

// Collect displayable signals grouped by row title (stable insertion order).
const rows = new Map(); // title -> [{network, message, sig}]
for (const { network, message } of iterMessages(project)) {
  for (const sig of message.signals ?? []) {
    if (!sig.display) continue; // only signals with viz hints
    const title = sig.display.dashboardGroup || message.name;
    if (!rows.has(title)) rows.set(title, []);
    rows.get(title).push({ network, message, sig });
  }
}

const panels = [];
let panelId = 1;
let y = 0;
const COLS = 24;
const PANEL_W = 8;
const PANEL_H = 8;

for (const [title, entries] of rows) {
  panels.push({
    id: panelId++,
    type: "row",
    title,
    collapsed: false,
    gridPos: { h: 1, w: COLS, x: 0, y },
  });
  y += 1;
  let x = 0;
  for (const { network, message, sig } of entries) {
    if (x >= COLS) { x = 0; y += PANEL_H; }
    panels.push({
      id: panelId++,
      type: CHART_TO_PANEL[sig.display.chartType] ?? "timeseries",
      title: sig.name,
      description: sig.description ?? "",
      gridPos: { h: PANEL_H, w: PANEL_W, x, y },
      fieldConfig: fieldConfig(sig),
      options: {},
      targets: [
        {
          refId: "A",
          // Datasource-agnostic identifier the engine/datasource resolves.
          target: `${network.id}.${message.name}.${sig.name}`,
        },
      ],
    });
    x += PANEL_W;
  }
  y += PANEL_H;
}

const dashboard = {
  title: project.metadata?.name ? `${project.metadata.name} — Telemetry` : "MDC Telemetry",
  schemaVersion: 39,
  editable: true,
  time: { from: "now-15m", to: "now" },
  timezone: "browser",
  tags: ["mdc", "telemetry"],
  panels,
};

const text = JSON.stringify(dashboard, null, 2) + "\n";
if (outFile) {
  await writeFile(outFile, text);
  console.error(`Generated: ${outFile} (${panels.length} panels)`);
} else {
  process.stdout.write(text);
}
