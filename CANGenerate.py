import pandas as pd

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

df = pd.read_excel("CANBusIDs.xlsx") #This generates the enum that assigns a CANID to a hash for the LUT
df = df.dropna(subset=['Data']) #remove all empty columns
df['Length(bytes)'] = df['Length(bytes)'].astype(int) #convert float to int
df['Index Used'] = df['Index Used'].astype(int) #convert float to int
df['Data'] = df['Data'].str.replace(' ', '_').str.upper().str.replace('(','').str.replace(')','').str.replace('/','') #convert variable names to follow coding styles guide
enum_def = "typedef enum {\n"
df['ColumnLength'] = df['Data'].apply(len) 
max_len = df['ColumnLength'].max() #find maximum length of string in Data column
for index, row in df.iterrows(): 
    enum_def += ("    " + row['Data']).ljust(max_len + 5) + (" = " + str(row['CAN ID']) + ",\n")
enum_def += "    LARGEST_CAN_ID\n} CANID_t;\n"

#This code will never change and should be written directly after the enum definition
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
    CANId_t id;
    CANPayload_t payload;
} CANMSG_t;

// Used to format the fields in the CAN metadata lookup table

struct CanLUTEntry {uint8_t idx_used : 1; uint8_t len : 7;};

extern const CanLUTEntry CanMetadataLUT[LARGEST_CAN_ID];

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
 *        Indexed by CANId_t values. Any changes or additions must be made in parallel with changes made to the CANID_t enum in CANbus.h
 */
"""

struct_def = "const struct CanLUTEntry CanMetadataLUT[LARGEST_CAN_ID] = {\n"
for index, row in df.iterrows(): 
    struct_def += ("    [" + row['Data'] + "]").ljust(max_len + 7) + " {.idx_used = " + str(row['Index Used']) + ", .len = " + str(row['Length(bytes)']) + "},\n"
struct_def = struct_def[:-2] +  "\n};\n"

with open("CANMetaData.c", "w") as file: file.write(c_header + struct_def)