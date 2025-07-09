# File Sys
import sys
import os
# CLI
import argparse
from pathlib import Path
# CAN Parsing
import cantools

##### PARSE #####
# Usage: python3 dbc-to-c.py --parse <dbc_file_path> <buff_size> <write_dir>
# 
# Args(4):
#   dbc_file_path: Path to DBC file
#   buff_size: Default size of recv queue (default: 10)
#   write_dir: Path to write parsed headers to (default: current_dir)
#
# Generates (2) files:
#   [1] canX_recv_entries.h
#       -- Contains CAN_RECV_ENTRY lines parsed from DBC
#   [2] canX_utils.h
#       --- Contains #define macros mapping CAN message names to frame IDs
##################

# User defaults
DEFAULT_Q_SIZE = 10

# ANSI color codes
RED = "\033[91m"
YELLOW = "\033[1;33m"
GREEN = "\033[92m"
BOLD = "\033[1m"
END = "\033[0m"

def ErrorMessage(msg):
    """Formats an error message in red and bold text"""
    print(f"{RED}{BOLD}Fatal Err{END}{END}", end=": ")
    print(msg)
    sys.exit()

def WarningMessage(msg):
    """Formats a warning message in yellow and bold text"""
    print(f"{YELLOW}{BOLD}Warning{END}{END}", end=": ")
    print(msg)

# ...existing code...

def get_c_type(sig_length, is_signed, is_float):
    """Return the appropriate C type for a signal."""
    if is_float:
        return "float"
    if is_signed:
        if sig_length <= 8:
            return "int8_t"
        elif sig_length <= 16:
            return "int16_t"
        elif sig_length <= 32:
            return "int32_t"
        else:
            return "int64_t"
    else:
        if sig_length <= 8:
            return "uint8_t"
        elif sig_length <= 16:
            return "uint16_t"
        elif sig_length <= 32:
            return "uint32_t"
        else:
            return "uint64_t"

def gen_mask_and_shift(length, sig_start, sig_length, sig_endian):
    """Generate the mask and shift length for a signal."""
    bin_string = list(("0" * (length * 8))) # all 0's

    if sig_endian == "big_endian":
        bin_string[sig_start:sig_start+sig_length] = list("1" * sig_length)
    else:
        bin_string[sig_start-sig_length:sig_start] = list("1" * sig_length)
            
    bin_string = "0b" + "".join(bin_string) # make string

    mask = hex(eval(bin_string))
    # calc trailing 0's
    shift_length = 0
    for i in bin_string[::-1]:
        if i == "1": break
        shift_length += 1
    return mask, shift_length

def DBC_Parse(db, size, dir, dbc_basename=""):
    """Generates headers according to DBC files"""
    if not (Path(dir)).exists():
        WarningMessage(f"{BOLD}{dir}{END} not found ... making it")
        os.mkdir(dir)

    for msg in db.messages:
        id = hex(msg.frame_id)
        name = msg.name


    # GENERATE can_utils.h
    utils_string = ""
    with open("docs/utils.txt", "r") as f:
        utils_string = f.read()

    for msg in db.messages:
        id = hex(msg.frame_id)
        name = msg.name
        length = msg.length

        utils_string += f"#define CAN_ID_{name} {id}\n\n"

        signals = msg.signals
        for s in signals:
            sig_name = s.name
            sig_endian = s.byte_order
            sig_start = s.start
            sig_length = s.length
            is_signed = s.is_signed
            is_float = getattr(s, "is_float", False)

            # --- GET C TYPE ---
            c_type = get_c_type(sig_length, is_signed, is_float)

            # --- GEN MASK AND SHIFT ---
            mask, shift_length = gen_mask_and_shift(length, sig_start, sig_length, sig_endian)

            # --- GEN MACRO FOR EXTRACTION ---
            macro_name = f"CANUTIL_GET_VALUE_{sig_name}"
            if is_float:
                utils_string += f"#define {macro_name}(d) (*((float*)(d))) // float\n"
            elif is_signed:
                utils_string += f"#define {macro_name}(d) ((({c_type})(((*( (uint64_t *) d ) & {mask}) >> {shift_length})))) // signed\n"
            else:
                utils_string += f"#define {macro_name}(d) (({c_type})(((*( (uint64_t *) d ) & {mask}) >> {shift_length}))) // unsigned\n"

            # --- GEN ENUMS FOR VALUE TABLES ---
            if s.choices:
                enum_name = f"canutil_{sig_name}_vals"
                utils_string += f"\ntypedef enum {enum_name} {{\n"
                for k, v in sorted(s.choices.items()):
                    enum_val = f"{enum_name.upper()}_{str(v).upper().replace(' ', '_')}"
                    utils_string += f"    {enum_val} = {k},\n"
                utils_string = utils_string.rstrip(",\n") + f"\n}} {enum_name};\n\n"

    utils_filename = f"{dir}/can_utils.h"
    with open(utils_filename, "w+") as f:
        f.write(utils_string)

# ...existing code...

def process_dbc_file(path, buff_size, write_dir):
    db = cantools.database.load_file(path)
    DBC_Parse(db, buff_size, write_dir)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("dbc_file_path", help="Path to DBC file or directory containing DBC files")
    parser.add_argument("buff_size", nargs='?', const=1, default=DEFAULT_Q_SIZE, type=int)
    parser.add_argument("write_dir", nargs='?', const=1, default=".", type=str)
    args = parser.parse_args()

    path = args.dbc_file_path

    # Check docs
    if not Path("docs/recv.txt").exists() or not Path("docs/utils.txt").exists():
        ErrorMessage(f"Make sure to include {BOLD}'docs'{END} folder in current directory containing {BOLD}recv.txt{END} and {BOLD}utils.txt{END}.")

    # If path is a directory, process all .dbc files
    if os.path.isdir(path):
        dbc_files = [f for f in os.listdir(path) if f.endswith(".dbc")]
        if not dbc_files:
            ErrorMessage(f"No .dbc files found in directory {BOLD}{path}{END}")
        for dbc_file in dbc_files:
            dbc_path = os.path.join(path, dbc_file)
            print(f"{GREEN}Processing {dbc_path}{END}")
            process_dbc_file(dbc_path, args.buff_size, args.write_dir)
    else:
        # Input checking for single file
        if not path.endswith(".dbc"):
            ErrorMessage("DBC file path must end with .dbc")
        if not Path(path).exists():
            ErrorMessage(f"{BOLD}{path}{END} is not a valid DBC file path")
        process_dbc_file(path, args.buff_size, args.write_dir)

    if args.write_dir == ".":
        WarningMessage("No output dir specified ... using current dir")

if __name__ == "__main__":
    main()