#!/usr/bin/env python3
"""
Reorder DBC exports (e.g. from candb++) for stable, ascending layout:

  * Messages: ascending by frame ID
  * Signals in each message: ascending by the DBC ``start`` field (number before ``|``), then name
  * VAL_ (per-signal choices): ascending by numeric value
  * VAL_TABLE_: ascending by numeric value (cantools dumps these descending; we fix)

Requires: cantools (see repo requirements.txt)

Usage (interactive — pick vehicle, then DBCs or \"all\"):
  python3 reorder_dbc.py

Usage (non-interactive):
  python3 reorder_dbc.py --vehicle Mcqueen --all
  python3 reorder_dbc.py --vehicle Daybreak --only bps.dbc,controls.dbc

By default writes <name>.reordered.dbc next to the source. Use --in-place to
overwrite the original. Use --backup with --in-place to copy each source to
<name>.dbc.bak before overwriting.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

try:
    import cantools.database
except ImportError as e:
    print("reorder_dbc: install cantools (e.g. pip install -r requirements.txt)", file=sys.stderr)
    raise SystemExit(1) from e

from cantools.database.utils import sort_choices_by_value


def sort_signals_by_dbc_start_field(signals: list) -> list:
    """Ascending by the DBC SG_ start index (number before |), then name.

    Note: cantools' ``sort_signals_by_start_bit`` uses physical ``start_bit()``,
    which can reorder Intel vs Motorola fields differently from the authored
    ``start`` field — we match the DBC line order the user expects.
    """
    return sorted(signals, key=lambda s: (s.start, s.name.lower()))

_HERE = Path(__file__).resolve().parent

VAL_TABLE_LINE = re.compile(r"^VAL_TABLE_\s+(\S+)\s+(.+?)\s*;\s*$")
CHOICE_PAIR = re.compile(r"(-?\d+)\s+\"((?:[^\"\\]|\\.)*)\"")


def fix_val_tables_ascending(dbc_text: str) -> str:
    """Rewrite VAL_TABLE_ lines so value entries are ascending (cantools dumps them descending)."""
    lines_out: list[str] = []
    for line in dbc_text.splitlines():
        m = VAL_TABLE_LINE.match(line)
        if not m:
            lines_out.append(line)
            continue
        name, rest = m.group(1), m.group(2).strip()
        if not rest:
            lines_out.append(line)
            continue
        pairs = CHOICE_PAIR.findall(rest)
        if not pairs:
            lines_out.append(line)
            continue
        sorted_pairs = sorted(((int(k), v) for k, v in pairs), key=lambda x: x[0])
        inner = " ".join(f'{n} "{v}"' for n, v in sorted_pairs)
        lines_out.append(f"VAL_TABLE_ {name} {inner} ;")
    out = "\n".join(lines_out)
    if dbc_text.endswith("\n"):
        out += "\n"
    return out


def list_vehicles(root: Path) -> list[Path]:
    out: list[Path] = []
    for p in sorted(root.iterdir(), key=lambda x: x.name.lower()):
        if not p.is_dir():
            continue
        if p.name.startswith("."):
            continue
        if any(p.glob("*.dbc")):
            out.append(p)
    return out


def list_dbcs(vehicle_dir: Path) -> list[Path]:
    return sorted(vehicle_dir.glob("*.dbc"), key=lambda p: p.name.lower())


def pick_vehicle_interactive(vehicles: list[Path]) -> Path:
    print("\nVehicles (subfolders of {}):".format(_HERE))
    for i, v in enumerate(vehicles, 1):
        n = len(list_dbcs(v))
        print(f"  {i}) {v.name}  ({n} .dbc)")
    while True:
        raw = input("\nSelect vehicle [1-{} or name]: ".format(len(vehicles))).strip()
        if not raw:
            continue
        if raw.isdigit():
            k = int(raw)
            if 1 <= k <= len(vehicles):
                return vehicles[k - 1]
            print("Out of range.")
            continue
        for v in vehicles:
            if v.name.lower() == raw.lower():
                return v
        print("No match; try again.")


def pick_dbcs_interactive(dbcs: list[Path]) -> list[Path]:
    print("\nDBC files in this vehicle:")
    for i, p in enumerate(dbcs, 1):
        print(f"  {i}) {p.name}")
    print('  a) Select ALL')
    while True:
        raw = input('\nSelect DBCs (comma-separated numbers, or "a" for all): ').strip().lower()
        if raw in ("a", "all"):
            return list(dbcs)
        if not raw:
            continue
        parts = [p.strip() for p in raw.split(",") if p.strip()]
        chosen: list[Path] = []
        ok = True
        for part in parts:
            if not part.isdigit():
                ok = False
                break
            k = int(part)
            if not (1 <= k <= len(dbcs)):
                ok = False
                break
            chosen.append(dbcs[k - 1])
        if ok and chosen:
            return chosen
        print("Invalid selection; use e.g. 1,3 or a")


def reorder_one_dbc(src: Path, dest: Path) -> None:
    db = cantools.database.load_file(str(src))
    db.messages.sort(key=lambda m: m.frame_id)

    text = db.as_dbc_string(
        sort_signals=sort_signals_by_dbc_start_field,
        sort_attribute_signals=sort_signals_by_dbc_start_field,
        sort_choices=sort_choices_by_value,
    )
    text = fix_val_tables_ascending(text)
    dest.parent.mkdir(parents=True, exist_ok=True)
    dest.write_text(text, encoding="utf-8", newline="\n")


def run_batch(
    vehicle_dir: Path,
    dbcs: list[Path],
    *,
    in_place: bool,
    backup: bool,
) -> None:
    for src in dbcs:
        if in_place:
            if backup:
                bak = src.with_suffix(src.suffix + ".bak")
                bak.write_bytes(src.read_bytes())
                print(f"Backup: {bak}")
            dest = src
        else:
            dest = src.with_name(src.stem + ".reordered" + src.suffix)
        reorder_one_dbc(src, dest)
        print(f"Wrote: {dest}")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument(
        "--root",
        type=Path,
        default=_HERE,
        help="Folder containing vehicle subfolders (default: directory of this script)",
    )
    ap.add_argument("--vehicle", type=str, help="Vehicle folder name (non-interactive)")
    ap.add_argument("--all", action="store_true", help="Process every .dbc in the vehicle (use with --vehicle)")
    ap.add_argument(
        "--only",
        type=str,
        help="Comma-separated DBC file names in the vehicle folder (non-interactive)",
    )
    ap.add_argument(
        "--in-place",
        action="store_true",
        help="Overwrite each selected .dbc (default: write *.reordered.dbc alongside)",
    )
    ap.add_argument(
        "--backup",
        action="store_true",
        help="With --in-place only: copy each source to <name>.dbc.bak before overwriting",
    )
    args = ap.parse_args()

    if args.backup and not args.in_place:
        print("reorder_dbc: --backup only applies with --in-place", file=sys.stderr)
        raise SystemExit(1)

    root: Path = args.root.resolve()
    if not root.is_dir():
        print(f"Not a directory: {root}", file=sys.stderr)
        raise SystemExit(1)

    vehicles = list_vehicles(root)
    if not vehicles:
        print(f"No vehicle subfolders with .dbc files under {root}", file=sys.stderr)
        raise SystemExit(1)

    if args.vehicle:
        vehicle_dir = next((v for v in vehicles if v.name.lower() == args.vehicle.lower()), None)
        if vehicle_dir is None:
            print(f"Unknown vehicle {args.vehicle!r}. Options: {', '.join(v.name for v in vehicles)}", file=sys.stderr)
            raise SystemExit(1)
        dbcs_all = list_dbcs(vehicle_dir)
        if args.all:
            chosen = dbcs_all
        elif args.only:
            names = {p.name.lower(): p for p in dbcs_all}
            chosen = []
            for part in args.only.split(","):
                key = part.strip().lower()
                if not key:
                    continue
                if key not in names:
                    print(f"Unknown DBC {part!r} in {vehicle_dir.name}", file=sys.stderr)
                    raise SystemExit(1)
                chosen.append(names[key])
            if not chosen:
                print("No DBCs listed after --only", file=sys.stderr)
                raise SystemExit(1)
        else:
            print("With --vehicle, pass --all or --only name.dbc,...", file=sys.stderr)
            raise SystemExit(1)
    else:
        vehicle_dir = pick_vehicle_interactive(vehicles)
        dbcs_all = list_dbcs(vehicle_dir)
        if not dbcs_all:
            print(f"No .dbc files in {vehicle_dir}", file=sys.stderr)
            raise SystemExit(1)
        chosen = pick_dbcs_interactive(dbcs_all)

    print(f"\nVehicle: {vehicle_dir.name}")
    print("Files: " + ", ".join(p.name for p in chosen))
    if not args.vehicle:
        confirm = input("\nProceed? [y/N]: ").strip().lower()
        if confirm not in ("y", "yes"):
            print("Aborted.")
            raise SystemExit(0)

    run_batch(
        vehicle_dir,
        chosen,
        in_place=bool(args.in_place),
        backup=bool(args.backup),
    )


if __name__ == "__main__":
    main()
