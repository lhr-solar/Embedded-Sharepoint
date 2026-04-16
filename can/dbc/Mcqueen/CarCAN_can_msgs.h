#pragma once

#include <stdint.h>

/* ================= CAN ID Macros ================= */

#define CAN_ID_BPS_STATUS 0x1
#define CAN_ID_BPS_AGGREGATE_ARR 0xB
#define CAN_ID_VCU_STATUS 0x18
#define CAN_ID_CONTROLS_STATUS 0x19
#define CAN_ID_VCU_PRECHARGE_VOLTAGES 0x21
#define CAN_ID_ACCEL_BRAKE_POSITION 0x50
#define CAN_ID_ACCEL_BRAKE_POSITION_VOLTAGE 0x51
#define CAN_ID_DRIVER_INPUT_STATUS 0x60
#define CAN_ID_MPPT_A_POWER 0x200
#define CAN_ID_MPPT_A_STATUS 0x201
#define CAN_ID_MPPT_A_SETMODE 0x209
#define CAN_ID_MPPT_B_POWER 0x210
#define CAN_ID_MPPT_B_STATUS 0x211
#define CAN_ID_MPPT_B_SETMODE 0x219
#define CAN_ID_MPPT_C_POWER 0x220
#define CAN_ID_MPPT_C_STATUS 0x221
#define CAN_ID_MPPT_C_SETMODE 0x229
#define CAN_ID_LWS_STANDARD 0x2B0
#define CAN_ID_SUPP_BATTERY_STATUS 0x300
#define CAN_ID_SUPP_BATTERY_CHARGER_STATUS 0x301
#define CAN_ID_BBPDU_STATUS 0x350
#define CAN_ID_BBPDU_SET_SWITCHES 0x351
#define CAN_ID_BBPDU_SET_CURRENT_LIMIT 0x352
#define CAN_ID_PUMP_STATUS_FLOWRATE 0x500
#define CAN_ID_COOLANT_TEMPERATURE 0x501
#define CAN_ID_RADIATOR_FANSPEED 0x502
#define CAN_ID_LV_CARRIER_STATUS 0x600
#define CAN_ID_BRAKE_PRESSURE 0x650
#define CAN_ID_DISPLAY_CAMERAS_STATUS 0x700
#define CAN_ID_TELEMETRY_STATUS 0x750
#define CAN_ID_LWS_CONFIG 0x7C0

/* ================= CAN Length Macros ================= */

#define CAN_DLC_BPS_STATUS 7
#define CAN_DLC_BPS_AGGREGATE_ARR 7
#define CAN_DLC_VCU_STATUS 6
#define CAN_DLC_CONTROLS_STATUS 4
#define CAN_DLC_VCU_PRECHARGE_VOLTAGES 6
#define CAN_DLC_ACCEL_BRAKE_POSITION 5
#define CAN_DLC_ACCEL_BRAKE_POSITION_VOLTAGE 8
#define CAN_DLC_DRIVER_INPUT_STATUS 2
#define CAN_DLC_MPPT_A_POWER 8
#define CAN_DLC_MPPT_A_STATUS 5
#define CAN_DLC_MPPT_A_SETMODE 1
#define CAN_DLC_MPPT_B_POWER 8
#define CAN_DLC_MPPT_B_STATUS 5
#define CAN_DLC_MPPT_B_SETMODE 1
#define CAN_DLC_MPPT_C_POWER 8
#define CAN_DLC_MPPT_C_STATUS 5
#define CAN_DLC_MPPT_C_SETMODE 1
#define CAN_DLC_LWS_STANDARD 5
#define CAN_DLC_SUPP_BATTERY_STATUS 8
#define CAN_DLC_SUPP_BATTERY_CHARGER_STATUS 8
#define CAN_DLC_BBPDU_STATUS 5
#define CAN_DLC_BBPDU_SET_SWITCHES 4
#define CAN_DLC_BBPDU_SET_CURRENT_LIMIT 4
#define CAN_DLC_PUMP_STATUS_FLOWRATE 6
#define CAN_DLC_COOLANT_TEMPERATURE 4
#define CAN_DLC_RADIATOR_FANSPEED 8
#define CAN_DLC_LV_CARRIER_STATUS 2
#define CAN_DLC_BRAKE_PRESSURE 8
#define CAN_DLC_DISPLAY_CAMERAS_STATUS 2
#define CAN_DLC_TELEMETRY_STATUS 8
#define CAN_DLC_LWS_CONFIG 2


