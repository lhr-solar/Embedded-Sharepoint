/**
 * Browser-safe MDC document helpers — shared by the app editor and Node tools.
 */

/** Iterate every (project, network, message) tuple. v3: flat root, no vehicles[]. */
export function* iterMessages(project) {
  const vehicle = { id: project?.id ?? "project" };
  for (const network of project?.networks ?? []) {
    for (const message of network.messages ?? []) {
      yield { vehicle, network, message };
    }
  }
}

/**
 * Resolve a signal's value-table entries from inline `choices`, a network-local
 * `valueTableRef`, or the project-shared table.
 */
export function resolveValueEntries(project, network, signal) {
  if (Array.isArray(signal?.choices) && signal.choices.length > 0) return signal.choices;
  const ref = signal?.valueTableRef;
  if (!ref) return [];
  const local = (network?.valueTables ?? []).find((t) => t.name === ref);
  if (local) return local.entries ?? [];
  const shared = (project?.valueTables ?? []).find((t) => t.name === ref);
  return shared?.entries ?? [];
}

/** Alias for codegen tools. */
export function resolveChoices(sig, network, project) {
  const entries = resolveValueEntries(project, network, sig);
  return entries.length > 0 ? entries : null;
}
