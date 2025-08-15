import os
from pathlib import Path
import cantools

# ----- CONFIG -----
DBC_DIR = Path("can/dbc")      # Folder containing your .dbc files
OUTPUT_MD = Path("docs/DBC.md")        # Output markdown file
# ------------------

def dbc_to_markdown(dbc_path: Path) -> str:
    """
    Converts a DBC file into markdown tables for messages and signals.
    """
    db = cantools.database.load_file(dbc_path)
    md = f"# {dbc_path.name}\n\n"

    # Messages table
    md += "## Messages\n\n"
    md += "| ID | Name | DLC | Signals |\n"
    md += "|----|------|-----|---------|\n"
    for msg in db.messages:
        signals_list = ", ".join(sig.name for sig in msg.signals)
        md += f"| {msg.frame_id} | {msg.name} | {msg.length} | {signals_list} |\n"

    # Signals table
    md += "\n## Signals\n\n"
    md += "| Name | Message | Start Bit | Length | Factor | Offset | Min | Max | Unit |\n"
    md += "|------|---------|-----------|--------|--------|--------|-----|-----|------|\n"
    for msg in db.messages:
        for sig in msg.signals:
            md += f"| {sig.name} | {msg.name} | {sig.start} | {sig.length} | {sig.scale} | {sig.offset} | {sig.minimum} | {sig.maximum} | {sig.unit or ''} |\n"

    return md

def main():
    dbc_files = list(DBC_DIR.glob("*.dbc"))
    if not dbc_files:
        print(f"No .dbc files found in {DBC_DIR}")
        return

    all_md = ""
    for dbc_file in dbc_files:
        all_md += dbc_to_markdown(dbc_file) + "\n\n"

    with open(OUTPUT_MD, "w") as f:
        f.write(all_md)
    print(f"Generated {OUTPUT_MD}")

if __name__ == "__main__":
    main()