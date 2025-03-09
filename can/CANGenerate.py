import pandas as pd
import os
import sys

##########################The following code will be written to a header file##########################
header = """
/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
/**
 * @file CANMetaData.h
 * @brief CAN MetaData Definitions
 * 
 * Defines all data types used to send CAN messages between all the embedded systems within the car.
 * 
 */

#ifndef CANMETADATA_H
#define CANMETADATA_H

#include <stdint.h>

/**
 * CAN Message structure copied from BPS
 */

// Enum for ID's of all messages that can be sent across CAN bus
// IDs must be listed in strictly increasing order!
"""

def validate_can_id(can_id):
    if pd.isna(can_id) or not can_id:
        print("Error: No CAN ID provided.")
        sys.exit(1)
    if not str(can_id).startswith("0x"):
        print(f"Error: CAN ID '{can_id}' must start with '0x'.")
        sys.exit(1)

# Read all CSV files in the directory and concatenate them into a single DataFrame
csv_files = [f for f in os.listdir('.') if f.endswith('.csv')]
df_list = [pd.read_csv(f) for f in csv_files]
df = pd.concat(df_list, ignore_index=True)

# Drop rows with NaN values in 'CAN ID' column
df = df.dropna(subset=['CAN ID'])

# Validate CAN IDs
df['CAN ID'].apply(validate_can_id)

# Process the DataFrame
df = df.dropna(subset=['Data']) # remove all empty columns
df = df.drop_duplicates(subset='Data')
df['Length(bytes)'] = pd.to_numeric(df['Length(bytes)'], errors='coerce').fillna(0).astype(int) # convert to numeric, fill NaN with 0, and convert to int
df['Index Used'] = pd.to_numeric(df['Index Used'], errors='coerce').fillna(0).astype(int) # convert to numeric, fill NaN with 0, and convert to int
df['Data'] = df['Data'].str.replace(' ', '_').str.upper().str.replace('(','').str.replace(')','').str.replace('/','') # convert variable names to follow coding styles guide

# Filter out rows where 'Length(bytes)' is not a positive integer
invalid_rows = df[df['Length(bytes)'] <= 0]
if not invalid_rows.empty:
    print("Excluding the following rows due to invalid 'Length(bytes)':")
    print(invalid_rows[['CAN ID', 'Data', 'Length(bytes)']])

df = df[df['Length(bytes)'] > 0]

df = df.sort_values(by='CAN ID') # ensure IDs are in ascending order

# Generate the enum definition
enum_def = "typedef enum {\n"
df['ColumnLength'] = df['Data'].apply(len) 
max_len = df['ColumnLength'].max() # find maximum length of string in Data column
for index, row in df.iterrows(): 
    enum_def += ("    " + row['Data']).ljust(max_len + 5) + (" = " + str(row['CAN ID']) + ",\n")
enum_def += "    LARGEST_CAN_ID\n} CANID_t;\n"

# This code will never change and should be written directly after the enum definition
header_data = """
// Union of data that can be sent across CAN bus. Only one field must be filled out
typedef union {
    uint8_t b;
    uint16_t h;
    uint32_t w;
    float f;
    uint8_t bytes[8];   // use when data matches no normal datatype
} CANData_t;

/**
 * @note    idx is only used when an array of data needs to be sent.
 * @note    data is a union so only one of the fields should be filled out or bad stuff will happen.
 */
typedef struct {
    uint8_t idx;
    CANData_t data;
} CANPayload_t;

// This data type is used to push messages onto the queue
typedef struct {
    CANID_t id;
    CANPayload_t payload;
} CANMSG_t;

// Used to format the fields in the CAN metadata lookup table

struct CANLUTEntry {uint8_t idx_used : 1; uint8_t len : 7;};

extern const struct CANLUTEntry CanMetadataLUT[LARGEST_CAN_ID];

#endif
"""
with open("CANMetaData.h", "w") as file: file.write(header + enum_def + header_data)
##########################The following code will be written to a c file##########################
c_header = """
/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
/**
 * @file CANMetaData.c
 * @brief CAN MetaData Definitions
 * 
 * Defines all data types used to send CAN messages between all the embedded systems within the car.
 * @copyright Copyright (c) 2023 UT Longhorn Racing Solar
 * 
 */

#include "CANMetaData.h"

/**
 * @brief Lookup table to simplify user-defined packet structs. Contains fields that are always the same for every message of a given ID.
 *        Indexed by CANID_t values. Any changes or additions must be made in parallel with changes made to the CANID_t enum in CANbus.h
 */
"""

struct_def = "const struct CANLUTEntry CanMetadataLUT[LARGEST_CAN_ID] = {\n"
for index, row in df.iterrows(): 
    struct_def += ("    [" + row['Data'] + "]").ljust(max_len + 7) + " {.idx_used = " + str(row['Index Used']) + ", .len = " + str(row['Length(bytes)']) + "},\n"
struct_def = struct_def[:-2] +  "\n};\n"

with open("CANMetaData.c", "w") as file: file.write(c_header + struct_def)