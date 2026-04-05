import type { CanMessage, SearchFields } from "./types";

/** Which representation to use when matching the CAN ID portion of search (when “CAN IDs” is on). */
export type IdSearchFormat = "hex" | "decimal";

/** Case-insensitive substring match for simple UI lists. */
export function matchesQuery(q: string, ...parts: (string | undefined | null)[]): boolean {
  const needle = q.trim().toLowerCase();
  if (!needle) return true;
  const hay = parts
    .filter(Boolean)
    .join(" ")
    .toLowerCase();
  return hay.includes(needle);
}

/** Tokens for CAN ID search: only hex forms, or only decimal `frameId`, per `idFormat`. */
function frameIdSearchTokens(m: CanMessage, idFormat: IdSearchFormat): string[] {
  if (idFormat === "decimal") {
    return [String(m.frameId)];
  }
  const full = m.hexId.trim().toLowerCase();
  const digits = full.replace(/^0x/i, "");
  return [full, digits];
}

/** Search messages using optional field scopes (IDs, sender ECUs, message names, signal names & choice text). */
export function messageMatchesSearch(
  m: CanMessage,
  q: string,
  fields: SearchFields,
  idFormat: IdSearchFormat = "hex",
): boolean {
  const needle = q.trim().toLowerCase();
  if (!needle) return true;

  const hay: string[] = [];

  if (fields.ids) {
    hay.push(...frameIdSearchTokens(m, idFormat));
  }
  if (fields.ecus) {
    hay.push(...m.senders);
  }
  if (fields.msgNames) {
    hay.push(m.name);
    if (m.comment) hay.push(m.comment);
  }
  if (fields.sigNames) {
    for (const s of m.signals) {
      hay.push(s.name);
      if (s.comment) hay.push(s.comment);
      for (const c of s.choices) {
        hay.push(c.label, String(c.value));
      }
    }
  }

  return hay.join(" ").includes(needle);
}
