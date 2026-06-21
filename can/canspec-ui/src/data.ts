import type {
  CanMessage,
  CanNetwork,
  CanSignal,
  CanspecPayload,
  ChoiceEntry,
  SignalConversion,
  Vehicle,
} from "./types";

let cache: CanspecPayload | null = null;

// --- MDC v3 raw document shapes (snake_case, cantools field names) ---

type MdcChoice = { value: number; label: string };
type MdcValueTable = { name: string; entries?: MdcChoice[] };
type MdcConversion = {
  kind: string;
  scale?: number;
  offset?: number;
  choices?: MdcChoice[];
};

type MdcSignalV3 = {
  name: string;
  comment?: string;
  start: number;
  length: number;
  byte_order: string;
  is_signed: boolean;
  is_float: boolean;
  scale?: number;
  offset?: number;
  minimum?: number | null;
  maximum?: number | null;
  unit?: string;
  choices?: MdcChoice[];
  valueTableRef?: string;
  conversion?: MdcConversion;
  is_multiplexer?: boolean;
  multiplexer_signal?: string | null;
  multiplexer_ids?: number[] | null;
  spn?: number | null;
  initial?: number | null;
  invalid?: number | null;
  raw_initial?: number | null;
  raw_invalid?: number | null;
};

type MdcMessageV3 = {
  name: string;
  comment?: string;
  frame_id: number;
  is_extended_frame: boolean;
  is_fd: boolean;
  length: number;
  cycle_time?: number | null;
  send_type?: string;
  protocol?: string | null;
  senders?: string[];
  signals?: MdcSignalV3[];
  attributes?: Record<string, string | number | boolean | null>;
  multiplexing?: { multiplexed?: boolean };
};

type MdcNetworkV3 = {
  id: string;
  name?: string;
  comment?: string;
  filename?: string;
  baudrate?: number;
  fd_baudrate?: number | null;
  nodes?: { name: string; comment?: string }[];
  valueTables?: MdcValueTable[];
  messages?: MdcMessageV3[];
  attributes?: Record<string, string | number | boolean | null>;
};

type MdcVehicleDocV3 = {
  schemaVersion: string;
  id: string;
  name?: string;
  description?: string;
  networks: MdcNetworkV3[];
  valueTables?: MdcValueTable[];
};

