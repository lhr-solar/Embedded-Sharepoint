#!/usr/bin/env bash
# Submodule-aware: operates on this Embedded-Sharepoint repo only (not the parent superproject).
# graphify (graphifyy) is installed in Phase 14 via uv and may not be present yet.
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$REPO_ROOT"

if ! command -v graphify >/dev/null 2>&1; then
  echo "graphify not found — install in Phase 14: uv tool install graphifyy"
  exit 0
fi

mkdir -p .graphify
# Code-only AST extract; see graphify.yaml for intended include/exclude scope.
graphify extract . --out .graphify --no-viz
