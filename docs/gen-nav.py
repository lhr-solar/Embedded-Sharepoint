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

            # Update parent stack
            if indent_level >= len(parent_stack):
                parent_stack.append(name)
            else:
                parent_stack = parent_stack[:indent_level] + [name]

        elif stripped.startswith("*"):
            # Section without file
            name = stripped[1:].strip()
            if indent_level >= len(parent_stack):
                parent_stack.append(name)
            else:
                parent_stack = parent_stack[:indent_level] + [name]

# --- Add new files ---
for file in mkdocs_gen_files.files:
    path = Path(file.src_path)
    if path.name.endswith(".md") and path.name != "nav.md" and \
       (path.name.endswith("8h.md") or path.parent == Path(".")):
        file_posix = path.as_posix()
        if file_posix in existing_files:
            continue

        # Nested key for folders
        if path.parent in (Path("."), Path("docs")):
            nav[path.stem] = file_posix
        else:
            key = (path.parent.name, path.stem.split("_8h")[0])
            nav[key] = file_posix

# --- Write merged nav ---
with mkdocs_gen_files.open(existing_nav_file, "w") as f:
    print("".join(nav.build_literate_nav()))
    f.writelines("".join(nav.build_literate_nav()))
