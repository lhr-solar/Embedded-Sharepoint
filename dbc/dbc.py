# Python Script to convert all IDs in a DBC file to a can_recv_entries.h file

# run pip install cantools

# test --> python3 dbc.py examples/tessie.dbc 0x01 10 test_write

import cantools
import sys
import os
from pathlib import Path

bold = "\033[1m"
end = "\033[0m"

if len(sys.argv) < 5: 
    print(f"{bold} not enough args (dbc file path, can bus number, default size, output dir) {end}")
    exit();

DBC_FILE_PATH = Path(sys.argv[1])
CAN_BUS = Path(sys.argv[2])
DEFAULT_SIZE = Path(sys.argv[3])
WRITE_FILE_DIR = Path(sys.argv[4])

if not os.path.exists(WRITE_FILE_DIR):
    print(f"{bold} write directory doesn't exist ... making one {end}")
    os.mkdir(WRITE_FILE_DIR)

db = cantools.database.load_file(DBC_FILE_PATH)

fname_recv = f"{WRITE_FILE_DIR}/can{CAN_BUS}_recv_entries.h"
fname_utils = f"{WRITE_FILE_DIR}/can{CAN_BUS}_utils.h"

recv_string = ""
utils_string = ""

for m in db.messages:
    id = hex(m.frame_id)
    
    if not m.length:
        size = DEFAULT_SIZE
    else:
        size = m.length
    size = m.length 

    name = m.name

    recv_string += f"CAN_RECV_ENTRY({id}, {size}, false)\n"
    utils_string += f"#define {name} {id}\n"

with open(fname_recv, "w+") as f1:
    f1.write(recv_string)

with open(fname_utils, "w+") as f2:
    f2.write(utils_string)

print("Done!")