/* ================= Value Table Enums ================= */

typedef enum {
    BPS_STATUS_BPS_FAULT_DISCHARGING_OVERCURRENT = 16,
    BPS_STATUS_BPS_FAULT_CHARGING_OVERCURRENT = 15,
    BPS_STATUS_BPS_FAULT_ESTOP_3 = 14,
    BPS_STATUS_BPS_FAULT_ESTOP_2 = 13,
    BPS_STATUS_BPS_FAULT_ESTOP_1 = 12,
    BPS_STATUS_BPS_FAULT_ARRAY_PCHG_CONTACTOR_SENSE = 11,
    BPS_STATUS_BPS_FAULT_ARRAY_CONTACTOR_SENSE = 10,
    BPS_STATUS_BPS_FAULT_HV_MINUS_CONTACTOR_SENSE = 9,
    BPS_STATUS_BPS_FAULT_HV_PLUS_CONTACTOR_SENSE = 8,
    BPS_STATUS_BPS_FAULT_WATCHDOG = 7,
    BPS_STATUS_BPS_FAULT_ARRAY_PRECHARGE_TIMEOUT = 6,
    BPS_STATUS_BPS_FAULT_ELCON = 5,
    BPS_STATUS_BPS_FAULT_OVERTEMPERATURE = 4,
    BPS_STATUS_BPS_FAULT_REGEN = 3,
    BPS_STATUS_BPS_FAULT_UNDERVOLTAGE = 2,
    BPS_STATUS_BPS_FAULT_OVERVOLTAGE = 1,
    BPS_STATUS_BPS_FAULT_NO_FAULT = 0,
} bps_status_bps_fault_e;

typedef enum {
    VCU_STATUS_VCU_FAULT_MOTOR_HV_UNDERVOLTAGE = 6,
    VCU_STATUS_VCU_FAULT_MOTOR_HV_OVERVOLTAGE = 5,
    VCU_STATUS_VCU_FAULT_MOTOR_CONTROLLER_FAULT = 4,
    VCU_STATUS_VCU_FAULT_MOTOR_PRECHARGE_TIMEOUT = 3,
    VCU_STATUS_VCU_FAULT_MOTOR_PCHG_CONTACTOR_SENSE = 2,
    VCU_STATUS_VCU_FAULT_MOTOR_CONTACTOR_SENSE = 1,
    VCU_STATUS_VCU_FAULT_NO_FAULT = 0,
} vcu_status_vcu_fault_e;

typedef enum {
    CONTROLS_STATUS_CONTROLS_LEADER_FAULT_DRIVE_INPUTS_INVALID_ = 3,
    CONTROLS_STATUS_CONTROLS_LEADER_FAULT_BPS_WATCHDOG = 2,
    CONTROLS_STATUS_CONTROLS_LEADER_FAULT_STEERING_ANGLE_SENSOR_WATCHDOG = 1,
    CONTROLS_STATUS_CONTROLS_LEADER_FAULT_NO_FAULT = 0,
} controls_status_controls_leader_fault_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_13_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_13_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_13_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_13_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_13_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_14_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_14_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_14_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_14_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_14_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_15_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_15_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_15_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_15_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_15_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_16_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_16_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_16_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_16_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_16_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_9_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_9_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_9_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_9_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_9_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_10_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_10_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_10_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_10_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_10_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_11_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_11_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_11_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_11_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_11_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_12_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_12_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_12_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_12_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_12_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_5_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_5_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_5_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_5_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_5_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_6_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_6_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_6_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_6_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_6_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_7_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_7_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_7_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_7_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_7_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_8_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_8_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_8_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_8_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_8_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_1_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_1_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_1_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_1_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_1_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_2_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_2_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_2_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_2_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_2_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_3_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_3_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_3_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_3_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_3_e;

typedef enum {
    BBPDU_SET_SWITCHES_HSS_CONTROL_4_DO_NOTHING_YOU_RE_STUPID = 3,
    BBPDU_SET_SWITCHES_HSS_CONTROL_4_TURN_OFF = 2,
    BBPDU_SET_SWITCHES_HSS_CONTROL_4_TURN_ON = 1,
    BBPDU_SET_SWITCHES_HSS_CONTROL_4_DO_NOTHING_KEEP_CURRENT_STATE = 0,
} bbpdu_set_switches_hss_control_4_e;