function assetUrl(relativePath: string): string {
  const base = import.meta.env.BASE_URL;
  const path = relativePath.replace(/^\//, "");
  if (base === "./" || base === ".") {
    return `./${path}`;
  }
  const withSlash = base.endsWith("/") ? base : `${base}/`;
  return `${withSlash}${path}`;
}

function fmtId(frameId: number, isExtended: boolean): string {
  if (isExtended) {
    return `0x${frameId.toString(16).toUpperCase().padStart(8, "0")}`;
  }
  return `0x${frameId.toString(16).toUpperCase().padStart(3, "0")}`;
}

function mapChoices(entries?: MdcChoice[]): ChoiceEntry[] {
  if (!entries?.length) return [];
  return [...entries]
    .map((e) => ({ value: e.value, label: e.label }))
    .sort((a, b) => a.value - b.value);
}

function buildValueTableIndex(
  netTables: MdcValueTable[] | undefined,
  projectTables: MdcValueTable[] | undefined,
): Map<string, ChoiceEntry[]> {
  const out = new Map<string, ChoiceEntry[]>();
  // network tables first; project tables fill gaps (same name overwrites — net wins).
  for (const group of [netTables, projectTables]) {
    for (const vt of group ?? []) {
      out.set(vt.name, mapChoices(vt.entries));
    }
  }
  return out;
}

function mapConversion(conv?: MdcConversion): SignalConversion {
  if (!conv) return { kind: "linear" };
  return {
    kind: conv.kind,
    scale: conv.scale,
    offset: conv.offset,
    choices: conv.choices ? mapChoices(conv.choices) : undefined,
  };
}

function dataTypeFrom(sig: MdcSignalV3): string {
  if (sig.is_float) return "float";
  if (sig.is_signed) return "signed";
  return "unsigned";
}

function mapSignal(sig: MdcSignalV3, msg: MdcMessageV3, valueTables: Map<string, ChoiceEntry[]>): CanSignal {
  const conv = mapConversion(sig.conversion);
  let choices = mapChoices(sig.choices);
  if (!choices.length && conv.choices?.length) {
    choices = conv.choices;
  }
  if (!choices.length && sig.valueTableRef) {
    choices = valueTables.get(sig.valueTableRef) ?? [];
  }

  return {
    name: sig.name,
    message: msg.name,
    startBit: sig.start,
    lengthBits: sig.length,
    byteOrder: sig.byte_order,
    isSigned: sig.is_signed,
    isFloat: sig.is_float,
    dataType: dataTypeFrom(sig),
    conversion: conv,
    choices,
    scale: sig.scale ?? 1,
    offset: sig.offset ?? 0,
    minimum: sig.minimum ?? null,
    maximum: sig.maximum ?? null,
    unit: sig.unit ?? "",
    comment: sig.comment ?? "",
    isMultiplexer: Boolean(sig.is_multiplexer),
    multiplexerSignal: sig.multiplexer_signal ?? null,
    multiplexerIds: sig.multiplexer_ids?.length ? [...sig.multiplexer_ids] : null,
    spn: sig.spn ?? null,
    initial: sig.initial ?? null,
    invalid: sig.invalid ?? null,
    rawInitial: sig.raw_initial ?? null,
    rawInvalid: sig.raw_invalid ?? null,
  };
}

function mapMessage(msg: MdcMessageV3, busName: string, valueTables: Map<string, ChoiceEntry[]>): CanMessage {
  const signals = [...(msg.signals ?? [])]
    .sort((a, b) => a.start - b.start || a.name.localeCompare(b.name))
    .map((s) => mapSignal(s, msg, valueTables));

  return {
    name: msg.name,
    frameId: msg.frame_id,
    hexId: fmtId(msg.frame_id, msg.is_extended_frame),
    isExtended: msg.is_extended_frame,
    dlc: msg.length,
    senders: msg.senders ? [...msg.senders] : [],
    cycleTimeMs: msg.cycle_time ?? null,
    isFd: msg.is_fd,
    protocol: msg.protocol ?? null,
    busName,
    isMultiplexed: Boolean(msg.multiplexing?.multiplexed),
    attributes: msg.attributes ?? {},
    comment: msg.comment ?? "",
    signals,
  };
}

function mapNetwork(net: MdcNetworkV3, projectValueTables: MdcValueTable[] | undefined): CanNetwork {
  const valueTables = buildValueTableIndex(net.valueTables, projectValueTables);
  const busName = net.name ?? net.id;

  const messages = [...(net.messages ?? [])]
    .map((m) => mapMessage(m, busName, valueTables))
    .sort((a, b) => a.frameId - b.frameId);

  return {
    id: net.id,
    filename: net.filename ?? `${net.id}.mdc.json`,
    nodes: (net.nodes ?? []).map((n) => ({ name: n.name })),
    dbcAttributes: net.attributes ?? {},
    valueTables: [...(net.valueTables ?? [])].map((vt) => ({
      name: vt.name,
      entries: mapChoices(vt.entries),
    })),
    messages,
  };
}

function mapVehicleDoc(doc: MdcVehicleDocV3): Vehicle {
  return {
    id: doc.id,
    networks: doc.networks.map((n) => mapNetwork(n, doc.valueTables)),
  };
}

async function fetchJson<T>(url: string): Promise<T> {
  const res = await fetch(url);
  if (!res.ok) {
    throw new Error(`Failed to load ${url} (${res.status})`);
  }
  return (await res.json()) as T;
}

export async function loadCanspecData(): Promise<CanspecPayload> {
  if (cache) return cache;

  const index = await fetchJson<string[]>(assetUrl("mdc-index.json"));
  if (!index.length) {
    throw new Error("mdc-index.json is empty — run npm run copy-mdc before build");
  }

  const docs = await Promise.all(index.map((rel) => fetchJson<MdcVehicleDocV3>(assetUrl(rel))));
  cache = { vehicles: docs.map(mapVehicleDoc) };
  return cache;
}

export function findVehicle(data: CanspecPayload, vehicleId: string): Vehicle | undefined {
  return data.vehicles.find((v) => v.id === vehicleId);
}

export function findNetwork(vehicle: Vehicle, networkId: string) {
  return vehicle.networks.find((n) => n.id === networkId);
}

// Exported for scripts/smoke-map.mjs structural checks and future tooling.
export { mapVehicleDoc };
