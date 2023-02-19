
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
typedef enum {
    DASH_KILL_SWITCH                                     = 0x001,
    BPS_TRIP                                             = 0x002,
    ANY_SYSTEM_FAILURES                                  = 0x003,
    IGNITION                                             = 0x004,
    ANY_SYSTEM_SHUTOFF                                   = 0x005,
    BPS_ALL_CLEAR                                        = 0x101,
    BPS_CONTACTOR_STATE                                  = 0x102,
    CURRENT_DATA                                         = 0x103,
    VOLTAGE_DATA_ARRAY                                   = 0x104,
    TEMPERATURE_DATA_ARRAY                               = 0x105,
    STATE_OF_CHARGE_DATA                                 = 0x106,
    WDOG_TRIGGERED                                       = 0x107,
    CAN_ERROR                                            = 0x108,
    BPS_COMMAND_MSG                                      = 0x109,
    SUPPLEMENTAL_VOLTAGE                                 = 0x10B,
    CHARGING_ENABLED                                     = 0x10C,
    CAR_STATE                                            = 0x580,
    CAR_DATA                                             = 0x581,
    MOTOR_CONTROLLER_BUS                                 = 0x242,
    VELOCITY                                             = 0x243,
    MOTOR_CONTROLLER_PHASE_CURRENT                       = 0x244,
    MOTOR_VOLTAGE_VECTOR                                 = 0x245,
    MOTOR_CURRENT_VECTOR                                 = 0x246,
    MOTOR_BACKEMF                                        = 0x247,
    MOTOR_TEMPERATURE                                    = 0x24B,
    ODOMETER__BUS_AMP_HOURS                              = 0x24E,
    ARRAY_CONTACTOR_STATE_CHANGE                         = 0x24F,
    SUNSCATTER_A_MPPT_1_ARRAY_VOLTAGE_SETPOINT           = 0x600,
    SUNSCATTER_A_ARRAY_VOLTAGE_MEASUREMENT               = 0x601,
    SUNSCATTER_A_ARRAY_CURRENT_MEASUREMENT               = 0x602,
    SUNSCATTER_A_BATTERY_VOLTAGE_MEASUREMENT             = 0x603,
    SUNSCATTER_A_BATTERY_CURRENT_MEASUREMENT             = 0x604,
    SUNSCATTER_A_OVERRIDE_ENABLEDISABLE_COMMAND          = 0x605,
    SUNSCATTER_A_FAULT                                   = 0x606,
    SUNSCATTER_B_MPPT_2_ARRAY_VOLTAGE_SETPOINT           = 0x610,
    SUNSCATTER_B_ARRAY_VOLTAGE_MEASUREMENT               = 0x611,
    SUNSCATTER_B_ARRAY_CURRENT_MEASUREMENT               = 0x612,
    SUNSCATTER_B_BATTERY_VOLTAGE_MEASUREMENT             = 0x613,
    SUNSCATTER_B_BATTERY_CURRENT_MEASUREMENT             = 0x614,
    SUNSCATTER_B_OVERRIDE_ENABLEDISABLE_COMMAND          = 0x615,
    SUNSCATTER_B_FAULT                                   = 0x616,
    BLACKBODY_RTD_SENSOR_MEASUREMENT                     = 0x620,
    BLACKBODY_IRRADIANCE_SENSOR_1_MEASUREMENT            = 0x630,
    BLACKBODY_IRRADIANCE_SENSOR_2_MEASUREMENT            = 0x631,
    BLACKBODY_IRRADIANCERTD_BOARD_ENABLEDISABLE_COMMAND  = 0x632,
    BLACKBODY_IRRADIANCERTD_BOARD_FAULT                  = 0x633,
    PV_CURVE_TRACER_PROFILE                              = 0x640,
    LARGEST_CAN_ID
} CANID_t;

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
