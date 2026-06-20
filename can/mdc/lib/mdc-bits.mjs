/**
 * Motorola/Intel bit-position math — shared by mdc-validate, the editor grid, and
 * live validation. Follows the cantools/DBC startBit convention encoded in MDC.
 */

/** Bit positions occupied by a little-endian (Intel) field. */
function littleEndianBits(startBit, length) {
  const bits = [];
  for (let i = 0; i < length; i++) bits.push(startBit + i);
  return bits;
}

/**
 * Bit positions for a big-endian (Motorola, sawtooth) field. DBC numbers bits
 * within a byte MSB-first; consecutive field bits move to the next-lower bit in
 * the byte, wrapping to the MSB of the following byte.
 */
function bigEndianBits(startBit, length) {
  const bits = [];
  let bit = startBit;
  for (let i = 0; i < length; i++) {
    bits.push(bit);
    if (bit % 8 === 0) bit += 15;
    else bit -= 1;
  }
  return bits;
}

/** Absolute bit indices a signal occupies, endianness-aware. */
export function signalBitIndices(signal) {
  const start = signal.startBit ?? 0;
  const len = signal.lengthBits ?? 0;
  return signal.byteOrder === "big_endian"
    ? bigEndianBits(start, len)
    : littleEndianBits(start, len);
}

/** Alias used by semantic validators — same walk as signalBitIndices. */
export function occupiedBits(sig) {
  return signalBitIndices(sig);
}
