# File Sys
import sys
import os
# CLI
import argparse
from pathlib import Path
# CAN Parsing
import cantools

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
    print(f"{RED}{BOLD}Err{END}{END}", end=": ")
    print(msg)
    sys.exit()

def WarningMessage(msg):
    """Formats and error message in red and bold text"""
    print(f"{YELLOW}{BOLD}Err{END}{END}", end=": ")
    print(msg)
    sys.exit()

def DBC_Verify(db):
    """Returns duplicate CAN IDs in file"""
    all_ids = []
    dups = []
    for msg in db.messages:
        id = hex(msg.frame_id)
        if id in all_ids:
            dups += id,

    return len(dups), dups

def DBC_Parse(db, bus, size, dir):
    """Generates headers according to DBC files"""
    # GENERATE can1_recv_entries.h
    dups, _ = DBC_Verify(db)
    if dups:
        ErrorMessage("Invalid DBC ... run with --verify for more info")
        
    # Convert hexstring to int
    if type(bus) == str: 
        bus = eval(bus)
    
    recv_string = ""

    for msg in db.messages:
        id = msg.frame_id
        name = msg.name

        recv_string += f"CAN_RECV_ENTRY({id}, {size}, false) // {name}\n"
    
    with open(f"{dir}/can{bus}_recv_entries.h", "w+") as f:
        f.write(recv_string)

    # GENERATE can_utils.h
    utils_string = ""

    for msg in db.messages:
        id = msg.id
        name = msg.name
        length = msg.length
        msg_endian = msg.byte_order

        utils_string += f"#define {name} {id}\n\n"

        signals = msg.signals
        for s in signals:
            # --- GEN MASKS ---
            sig_name = s.name
            sig_endian = s.byte_order
            sig_start = s.start
            sig_length = s.length

            bin_string = "0b" + ("0" * (length * 8)) # all 0's

            if sig_endian == "big_endian":
                bin_string[sig_start:sig_start+sig_length] = "1" * sig_length
            else:
                bin_string[sig_start-sig_length:sig_start] = "1" * sig_length
            
            if msg_endian == "little_endian":
                bin_string = bin_string[::-1] # reverse

            mask = hex(bin_string)
            # calc trailing 0's
            shift_length = 0
            for i in bin_string[::-1]:
                if i == "1": break
                shift_length += 1
            
            utils_string += "#define Parse_{sig_name}(d) (d & {mask}) >> {shift_length}\n"

            # --- GEN ENUMS ---
            utils_string += "\nenum {s}_VALUES { "
            if s.choices:
                idx = 0
                for c in s.choices:
                    utils_string += "{s.choices[idx]} = {c},\n"

                    idx += 1
            
            utils_string = utils_string[:-2] + " }" # trim ".\n" off the last entry

    with open(f"{dir}/can_utils.h", "w+") as f:
        f.write(utils_string)

parser = argparse.ArgumentParser()
group = parser.add_mutually_exclusive_group()

##### VERIFY #####
# Usage: python3 dbc-to-c.py --verify <dbc_file_path>
# 
# Args (1):
#   dbc_file_path: Path to DBC file
#
# Verifies that DBC file does NOT contain repeated CAN IDs
##################
group.add_argument("-v", "--verify", action="store_true")

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
group.add_argument("-p", "--parse", action="store_true")

parser.add_argument("dbc_file_path")
parser.add_argument("bus", nargs='?', const="") # Default bus arg so no conflicts between -v and -p
parser.add_argument("buff_size", nargs='?', const=DEFAULT_Q_SIZE)
parser.add_argument("write_dir", nargs='?', const=".")

args = parser.parse_args()

path = args.dbc_file_path

# Input checking
if not path.endswith(".dbc"):
    ErrorMessage("DBC file path must end with .dbc")
if not Path(path).exists():
    ErrorMessage(f"{path} is not a valid path")

db = cantools.database.load_file(path)

if args.verify:
    # Verify    
    cnt, dups = DBC_Verify(db)
    
    if not cnt:
        print(f"{GREEN}{BOLD}Valid DBC!{END}{END} --- no duplicate CAN IDs")

    else:
        print(f"{RED}{BOLD}Invalid DBC!{END}{END}, found {cnt} duplicates:")
        for d in dups:
            print(f"{BOLD}-->{END} {d}")
        
if args.parse:
    if not args.bus:
        ErrorMessage("No <bus> parameter passed to parse command")
    if args.write_dir == ".":
        WarningMessage("No output dir specified ... using current dir")

    DBC_Parse(db, args.bus, args.buff_size, args.write_dir)