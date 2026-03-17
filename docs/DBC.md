# Table of Contents

- [Bpscan](#BPSCAN)
- [Carcan](#CarCAN)
- [Motorcan](#MotorCAN)

<a name="BPSCAN"></a>
# BPSCAN.dbc

[GitHub Link](https://github.com/lhr-solar/Embedded-Sharepoint/tree/main/can/dbc/BPSCAN.dbc)

## Messages

| ID | Type | Name | DLC | Signals |
|----|------|------|-----|---------|
| 0x001 | Std | BPS_Status | 7 | BPS_Fault, BPS_Charge_OK, BPS_Regen_OK, HV_Plus_Contactor_State, HV_Minus_Contactor_State, Array_Contactor_State, Array_Precharge_Contactor_State, Main_Battery_Voltage, Main_Battery_Avg_Temperature |
| 0x002 | Std | BPS_Voltage_Temperature_Arr_0 | 7 | BPS_Tap_idx, BPS_VoltTemp_BQ_Fault, BPS_Temperature_Tap_Fault, BPS_Voltage_Tap_Data, BPS_Temperature_Tap_Data, BPS_Temperature_Tap_RawV |
| 0x003 | Std | BPS_Voltage_Temperature_Arr_1 | 7 | BPS_Tap_idx, BPS_VoltTemp_BQ_Fault, BPS_Temperature_Tap_Fault, BPS_Voltage_Tap_Data, BPS_Temperature_Tap_Data, BPS_Temperature_Tap_RawV |
| 0x004 | Std | BPS_Voltage_Temperature_Arr_2 | 7 | BPS_Tap_idx, BPS_VoltTemp_BQ_Fault, BPS_Temperature_Tap_Fault, BPS_Voltage_Tap_Data, BPS_Temperature_Tap_Data, BPS_Temperature_Tap_RawV |
| 0x005 | Std | BPS_Voltage_Temperature_Arr_3 | 7 | BPS_Tap_idx, BPS_VoltTemp_BQ_Fault, BPS_Temperature_Tap_Fault, BPS_Voltage_Tap_Data, BPS_Temperature_Tap_Data, BPS_Temperature_Tap_RawV |
| 0x006 | Std | BPS_Voltage_Temperature_Arr_4 | 7 | BPS_Tap_idx, BPS_VoltTemp_BQ_Fault, BPS_Temperature_Tap_Fault, BPS_Voltage_Tap_Data, BPS_Temperature_Tap_Data, BPS_Temperature_Tap_RawV |
| 0x007 | Std | BPS_Voltage_Temperature_Arr_5 | 7 | BPS_Tap_idx, BPS_VoltTemp_BQ_Fault, BPS_Temperature_Tap_Fault, BPS_Voltage_Tap_Data, BPS_Temperature_Tap_Data, BPS_Temperature_Tap_RawV |
| 0x008 | Std | BPS_Voltage_Temperature_Arr_6 | 7 | BPS_Tap_idx, BPS_VoltTemp_BQ_Fault, BPS_Temperature_Tap_Fault, BPS_Voltage_Tap_Data, BPS_Temperature_Tap_Data, BPS_Temperature_Tap_RawV |
| 0x009 | Std | BPS_Voltage_Temperature_Arr_7 | 7 | BPS_Tap_idx, BPS_VoltTemp_BQ_Fault, BPS_Temperature_Tap_Fault, BPS_Voltage_Tap_Data, BPS_Temperature_Tap_Data, BPS_Temperature_Tap_RawV |
| 0x00A | Std | BPS_Pack_Current | 5 | Main_Battery_Current, Main_Battery_Current_RawV |
| 0x020 | Std | BPS_Precharge_Voltages | 6 | Precharge_Battery_Voltage, Precharge_Array_Voltage |

## Signals

| Name | Message | Bits [start:end] | Length | Factor | Offset | Min | Max | Unit |
|------|---------|------------------|--------|--------|--------|-----|-----|------|
| BPS_Fault | BPS_Status | [0:7] | 8 | 1 | 0 | None | None |  |
| BPS_Charge_OK | BPS_Status | [8:8] | 1 | 1 | 0 | None | None |  |
| BPS_Regen_OK | BPS_Status | [9:9] | 1 | 1 | 0 | None | None |  |
| HV_Plus_Contactor_State | BPS_Status | [10:10] | 1 | 1 | 0 | None | None |  |
| HV_Minus_Contactor_State | BPS_Status | [11:11] | 1 | 1 | 0 | None | None |  |
| Array_Contactor_State | BPS_Status | [12:12] | 1 | 1 | 0 | None | None |  |
| Array_Precharge_Contactor_State | BPS_Status | [13:13] | 1 | 1 | 0 | None | None |  |
| Main_Battery_Voltage | BPS_Status | [16:39] | 24 | 0.001 | 0 | 0 | 16777.215 | V |
| Main_Battery_Avg_Temperature | BPS_Status | [40:55] | 16 | 0.01 | 0 | -327.68 | 327.67 |  |
| BPS_Tap_idx | BPS_Voltage_Temperature_Arr_0 | [0:4] | 5 | 1 | 0 | 0 | 31 |  |
| BPS_VoltTemp_BQ_Fault | BPS_Voltage_Temperature_Arr_0 | [5:5] | 1 | 1 | 0 | None | None |  |
| BPS_Temperature_Tap_Fault | BPS_Voltage_Temperature_Arr_0 | [6:6] | 1 | 1 | 0 | None | None |  |
| BPS_Voltage_Tap_Data | BPS_Voltage_Temperature_Arr_0 | [8:23] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| BPS_Temperature_Tap_Data | BPS_Voltage_Temperature_Arr_0 | [24:39] | 16 | 0.01 | 0 | -327.68 | 327.67 | C |
| BPS_Temperature_Tap_RawV | BPS_Voltage_Temperature_Arr_0 | [40:55] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| BPS_Tap_idx | BPS_Voltage_Temperature_Arr_1 | [0:4] | 5 | 1 | 0 | 0 | 31 |  |
| BPS_VoltTemp_BQ_Fault | BPS_Voltage_Temperature_Arr_1 | [5:5] | 1 | 1 | 0 | None | None |  |
| BPS_Temperature_Tap_Fault | BPS_Voltage_Temperature_Arr_1 | [6:6] | 1 | 1 | 0 | None | None |  |
| BPS_Voltage_Tap_Data | BPS_Voltage_Temperature_Arr_1 | [8:23] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| BPS_Temperature_Tap_Data | BPS_Voltage_Temperature_Arr_1 | [24:39] | 16 | 0.01 | 0 | -327.68 | 327.67 | C |
| BPS_Temperature_Tap_RawV | BPS_Voltage_Temperature_Arr_1 | [40:55] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| BPS_Tap_idx | BPS_Voltage_Temperature_Arr_2 | [0:4] | 5 | 1 | 0 | 0 | 31 |  |
| BPS_VoltTemp_BQ_Fault | BPS_Voltage_Temperature_Arr_2 | [5:5] | 1 | 1 | 0 | None | None |  |
| BPS_Temperature_Tap_Fault | BPS_Voltage_Temperature_Arr_2 | [6:6] | 1 | 1 | 0 | None | None |  |
| BPS_Voltage_Tap_Data | BPS_Voltage_Temperature_Arr_2 | [8:23] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| BPS_Temperature_Tap_Data | BPS_Voltage_Temperature_Arr_2 | [24:39] | 16 | 0.01 | 0 | -327.68 | 327.67 | C |
| BPS_Temperature_Tap_RawV | BPS_Voltage_Temperature_Arr_2 | [40:55] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| BPS_Tap_idx | BPS_Voltage_Temperature_Arr_3 | [0:4] | 5 | 1 | 0 | 0 | 31 |  |
| BPS_VoltTemp_BQ_Fault | BPS_Voltage_Temperature_Arr_3 | [5:5] | 1 | 1 | 0 | None | None |  |
| BPS_Temperature_Tap_Fault | BPS_Voltage_Temperature_Arr_3 | [6:6] | 1 | 1 | 0 | None | None |  |
| BPS_Voltage_Tap_Data | BPS_Voltage_Temperature_Arr_3 | [8:23] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| BPS_Temperature_Tap_Data | BPS_Voltage_Temperature_Arr_3 | [24:39] | 16 | 0.01 | 0 | -327.68 | 327.67 | C |
| BPS_Temperature_Tap_RawV | BPS_Voltage_Temperature_Arr_3 | [40:55] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| BPS_Tap_idx | BPS_Voltage_Temperature_Arr_4 | [0:4] | 5 | 1 | 0 | 0 | 31 |  |
| BPS_VoltTemp_BQ_Fault | BPS_Voltage_Temperature_Arr_4 | [5:5] | 1 | 1 | 0 | None | None |  |
| BPS_Temperature_Tap_Fault | BPS_Voltage_Temperature_Arr_4 | [6:6] | 1 | 1 | 0 | None | None |  |
| BPS_Voltage_Tap_Data | BPS_Voltage_Temperature_Arr_4 | [8:23] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| BPS_Temperature_Tap_Data | BPS_Voltage_Temperature_Arr_4 | [24:39] | 16 | 0.01 | 0 | -327.68 | 327.67 | C |
| BPS_Temperature_Tap_RawV | BPS_Voltage_Temperature_Arr_4 | [40:55] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| BPS_Tap_idx | BPS_Voltage_Temperature_Arr_5 | [0:4] | 5 | 1 | 0 | 0 | 31 |  |
| BPS_VoltTemp_BQ_Fault | BPS_Voltage_Temperature_Arr_5 | [5:5] | 1 | 1 | 0 | None | None |  |
| BPS_Temperature_Tap_Fault | BPS_Voltage_Temperature_Arr_5 | [6:6] | 1 | 1 | 0 | None | None |  |
| BPS_Voltage_Tap_Data | BPS_Voltage_Temperature_Arr_5 | [8:23] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| BPS_Temperature_Tap_Data | BPS_Voltage_Temperature_Arr_5 | [24:39] | 16 | 0.01 | 0 | -327.68 | 327.67 | C |
| BPS_Temperature_Tap_RawV | BPS_Voltage_Temperature_Arr_5 | [40:55] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| BPS_Tap_idx | BPS_Voltage_Temperature_Arr_6 | [0:4] | 5 | 1 | 0 | 0 | 31 |  |
| BPS_VoltTemp_BQ_Fault | BPS_Voltage_Temperature_Arr_6 | [5:5] | 1 | 1 | 0 | None | None |  |
| BPS_Temperature_Tap_Fault | BPS_Voltage_Temperature_Arr_6 | [6:6] | 1 | 1 | 0 | None | None |  |
| BPS_Voltage_Tap_Data | BPS_Voltage_Temperature_Arr_6 | [8:23] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| BPS_Temperature_Tap_Data | BPS_Voltage_Temperature_Arr_6 | [24:39] | 16 | 0.01 | 0 | -327.68 | 327.67 | C |
| BPS_Temperature_Tap_RawV | BPS_Voltage_Temperature_Arr_6 | [40:55] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| BPS_Tap_idx | BPS_Voltage_Temperature_Arr_7 | [0:4] | 5 | 1 | 0 | 0 | 31 |  |
| BPS_VoltTemp_BQ_Fault | BPS_Voltage_Temperature_Arr_7 | [5:5] | 1 | 1 | 0 | None | None |  |
| BPS_Temperature_Tap_Fault | BPS_Voltage_Temperature_Arr_7 | [6:6] | 1 | 1 | 0 | None | None |  |
| BPS_Voltage_Tap_Data | BPS_Voltage_Temperature_Arr_7 | [8:23] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| BPS_Temperature_Tap_Data | BPS_Voltage_Temperature_Arr_7 | [24:39] | 16 | 0.01 | 0 | -327.68 | 327.67 | C |
| BPS_Temperature_Tap_RawV | BPS_Voltage_Temperature_Arr_7 | [40:55] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| Main_Battery_Current | BPS_Pack_Current | [0:23] | 24 | 0.001 | 0 | -8388.608 | 8388.607 | A |
| Main_Battery_Current_RawV | BPS_Pack_Current | [24:39] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| Precharge_Battery_Voltage | BPS_Precharge_Voltages | [0:23] | 24 | 0.001 | 0 | 0 | 16777.215 | V |
| Precharge_Array_Voltage | BPS_Precharge_Voltages | [24:47] | 24 | 0.001 | 0 | 0 | 16777.215 | V |


<a name="CarCAN"></a>
# CarCAN.dbc

[GitHub Link](https://github.com/lhr-solar/Embedded-Sharepoint/tree/main/can/dbc/CarCAN.dbc)

## Messages

| ID | Type | Name | DLC | Signals |
|----|------|------|-----|---------|
| 0x010 | Std | VCU_Status | 3 | VCU_Fault, Motor_Contactor_State, Motor_Precharge_Contactor_State, Motor_Ready_To_Drive, VCU_Driver_Input_OK, VCU_Pedals_OK, VCU_Regen_OK, VCU_Regen_Active, VCU_FSM_State |
| 0x015 | Std | Controls_Status | 4 | Controls_Leader_Fault, Controls_Lighting_Fault |
| 0x021 | Std | VCU_Precharge_Voltages | 6 | Precharge_Battery_Voltage, Precharge_Motor_Voltage |
| 0x050 | Std | Accel_Brake_Position | 5 | Accel_Pos_Main, Accel_Pos_Redundant, Brake_Pos_Main, Brake_Pos_Redundant, Accel_Pos_Main_Fault, Accel_Pos_Redundant_Fault, Brake_Pos_Main_Fault, Brake_Pos_Redundant_Fault, Brake_Pressure_1_Fault, Brake_Pressure_2_Fault |
| 0x051 | Std | Accel_Brake_Position_Voltage | 8 | Accel_Pos_Voltage_Main, Accel_Pos_Voltage_Redundant, Brake_Pos_Voltage_Main, Brake_Pos_Voltage_Redundant |
| 0x060 | Std | Driver_Input_Status | 2 | Ignition_Array, Ignition_Motor, Ignition_Off, Cruise_Enable, Cruise_Set, Gear_Forward, Gear_Neutral, Gear_Reverse, Hazard_Pressed, Horn_Pressed, Blinker_Left, Blinker_Right, PushToTalk_Pressed, Regen_Activate, Regen_Enable |
| 0x200 | Std | MPPT_A_Power | 8 | MPPT_Vin, MPPT_Iin, MPPT_Vout, MPPT_Iout |
| 0x201 | Std | MPPT_A_Status | 5 | MPPT_Mode, MPPT_Fault, MPPT_Enabled, MPPT_AmbientTemperature, MPPT_HeatsinkTemperature |
| 0x209 | Std | MPPT_A_SetMode | 1 | Boost_Enable |
| 0x210 | Std | MPPT_B_Power | 8 | MPPT_Vin, MPPT_Iin, MPPT_Vout, MPPT_Iout |
| 0x211 | Std | MPPT_B_Status | 5 | MPPT_Mode, MPPT_Fault, MPPT_Enabled, MPPT_AmbientTemperature, MPPT_HeatsinkTemperature |
| 0x219 | Std | MPPT_B_SetMode | 1 | Boost_Enable |
| 0x220 | Std | MPPT_C_Power | 8 | MPPT_Vin, MPPT_Iin, MPPT_Vout, MPPT_Iout |
| 0x221 | Std | MPPT_C_Status | 5 | MPPT_Mode, MPPT_Fault, MPPT_Enabled, MPPT_AmbientTemperature, MPPT_HeatsinkTemperature |
| 0x229 | Std | MPPT_C_SetMode | 1 | Boost_Enable |
| 0x2B0 | Std | LWS_Standard | 5 | LWS_Angle, LWS_Speed, LWS_OK, LWS_CAL, LWS_TRIM |
| 0x300 | Std | Supp_Battery_Status | 8 | Supplemental_Battery_Voltage |
| 0x301 | Std | Supp_Battery_Charger_Status | 8 | SuppCharger_Status, Supplemental_Battery_Current, Supplemental_DCDC_Voltage, Supplemental_DCDC_Current |
| 0x350 | Std | BBPDU_Status | 5 | HSS_Channel, HSS_Fault, HSS_Enabled, HSS_Measured_Voltage, HSS_Measured_Current |
| 0x351 | Std | BBPDU_Set_Switches | 4 | HSS_Control_13, HSS_Control_14, HSS_Control_15, HSS_Control_16, HSS_Control_9, HSS_Control_10, HSS_Control_11, HSS_Control_12, HSS_Control_5, HSS_Control_6, HSS_Control_7, HSS_Control_8, HSS_Control_1, HSS_Control_2, HSS_Control_3, HSS_Control_4 |
| 0x352 | Std | BBPDU_Set_Current_Limit | 4 | HSS_Channel, HSS_Current_Limit |
| 0x500 | Std | Pump_Status_FlowRate | 6 | Pump_DutyCycle, Pump_Fault, FlowRate_1, FlowRate_2 |
| 0x501 | Std | Coolant_Temperature | 4 | Coolant_Temperature_1, Coolant_Temperature_2 |
| 0x502 | Std | Radiator_FanSpeed | 8 | Radiator_Fan_Speed_Measurement_1, Radiator_Fan_Speed_Measurement_2, Radiator_Fan_Speed_Target_1, Radiator_Fan_Speed_Target_2 |
| 0x600 | Std | LV_Carrier_Status | 2 | LTC4421_HVDCDC_Selected, LTC4421_HVDCDC_Fault, LTC4421_HVDCDC_Valid, LTC4421_SuppBatt_Selected, LTC4421_SuppBatt_Fault, LTC4421_SuppBatt_Valid, LV_EN_SupplementalBattery, LV_EN_PowerSupply |
| 0x650 | Std | Brake_Pressure | 8 | Brake_Pressure_1, Brake_Pressure_1_Voltage, Brake_Pressure_2, Brake_Pressure_2_Voltage |
| 0x670 | Std | Lighting_Board0_Status | 8 | Lights_Faults, Active_Light_Headlights, Active_Light_Brake, Active_Light_RTurn, Active_Light_LTurn, Active_Light_Strobe, Active_Light_Custom1, Active_Light_Custom2, Active_Light_Custom3, Addr_LED_Current, LED1_Current, LED2_Current |
| 0x671 | Std | Lighting_Board1_Status | 8 | Lights_Faults, Active_Light_Headlights, Active_Light_Brake, Active_Light_RTurn, Active_Light_LTurn, Active_Light_Strobe, Active_Light_Custom1, Active_Light_Custom2, Active_Light_Custom3, Addr_LED_Current, LED1_Current, LED2_Current |
| 0x672 | Std | Lighting_Board2_Status | 8 | Lights_Faults, Active_Light_Headlights, Active_Light_Brake, Active_Light_RTurn, Active_Light_LTurn, Active_Light_Strobe, Active_Light_Custom1, Active_Light_Custom2, Active_Light_Custom3, Addr_LED_Current, LED1_Current, LED2_Current |
| 0x673 | Std | Lighting_Board3_Status | 8 | Lights_Faults, Active_Light_Headlights, Active_Light_Brake, Active_Light_RTurn, Active_Light_LTurn, Active_Light_Strobe, Active_Light_Custom1, Active_Light_Custom2, Active_Light_Custom3, Addr_LED_Current, LED1_Current, LED2_Current |
| 0x674 | Std | Lighting_Board4_Status | 8 | Lights_Faults, Active_Light_Headlights, Active_Light_Brake, Active_Light_RTurn, Active_Light_LTurn, Active_Light_Strobe, Active_Light_Custom1, Active_Light_Custom2, Active_Light_Custom3, Addr_LED_Current, LED1_Current, LED2_Current |
| 0x675 | Std | Lighting_Board5_Status | 8 | Lights_Faults, Active_Light_Headlights, Active_Light_Brake, Active_Light_RTurn, Active_Light_LTurn, Active_Light_Strobe, Active_Light_Custom1, Active_Light_Custom2, Active_Light_Custom3, Addr_LED_Current, LED1_Current, LED2_Current |
| 0x676 | Std | Lighting_Board6_Status | 8 | Lights_Faults, Active_Light_Headlights, Active_Light_Brake, Active_Light_RTurn, Active_Light_LTurn, Active_Light_Strobe, Active_Light_Custom1, Active_Light_Custom2, Active_Light_Custom3, Addr_LED_Current, LED1_Current, LED2_Current |
| 0x700 | Std | Display_Cameras_Status | 2 | Display_FrameRate, Camera_Status_Backup, Camera_Status_Left, Camera_Status_Right |
| 0x750 | Std | Telemetry_Status | 8 |  |
| 0x7C0 | Std | LWS_Config | 2 | LWS_CCW |

## Signals

| Name | Message | Bits [start:end] | Length | Factor | Offset | Min | Max | Unit |
|------|---------|------------------|--------|--------|--------|-----|-----|------|
| VCU_Fault | VCU_Status | [0:7] | 8 | 1 | 0 | None | None |  |
| Motor_Contactor_State | VCU_Status | [8:8] | 1 | 1 | 0 | None | None |  |
| Motor_Precharge_Contactor_State | VCU_Status | [9:9] | 1 | 1 | 0 | None | None |  |
| Motor_Ready_To_Drive | VCU_Status | [10:10] | 1 | 1 | 0 | None | None |  |
| VCU_Driver_Input_OK | VCU_Status | [11:11] | 1 | 1 | 0 | None | None |  |
| VCU_Pedals_OK | VCU_Status | [12:12] | 1 | 1 | 0 | None | None |  |
| VCU_Regen_OK | VCU_Status | [13:13] | 1 | 1 | 0 | None | None |  |
| VCU_Regen_Active | VCU_Status | [14:14] | 1 | 1 | 0 | None | None |  |
| VCU_FSM_State | VCU_Status | [16:19] | 4 | 1 | 0 | None | None |  |
| Controls_Leader_Fault | Controls_Status | [0:7] | 8 | 1 | 0 | None | None |  |
| Controls_Lighting_Fault | Controls_Status | [16:31] | 16 | 1 | 0 | None | None |  |
| Precharge_Battery_Voltage | VCU_Precharge_Voltages | [0:23] | 24 | 0.001 | 0 | 0 | 16777.215 | V |
| Precharge_Motor_Voltage | VCU_Precharge_Voltages | [24:47] | 24 | 0.001 | 0 | 0 | 16777.215 | V |
| Accel_Pos_Main | Accel_Brake_Position | [0:7] | 8 | 1 | 0 | 0 | 100 | Percent |
| Accel_Pos_Redundant | Accel_Brake_Position | [8:15] | 8 | 1 | 0 | 0 | 100 | Percent |
| Brake_Pos_Main | Accel_Brake_Position | [16:23] | 8 | 1 | 0 | 0 | 100 | Percent |
| Brake_Pos_Redundant | Accel_Brake_Position | [24:31] | 8 | 1 | 0 | 0 | 100 | Percent |
| Accel_Pos_Main_Fault | Accel_Brake_Position | [32:32] | 1 | 1 | 0 | None | None |  |
| Accel_Pos_Redundant_Fault | Accel_Brake_Position | [33:33] | 1 | 1 | 0 | None | None |  |
| Brake_Pos_Main_Fault | Accel_Brake_Position | [34:34] | 1 | 1 | 0 | None | None |  |
| Brake_Pos_Redundant_Fault | Accel_Brake_Position | [35:35] | 1 | 1 | 0 | None | None |  |
| Brake_Pressure_1_Fault | Accel_Brake_Position | [36:36] | 1 | 1 | 0 | None | None |  |
| Brake_Pressure_2_Fault | Accel_Brake_Position | [37:37] | 1 | 1 | 0 | None | None |  |
| Accel_Pos_Voltage_Main | Accel_Brake_Position_Voltage | [0:15] | 16 | 0.001 | 0 | 0 | 3.3 | V |
| Accel_Pos_Voltage_Redundant | Accel_Brake_Position_Voltage | [16:31] | 16 | 0.001 | 0 | 0 | 3.3 | V |
| Brake_Pos_Voltage_Main | Accel_Brake_Position_Voltage | [32:47] | 16 | 0.001 | 0 | 0 | 3.3 | V |
| Brake_Pos_Voltage_Redundant | Accel_Brake_Position_Voltage | [48:63] | 16 | 0.001 | 0 | 0 | 3.3 | V |
| Ignition_Array | Driver_Input_Status | [0:0] | 1 | 1 | 0 | None | None |  |
| Ignition_Motor | Driver_Input_Status | [1:1] | 1 | 1 | 0 | None | None |  |
| Ignition_Off | Driver_Input_Status | [2:2] | 1 | 1 | 0 | None | None |  |
| Cruise_Enable | Driver_Input_Status | [3:3] | 1 | 1 | 0 | None | None |  |
| Cruise_Set | Driver_Input_Status | [4:4] | 1 | 1 | 0 | None | None |  |
| Gear_Forward | Driver_Input_Status | [5:5] | 1 | 1 | 0 | None | None |  |
| Gear_Neutral | Driver_Input_Status | [6:6] | 1 | 1 | 0 | None | None |  |
| Gear_Reverse | Driver_Input_Status | [7:7] | 1 | 1 | 0 | None | None |  |
| Hazard_Pressed | Driver_Input_Status | [8:8] | 1 | 1 | 0 | None | None |  |
| Horn_Pressed | Driver_Input_Status | [9:9] | 1 | 1 | 0 | None | None |  |
| Blinker_Left | Driver_Input_Status | [10:10] | 1 | 1 | 0 | None | None |  |
| Blinker_Right | Driver_Input_Status | [11:11] | 1 | 1 | 0 | None | None |  |
| PushToTalk_Pressed | Driver_Input_Status | [12:12] | 1 | 1 | 0 | None | None |  |
| Regen_Activate | Driver_Input_Status | [13:13] | 1 | 1 | 0 | None | None |  |
| Regen_Enable | Driver_Input_Status | [14:14] | 1 | 1 | 0 | None | None |  |
| MPPT_Vin | MPPT_A_Power | [7:22] | 16 | 0.01 | 0 | -327.68 | 327.67 | V |
| MPPT_Iin | MPPT_A_Power | [23:38] | 16 | 0.0005 | 0 | -16.384 | 16.3835 | A |
| MPPT_Vout | MPPT_A_Power | [39:54] | 16 | 0.01 | 0 | -327.68 | 327.67 | V |
| MPPT_Iout | MPPT_A_Power | [55:70] | 16 | 0.0005 | 0 | None | None | A |
| MPPT_Mode | MPPT_A_Status | [0:7] | 8 | 1 | 0 | None | None |  |
| MPPT_Fault | MPPT_A_Status | [8:15] | 8 | 1 | 0 | None | None |  |
| MPPT_Enabled | MPPT_A_Status | [16:23] | 8 | 1 | 0 | None | None |  |
| MPPT_AmbientTemperature | MPPT_A_Status | [31:38] | 8 | 1 | 0 | -128 | 127 | C |
| MPPT_HeatsinkTemperature | MPPT_A_Status | [39:46] | 8 | 1 | 0 | -128 | 127 | C |
| Boost_Enable | MPPT_A_SetMode | [0:0] | 1 | 1 | 0 | 0 | 1 |  |
| MPPT_Vin | MPPT_B_Power | [7:22] | 16 | 0.01 | 0 | -327.68 | 327.67 | V |
| MPPT_Iin | MPPT_B_Power | [23:38] | 16 | 0.0005 | 0 | -16.384 | 16.3835 | A |
| MPPT_Vout | MPPT_B_Power | [39:54] | 16 | 0.01 | 0 | -327.68 | 327.67 | V |
| MPPT_Iout | MPPT_B_Power | [55:70] | 16 | 0.0005 | 0 | None | None | A |
| MPPT_Mode | MPPT_B_Status | [0:7] | 8 | 1 | 0 | None | None |  |
| MPPT_Fault | MPPT_B_Status | [8:15] | 8 | 1 | 0 | None | None |  |
| MPPT_Enabled | MPPT_B_Status | [16:23] | 8 | 1 | 0 | None | None |  |
| MPPT_AmbientTemperature | MPPT_B_Status | [31:38] | 8 | 1 | 0 | -128 | 127 | C |
| MPPT_HeatsinkTemperature | MPPT_B_Status | [39:46] | 8 | 1 | 0 | -128 | 127 | C |
| Boost_Enable | MPPT_B_SetMode | [0:0] | 1 | 1 | 0 | 0 | 1 |  |
| MPPT_Vin | MPPT_C_Power | [7:22] | 16 | 0.01 | 0 | -327.68 | 327.67 | V |
| MPPT_Iin | MPPT_C_Power | [23:38] | 16 | 0.0005 | 0 | -16.384 | 16.3835 | A |
| MPPT_Vout | MPPT_C_Power | [39:54] | 16 | 0.01 | 0 | -327.68 | 327.67 | V |
| MPPT_Iout | MPPT_C_Power | [55:70] | 16 | 0.0005 | 0 | None | None | A |
| MPPT_Mode | MPPT_C_Status | [0:7] | 8 | 1 | 0 | None | None |  |
| MPPT_Fault | MPPT_C_Status | [8:15] | 8 | 1 | 0 | None | None |  |
| MPPT_Enabled | MPPT_C_Status | [16:23] | 8 | 1 | 0 | None | None |  |
| MPPT_AmbientTemperature | MPPT_C_Status | [31:38] | 8 | 1 | 0 | -128 | 127 | C |
| MPPT_HeatsinkTemperature | MPPT_C_Status | [39:46] | 8 | 1 | 0 | -128 | 127 | C |
| Boost_Enable | MPPT_C_SetMode | [0:0] | 1 | 1 | 0 | 0 | 1 |  |
| LWS_Angle | LWS_Standard | [0:15] | 16 | 0.1 | 0 | -3276.8 | 3276.7 | Degrees |
| LWS_Speed | LWS_Standard | [16:23] | 8 | 4 | 0 | 0 | 1020 | Degrees/s |
| LWS_OK | LWS_Standard | [24:24] | 1 | 1 | 0 | None | None |  |
| LWS_CAL | LWS_Standard | [25:25] | 1 | 1 | 0 | None | None |  |
| LWS_TRIM | LWS_Standard | [26:26] | 1 | 1 | 0 | None | None |  |
| Supplemental_Battery_Voltage | Supp_Battery_Status | [0:15] | 16 | 1 | 0 | 0 | 40 | V |
| SuppCharger_Status | Supp_Battery_Charger_Status | [0:1] | 2 | 1 | 0 | None | None |  |
| Supplemental_Battery_Current | Supp_Battery_Charger_Status | [16:31] | 16 | 1 | 0 | -2300 | 28700 | mA |
| Supplemental_DCDC_Voltage | Supp_Battery_Charger_Status | [32:47] | 16 | 1 | 0 | 0 | 26 | V |
| Supplemental_DCDC_Current | Supp_Battery_Charger_Status | [48:63] | 16 | 1 | 0 | -2300 | 28700 | mA |
| HSS_Channel | BBPDU_Status | [0:3] | 4 | 1 | 0 | 0 | 16 |  |
| HSS_Fault | BBPDU_Status | [4:4] | 1 | 1 | 0 | None | None |  |
| HSS_Enabled | BBPDU_Status | [5:5] | 1 | 1 | 0 | None | None |  |
| HSS_Measured_Voltage | BBPDU_Status | [8:23] | 16 | 0.001 | 0 | 0 | 65.535 | V |
| HSS_Measured_Current | BBPDU_Status | [24:39] | 16 | 0.001 | 0 | 0 | 65.535 | A |
| HSS_Control_13 | BBPDU_Set_Switches | [0:1] | 2 | 1 | 0 | None | None |  |
| HSS_Control_14 | BBPDU_Set_Switches | [2:3] | 2 | 1 | 0 | None | None |  |
| HSS_Control_15 | BBPDU_Set_Switches | [4:5] | 2 | 1 | 0 | None | None |  |
| HSS_Control_16 | BBPDU_Set_Switches | [6:7] | 2 | 1 | 0 | None | None |  |
| HSS_Control_9 | BBPDU_Set_Switches | [8:9] | 2 | 1 | 0 | None | None |  |
| HSS_Control_10 | BBPDU_Set_Switches | [10:11] | 2 | 1 | 0 | None | None |  |
| HSS_Control_11 | BBPDU_Set_Switches | [12:13] | 2 | 1 | 0 | None | None |  |
| HSS_Control_12 | BBPDU_Set_Switches | [14:15] | 2 | 1 | 0 | None | None |  |
| HSS_Control_5 | BBPDU_Set_Switches | [16:17] | 2 | 1 | 0 | None | None |  |
| HSS_Control_6 | BBPDU_Set_Switches | [18:19] | 2 | 1 | 0 | None | None |  |
| HSS_Control_7 | BBPDU_Set_Switches | [20:21] | 2 | 1 | 0 | None | None |  |
| HSS_Control_8 | BBPDU_Set_Switches | [22:23] | 2 | 1 | 0 | None | None |  |
| HSS_Control_1 | BBPDU_Set_Switches | [24:25] | 2 | 1 | 0 | None | None |  |
| HSS_Control_2 | BBPDU_Set_Switches | [26:27] | 2 | 1 | 0 | None | None |  |
| HSS_Control_3 | BBPDU_Set_Switches | [28:29] | 2 | 1 | 0 | None | None |  |
| HSS_Control_4 | BBPDU_Set_Switches | [30:31] | 2 | 1 | 0 | None | None |  |
| HSS_Channel | BBPDU_Set_Current_Limit | [4:7] | 4 | 1 | 0 | 0 | 16 |  |
| HSS_Current_Limit | BBPDU_Set_Current_Limit | [8:23] | 16 | 0.001 | 0 | 0 | 8 | A |
| Pump_DutyCycle | Pump_Status_FlowRate | [0:7] | 8 | 1 | 0 | 0 | 100 |  |
| Pump_Fault | Pump_Status_FlowRate | [8:8] | 1 | 1 | 0 | None | None |  |
| FlowRate_1 | Pump_Status_FlowRate | [16:31] | 16 | 0.001 | 0 | 0 | 65.535 | L/min |
| FlowRate_2 | Pump_Status_FlowRate | [32:47] | 16 | 0.001 | 0 | 0 | 65.535 | L/min |
| Coolant_Temperature_1 | Coolant_Temperature | [0:15] | 16 | 0.01 | 0 | -327.68 | 327.67 | C |
| Coolant_Temperature_2 | Coolant_Temperature | [16:31] | 16 | 0.01 | 0 | -327.68 | 327.67 | C |
| Radiator_Fan_Speed_Measurement_1 | Radiator_FanSpeed | [0:15] | 16 | 1 | 0 | 0 | 65535 | RPM |
| Radiator_Fan_Speed_Measurement_2 | Radiator_FanSpeed | [16:31] | 16 | 1 | 0 | 0 | 65535 | RPM |
| Radiator_Fan_Speed_Target_1 | Radiator_FanSpeed | [32:47] | 16 | 1 | 0 | 0 | 65535 | RPM |
| Radiator_Fan_Speed_Target_2 | Radiator_FanSpeed | [48:63] | 16 | 1 | 0 | 0 | 65535 | RPM |
| LTC4421_HVDCDC_Selected | LV_Carrier_Status | [0:0] | 1 | 1 | 0 | 0 | 1 |  |
| LTC4421_HVDCDC_Fault | LV_Carrier_Status | [1:1] | 1 | 1 | 0 | 0 | 1 |  |
| LTC4421_HVDCDC_Valid | LV_Carrier_Status | [2:2] | 1 | 1 | 0 | 0 | 1 |  |
| LTC4421_SuppBatt_Selected | LV_Carrier_Status | [3:3] | 1 | 1 | 0 | 0 | 1 |  |
| LTC4421_SuppBatt_Fault | LV_Carrier_Status | [4:4] | 1 | 1 | 0 | 0 | 1 |  |
| LTC4421_SuppBatt_Valid | LV_Carrier_Status | [5:5] | 1 | 1 | 0 | 0 | 1 |  |
| LV_EN_SupplementalBattery | LV_Carrier_Status | [6:6] | 1 | 1 | 0 | 0 | 1 |  |
| LV_EN_PowerSupply | LV_Carrier_Status | [7:7] | 1 | 1 | 0 | 0 | 1 |  |
| Brake_Pressure_1 | Brake_Pressure | [0:15] | 16 | 0.1 | 0 | 0 | 3000 | PSI |
| Brake_Pressure_1_Voltage | Brake_Pressure | [16:31] | 16 | 0.001 | 0 | 0 | 3.3 | V |
| Brake_Pressure_2 | Brake_Pressure | [32:47] | 16 | 0.1 | 0 | 0 | 3000 | PSI |
| Brake_Pressure_2_Voltage | Brake_Pressure | [48:63] | 16 | 0.001 | 0 | 0 | 3.3 | V |
| Lights_Faults | Lighting_Board0_Status | [0:7] | 8 | 1 | 0 | 0 | 255 |  |
| Active_Light_Headlights | Lighting_Board0_Status | [8:8] | 1 | 1 | 0 | None | None |  |
| Active_Light_Brake | Lighting_Board0_Status | [9:9] | 1 | 1 | 0 | None | None |  |
| Active_Light_RTurn | Lighting_Board0_Status | [10:10] | 1 | 1 | 0 | None | None |  |
| Active_Light_LTurn | Lighting_Board0_Status | [11:11] | 1 | 1 | 0 | None | None |  |
| Active_Light_Strobe | Lighting_Board0_Status | [12:12] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom1 | Lighting_Board0_Status | [13:13] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom2 | Lighting_Board0_Status | [14:14] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom3 | Lighting_Board0_Status | [15:15] | 1 | 1 | 0 | None | None |  |
| Addr_LED_Current | Lighting_Board0_Status | [16:27] | 12 | 1 | 0 | 0 | 4000 | mA |
| LED1_Current | Lighting_Board0_Status | [32:43] | 12 | 1 | 0 | 0 | 4000 | mA |
| LED2_Current | Lighting_Board0_Status | [48:59] | 12 | 1 | 0 | 0 | 4000 | mA |
| Lights_Faults | Lighting_Board1_Status | [0:7] | 8 | 1 | 0 | 0 | 255 |  |
| Active_Light_Headlights | Lighting_Board1_Status | [8:8] | 1 | 1 | 0 | None | None |  |
| Active_Light_Brake | Lighting_Board1_Status | [9:9] | 1 | 1 | 0 | None | None |  |
| Active_Light_RTurn | Lighting_Board1_Status | [10:10] | 1 | 1 | 0 | None | None |  |
| Active_Light_LTurn | Lighting_Board1_Status | [11:11] | 1 | 1 | 0 | None | None |  |
| Active_Light_Strobe | Lighting_Board1_Status | [12:12] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom1 | Lighting_Board1_Status | [13:13] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom2 | Lighting_Board1_Status | [14:14] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom3 | Lighting_Board1_Status | [15:15] | 1 | 1 | 0 | None | None |  |
| Addr_LED_Current | Lighting_Board1_Status | [16:27] | 12 | 1 | 0 | 0 | 4000 | mA |
| LED1_Current | Lighting_Board1_Status | [32:43] | 12 | 1 | 0 | 0 | 4000 | mA |
| LED2_Current | Lighting_Board1_Status | [48:59] | 12 | 1 | 0 | 0 | 4000 | mA |
| Lights_Faults | Lighting_Board2_Status | [0:7] | 8 | 1 | 0 | 0 | 255 |  |
| Active_Light_Headlights | Lighting_Board2_Status | [8:8] | 1 | 1 | 0 | None | None |  |
| Active_Light_Brake | Lighting_Board2_Status | [9:9] | 1 | 1 | 0 | None | None |  |
| Active_Light_RTurn | Lighting_Board2_Status | [10:10] | 1 | 1 | 0 | None | None |  |
| Active_Light_LTurn | Lighting_Board2_Status | [11:11] | 1 | 1 | 0 | None | None |  |
| Active_Light_Strobe | Lighting_Board2_Status | [12:12] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom1 | Lighting_Board2_Status | [13:13] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom2 | Lighting_Board2_Status | [14:14] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom3 | Lighting_Board2_Status | [15:15] | 1 | 1 | 0 | None | None |  |
| Addr_LED_Current | Lighting_Board2_Status | [16:27] | 12 | 1 | 0 | 0 | 4000 | mA |
| LED1_Current | Lighting_Board2_Status | [32:43] | 12 | 1 | 0 | 0 | 4000 | mA |
| LED2_Current | Lighting_Board2_Status | [48:59] | 12 | 1 | 0 | 0 | 4000 | mA |
| Lights_Faults | Lighting_Board3_Status | [0:7] | 8 | 1 | 0 | 0 | 255 |  |
| Active_Light_Headlights | Lighting_Board3_Status | [8:8] | 1 | 1 | 0 | None | None |  |
| Active_Light_Brake | Lighting_Board3_Status | [9:9] | 1 | 1 | 0 | None | None |  |
| Active_Light_RTurn | Lighting_Board3_Status | [10:10] | 1 | 1 | 0 | None | None |  |
| Active_Light_LTurn | Lighting_Board3_Status | [11:11] | 1 | 1 | 0 | None | None |  |
| Active_Light_Strobe | Lighting_Board3_Status | [12:12] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom1 | Lighting_Board3_Status | [13:13] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom2 | Lighting_Board3_Status | [14:14] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom3 | Lighting_Board3_Status | [15:15] | 1 | 1 | 0 | None | None |  |
| Addr_LED_Current | Lighting_Board3_Status | [16:27] | 12 | 1 | 0 | 0 | 4000 | mA |
| LED1_Current | Lighting_Board3_Status | [32:43] | 12 | 1 | 0 | 0 | 4000 | mA |
| LED2_Current | Lighting_Board3_Status | [48:59] | 12 | 1 | 0 | 0 | 4000 | mA |
| Lights_Faults | Lighting_Board4_Status | [0:7] | 8 | 1 | 0 | 0 | 255 |  |
| Active_Light_Headlights | Lighting_Board4_Status | [8:8] | 1 | 1 | 0 | None | None |  |
| Active_Light_Brake | Lighting_Board4_Status | [9:9] | 1 | 1 | 0 | None | None |  |
| Active_Light_RTurn | Lighting_Board4_Status | [10:10] | 1 | 1 | 0 | None | None |  |
| Active_Light_LTurn | Lighting_Board4_Status | [11:11] | 1 | 1 | 0 | None | None |  |
| Active_Light_Strobe | Lighting_Board4_Status | [12:12] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom1 | Lighting_Board4_Status | [13:13] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom2 | Lighting_Board4_Status | [14:14] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom3 | Lighting_Board4_Status | [15:15] | 1 | 1 | 0 | None | None |  |
| Addr_LED_Current | Lighting_Board4_Status | [16:27] | 12 | 1 | 0 | 0 | 4000 | mA |
| LED1_Current | Lighting_Board4_Status | [32:43] | 12 | 1 | 0 | 0 | 4000 | mA |
| LED2_Current | Lighting_Board4_Status | [48:59] | 12 | 1 | 0 | 0 | 4000 | mA |
| Lights_Faults | Lighting_Board5_Status | [0:7] | 8 | 1 | 0 | 0 | 255 |  |
| Active_Light_Headlights | Lighting_Board5_Status | [8:8] | 1 | 1 | 0 | None | None |  |
| Active_Light_Brake | Lighting_Board5_Status | [9:9] | 1 | 1 | 0 | None | None |  |
| Active_Light_RTurn | Lighting_Board5_Status | [10:10] | 1 | 1 | 0 | None | None |  |
| Active_Light_LTurn | Lighting_Board5_Status | [11:11] | 1 | 1 | 0 | None | None |  |
| Active_Light_Strobe | Lighting_Board5_Status | [12:12] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom1 | Lighting_Board5_Status | [13:13] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom2 | Lighting_Board5_Status | [14:14] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom3 | Lighting_Board5_Status | [15:15] | 1 | 1 | 0 | None | None |  |
| Addr_LED_Current | Lighting_Board5_Status | [16:27] | 12 | 1 | 0 | 0 | 4000 | mA |
| LED1_Current | Lighting_Board5_Status | [32:43] | 12 | 1 | 0 | 0 | 4000 | mA |
| LED2_Current | Lighting_Board5_Status | [48:59] | 12 | 1 | 0 | 0 | 4000 | mA |
| Lights_Faults | Lighting_Board6_Status | [0:7] | 8 | 1 | 0 | 0 | 255 |  |
| Active_Light_Headlights | Lighting_Board6_Status | [8:8] | 1 | 1 | 0 | None | None |  |
| Active_Light_Brake | Lighting_Board6_Status | [9:9] | 1 | 1 | 0 | None | None |  |
| Active_Light_RTurn | Lighting_Board6_Status | [10:10] | 1 | 1 | 0 | None | None |  |
| Active_Light_LTurn | Lighting_Board6_Status | [11:11] | 1 | 1 | 0 | None | None |  |
| Active_Light_Strobe | Lighting_Board6_Status | [12:12] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom1 | Lighting_Board6_Status | [13:13] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom2 | Lighting_Board6_Status | [14:14] | 1 | 1 | 0 | None | None |  |
| Active_Light_Custom3 | Lighting_Board6_Status | [15:15] | 1 | 1 | 0 | None | None |  |
| Addr_LED_Current | Lighting_Board6_Status | [16:27] | 12 | 1 | 0 | 0 | 4000 | mA |
| LED1_Current | Lighting_Board6_Status | [32:43] | 12 | 1 | 0 | 0 | 4000 | mA |
| LED2_Current | Lighting_Board6_Status | [48:59] | 12 | 1 | 0 | 0 | 4000 | mA |
| Display_FrameRate | Display_Cameras_Status | [0:7] | 8 | 1 | 0 | 0 | 60 | FPS |
| Camera_Status_Backup | Display_Cameras_Status | [8:8] | 1 | 1 | 0 | None | None |  |
| Camera_Status_Left | Display_Cameras_Status | [9:9] | 1 | 1 | 0 | None | None |  |
| Camera_Status_Right | Display_Cameras_Status | [10:10] | 1 | 1 | 0 | None | None |  |
| LWS_CCW | LWS_Config | [0:2] | 3 | 1 | 0 | None | None |  |


<a name="MotorCAN"></a>
# MotorCAN.dbc

[GitHub Link](https://github.com/lhr-solar/Embedded-Sharepoint/tree/main/can/dbc/MotorCAN.dbc)

## Messages

| ID | Type | Name | DLC | Signals |
|----|------|------|-----|---------|
| 0x401 | Std | MC_DriveCommand | 8 | MC_MotorVelocitySetpoint, MC_MotorCurrentSetpoint |
| 0x402 | Std | MC_PowerCommand | 4 | MC_MotorPowerSetpoint |
| 0x403 | Std | MC_ResetCommand | 1 | MC_Reset |
| 0x420 | Std | MC_Info | 8 | MC_TritiumID, MC_SerialNumber |
| 0x421 | Std | MC_Status | 8 | MC_LIMIT_OutputVoltagePWM, MC_LIMIT_MotorCurrent, MC_LIMIT_Velocity, MC_LIMIT_BusCurrent, MC_LIMIT_BusVoltageUpper, MC_LIMIT_BusVoltageLower, MC_LIMIT_MotorTemp, MC_LIMIT_Reserved, MC_FAULT_HardwareOverCurrent, MC_FAULT_SoftwareOverCurrent, MC_FAULT_DcBusOverVoltage, MC_FAULT_BadMotorPositionHallSeq, MC_FAULT_WatchdogCausedLastReset, MC_FAULT_ConfigRead, MC_FAULT_15vRailUnderVoltage, MC_FAULT_DesaturationFault, MC_FAULT_MotorOverSpeed, MC_FAULT_Reserved, MC_ActiveMotor, MC_TxErrorCount, MC_RxErrorCount |
| 0x422 | Std | MC_BusMeasurement | 8 | MC_BusVoltage, MC_BusCurrent |
| 0x423 | Std | MC_VelocityMeasurement | 8 | MC_MotorVelocity, MC_VehicleVelocity |
| 0x424 | Std | MC_PhaseCurrentMeasurement | 8 | MC_PhaseCurrentB, MC_PhaseCurrentC |
| 0x425 | Std | MC_MotorVoltageVectorMeasurement | 8 | MC_Vq, MC_Vd |
| 0x426 | Std | MC_MotorCurrentVectorMeasurement | 8 | MC_Iq, MC_Id |
| 0x427 | Std | MC_BackEMFMeasurementPrediction | 8 | MC_BEMFq, MC_BEMFd |
| 0x428 | Std | MC_15VRailMeasurement | 8 | MC_Supply15V |
| 0x429 | Std | MC_3V319VRailMeasurement | 8 | MC_Supply1V9, MC_Supply3V3 |
| 0x42B | Std | MC_Motor_TempMeasurement | 8 | MC_MotorTemp, MC_HeatsinkTemp |
| 0x42C | Std | MC_DspBoardTempMeasurement | 8 | MC_DspBoardTemp |
| 0x42E | Std | MC_OdometerBusAhMeasurement | 8 | MC_TripOdometer, MC_DCBusAh |
| 0x437 | Std | MC_SlipSpeedMeasurement | 8 | MC_SlipSpeed |

## Signals

| Name | Message | Bits [start:end] | Length | Factor | Offset | Min | Max | Unit |
|------|---------|------------------|--------|--------|--------|-----|-----|------|
| MC_MotorVelocitySetpoint | MC_DriveCommand | [0:31] | 32 | 1 | 0 | 0 | 12000 | rpm |
| MC_MotorCurrentSetpoint | MC_DriveCommand | [32:63] | 32 | 1 | 0 | 0 | 1 | % |
| MC_MotorPowerSetpoint | MC_PowerCommand | [0:31] | 32 | 1 | 0 | 0 | 1 | % |
| MC_Reset | MC_ResetCommand | [0:7] | 8 | 1 | 0 | None | None |  |
| MC_TritiumID | MC_Info | [0:31] | 32 | 1 | 0 | None | None |  |
| MC_SerialNumber | MC_Info | [32:63] | 32 | 1 | 0 | None | None |  |
| MC_LIMIT_OutputVoltagePWM | MC_Status | [0:0] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_LIMIT_MotorCurrent | MC_Status | [1:1] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_LIMIT_Velocity | MC_Status | [2:2] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_LIMIT_BusCurrent | MC_Status | [3:3] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_LIMIT_BusVoltageUpper | MC_Status | [4:4] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_LIMIT_BusVoltageLower | MC_Status | [5:5] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_LIMIT_MotorTemp | MC_Status | [6:6] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_LIMIT_Reserved | MC_Status | [7:15] | 9 | 1 | 0 | 0 | 511 | On / Off |
| MC_FAULT_HardwareOverCurrent | MC_Status | [16:16] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_FAULT_SoftwareOverCurrent | MC_Status | [17:17] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_FAULT_DcBusOverVoltage | MC_Status | [18:18] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_FAULT_BadMotorPositionHallSeq | MC_Status | [19:19] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_FAULT_WatchdogCausedLastReset | MC_Status | [20:20] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_FAULT_ConfigRead | MC_Status | [21:21] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_FAULT_15vRailUnderVoltage | MC_Status | [22:22] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_FAULT_DesaturationFault | MC_Status | [23:23] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_FAULT_MotorOverSpeed | MC_Status | [24:24] | 1 | 1 | 0 | 0 | 1 | On / Off |
| MC_FAULT_Reserved | MC_Status | [25:31] | 7 | 1 | 0 | None | None | On / Off |
| MC_ActiveMotor | MC_Status | [32:47] | 16 | 1 | 0 | None | None |  |
| MC_TxErrorCount | MC_Status | [48:55] | 8 | 1 | 0 | None | None |  |
| MC_RxErrorCount | MC_Status | [56:63] | 8 | 1 | 0 | None | None |  |
| MC_BusVoltage | MC_BusMeasurement | [0:31] | 32 | 1 | 0 | None | None | V |
| MC_BusCurrent | MC_BusMeasurement | [32:63] | 32 | 1 | 0 | None | None | A |
| MC_MotorVelocity | MC_VelocityMeasurement | [0:31] | 32 | 1 | 0 | None | None | rpm |
| MC_VehicleVelocity | MC_VelocityMeasurement | [32:63] | 32 | 1 | 0 | None | None | m/s |
| MC_PhaseCurrentB | MC_PhaseCurrentMeasurement | [0:31] | 32 | 1 | 0 | None | None | A_rms |
| MC_PhaseCurrentC | MC_PhaseCurrentMeasurement | [32:63] | 32 | 1 | 0 | None | None | A_rms |
| MC_Vq | MC_MotorVoltageVectorMeasurement | [0:31] | 32 | 1 | 0 | None | None | V |
| MC_Vd | MC_MotorVoltageVectorMeasurement | [32:63] | 32 | 1 | 0 | None | None | V |
| MC_Iq | MC_MotorCurrentVectorMeasurement | [0:31] | 32 | 1 | 0 | None | None | A |
| MC_Id | MC_MotorCurrentVectorMeasurement | [32:63] | 32 | 1 | 0 | None | None | A |
| MC_BEMFq | MC_BackEMFMeasurementPrediction | [0:31] | 32 | 1 | 0 | None | None | V |
| MC_BEMFd | MC_BackEMFMeasurementPrediction | [32:63] | 32 | 1 | 0 | None | None | V |
| MC_Supply15V | MC_15VRailMeasurement | [32:63] | 32 | 1 | 0 | None | None | V |
| MC_Supply1V9 | MC_3V319VRailMeasurement | [0:31] | 32 | 1 | 0 | None | None | V |
| MC_Supply3V3 | MC_3V319VRailMeasurement | [32:63] | 32 | 1 | 0 | None | None | V |
| MC_MotorTemp | MC_Motor_TempMeasurement | [0:31] | 32 | 1 | 0 | None | None | C |
| MC_HeatsinkTemp | MC_Motor_TempMeasurement | [32:63] | 32 | 1 | 0 | None | None | C |
| MC_DspBoardTemp | MC_DspBoardTempMeasurement | [0:31] | 32 | 1 | 0 | None | None | C |
| MC_TripOdometer | MC_OdometerBusAhMeasurement | [0:31] | 32 | 1 | 0 | None | None | m |
| MC_DCBusAh | MC_OdometerBusAhMeasurement | [32:63] | 32 | 1 | 0 | None | None | Ah |
| MC_SlipSpeed | MC_SlipSpeedMeasurement | [0:31] | 32 | 1 | 0 | None | None | Hz |
