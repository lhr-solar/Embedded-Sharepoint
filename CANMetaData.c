
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
const struct CanLUTEntry CanMetadataLUT[LARGEST_CAN_ID] = {
    [DASH_KILL_SWITCH]                                     {.idx_used = 0, .len = 1},
    [BPS_TRIP]                                             {.idx_used = 0, .len = 1},
    [ANY_SYSTEM_FAILURES]                                  {.idx_used = 0, .len = 1},
    [IGNITION]                                             {.idx_used = 0, .len = 1},
    [ANY_SYSTEM_SHUTOFF]                                   {.idx_used = 0, .len = 1},
    [BPS_TRIP]                                             {.idx_used = 0, .len = 1},
    [BPS_ALL_CLEAR]                                        {.idx_used = 0, .len = 1},
    [BPS_CONTACTOR_STATE]                                  {.idx_used = 0, .len = 1},
    [CURRENT_DATA]                                         {.idx_used = 0, .len = 4},
    [VOLTAGE_DATA_ARRAY]                                   {.idx_used = 1, .len = 5},
    [TEMPERATURE_DATA_ARRAY]                               {.idx_used = 1, .len = 5},
    [STATE_OF_CHARGE_DATA]                                 {.idx_used = 0, .len = 4},
    [WDOG_TRIGGERED]                                       {.idx_used = 0, .len = 1},
    [CAN_ERROR]                                            {.idx_used = 0, .len = 1},
    [BPS_COMMAND_MSG]                                      {.idx_used = 0, .len = 8},
    [SUPPLEMENTAL_VOLTAGE]                                 {.idx_used = 0, .len = 2},
    [CHARGING_ENABLED]                                     {.idx_used = 0, .len = 1},
    [CAR_STATE]                                            {.idx_used = 0, .len = 1},
    [CAR_DATA]                                             {.idx_used = 0, .len = 8},
    [MOTOR_CONTROLLER_BUS]                                 {.idx_used = 0, .len = 8},
    [VELOCITY]                                             {.idx_used = 0, .len = 8},
    [MOTOR_CONTROLLER_PHASE_CURRENT]                       {.idx_used = 0, .len = 8},
    [MOTOR_VOLTAGE_VECTOR]                                 {.idx_used = 0, .len = 8},
    [MOTOR_CURRENT_VECTOR]                                 {.idx_used = 0, .len = 8},
    [MOTOR_BACKEMF]                                        {.idx_used = 0, .len = 8},
    [MOTOR_TEMPERATURE]                                    {.idx_used = 0, .len = 8},
    [ODOMETER__BUS_AMP_HOURS]                              {.idx_used = 0, .len = 8},
    [ARRAY_CONTACTOR_STATE_CHANGE]                         {.idx_used = 0, .len = 1},
    [SUNSCATTER_A_MPPT_1_ARRAY_VOLTAGE_SETPOINT]           {.idx_used = 0, .len = 4},
    [SUNSCATTER_A_ARRAY_VOLTAGE_MEASUREMENT]               {.idx_used = 0, .len = 4},
    [SUNSCATTER_A_ARRAY_CURRENT_MEASUREMENT]               {.idx_used = 0, .len = 4},
    [SUNSCATTER_A_BATTERY_VOLTAGE_MEASUREMENT]             {.idx_used = 0, .len = 4},
    [SUNSCATTER_A_BATTERY_CURRENT_MEASUREMENT]             {.idx_used = 0, .len = 4},
    [SUNSCATTER_A_OVERRIDE_ENABLEDISABLE_COMMAND]          {.idx_used = 0, .len = 1},
    [SUNSCATTER_A_FAULT]                                   {.idx_used = 0, .len = 1},
    [SUNSCATTER_B_MPPT_2_ARRAY_VOLTAGE_SETPOINT]           {.idx_used = 0, .len = 4},
    [SUNSCATTER_B_ARRAY_VOLTAGE_MEASUREMENT]               {.idx_used = 0, .len = 4},
    [SUNSCATTER_B_ARRAY_CURRENT_MEASUREMENT]               {.idx_used = 0, .len = 4},
    [SUNSCATTER_B_BATTERY_VOLTAGE_MEASUREMENT]             {.idx_used = 0, .len = 4},
    [SUNSCATTER_B_BATTERY_CURRENT_MEASUREMENT]             {.idx_used = 0, .len = 4},
    [SUNSCATTER_B_OVERRIDE_ENABLEDISABLE_COMMAND]          {.idx_used = 0, .len = 1},
    [SUNSCATTER_B_FAULT]                                   {.idx_used = 0, .len = 1},
    [BLACKBODY_RTD_SENSOR_MEASUREMENT]                     {.idx_used = 0, .len = 5},
    [BLACKBODY_IRRADIANCE_SENSOR_1_MEASUREMENT]            {.idx_used = 0, .len = 4},
    [BLACKBODY_IRRADIANCE_SENSOR_2_MEASUREMENT]            {.idx_used = 0, .len = 4},
    [BLACKBODY_IRRADIANCERTD_BOARD_ENABLEDISABLE_COMMAND]  {.idx_used = 0, .len = 1},
    [BLACKBODY_IRRADIANCERTD_BOARD_FAULT]                  {.idx_used = 0, .len = 1},
    [PV_CURVE_TRACER_PROFILE]                              {.idx_used = 0, .len = 5}
};