typedef enum {
    LWS_CONFIG_LWS_CCW_RESETS_CALIBRATION_STATUS = 5,
    LWS_CONFIG_LWS_CCW_SETS_THE_SIGNAL_LWS_ANGLE_TO_0_ = 3,
} lws_config_lws_ccw_e;

/* ================= Message Structs ================= */

typedef struct {
    uint8_t BPS_Fault;
    uint8_t BPS_Charge_OK;
    uint8_t BPS_Regen_OK;
    uint8_t HV_Plus_Contactor_State;
    uint8_t HV_Minus_Contactor_State;
    uint8_t Array_Contactor_State;
    uint8_t Array_Precharge_Contactor_State;
    uint32_t Main_Battery_Voltage;
    int16_t Main_Battery_Avg_Temperature;
} bps_status_t;

typedef struct {
    uint8_t BPS_Tap_idx;
    uint16_t BPS_Voltage_Tap_Data;
    int32_t BPS_Temperature_Tap_Data;
} bps_aggregate_arr_t;

typedef struct {
    uint8_t VCU_Fault;
    uint8_t Motor_Contactor_State;
    uint8_t Motor_Precharge_Contactor_State;
    uint8_t Motor_Ready_To_Drive;
    uint8_t VCU_Driver_Input_OK;
    uint8_t VCU_Pedals_OK;
    uint8_t VCU_Regen_OK;
    uint8_t VCU_Regen_Active;
    uint8_t VCU_Steering_Angle_OK;
    uint8_t VCU_FSM_State;
    uint16_t VCU_FSM_Inputs;
} vcu_status_t;

typedef struct {
    uint8_t Controls_Leader_Fault;
    uint16_t Controls_Lighting_Fault;
} controls_status_t;

typedef struct {
    uint32_t Precharge_Battery_Voltage;
    uint32_t Precharge_Motor_Voltage;
} vcu_precharge_voltages_t;

typedef struct {
    uint8_t Accel_Pos_Main;
    uint8_t Accel_Pos_Redundant;
    uint8_t Brake_Pos_Main;
    uint8_t Brake_Pos_Redundant;
    uint8_t Accel_Pos_Main_Fault;
    uint8_t Accel_Pos_Redundant_Fault;
    uint8_t Brake_Pos_Main_Fault;
    uint8_t Brake_Pos_Redundant_Fault;
    uint8_t Brake_Pressure_1_Fault;
    uint8_t Brake_Pressure_2_Fault;
} accel_brake_position_t;

typedef struct {
    uint16_t Accel_Pos_Voltage_Main;
    uint16_t Accel_Pos_Voltage_Redundant;
    uint16_t Brake_Pos_Voltage_Main;
    uint16_t Brake_Pos_Voltage_Redundant;
} accel_brake_position_voltage_t;

typedef struct {
    uint8_t Ignition_Array;
    uint8_t Ignition_Motor;
    uint8_t Ignition_Off;
    uint8_t Cruise_Enable;
    uint8_t Cruise_Set;
    uint8_t Gear_Forward;
    uint8_t Gear_Neutral;
    uint8_t Gear_Reverse;
    uint8_t Hazard_Pressed;
    uint8_t Horn_Pressed;
    uint8_t Blinker_Left;
    uint8_t Blinker_Right;
    uint8_t PushToTalk_Pressed;
    uint8_t Regen_Activate;
    uint8_t Regen_Enable;
} driver_input_status_t;

typedef struct {
    int16_t MPPT_Vin;
    int16_t MPPT_Iin;
    int16_t MPPT_Vout;
    int16_t MPPT_Iout;
} mppt_a_power_t;

typedef struct {
    uint8_t MPPT_Mode;
    uint8_t MPPT_Fault;
    int8_t MPPT_Enabled;
    int8_t MPPT_AmbientTemperature;
    int8_t MPPT_HeatsinkTemperature;
} mppt_a_status_t;

typedef struct {
    uint8_t Boost_Enable;
} mppt_a_setmode_t;

typedef struct {
    int16_t MPPT_Vin;
    int16_t MPPT_Iin;
    int16_t MPPT_Vout;
    int16_t MPPT_Iout;
} mppt_b_power_t;

