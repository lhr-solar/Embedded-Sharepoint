#!/bin/bash

# Run Python script to generate .c and .h files
python ./parse_dbc.py ./dbc dbc/   # make sure output dir is writable

# Source directory (where Python script put the files)
SRC_DIR="dbc"

# Destination directory (relative to current working directory)
DEST_DIR="../driver"

# Create destination if it doesn't exist
mkdir -p "$DEST_DIR"

# Move all .c and .h files
mv "$SRC_DIR"/*.c "$DEST_DIR/Src"
mv "$SRC_DIR"/*.h "$DEST_DIR/Inc"

echo "All .c and .h files moved from $SRC_DIR to $DEST_DIR."
