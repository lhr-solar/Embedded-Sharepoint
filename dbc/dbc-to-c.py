# File Sys
import sys
import os
# CLI
import argparse
from pathlib import Path
# CAN Parsing
import cantools

##### PARSE #####
# Usage: python3 dbc-to-c.py --parse <dbc_file_path> <bus> <buff_size> <write_dir>
# 
# Args(4):
#   dbc_file_path: Path to DBC file
#   bus: CAN bus number
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
    """Formats and error message in red and bold text"""
    print(f"{RED}{BOLD}Fatal Err{END}{END}", end=": ")
    print(msg)
    sys.exit()

def WarningMessage(msg):
    """Formats and error message in red and bold text"""
    print(f"{YELLOW}{BOLD}Warning{END}{END}", end=": ")
    print(msg)

def DBC_Parse(db, bus, size, dir):
    """Generates headers according to DBC files"""
    # GENERATE canX_recv_entries.h

    if not (Path(dir)).exists():
        WarningMessage(f"{BOLD}{dir}{END} not found ... making it")
        os.mkdir(dir)
        
    # Convert hexstring to int
    if type(bus) == str: 
        bus = eval(bus)
    
    recv_string = ""
    with open("docs/recv.txt", "r") as f:
        recv_string = f.read().format(num=bus)

    for msg in db.messages:
        id = hex(msg.frame_id)
        name = msg.name

        recv_string += f"CAN_RECV_ENTRY({id}, {size}, false) // {name}\n"
    
    with open(f"{dir}/can{bus}_recv_entries.h", "w+") as f:
        f.write(recv_string)

    # GENERATE can_utils.h
    utils_string = ""
    with open("docs/utils.txt", "r") as f:
        utils_string = f.read()

    for msg in db.messages:
        id = hex(msg.frame_id)
        name = msg.name
        length = msg.length

        utils_string += f"#define CANUTIL_{name} {id}\n\n"

        signals = msg.signals
        for s in signals:
            # --- GEN MASKS ---
            sig_name = s.name
            sig_endian = s.byte_order
            sig_start = s.start
            sig_length = s.length

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
            
            utils_string += f"#define CANUTIL_GET_VALUE_{sig_name}(d) ((*( (uint64_t *) d ) & {mask}) >> {shift_length})\n"

            # --- GEN ENUMS ---
            if s.choices:
                keys = sorted(s.choices.keys())
                utils_string += f"\ntypedef enum {{ \n"

                for k in keys:
                    utils_string += f"  {s.choices[k]} = {k},\n"
            
                utils_string = utils_string[:-2] + f"\n }} canutil_{sig_name}_vals;\n\n" # trim ".\n" off the last entry

    with open(f"{dir}/can_utils.h", "w+") as f:
        f.write(utils_string)

parser = argparse.ArgumentParser()

parser.add_argument("-p", "--parse", action="store_true")

parser.add_argument("dbc_file_path")
parser.add_argument("bus") 
parser.add_argument("buff_size", nargs='?', const=1, default=DEFAULT_Q_SIZE, type=int)
parser.add_argument("write_dir", nargs='?', const=1, default=".", type=str)

args = parser.parse_args()

path = args.dbc_file_path

# Input checking
if not path.endswith(".dbc"):
    ErrorMessage("DBC file path must end with .dbc")
if not Path(path).exists():
    ErrorMessage(f"{BOLD}{path}{END} is not a valid DBC file path")

# Check if documentation files exist
if not Path("docs/recv.txt").exists() or not Path("docs/utils.txt").exists():
    ErrorMessage(f"Make sure to include {BOLD}'docs'{END} folder in current directory containing {BOLD}recv.txt{END} and {BOLD}utils.txt{END}.")

db = cantools.database.load_file(path)

if args.parse:
    if args.write_dir == ".":
        WarningMessage("No output dir specified ... using current dir")

    DBC_Parse(db, args.bus, args.buff_size, args.write_dir)