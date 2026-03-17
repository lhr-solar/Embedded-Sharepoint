import cantools
import argparse
import re
from pathlib import Path


def sanitize_name(name: str) -> str:
    """Sanitize a string to be a valid C identifier."""
    name = re.sub(r'\W+', '_', name)
    if name and name[0].isdigit():
        name = "_" + name
    return name


def c_type_for_signal(signal):
    length = signal.length

    if signal.is_float:
        return "float"

    if signal.is_signed:
        if length <= 8:
            return "int8_t"
        elif length <= 16:
            return "int16_t"
        elif length <= 32:
            return "int32_t"
        else:
            return "int64_t"
    else:
        if length <= 8:
            return "uint8_t"
        elif length <= 16:
            return "uint16_t"
        elif length <= 32:
            return "uint32_t"
        else:
            return "uint64_t"


def generate_enum(signal, message_name):
    enum_name = sanitize_name(f"{message_name}_{signal.name}_e".lower())

    lines = []
    lines.append("typedef enum {")

    for value, choice in signal.choices.items():
        if hasattr(choice, "name"):
            text = choice.name
        else:
            text = str(choice)

        member = sanitize_name(
            f"{message_name}_{signal.name}_{text}".upper()
        )
        lines.append(f"    {member} = {value},")

    lines.append(f"}} {enum_name};\n")
    return "\n".join(lines)


def generate_struct(message):
    struct_name = sanitize_name(message.name.lower() + "_t")

    lines = []
    lines.append("typedef struct {")

    for signal in message.signals:
        ctype = c_type_for_signal(signal)
        member = sanitize_name(signal.name)
        lines.append(f"    {ctype} {member};")

    lines.append(f"}} {struct_name};\n")
    return "\n".join(lines)


def generate_header(dbc_path: Path, output_file: Path | None):
    db = cantools.database.load_file(str(dbc_path))

    if output_file is None:
        output_file = dbc_path.with_name(f"{dbc_path.stem}_can_msgs.h")

    can_id_macros = []
    can_len_macros = []
    enums = []
    structs = []

    # Sort messages by numeric CAN ID
    sorted_messages = sorted(db.messages, key=lambda m: m.frame_id)

    for message in sorted_messages:
        msg_macro_name = sanitize_name(message.name.upper())

        # CAN ID macro
        can_id_macros.append(
            f"#define CAN_ID_{msg_macro_name} 0x{message.frame_id:X}"
        )

        # CAN length macro
        can_len_macros.append(
            f"#define CAN_DLC_{msg_macro_name} {message.length}"
        )

        for signal in message.signals:
            if signal.choices:
                enums.append(generate_enum(signal, message.name.lower()))

        structs.append(generate_struct(message))

    with open(output_file, "w") as f:
        f.write("#pragma once\n\n")
        f.write("#include <stdint.h>\n\n")

        f.write("/* ================= CAN ID Macros ================= */\n\n")
        for macro in can_id_macros:
            f.write(macro + "\n")

        f.write("\n/* ================= CAN Length Macros ================= */\n\n")
        for macro in can_len_macros:
            f.write(macro + "\n")

        f.write("\n\n")

        if enums:
            f.write("/* ================= Value Table Enums ================= */\n\n")
            for enum_code in enums:
                f.write(enum_code + "\n")

        f.write("/* ================= Message Structs ================= */\n\n")
        for struct_code in structs:
            f.write(struct_code + "\n")

    print(f"Generated: {output_file}")


def main():
    parser = argparse.ArgumentParser(
        description="Generate C header files from a CAN DBC file."
    )

    parser.add_argument(
        "dbc",
        type=Path,
        help="Input DBC file"
    )

    parser.add_argument(
        "-o",
        "--output",
        type=Path,
        help="Output header filename (optional)"
    )

    args = parser.parse_args()

    if not args.dbc.is_file():
        parser.error(f"DBC file not found: {args.dbc}")

    generate_header(args.dbc, args.output)


if __name__ == "__main__":
    main()