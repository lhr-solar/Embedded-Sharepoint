/**
 * Canonical CAN bus-load math — shared by tools/mdc-busload.mjs and the app editor.
 */
import { iterMessages } from "./model.mjs";

/** Standard CAN bitrates (bps), identical set + order everywhere. */
export const STANDARD_SPEEDS = [125000, 250000, 500000, 1000000, 33333, 50000, 800000];

const DEFAULT_CYCLE_TIME_MS = 1000;

/**
 * Worst-case bit count for one frame on the wire (ISO 11898-1 classic CAN with
 * maximum bit-stuffing; documented FD model with fixed stuff bits + sized CRC).
 */
export function bitsPerFrame({ length, isExtended = false, isFd = false }) {
  const dlc = length;
  const dataBits = 8 * dlc;
  if (isFd) {
    const crc = dlc > 16 ? 21 : 17;
    const overhead = isExtended ? 32 : 28;
    const fixedStuff = 6;
    return overhead + dataBits + crc + fixedStuff;
  }
  const base = isExtended ? 67 : 47;
  const stuffableBase = isExtended ? 39 : 34;
  const stuffing = Math.ceil((stuffableBase + dataBits) / 4);
  return base + dataBits + stuffing;
}

function messageBitsPerSecond(message) {
  const cycleTimeMs =
    typeof message.cycleTimeMs === "number" && message.cycleTimeMs > 0
      ? message.cycleTimeMs
      : DEFAULT_CYCLE_TIME_MS;
  const framesPerSecond = 1000 / cycleTimeMs;
  return (
    framesPerSecond *
    bitsPerFrame({
      length: message.length,
      isExtended: message.isExtended,
      isFd: message.isFd || message.protocol === "canfd",
    })
  );
}

/** True when a message counts toward bus load (cyclic, the schema default). */
export function isCyclic(message) {
  return (message.transmissionType ?? "cyclic") === "cyclic";
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

/**
 * Structured bus-load result for a whole project:
 * { networks: [{ vehicle, network, protocol, bitrate, cyclicMessageCount, speeds }] }
 */
export function computeBusLoad(project) {
  const byNetwork = new Map();
  for (const { vehicle, network, message } of iterMessages(project)) {
    const key = `${vehicle.id}/${network.id}`;
    if (!byNetwork.has(key)) byNetwork.set(key, { vehicle, network, messages: [] });
    byNetwork.get(key).messages.push({ ...message, protocol: network.protocol });
  }

  const networks = [];
  for (const { vehicle, network, messages } of byNetwork.values()) {
    networks.push({
      vehicle: vehicle.id,
      network: network.id,
      protocol: network.protocol,
      bitrate: network.bitrate ?? null,
      cyclicMessageCount: messages.filter(isCyclic).length,
      speeds: networkSpeeds(messages, network.bitrate),
    });
  }
  return { networks };
}

/** Sweep for a single network object. */
export function busLoadForNetwork(network) {
  const messages = (network?.messages ?? []).map((m) => ({ ...m, protocol: network.protocol }));
  return {
    protocol: network?.protocol,
    bitrate: network?.bitrate ?? null,
    cyclicMessageCount: messages.filter(isCyclic).length,
    speeds: networkSpeeds(messages, network?.bitrate),
  };
}

/** Format a bitrate in bps to a compact kbps/Mbps label. */
export function formatSpeed(bps) {
  if (bps >= 1_000_000) return `${(bps / 1_000_000).toFixed(bps % 1_000_000 ? 3 : 0)} Mbit/s`;
  return `${Math.round(bps / 1000)} kbit/s`;
}
