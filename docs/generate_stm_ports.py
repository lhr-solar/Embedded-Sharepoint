# ...existing code...
import os
from pathlib import Path

STM_DIR = Path("stm")
OUTPUT_MD = Path("docs/STM32_Ports.md")

def find_flash_ld_files(stm_dir):
    flash_ld_names = []
    for root, dirs, files in os.walk(stm_dir):
        for file in files:
            if file.endswith("_FLASH.ld"):
                name = Path(file).stem  # e.g., STM32F401REx_FLASH
                if name.endswith("_FLASH"):
                    name = name[:-6]
                flash_ld_names.append(name)
    return flash_ld_names

def project_target_from_port(port_name: str) -> str:
    pt = port_name.lower()
    if pt.endswith("x"):
        pt = pt[:-1]
    return pt

def series_from_port(port_name: str) -> str:
    """Return series like 'L4', 'F4', 'G4', or 'OTHER'."""
    pn = port_name.lower()
    if pn.startswith("stm32") and len(pn) >= 7:
        letter = pn[5]
        digit = pn[6] if len(pn) > 6 and pn[6].isdigit() else ""
        return (letter + digit).upper()
    return "OTHER"

def main():
    flash_ld_names = find_flash_ld_files(STM_DIR)
    OUTPUT_MD.parent.mkdir(parents=True, exist_ok=True)

    # group by series
    grouped = {}
    for name in sorted(set(flash_ld_names), key=lambda s: s.lower()):
        series = series_from_port(name)
        grouped.setdefault(series, []).append(name)

    # prefer order L4, F4, G4 then others alphabetically
    preferred = ["L4", "F4", "G4"]
    series_keys = [k for k in preferred if k in grouped] + \
                  sorted([k for k in grouped.keys() if k not in preferred])

    with open(OUTPUT_MD, "w") as f:
        f.write("# List of STM32 Port Names\n\n")
        if not flash_ld_names:
            f.write("No *_FLASH.ld files found.\n")
            return
        for series in series_keys:
            f.write(f"## {series}\n\n")
            for name in grouped[series]:
                proj_target = project_target_from_port(name)
                f.write(f"- `{name}`\n")
                f.write(f"  - `PROJECT_TARGET={proj_target}`\n")
            f.write("\n")
    print(f"Generated {OUTPUT_MD}")

if __name__ == "__main__":
    main()