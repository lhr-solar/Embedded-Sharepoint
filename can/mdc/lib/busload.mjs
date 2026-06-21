/**
 * Canonical CAN bus-load math — shared by tools/mdc-busload.mjs and the app editor.
 */
import { iterMessages } from "./model.mjs";

/** Standard CAN bitrates (bps), identical set + order everywhere. */
export const STANDARD_SPEEDS = [125000, 250000, 500000, 1000000, 33333, 50000, 800000];

const DEFAULT_CYCLE_TIME_MS = 1000;

/** send_type labels excluded from bus load (contract §7). */
export const SEND_TYPE_TRIGGERED = new Set([
  "ifactive",
  "triggered",
  "event",
  "nosig",
  "nosigsend",
  "spontaneous",
  "none",
  "nomsgsendtype",
]);

/**
 * Worst-case bit count for one frame on the wire (ISO 11898-1 classic CAN with
 * maximum bit-stuffing; documented FD model with fixed stuff bits + sized CRC).
 */
export function bitsPerFrame({ length, is_extended_frame = false, is_fd = false }) {
  const dlc = length;
  const dataBits = 8 * dlc;
  if (is_fd) {
    const crc = dlc > 16 ? 21 : 17;
    const overhead = is_extended_frame ? 32 : 28;
    const fixedStuff = 6;
    return overhead + dataBits + crc + fixedStuff;
  }
  const base = is_extended_frame ? 67 : 47;
  const stuffableBase = is_extended_frame ? 39 : 34;
  const stuffing = Math.ceil((stuffableBase + dataBits) / 4);
  return base + dataBits + stuffing;
}

function messageBitsPerSecond(message) {
  const cycleTimeMs =
    typeof message.cycle_time === "number" && message.cycle_time > 0
      ? message.cycle_time
      : DEFAULT_CYCLE_TIME_MS;
  const framesPerSecond = 1000 / cycleTimeMs;
  return (
    framesPerSecond *
    bitsPerFrame({
      length: message.length,
      is_extended_frame: message.is_extended_frame,
      is_fd: message.is_fd,
    })
  );
}

/** True when a message counts toward bus load (not triggered + has cycle_time). */
export function isCyclic(message) {
  const label = (message.send_type ?? "").toLowerCase();
  if (SEND_TYPE_TRIGGERED.has(label)) return false;
  const ct = message.cycle_time;
  return typeof ct === "number" && ct > 0;
}

/** Bus-load percentage for a set of messages at a given bitrate. */
export function loadPercentAt(messages, bitrate) {
  const bitsPerSecond = messages
    .filter(isCyclic)
    .reduce((sum, m) => sum + messageBitsPerSecond(m), 0);
  return (bitsPerSecond / bitrate) * 100;
}

/** Per-network sweep: configured speed first, then ascending. */
export function networkSpeeds(messages, configured) {
  const speedSet = new Set(STANDARD_SPEEDS);
  if (typeof configured === "number") speedSet.add(configured);
  return [...speedSet]
    .map((speed) => ({
      speed,
      loadPercent: loadPercentAt(messages, speed),
      isConfigured: speed === configured,
    }))
    .sort((a, b) => {
      if (a.isConfigured !== b.isConfigured) return a.isConfigured ? -1 : 1;
      return a.speed - b.speed;
    });
}

function networkProtocolLabel(network) {
  return network?.fd_baudrate != null ? "canfd" : "can";
}

export { networkProtocolLabel };

/**
 * Structured bus-load result for a whole project:
 * { networks: [{ vehicle, network, protocol, baudrate, cyclicMessageCount, speeds }] }
 */
export function computeBusLoad(project) {
  const byNetwork = new Map();
  for (const { vehicle, network, message } of iterMessages(project)) {
    const key = `${vehicle.id}/${network.id}`;
    if (!byNetwork.has(key)) byNetwork.set(key, { vehicle, network, messages: [] });
    byNetwork.get(key).messages.push(message);
  }

  const networks = [];
  for (const { vehicle, network, messages } of byNetwork.values()) {
    networks.push({
      vehicle: vehicle.id,
      network: network.id,
      protocol: networkProtocolLabel(network), /* derived: "can" | "canfd" */
      baudrate: network.baudrate ?? null,
      cyclicMessageCount: messages.filter(isCyclic).length,
      speeds: networkSpeeds(messages, network.baudrate),
    });
  }
  return { networks };
}

/** Sweep for a single network object. */
export function busLoadForNetwork(network) {
  const messages = network?.messages ?? [];
  return {
    protocol: networkProtocolLabel(network),
    baudrate: network?.baudrate ?? null,
    cyclicMessageCount: messages.filter(isCyclic).length,
    speeds: networkSpeeds(messages, network?.baudrate),
  };
}

/** Format a bitrate in bps to a compact kbps/Mbps label. */
export function formatSpeed(bps) {
  if (bps >= 1_000_000) return `${(bps / 1_000_000).toFixed(bps % 1_000_000 ? 3 : 0)} Mbit/s`;
  return `${Math.round(bps / 1000)} kbit/s`;
}
