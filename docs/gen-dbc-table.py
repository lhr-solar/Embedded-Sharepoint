import mkdocs_gen_files
import os
from pathlib import Path
import cantools

# ----- CONFIG -----
DBC_DIR = Path("can/dbc")      # Folder containing your .dbc files
OUTPUT_MD = Path("DBC.md")        # Output markdown file
GITHUB_DBC_BASE = "https://github.com/lhr-solar/Embedded-Sharepoint/tree/main/can/dbc"
# ------------------

def clean_name_for_toc(filename: str) -> str:
    """Remove .dbc, replace dashes/underscores with spaces, capitalize words."""
    name = filename
    if name.lower().endswith('.dbc'):
        name = name[:-4]
    name = name.replace('-', ' ').replace('_', ' ')
    name = ' '.join(word.capitalize() for word in name.split())
    return name

def dbc_to_markdown(dbc_path: Path) -> str:
    """
    Converts a DBC file into markdown tables for messages and signals.
    """
    db = cantools.database.load_file(dbc_path)
    anchor = dbc_path.stem.replace(' ', '-').replace('.', '-')
    github_link = f"{GITHUB_DBC_BASE}/{dbc_path.name}"
    md = f"<a name=\"{anchor}\"></a>\n"
    md += f"# {dbc_path.name}\n\n"
    md += f"[GitHub Link]({github_link})\n\n"

    # Messages table
    md += "## Messages\n\n"
    md += "| ID | Name | DLC | Signals |\n"
    md += "|----|------|-----|---------|\n"
    for msg in db.messages:
        signals_list = ", ".join(sig.name for sig in msg.signals)
        md += f"| 0x{msg.frame_id:X} | {msg.name} | {msg.length} | {signals_list} |\n"

    # Signals table
    md += "\n## Signals\n\n"
    md += "| Name | Message | Bits [start:end] | Length | Factor | Offset | Min | Max | Unit |\n"
    md += "|------|---------|------------------|--------|--------|--------|-----|-----|------|\n"
    for msg in db.messages:
        for sig in msg.signals:
            start = sig.start
            end = sig.start + sig.length - 1
            md += f"| {sig.name} | {msg.name} | [{start}:{end}] | {sig.length} | {sig.scale} | {sig.offset} | {sig.minimum} | {sig.maximum} | {sig.unit or ''} |\n"

    return anchor, md

def main():
    dbc_files = sorted(DBC_DIR.glob("*.dbc"), key=lambda p: p.name.lower())
    if not dbc_files:
        print(f"No .dbc files found in {DBC_DIR}")
        return

    toc = "# Table of Contents\n\n"
    sections = []
    for dbc_file in dbc_files:
        anchor, section_md = dbc_to_markdown(dbc_file)
        toc_name = clean_name_for_toc(dbc_file.name)
        toc += f"- [{toc_name}](#{anchor})\n"
        sections.append(section_md)

    all_md = toc + "\n" + "\n\n".join(sections)

    with mkdocs_gen_files.open(OUTPUT_MD, "w") as f:
        f.write(all_md)
    print(f"Generated {OUTPUT_MD}")

main()
