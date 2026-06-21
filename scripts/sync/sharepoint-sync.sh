#!/usr/bin/env bash
# Export shared Sharepoint assets to a parent superproject.
set -euo pipefail

DRY_RUN=0
if [[ "${1:-}" == "--dry-run" ]]; then
  DRY_RUN=1
  shift
fi

if [[ $# -lt 1 ]]; then
  echo "Usage: sharepoint-sync.sh [--dry-run] <dest-superproject-dir>" >&2
  exit 1
fi

DEST="$1"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
MANIFEST="$SCRIPT_DIR/sync-manifest.txt"

copied=0
skipped=0

while IFS= read -r line || [[ -n "$line" ]]; do
  line="${line%%#*}"
  line="${line#"${line%%[![:space:]]*}"}"
  line="${line%"${line##*[![:space:]]}"}"
  [[ -z "$line" ]] && continue

  src="$REPO_ROOT/$line"
  if [[ ! -e "$src" ]]; then
    echo "skip (missing): $line"
    skipped=$((skipped + 1))
    continue
  fi

  if [[ $DRY_RUN -eq 1 ]]; then
    echo "would copy: $line"
  else
    cd "$REPO_ROOT"
    if command -v rsync >/dev/null 2>&1; then
      rsync -a --relative "./$line" "$DEST/"
    else
      dest_path="$DEST/$line"
      mkdir -p "$(dirname "$dest_path")"
      cp -R "$src" "$dest_path"
    fi
  fi
  copied=$((copied + 1))
done < "$MANIFEST"

echo "summary: copied=$copied skipped=$skipped"