typedef struct {
    uint8_t MPPT_Mode;
    uint8_t MPPT_Fault;
    int8_t MPPT_Enabled;
    int8_t MPPT_AmbientTemperature;
    int8_t MPPT_HeatsinkTemperature;
} mppt_b_status_t;

typedef struct {
    uint8_t Boost_Enable;
} mppt_b_setmode_t;

typedef struct {
    int16_t MPPT_Vin;
    int16_t MPPT_Iin;
    int16_t MPPT_Vout;
    int16_t MPPT_Iout;
} mppt_c_power_t;

typedef struct {
    uint8_t MPPT_Mode;
    uint8_t MPPT_Fault;
    int8_t MPPT_Enabled;
    int8_t MPPT_AmbientTemperature;
    int8_t MPPT_HeatsinkTemperature;
} mppt_c_status_t;

typedef struct {
    uint8_t Boost_Enable;
} mppt_c_setmode_t;

typedef struct {
    int16_t LWS_Angle;
    uint8_t LWS_Speed;
    uint8_t LWS_OK;
    uint8_t LWS_CAL;
    uint8_t LWS_TRIM;
} lws_standard_t;

typedef struct {
    uint16_t Supplemental_Battery_Voltage;
} supp_battery_status_t;

typedef struct {
    uint8_t SuppCharger_Status;
    int16_t Supplemental_Battery_Current;
    uint16_t Supplemental_DCDC_Voltage;
    int16_t Supplemental_DCDC_Current;
} supp_battery_charger_status_t;

typedef struct {
    uint8_t HSS_Channel;
    uint8_t HSS_Fault;
    uint8_t HSS_Enabled;
    uint16_t HSS_Measured_Voltage;
    uint16_t HSS_Measured_Current;
} bbpdu_status_t;

typedef struct {
    uint8_t HSS_Control_13;
    uint8_t HSS_Control_14;
    uint8_t HSS_Control_15;
    uint8_t HSS_Control_16;
    uint8_t HSS_Control_9;
    uint8_t HSS_Control_10;
    uint8_t HSS_Control_11;
    uint8_t HSS_Control_12;
    uint8_t HSS_Control_5;
    uint8_t HSS_Control_6;
    uint8_t HSS_Control_7;
    uint8_t HSS_Control_8;
    uint8_t HSS_Control_1;
    uint8_t HSS_Control_2;
    uint8_t HSS_Control_3;
    uint8_t HSS_Control_4;
} bbpdu_set_switches_t;

typedef struct {
    uint8_t HSS_Channel;
    uint16_t HSS_Current_Limit;
} bbpdu_set_current_limit_t;

typedef struct {
    uint8_t Pump_DutyCycle;
    uint8_t Pump_Fault;
    uint16_t FlowRate_1;
    uint16_t FlowRate_2;
} pump_status_flowrate_t;

typedef struct {
    int16_t Coolant_Temperature_1;
    int16_t Coolant_Temperature_2;
} coolant_temperature_t;

typedef struct {
    uint16_t Radiator_Fan_Speed_Measurement_1;
    uint16_t Radiator_Fan_Speed_Measurement_2;
    uint16_t Radiator_Fan_Speed_Target_1;
    uint16_t Radiator_Fan_Speed_Target_2;
} radiator_fanspeed_t;

typedef struct {
    uint8_t LTC4421_HVDCDC_Selected;
    uint8_t LTC4421_HVDCDC_Fault;
    uint8_t LTC4421_HVDCDC_Valid;
    uint8_t LTC4421_SuppBatt_Selected;
    uint8_t LTC4421_SuppBatt_Fault;
    uint8_t LTC4421_SuppBatt_Valid;
    uint8_t LV_EN_SupplementalBattery;
    uint8_t LV_EN_PowerSupply;
} lv_carrier_status_t;

typedef struct {
    uint16_t Brake_Pressure_1;
    uint16_t Brake_Pressure_1_Voltage;
    uint16_t Brake_Pressure_2;
    uint16_t Brake_Pressure_2_Voltage;
} brake_pressure_t;

typedef struct {
    uint8_t Display_FrameRate;
    uint8_t Camera_Status_Backup;
    uint8_t Camera_Status_Left;
    uint8_t Camera_Status_Right;
} display_cameras_status_t;

typedef struct {
} telemetry_status_t;

typedef struct {
    uint8_t LWS_CCW;
} lws_config_t;

