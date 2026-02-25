import cantools
import re
from pathlib import Path
import sys

def sanitize_name(name):
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

def generate_header(dbc_path, output_file=None):
    db = cantools.database.load_file(dbc_path)

    # If no output filename provided, generate default
    if output_file is None:
        input_name = Path(dbc_path).stem
        output_file = f"{input_name}_can_msgs.h"

    can_id_macros = []
    enums = []
    structs = []

    for message in db.messages:
        msg_macro_name = sanitize_name(message.name.upper())

        can_id_macros.append(
            f"#define CAN_ID_{msg_macro_name} 0x{message.frame_id:X}"
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

        f.write("\n\n")

        if enums:
            f.write("/* ================= Value Table Enums ================= */\n\n")
            for enum_code in enums:
                f.write(enum_code + "\n")

        f.write("/* ================= Message Structs ================= */\n\n")
        for struct_code in structs:
            f.write(struct_code + "\n")

    print(f"Generated: {output_file}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage:")
        print("  python generate_can_headers.py input.dbc [output.h]")
        sys.exit(1)

    dbc_path = sys.argv[1]

    if len(sys.argv) >= 3:
        output_file = sys.argv[2]
    else:
        output_file = None

    generate_header(dbc_path, output_file)