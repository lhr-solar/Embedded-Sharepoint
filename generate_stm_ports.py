import os
from pathlib import Path

STM_DIR = Path("stm")
OUTPUT_MD = Path("docs/STM32_Ports.md")

def find_flash_ld_files(stm_dir):
    flash_ld_names = []
    for root, dirs, files in os.walk(stm_dir):
        for file in files:
            if file.endswith("_FLASH.ld"):
                # Extract just the filename without extension
                name = Path(file).stem  # e.g., STM32F401REx_FLASH
                # Remove the trailing _FLASH
                if name.endswith("_FLASH"):
                    name = name[:-6]
                flash_ld_names.append(name)
    return flash_ld_names

def main():
    flash_ld_names = find_flash_ld_files(STM_DIR)
    with open(OUTPUT_MD, "w") as f:
        f.write("# List of STM32 Port Names\n\n")
        if not flash_ld_names:
            f.write("No *_FLASH.ld files found.\n")
        else:
            for name in sorted(flash_ld_names):
                f.write(f"- `{name}`\n")
    print(f"Generated {OUTPUT_MD}")

if __name__ == "__main__":
    main()