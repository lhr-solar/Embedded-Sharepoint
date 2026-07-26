#!/usr/bin/env bash
# Syntax-check a Claude Code workflow script. Workflow bodies are not plain
# ESM (top-level `return` is legal because the runtime wraps them), so wrap
# in an async arrow that declares the runtime globals before parsing.
set -euo pipefail
f="${1:?usage: check.sh <workflow.js>}"
tmp="$(mktemp --suffix=.mjs)"
trap 'rm -f "$tmp"' EXIT
{
  echo '(async (args, agent, pipeline, parallel, phase, log, workflow, budget) => {'
  sed 's/^export //' "$f"
  echo '})'
} > "$tmp"
node --check "$tmp" && echo "OK: $f"
