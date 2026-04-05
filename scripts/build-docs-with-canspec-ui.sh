#!/usr/bin/env bash
# Local / manual only: GitHub Actions does not run this script; it runs the same
# steps inline in .github/workflows/build-docs.yml. Use this to build site/ like CI.
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
CANSPEC_UI="$ROOT/can/canspec-ui"
cd "$ROOT"

python3 "$CANSPEC_UI/export_canspec_json.py"
mkdocs build

cd "$CANSPEC_UI"
if [[ ! -d node_modules ]]; then
  npm ci
fi
npm run build
rm -rf "$ROOT/site/CAN-Spec"
cp -r dist "$ROOT/site/CAN-Spec"

echo "Built: $ROOT/site (MkDocs root + CAN spec UI at site/CAN-Spec/)"
