# Table of Contents

- [Prohelion Wavesculptor22](#prohelion_wavesculptor22)
- [Tpee Mppt[a]](#tpee_mppt[A])
- [Controls](#controls)
- [Tpee Mppt[b]](#tpee_mppt[B])
- [Bps (daybreak)](#bps-(Daybreak))
- [Contactor](#contactor)

<a name="prohelion_wavesculptor22"></a>
# prohelion_wavesculptor22.dbc

## Messages

| ID | Name | DLC | Signals |
|----|------|-----|---------|
| 0x240 | IDInfo | 8 | TritiumID, SerialNumber |
| 0x241 | Status | 8 | LimitOutputVoltagePWM, LimitMotorCurrent, LimitVelocity, LimitBusCurrent, LimitBusVoltageUpper, LimitBusVoltageLower, LimitIpmOrMotorTemp, LimitReserved, ErrorHardwareOverCurrent, ErrorSoftwareOverCurrent, ErrorDcBusOverVoltage, ErrorBadMotorPositionHallSeq, ErrorWatchdogCausedLastReset, ErrorConfigRead, Error15vRailUnderVoltage, ErrorDesaturationFault, ErrorMotorOverSpeed, ErrorReserved, ActiveMotor, TxErrorCount, RxErrorCount |
| 0x242 | BusMeasurement | 8 | BusVoltage, BusCurrent |
| 0x243 | VelocityMeasurement | 8 | MotorVelocity, VehicleVelocity |
| 0x244 | PhaseCurrentMeasurement | 8 | PhaseCurrentB, PhaseCurrentC |
| 0x245 | MotorVoltageVectorMeasurement | 8 | Vq, Vd |
| 0x246 | MotorCurrentVectorMeasurement | 8 | Iq, Id |
| 0x247 | BackEMFMeasurementPrediction | 8 | BEMFq, BEMFd |
| 0x248 | VoltageRail15VMeasurement | 8 | ReservedSupply15V, Supply15V |
| 0x249 | VoltageRail3V31V9Measurement | 8 | Supply1V9, Supply3V3 |
| 0x24A | Reserved0A | 8 | Reserved0A0, Reserved0A1 |
| 0x24B | HeatsinkMotorTempMeasurement | 8 | MotorTemp, HeatsinkTemp |
| 0x24C | DspBoardTempMeasurement | 8 | DspBoardTemp, ReservedDspBoardTemp |
| 0x24D | Reserved0D | 8 | Reserved0D0, Reserved0D1 |
| 0x24E | OdometerBusAhMeasurement | 8 | Odometer, DCBusAh |
| 0x257 | SlipSpeedMeasurement | 8 | SlipSpeed, ReservedSlipSpeed |

## Signals

| Name | Message | Bits [start:end] | Length | Factor | Offset | Min | Max | Unit |
|------|---------|------------------|--------|--------|--------|-----|-----|------|
| TritiumID | IDInfo | [0:31] | 32 | 1 | 0 | None | None |  |
| SerialNumber | IDInfo | [32:63] | 32 | 1 | 0 | None | None |  |
| LimitOutputVoltagePWM | Status | [0:0] | 1 | 1 | 0 | 0 | 1 | On / Off |
| LimitMotorCurrent | Status | [1:1] | 1 | 1 | 0 | 0 | 1 | On / Off |
| LimitVelocity | Status | [2:2] | 1 | 1 | 0 | 0 | 1 | On / Off |
| LimitBusCurrent | Status | [3:3] | 1 | 1 | 0 | 0 | 1 | On / Off |
| LimitBusVoltageUpper | Status | [4:4] | 1 | 1 | 0 | 0 | 1 | On / Off |
| LimitBusVoltageLower | Status | [5:5] | 1 | 1 | 0 | 0 | 1 | On / Off |
| LimitIpmOrMotorTemp | Status | [6:6] | 1 | 1 | 0 | 0 | 1 | On / Off |
| LimitReserved | Status | [7:15] | 9 | 1 | 0 | 0 | 511 | On / Off |
| ErrorHardwareOverCurrent | Status | [16:16] | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorSoftwareOverCurrent | Status | [17:17] | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorDcBusOverVoltage | Status | [18:18] | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorBadMotorPositionHallSeq | Status | [19:19] | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorWatchdogCausedLastReset | Status | [20:20] | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorConfigRead | Status | [21:21] | 1 | 1 | 0 | 0 | 1 | On / Off |
| Error15vRailUnderVoltage | Status | [22:22] | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorDesaturationFault | Status | [23:23] | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorMotorOverSpeed | Status | [24:24] | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorReserved | Status | [25:31] | 7 | 1 | 0 | None | None | On / Off |
| ActiveMotor | Status | [32:47] | 16 | 1 | 0 | None | None |  |
| TxErrorCount | Status | [48:55] | 8 | 1 | 0 | None | None |  |
| RxErrorCount | Status | [56:63] | 8 | 1 | 0 | None | None |  |
| BusVoltage | BusMeasurement | [0:31] | 32 | 1 | 0 | 0 | 100 | V |
| BusCurrent | BusMeasurement | [32:63] | 32 | 1 | 0 | 0 | 1000 | A |
| MotorVelocity | VelocityMeasurement | [0:31] | 32 | 1 | 0 | None | None | rpm |
| VehicleVelocity | VelocityMeasurement | [32:63] | 32 | 2.237 | 0 | None | None | mph |
| PhaseCurrentB | PhaseCurrentMeasurement | [0:31] | 32 | 1 | 0 | None | None | A_rms |
| PhaseCurrentC | PhaseCurrentMeasurement | [32:63] | 32 | 1 | 0 | None | None | A_rms |
| Vq | MotorVoltageVectorMeasurement | [0:31] | 32 | 1 | 0 | None | None | V |
| Vd | MotorVoltageVectorMeasurement | [32:63] | 32 | 1 | 0 | None | None | V |
| Iq | MotorCurrentVectorMeasurement | [0:31] | 32 | 1 | 0 | None | None | A |
| Id | MotorCurrentVectorMeasurement | [32:63] | 32 | 1 | 0 | None | None | A |
| BEMFq | BackEMFMeasurementPrediction | [0:31] | 32 | 1 | 0 | None | None | V |
| BEMFd | BackEMFMeasurementPrediction | [32:63] | 32 | 1 | 0 | None | None | V |
| ReservedSupply15V | VoltageRail15VMeasurement | [0:31] | 32 | 1 | 0 | None | None |  |
| Supply15V | VoltageRail15VMeasurement | [32:63] | 32 | 1 | 0 | None | None | V |
| Supply1V9 | VoltageRail3V31V9Measurement | [0:31] | 32 | 1 | 0 | None | None | V |
| Supply3V3 | VoltageRail3V31V9Measurement | [32:63] | 32 | 1 | 0 | None | None | V |
| Reserved0A0 | Reserved0A | [0:31] | 32 | 1 | 0 | None | None |  |
| Reserved0A1 | Reserved0A | [32:63] | 32 | 1 | 0 | None | None |  |
| MotorTemp | HeatsinkMotorTempMeasurement | [0:31] | 32 | 1 | 0 | None | None | C |
| HeatsinkTemp | HeatsinkMotorTempMeasurement | [32:63] | 32 | 1 | 0 | None | None | C |
| DspBoardTemp | DspBoardTempMeasurement | [0:31] | 32 | 1 | 0 | None | None | C |
| ReservedDspBoardTemp | DspBoardTempMeasurement | [32:63] | 32 | 1 | 0 | None | None |  |
| Reserved0D0 | Reserved0D | [0:31] | 32 | 1 | 0 | None | None |  |
| Reserved0D1 | Reserved0D | [32:63] | 32 | 1 | 0 | None | None |  |
| Odometer | OdometerBusAhMeasurement | [0:31] | 32 | 1 | 0 | None | None | m |
| DCBusAh | OdometerBusAhMeasurement | [32:63] | 32 | 1 | 0 | None | None | Ah |
| SlipSpeed | SlipSpeedMeasurement | [0:31] | 32 | 1 | 0 | None | None | Hz |
| ReservedSlipSpeed | SlipSpeedMeasurement | [32:63] | 32 | 1 | 0 | None | None |  |


<a name="tpee_mppt[A]"></a>
# tpee_mppt[A].dbc

## Messages

| ID | Name | DLC | Signals |
|----|------|-----|---------|
| 0x201 | MPPT_32_0_Status | 5 | MPPT_Mode, MPPT_Fault, MPPT_Enabled, MPPT_AmbientTemperature, MPPT_HeatsinkTemperature |
| 0x200 | MPPT_32_0_Power | 8 | MPPT_Vin, MPPT_Iin, MPPT_Vout, MPPT_Iout |
| 0x209 | MPPT_32_0_SetMode | 1 | Boost_Enable |

## Signals

| Name | Message | Bits [start:end] | Length | Factor | Offset | Min | Max | Unit |
|------|---------|------------------|--------|--------|--------|-----|-----|------|
| MPPT_Mode | MPPT_32_0_Status | [0:7] | 8 | 1 | 0 | None | None |  |
| MPPT_Fault | MPPT_32_0_Status | [8:15] | 8 | 1 | 0 | None | None |  |
| MPPT_Enabled | MPPT_32_0_Status | [16:23] | 8 | 1 | 0 | None | None |  |
| MPPT_AmbientTemperature | MPPT_32_0_Status | [31:38] | 8 | 1 | 0 | -128 | 127 | C |
| MPPT_HeatsinkTemperature | MPPT_32_0_Status | [39:46] | 8 | 1 | 0 | -128 | 127 | C |
| MPPT_Vin | MPPT_32_0_Power | [7:22] | 16 | 0.01 | 0 | -327.68 | 327.67 | V |
| MPPT_Iin | MPPT_32_0_Power | [23:38] | 16 | 0.0005 | 0 | -16.384 | 16.3835 | A |
| MPPT_Vout | MPPT_32_0_Power | [39:54] | 16 | 0.01 | 0 | -327.68 | 327.67 | V |
| MPPT_Iout | MPPT_32_0_Power | [55:70] | 16 | 0.0005 | 0 | None | None | A |
| Boost_Enable | MPPT_32_0_SetMode | [0:0] | 1 | 1 | 0 | 0 | 1 |  |


<a name="controls"></a>
# controls.dbc

## Messages

| ID | Name | DLC | Signals |
|----|------|-----|---------|
| 0x581 | IO_State | 3 | Acceleration_Percentage, Brake_Percentage, IGN_Array, IGN_Motor, Regen_SW, Forward_Gear, Reverse_Gear, Cruz_EN, Cruz_Set, Brake_Light |
| 0x583 | Controls_Fault | 1 | Controls_Fault, Motor_Controller_Fault, BPS_Fault, Pedals_Fault, CarCAN_Fault, Internal_Controls_Fault, OS_Fault, Lakshay_Fault |
| 0x584 | Motor_Controller_Safe | 1 | Motor_Safe, Motor_Controller_Error |
| 0x221 | Motor_Drive_Command | 8 | Motor_Velocity_Setpoint, Motor_Current_Setpoint |
| 0x222 | Motor_Power_Command | 8 | Motor_Power_Setpoint |
| 0x585 | Pedals_Raw_Voltage | 4 | Brake_Raw, Accel_Raw |

## Signals

| Name | Message | Bits [start:end] | Length | Factor | Offset | Min | Max | Unit |
|------|---------|------------------|--------|--------|--------|-----|-----|------|
| Acceleration_Percentage | IO_State | [0:7] | 8 | 1 | 0 | 0 | 100 | Percentage |
| Brake_Percentage | IO_State | [8:15] | 8 | 1 | 0 | 0 | 100 | Percentage the brake is pressed |
| IGN_Array | IO_State | [16:16] | 1 | 1 | 0 | 0 | 1 | Array Ignition On |
| IGN_Motor | IO_State | [17:17] | 1 | 1 | 0 | 0 | 1 | Motor Ignition On |
| Regen_SW | IO_State | [18:18] | 1 | 1 | 0 | 0 | 1 | Regen Switched On |
| Forward_Gear | IO_State | [19:19] | 1 | 1 | 0 | 0 | 1 | Forward switch |
| Reverse_Gear | IO_State | [20:20] | 1 | 1 | 0 | 0 | 1 | Reverse switch |
| Cruz_EN | IO_State | [21:21] | 1 | 1 | 0 | 0 | 1 | Cruise Enable |
| Cruz_Set | IO_State | [22:22] | 1 | 1 | 0 | 0 | 1 | Cruise Set |
| Brake_Light | IO_State | [23:23] | 1 | 1 | 0 | 0 | 1 | Brakelight state |
| Controls_Fault | Controls_Fault | [0:0] | 1 | 1 | 0 | 0 | 1 |  |
| Motor_Controller_Fault | Controls_Fault | [1:1] | 1 | 1 | 0 | 0 | 1 |  |
| BPS_Fault | Controls_Fault | [2:2] | 1 | 1 | 0 | 0 | 1 |  |
| Pedals_Fault | Controls_Fault | [3:3] | 1 | 1 | 0 | None | None |  |
| CarCAN_Fault | Controls_Fault | [4:4] | 1 | 1 | 0 | 0 | 1 |  |
| Internal_Controls_Fault | Controls_Fault | [5:5] | 1 | 1 | 0 | 0 | 1 |  |
| OS_Fault | Controls_Fault | [6:6] | 1 | 1 | 0 | 0 | 1 |  |
| Lakshay_Fault | Controls_Fault | [7:7] | 1 | 1 | 0 | 0 | 1 |  |
| Motor_Safe | Motor_Controller_Safe | [0:0] | 1 | 1 | 0 | 0 | 1 |  |
| Motor_Controller_Error | Motor_Controller_Safe | [1:1] | 1 | 1 | 0 | 0 | 1 |  |
| Motor_Velocity_Setpoint | Motor_Drive_Command | [0:31] | 32 | 1 | 0 | None | None | rpm |
| Motor_Current_Setpoint | Motor_Drive_Command | [32:63] | 32 | 1 | 0 | None | None | % |
| Motor_Power_Setpoint | Motor_Power_Command | [32:63] | 32 | 1 | 0 | None | None | % |
| Brake_Raw | Pedals_Raw_Voltage | [0:14] | 15 | 1 | 0 | 0 | 3300 | mV |
| Accel_Raw | Pedals_Raw_Voltage | [16:30] | 15 | 1 | 0 | 0 | 3300 | mV |


<a name="tpee_mppt[B]"></a>
# tpee_mppt[B].dbc

## Messages

| ID | Name | DLC | Signals |
|----|------|-----|---------|
| 0x211 | MPPT_33_0_Status | 5 | MPPT_Mode, MPPT_Fault, MPPT_Enabled, MPPT_AmbientTemperature, MPPT_HeatsinkTemperature |
| 0x210 | MPPT_33_0_Power | 8 | MPPT_Vin, MPPT_Iin, MPPT_Vout, MPPT_Iout |
| 0x219 | MPPT_33_0_SetMode | 1 | Boost_Enable |

## Signals

| Name | Message | Bits [start:end] | Length | Factor | Offset | Min | Max | Unit |
|------|---------|------------------|--------|--------|--------|-----|-----|------|
| MPPT_Mode | MPPT_33_0_Status | [0:7] | 8 | 1 | 0 | None | None |  |
| MPPT_Fault | MPPT_33_0_Status | [8:15] | 8 | 1 | 0 | None | None |  |
| MPPT_Enabled | MPPT_33_0_Status | [16:23] | 8 | 1 | 0 | None | None |  |
| MPPT_AmbientTemperature | MPPT_33_0_Status | [31:38] | 8 | 1 | 0 | -128 | 127 | C |
| MPPT_HeatsinkTemperature | MPPT_33_0_Status | [39:46] | 8 | 1 | 0 | -128 | 127 | C |
| MPPT_Vin | MPPT_33_0_Power | [7:22] | 16 | 0.01 | 0 | -327.68 | 327.67 | V |
| MPPT_Iin | MPPT_33_0_Power | [23:38] | 16 | 0.0005 | 0 | -16.384 | 16.3835 | A |
| MPPT_Vout | MPPT_33_0_Power | [39:54] | 16 | 0.01 | 0 | -327.68 | 327.67 | V |
| MPPT_Iout | MPPT_33_0_Power | [55:70] | 16 | 0.0005 | 0 | None | None | A |
| Boost_Enable | MPPT_33_0_SetMode | [0:0] | 1 | 1 | 0 | 0 | 1 |  |


<a name="bps-(Daybreak)"></a>
# bps (Daybreak).dbc

## Messages

| ID | Name | DLC | Signals |
|----|------|-----|---------|
| 0x2 | BPS_Trip | 1 | BPS_Trip |
| 0x101 | BPS_All_Clea | 1 | BPS_All_Clear |
| 0x102 | BPS_Contactor_State | 1 | HV_Contactor, Array_Contactor |
| 0x103 | BPS_Current | 4 | Current |
| 0x104 | BPS_Voltage_Array | 5 | Voltage_idx, Voltage_Value |
| 0x105 | BPS_Temperature_Array | 5 | Temperature_idx, Temperature_Value |
| 0x106 | BPS_SOC | 4 | SoC |
| 0x107 | BPS_WDog_Trigger | 1 | WDog_Trig |
| 0x108 | BPS_CAN_Error | 1 | BPS_CAN_Error |
| 0x109 | BPS_Command | 8 | BPS_Command |
| 0x10B | BPS_Supplemental_Voltage | 2 | Supplemental_Voltage |
| 0x10C | BPS_Charge_Enabled | 1 | Charge_Enabled |
| 0x10D | BPS_Voltage_Summary | 8 | Pack_Voltage, Voltage_Range, Voltage_Timestamp |
| 0x10E | BPS_Temperature_Summary | 8 | Average_Temp, Temperature_Range, Temperature_Timestamp |
| 0x10F | BPS_Fault_State | 1 | BPS_Fault_State |

## Signals

| Name | Message | Bits [start:end] | Length | Factor | Offset | Min | Max | Unit |
|------|---------|------------------|--------|--------|--------|-----|-----|------|
| BPS_Trip | BPS_Trip | [0:0] | 1 | 1 | 0 | 0 | 1 |  |
| BPS_All_Clear | BPS_All_Clea | [0:0] | 1 | 1 | 0 | 0 | 1 |  |
| HV_Contactor | BPS_Contactor_State | [2:2] | 1 | 1 | 0 | 0 | 1 |  |
| Array_Contactor | BPS_Contactor_State | [0:0] | 1 | 1 | 0 | 0 | 1 |  |
| Current | BPS_Current | [0:31] | 32 | 1 | 0 | -20000 | 100000 | mA |
| Voltage_idx | BPS_Voltage_Array | [0:7] | 8 | 1 | 0 | 0 | 31 |  |
| Voltage_Value | BPS_Voltage_Array | [8:39] | 32 | 1 | 0 | 0 | 1000000 | mV |
| Temperature_idx | BPS_Temperature_Array | [0:7] | 8 | 1 | 0 | 0 | 31 |  |
| Temperature_Value | BPS_Temperature_Array | [8:39] | 32 | 1 | 0 | 0 | 1000000 | mC |
| SoC | BPS_SOC | [0:31] | 32 | 1 | 0 | 0 | 100 | % |
| WDog_Trig | BPS_WDog_Trigger | [0:0] | 1 | 1 | 0 | 0 | 1 |  |
| BPS_CAN_Error | BPS_CAN_Error | [0:0] | 1 | 1 | 0 | 0 | 1 |  |
| BPS_Command | BPS_Command | [0:31] | 32 | 1 | 0 | None | None |  |
| Supplemental_Voltage | BPS_Supplemental_Voltage | [0:15] | 16 | 1 | 0 | 0 | 65535 | mV |
| Charge_Enabled | BPS_Charge_Enabled | [0:0] | 1 | 1 | 0 | 0 | 1 |  |
| Pack_Voltage | BPS_Voltage_Summary | [0:23] | 24 | 1 | 0 | 0 | 16777215 | mV |
| Voltage_Range | BPS_Voltage_Summary | [24:47] | 24 | 1 | 0 | 0 | 16777215 | mV |
| Voltage_Timestamp | BPS_Voltage_Summary | [48:63] | 16 | 1 | 0 | 0 | 65535 | ms |
| Average_Temp | BPS_Temperature_Summary | [0:23] | 24 | 1 | 0 | None | None | mC |
| Temperature_Range | BPS_Temperature_Summary | [24:47] | 24 | 1 | 0 | None | None | mC |
| Temperature_Timestamp | BPS_Temperature_Summary | [48:63] | 16 | 1 | 0 | None | None | ms |
| BPS_Fault_State | BPS_Fault_State | [0:7] | 8 | 1 | 0 | 0 | 12 |  |


<a name="contactor"></a>
# contactor.dbc

## Messages

| ID | Name | DLC | Signals |
|----|------|-----|---------|
| 0x401 | Precharge_Timeout | 1 | Motor_Precharge_Timeout, Array_Precharge_Timeout |
| 0x400 | Contactor_Sense | 2 | Actual_Motor_Sense, Expected_Motor_Sense, Motor_Sense_Fault, Motor_Precharge_Sense, Expected_Motor_Precharge_Sense, Motor_Precharge_Sense_Fault, Array_Precharge_Sense, Expected_Array_Precharge_Sense, Array_Prechage_Sense_Fault |

## Signals

| Name | Message | Bits [start:end] | Length | Factor | Offset | Min | Max | Unit |
|------|---------|------------------|--------|--------|--------|-----|-----|------|
| Motor_Precharge_Timeout | Precharge_Timeout | [0:0] | 1 | 1 | 0 | 0 | 1 |  |
| Array_Precharge_Timeout | Precharge_Timeout | [1:1] | 1 | 1 | 0 | None | None |  |
| Actual_Motor_Sense | Contactor_Sense | [0:0] | 1 | 1 | 0 | 0 | 1 |  |
| Expected_Motor_Sense | Contactor_Sense | [1:1] | 1 | 1 | 0 | None | None |  |
| Motor_Sense_Fault | Contactor_Sense | [2:2] | 1 | 1 | 0 | 0 | 1 |  |
| Motor_Precharge_Sense | Contactor_Sense | [3:3] | 1 | 1 | 0 | 0 | 1 |  |
| Expected_Motor_Precharge_Sense | Contactor_Sense | [4:4] | 1 | 1 | 0 | 0 | 1 |  |
| Motor_Precharge_Sense_Fault | Contactor_Sense | [5:5] | 1 | 1 | 0 | 0 | 1 |  |
| Array_Precharge_Sense | Contactor_Sense | [6:6] | 1 | 1 | 0 | 0 | 1 |  |
| Expected_Array_Precharge_Sense | Contactor_Sense | [7:7] | 1 | 1 | 0 | 0 | 1 |  |
| Array_Prechage_Sense_Fault | Contactor_Sense | [8:8] | 1 | 1 | 0 | 0 | 1 |  |
