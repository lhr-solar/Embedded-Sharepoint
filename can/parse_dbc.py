#!/usr/bin/env python3

import cantools
from pathlib import Path
import sys

CTYPE_MAP = {
    (True, 8):  "int8_t",
    (True, 16): "int16_t",
    (True, 32): "int32_t",
    (False, 8):  "uint8_t",
    (False, 16): "uint16_t",
    (False, 32): "uint32_t",
}

def signal_ctype(sig):
    for (signed, bits), ctype in CTYPE_MAP.items():
        if sig.is_signed == signed and sig.length <= bits:
            return ctype
    return "uint32_t"

# ---------------------------------------------------------------------------
# HEADER GENERATION
# ---------------------------------------------------------------------------

def generate_header(db, out_path, dbc_name):
    guard = f"{dbc_name.upper()}_H"
    lines = []

    lines += [
        f"#ifndef {guard}",
        f"#define {guard}",
        "",
        "#include <stdint.h>",
        "",
    ]

    for msg in db.messages:
        msg_name = msg.name
        macro_name = msg_name.upper()
        struct_name = f"{msg_name}_can_msg_t"

        lines += [
            f"/* ===== {msg_name} ===== */",
            f"#define CAN_ID_{macro_name}   0x{msg.frame_id:X}",
            f"#define {macro_name}_DLC      {msg.length}",
            "",
            f"typedef struct {{",
        ]

        for sig in msg.signals:
            ctype = signal_ctype(sig)
            scale = sig.scale
            offset = sig.offset
            unit = sig.unit or ""
            lines.append(
                f"    {ctype} {sig.name};  // phys = raw * {scale} + {offset} {unit}"
            )

        lines += [
            f"}} {struct_name};",
            "",
            f"void pack_{msg_name}(uint8_t *dst, const {struct_name} *src);",
            f"void unpack_{msg_name}({struct_name} *dst, const uint8_t *src);",
            "",
        ]

    lines.append(f"#endif /* {guard} */\n")
    out_path.write_text("\n".join(lines))


# ---------------------------------------------------------------------------
# SOURCE GENERATION
# ---------------------------------------------------------------------------

def generate_source(db, out_path, header_name):
    lines = []

    lines += [
        f'#include "{header_name}"',
        "#include <string.h>",
        "",
    ]

    for msg in db.messages:
        msg_name = msg.name
        struct_name = f"{msg_name}_can_msg_t"
        dlc = msg.length

        # ---------------- PACK ----------------
        lines += [
            f"__weak void pack_{msg_name}(uint8_t *dst, const {struct_name} *src)",
            "{",
            f"    memset(dst, 0, {dlc});",
        ]

        for sig in msg.signals:
            start = sig.start
            length = sig.length
            scale = sig.scale
            offset = sig.offset
            signed = sig.is_signed
            endian = sig.byte_order  # 'little_endian' or 'big_endian'

            raw_type = "int32_t" if signed else "uint32_t"

            lines += [
                f"    /* {sig.name} */",
                "    {",
                f"        {raw_type} raw = ({raw_type})((src->{sig.name} - {offset}) / {scale});",
            ]

            if endian == "little_endian":
                lines += [
                    f"        for (uint32_t i = 0; i < {length}; i++) {{",
                    f"            uint32_t bit = {start} + i;",
                ]
            else:
                lines += [
                    f"        for (uint32_t i = 0; i < {length}; i++) {{",
                    f"            uint32_t bit = {start} - i;",
                ]

            lines += [
                "            dst[bit / 8] |= ((raw >> i) & 0x1) << (bit % 8);",
                "        }",
                "    }",
            ]

        lines += [
            "}",
            "",
        ]

        # ---------------- UNPACK ----------------
        lines += [
            f"__weak void unpack_{msg_name}({struct_name} *dst, const uint8_t *src)",
            "{",
        ]

        for sig in msg.signals:
            start = sig.start
            length = sig.length
            scale = sig.scale
            offset = sig.offset
            signed = sig.is_signed
            endian = sig.byte_order

            raw_type = "int32_t" if signed else "uint32_t"

            lines += [
                f"    /* {sig.name} */",
                "    {",
                f"        {raw_type} raw = 0;",
            ]

            if endian == "little_endian":
                lines += [
                    f"        for (uint32_t i = 0; i < {length}; i++) {{",
                    f"            uint32_t bit = {start} + i;",
                ]
            else:
                lines += [
                    f"        for (uint32_t i = 0; i < {length}; i++) {{",
                    f"            uint32_t bit = {start} - i;",
                ]

            lines += [
                "            raw |= ((src[bit / 8] >> (bit % 8)) & 0x1) << i;",
                "        }",
            ]

            if signed:
                lines.append(
                    f"        if (raw & (1 << ({length} - 1))) raw |= ~((1 << {length}) - 1);"
                )

            lines += [
                f"        dst->{sig.name} = (raw * {scale}) + {offset};",
                "    }",
            ]

        lines += [
            "}",
            "",
        ]

    out_path.write_text("\n".join(lines))


def collect_dbc_files(paths):
    dbc_files = []

    for p in paths:
        path = Path(p)

        if path.is_dir():
            dbc_files.extend(sorted(path.glob("*.dbc")))
        elif path.is_file() and path.suffix == ".dbc":
            dbc_files.append(path)
        else:
            print(f"Warning: skipping {p}")

    return dbc_files



# ---------------------------------------------------------------------------
# MAIN
# ---------------------------------------------------------------------------
def main(input_paths, out_dir):
    out_dir = Path(out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    dbc_files = collect_dbc_files(input_paths)

    if not dbc_files:
        print("No DBC files found.")
        sys.exit(1)

    for dbc_file in dbc_files:
        db = cantools.database.load_file(dbc_file)
        name = dbc_file.stem

        header = out_dir / f"can_dbc_{name}.h"
        source = out_dir / f"can_dbc_{name}.c"

        generate_header(db, header, name)
        generate_source(db, source, header.name)

        print(f"Generated {header} and {source}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: ./parse_dbc.py <out_dir> <dbc1> <dbc2> ...")
        sys.exit(1)

    main(sys.argv[2:], sys.argv[1])
