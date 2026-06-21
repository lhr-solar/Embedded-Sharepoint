/**
 * Shared semantic checks for MDC messages — array integrity and multiplexing.
 * Used by mdc/tools/mdc-validate.mjs and the editor's live validate.js.
 */
import { occupiedBits } from "./mdc-bits.mjs";

export function setsIntersect(a, b) {
  const [small, large] = a.size <= b.size ? [a, b] : [b, a];
  for (const x of small) if (large.has(x)) return true;
  return false;
}

/**
 * Two signals can be present in the same frame unless both are gated by the SAME
 * multiplexer selector with disjoint ids. A null id set means "always present";
 * signals gated by different selectors (extended multiplexing) can be active at
 * the same time, so they count as potentially co-present.
 */
export function simultaneouslyPresent(aName, aIds, bName, bIds) {
  if (!aIds || !bIds) return true;
  if (aName !== bName) return true;
  return setsIntersect(aIds, bIds);
}

/**
 * Validate a message-level `array` block. `report(severity, message)` is called
 * for each finding (severity: 'error' | 'warning').
 */
export function validateArrayBlock(message, report) {
  const array = message.array;
  if (!array) return;

  const sigByName = new Map((message.signals ?? []).map((s) => [s.name, s]));

  const indexSig = sigByName.get(array.indexSignal);
  if (!array.indexSignal) report("error", "array.indexSignal is required");
  else if (!indexSig)
    report("error", `array.indexSignal "${array.indexSignal}" does not resolve to a signal in this message`);

  if (!Array.isArray(array.elementSignals) || array.elementSignals.length === 0)
    report("error", "array.elementSignals must list at least one signal");

  for (const elemName of array.elementSignals ?? []) {
    if (!sigByName.has(elemName))
      report("error", `array.elementSignals entry "${elemName}" does not resolve to a signal in this message`);
    if (elemName === array.indexSignal)
      report("error", `array.indexSignal "${array.indexSignal}" must not also be an elementSignals entry`);
  }

  if (typeof array.size === "number" && indexSig) {
    const bitLen = indexSig.length ?? indexSig.lengthBits;
    const capacity = 2 ** bitLen;
    if (array.size > capacity)
      report(
        "error",
        `array.size ${array.size} cannot fit index signal "${indexSig.name}" (${bitLen} bits, max ${capacity} values)`,
      );
  }
}

function muxGateForSignal(sig) {
  // Selector signals and unconditional signals are always present (null id set).
  // A gated signal carries the selector it answers to plus the ids that enable it.
  if (sig.is_multiplexer) return { name: null, ids: null };
  if (sig.multiplexer_ids?.length)
    return { name: sig.multiplexer_signal ?? "", ids: new Set(sig.multiplexer_ids) };
  return { name: null, ids: null };
}

/**
 * Bit-overlap check using occupied-bit Sets (correct for Motorola sawtooth).
 * `report(severity, message)` per finding.
 */
export function validateBitOverlaps(message, report, { includeMultiplexed = true } = {}) {
  const frameBits = (message.length ?? 0) * 8;
  const muxNames = new Set();
  for (const sig of message.signals ?? []) {
    if (sig.is_multiplexer) muxNames.add(sig.name);
  }

  const placed = [];
  for (const sig of message.signals ?? []) {
    if (!includeMultiplexed && sig.multiplexer_ids?.length) continue;

    const bits = occupiedBits(sig);
    const highBit = bits.length ? Math.max(...bits) : -1;
    if (highBit >= frameBits && frameBits > 0)
      report("warning", `${sig.name} overflows the ${message.length}-byte payload (bit ${highBit})`);

    const gate = muxGateForSignal(sig);
    const bitSet = new Set(bits);
    for (const prior of placed) {
      if (!setsIntersect(prior.bits, bitSet)) continue;
      if (!simultaneouslyPresent(prior.gate.name, prior.gate.ids, gate.name, gate.ids)) continue;
      const shared = [...bitSet].find((b) => prior.bits.has(b));
      report("warning", `bit overlap: ${prior.name} and ${sig.name} share bit ${shared}`);
      break;
    }
    placed.push({ name: sig.name, bits: bitSet, gate });
  }

  if (message.multiplexing?.multiplexed && muxNames.size === 0)
    report("error", 'multiplexing.multiplexed is true but no signal has is_multiplexer');

  for (const sig of message.signals ?? []) {
    if (sig.multiplexer_signal && !muxNames.has(sig.multiplexer_signal))
      report("error", `multiplexer_signal "${sig.multiplexer_signal}" has no matching multiplexor signal`);
  }
}
