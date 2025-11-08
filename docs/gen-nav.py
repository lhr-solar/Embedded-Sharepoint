import mkdocs_gen_files
from pathlib import Path

nav = mkdocs_gen_files.Nav()
existing_nav_file = Path("docs/nav.md")
existing_files = set()

# --- Load existing nav.md ---
if existing_nav_file.exists():
    parent_stack = []
    for line in existing_nav_file.read_text().splitlines():
        stripped = line.strip()
        indent_level = (len(line) - len(line.lstrip(' '))) // 4

        if stripped.startswith("* [") and "](" in stripped:
            # Extract display name and file path
            name = stripped[stripped.find("[")+1 : stripped.find("]")]
            path = stripped[stripped.find("(")+1 : stripped.find(")")]
            existing_files.add(path)

            # Update nav
            if indent_level == 0:
                nav[name] = path
            else:
                key = tuple(parent_stack[:indent_level] + [name])
                nav[key] = path

        elif stripped.startswith("*"):
            # Section without file
            name = stripped[1:].strip()

        parent_stack = parent_stack[:indent_level] + [name]

# --- Add new files ---
for file in mkdocs_gen_files.files:
    path = Path(file.src_path)
    file_posix = path.as_posix()
    if file_posix.endswith(".md") and file_posix != "nav.md" and \
       (file_posix.endswith("8h.md") or path.parent == Path(".")):
        if file_posix in existing_files:
            continue

        # Nested key for folders
        if path.parent.name in (".", "docs"):
            nav[path.stem] = file_posix
        else:
            key = (path.parent.name + " Documentation", path.stem.split("_8h")[0])
            nav[key] = file_posix

# --- Write merged nav ---
with mkdocs_gen_files.open(existing_nav_file, "w") as f:
    lit_nav = nav.build_literate_nav()
    print("".join(lit_nav))
    f.writelines("".join(lit_nav))
