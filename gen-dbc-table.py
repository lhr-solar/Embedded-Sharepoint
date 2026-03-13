import mkdocs_gen_files
from pathlib import Path
import cantools

# ----- CONFIG -----
DBC_DIR = Path("can/dbc/Mcqueen")
OUTPUT_MD = Path("DBC.md")
GITHUB_DBC_BASE = "https://github.com/lhr-solar/Embedded-Sharepoint/tree/main/can/dbc"
# ------------------


def clean_name_for_toc(filename: str) -> str:
    name = filename
    if name.lower().endswith(".dbc"):
        name = name[:-4]
    name = name.replace("-", " ").replace("_", " ")
    name = " ".join(word.capitalize() for word in name.split())
    return name


def format_can_id(msg):
    """Return formatted CAN ID with proper width."""
    if msg.is_extended_frame:
        return f"0x{msg.frame_id:08X}"  # 29-bit extended
    else:
        return f"0x{msg.frame_id:03X}"  # 11-bit standard


def dbc_to_markdown(dbc_path: Path) -> str:
    db = cantools.database.load_file(dbc_path)

    anchor = dbc_path.stem.replace(" ", "-").replace(".", "-")
    github_link = f"{GITHUB_DBC_BASE}/{dbc_path.name}"

    md = f"<a name=\"{anchor}\"></a>\n"
    md += f"# {dbc_path.name}\n\n"
    md += f"[GitHub Link]({github_link})\n\n"

    # ---- Messages ----
    md += "## Messages\n\n"
    md += "| ID | Type | Name | DLC | Signals |\n"
    md += "|----|------|------|-----|---------|\n"

    # sort by numeric CAN ID
    messages = sorted(db.messages, key=lambda m: m.frame_id)

    for msg in messages:
        signals_list = ", ".join(sig.name for sig in msg.signals)
        can_id = format_can_id(msg)
        frame_type = "Ext" if msg.is_extended_frame else "Std"

        md += f"| {can_id} | {frame_type} | {msg.name} | {msg.length} | {signals_list} |\n"

    # ---- Signals ----
    md += "\n## Signals\n\n"
    md += "| Name | Message | Bits [start:end] | Length | Factor | Offset | Min | Max | Unit |\n"
    md += "|------|---------|------------------|--------|--------|--------|-----|-----|------|\n"

    for msg in messages:
        for sig in msg.signals:
            start = sig.start
            end = sig.start + sig.length - 1

            md += (
                f"| {sig.name} | {msg.name} | [{start}:{end}] | "
                f"{sig.length} | {sig.scale} | {sig.offset} | "
                f"{sig.minimum} | {sig.maximum} | {sig.unit or ''} |\n"
            )

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