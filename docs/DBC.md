# prohelion_wavesculptor22.dbc

## Messages

| ID | Name | DLC | Signals |
|----|------|-----|---------|
| 576 | IDInfo | 8 | TritiumID, SerialNumber |
| 577 | Status | 8 | LimitOutputVoltagePWM, LimitMotorCurrent, LimitVelocity, LimitBusCurrent, LimitBusVoltageUpper, LimitBusVoltageLower, LimitIpmOrMotorTemp, LimitReserved, ErrorHardwareOverCurrent, ErrorSoftwareOverCurrent, ErrorDcBusOverVoltage, ErrorBadMotorPositionHallSeq, ErrorWatchdogCausedLastReset, ErrorConfigRead, Error15vRailUnderVoltage, ErrorDesaturationFault, ErrorMotorOverSpeed, ErrorReserved, ActiveMotor, TxErrorCount, RxErrorCount |
| 578 | BusMeasurement | 8 | BusVoltage, BusCurrent |
| 579 | VelocityMeasurement | 8 | MotorVelocity, VehicleVelocity |
| 580 | PhaseCurrentMeasurement | 8 | PhaseCurrentB, PhaseCurrentC |
| 581 | MotorVoltageVectorMeasurement | 8 | Vq, Vd |
| 582 | MotorCurrentVectorMeasurement | 8 | Iq, Id |
| 583 | BackEMFMeasurementPrediction | 8 | BEMFq, BEMFd |
| 584 | VoltageRail15VMeasurement | 8 | ReservedSupply15V, Supply15V |
| 585 | VoltageRail3V31V9Measurement | 8 | Supply1V9, Supply3V3 |
| 586 | Reserved0A | 8 | Reserved0A0, Reserved0A1 |
| 587 | HeatsinkMotorTempMeasurement | 8 | MotorTemp, HeatsinkTemp |
| 588 | DspBoardTempMeasurement | 8 | DspBoardTemp, ReservedDspBoardTemp |
| 589 | Reserved0D | 8 | Reserved0D0, Reserved0D1 |
| 590 | OdometerBusAhMeasurement | 8 | Odometer, DCBusAh |
| 599 | SlipSpeedMeasurement | 8 | SlipSpeed, ReservedSlipSpeed |

## Signals

| Name | Message | Start Bit | Length | Factor | Offset | Min | Max | Unit |
|------|---------|-----------|--------|--------|--------|-----|-----|------|
| TritiumID | IDInfo | 0 | 32 | 1 | 0 | None | None |  |
| SerialNumber | IDInfo | 32 | 32 | 1 | 0 | None | None |  |
| LimitOutputVoltagePWM | Status | 0 | 1 | 1 | 0 | 0 | 1 | On / Off |
| LimitMotorCurrent | Status | 1 | 1 | 1 | 0 | 0 | 1 | On / Off |
| LimitVelocity | Status | 2 | 1 | 1 | 0 | 0 | 1 | On / Off |
| LimitBusCurrent | Status | 3 | 1 | 1 | 0 | 0 | 1 | On / Off |
| LimitBusVoltageUpper | Status | 4 | 1 | 1 | 0 | 0 | 1 | On / Off |
| LimitBusVoltageLower | Status | 5 | 1 | 1 | 0 | 0 | 1 | On / Off |
| LimitIpmOrMotorTemp | Status | 6 | 1 | 1 | 0 | 0 | 1 | On / Off |
| LimitReserved | Status | 7 | 9 | 1 | 0 | 0 | 511 | On / Off |
| ErrorHardwareOverCurrent | Status | 16 | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorSoftwareOverCurrent | Status | 17 | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorDcBusOverVoltage | Status | 18 | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorBadMotorPositionHallSeq | Status | 19 | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorWatchdogCausedLastReset | Status | 20 | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorConfigRead | Status | 21 | 1 | 1 | 0 | 0 | 1 | On / Off |
| Error15vRailUnderVoltage | Status | 22 | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorDesaturationFault | Status | 23 | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorMotorOverSpeed | Status | 24 | 1 | 1 | 0 | 0 | 1 | On / Off |
| ErrorReserved | Status | 25 | 7 | 1 | 0 | None | None | On / Off |
| ActiveMotor | Status | 32 | 16 | 1 | 0 | None | None |  |
| TxErrorCount | Status | 48 | 8 | 1 | 0 | None | None |  |
| RxErrorCount | Status | 56 | 8 | 1 | 0 | None | None |  |
| BusVoltage | BusMeasurement | 0 | 32 | 1 | 0 | 0 | 100 | V |
| BusCurrent | BusMeasurement | 32 | 32 | 1 | 0 | 0 | 1000 | A |
| MotorVelocity | VelocityMeasurement | 0 | 32 | 1 | 0 | None | None | rpm |
| VehicleVelocity | VelocityMeasurement | 32 | 32 | 2.237 | 0 | None | None | mph |
| PhaseCurrentB | PhaseCurrentMeasurement | 0 | 32 | 1 | 0 | None | None | A_rms |
| PhaseCurrentC | PhaseCurrentMeasurement | 32 | 32 | 1 | 0 | None | None | A_rms |
| Vq | MotorVoltageVectorMeasurement | 0 | 32 | 1 | 0 | None | None | V |
| Vd | MotorVoltageVectorMeasurement | 32 | 32 | 1 | 0 | None | None | V |
| Iq | MotorCurrentVectorMeasurement | 0 | 32 | 1 | 0 | None | None | A |
| Id | MotorCurrentVectorMeasurement | 32 | 32 | 1 | 0 | None | None | A |
| BEMFq | BackEMFMeasurementPrediction | 0 | 32 | 1 | 0 | None | None | V |
| BEMFd | BackEMFMeasurementPrediction | 32 | 32 | 1 | 0 | None | None | V |
| ReservedSupply15V | VoltageRail15VMeasurement | 0 | 32 | 1 | 0 | None | None |  |
| Supply15V | VoltageRail15VMeasurement | 32 | 32 | 1 | 0 | None | None | V |
| Supply1V9 | VoltageRail3V31V9Measurement | 0 | 32 | 1 | 0 | None | None | V |
| Supply3V3 | VoltageRail3V31V9Measurement | 32 | 32 | 1 | 0 | None | None | V |
| Reserved0A0 | Reserved0A | 0 | 32 | 1 | 0 | None | None |  |
| Reserved0A1 | Reserved0A | 32 | 32 | 1 | 0 | None | None |  |
| MotorTemp | HeatsinkMotorTempMeasurement | 0 | 32 | 1 | 0 | None | None | C |
| HeatsinkTemp | HeatsinkMotorTempMeasurement | 32 | 32 | 1 | 0 | None | None | C |
| DspBoardTemp | DspBoardTempMeasurement | 0 | 32 | 1 | 0 | None | None | C |
| ReservedDspBoardTemp | DspBoardTempMeasurement | 32 | 32 | 1 | 0 | None | None |  |
| Reserved0D0 | Reserved0D | 0 | 32 | 1 | 0 | None | None |  |
| Reserved0D1 | Reserved0D | 32 | 32 | 1 | 0 | None | None |  |
| Odometer | OdometerBusAhMeasurement | 0 | 32 | 1 | 0 | None | None | m |
| DCBusAh | OdometerBusAhMeasurement | 32 | 32 | 1 | 0 | None | None | Ah |
| SlipSpeed | SlipSpeedMeasurement | 0 | 32 | 1 | 0 | None | None | Hz |
| ReservedSlipSpeed | SlipSpeedMeasurement | 32 | 32 | 1 | 0 | None | None |  |


# tpee_mppt[A].dbc

## Messages

| ID | Name | DLC | Signals |
|----|------|-----|---------|
| 513 | MPPT_32_0_Status | 5 | MPPT_Mode, MPPT_Fault, MPPT_Enabled, MPPT_AmbientTemperature, MPPT_HeatsinkTemperature |
| 512 | MPPT_32_0_Power | 8 | MPPT_Vin, MPPT_Iin, MPPT_Vout, MPPT_Iout |
| 521 | MPPT_32_0_SetMode | 1 | Boost_Enable |

## Signals

| Name | Message | Start Bit | Length | Factor | Offset | Min | Max | Unit |
|------|---------|-----------|--------|--------|--------|-----|-----|------|
| MPPT_Mode | MPPT_32_0_Status | 0 | 8 | 1 | 0 | None | None |  |
| MPPT_Fault | MPPT_32_0_Status | 8 | 8 | 1 | 0 | None | None |  |
| MPPT_Enabled | MPPT_32_0_Status | 16 | 8 | 1 | 0 | None | None |  |
| MPPT_AmbientTemperature | MPPT_32_0_Status | 31 | 8 | 1 | 0 | -128 | 127 | C |
| MPPT_HeatsinkTemperature | MPPT_32_0_Status | 39 | 8 | 1 | 0 | -128 | 127 | C |
| MPPT_Vin | MPPT_32_0_Power | 7 | 16 | 0.01 | 0 | -327.68 | 327.67 | V |
| MPPT_Iin | MPPT_32_0_Power | 23 | 16 | 0.0005 | 0 | -16.384 | 16.3835 | A |
| MPPT_Vout | MPPT_32_0_Power | 39 | 16 | 0.01 | 0 | -327.68 | 327.67 | V |
| MPPT_Iout | MPPT_32_0_Power | 55 | 16 | 0.0005 | 0 | None | None | A |
| Boost_Enable | MPPT_32_0_SetMode | 0 | 1 | 1 | 0 | 0 | 1 |  |


# controls.dbc

## Messages

| ID | Name | DLC | Signals |
|----|------|-----|---------|
| 1409 | IO_State | 3 | Acceleration_Percentage, Brake_Percentage, IGN_Array, IGN_Motor, Regen_SW, Forward_Gear, Reverse_Gear, Cruz_EN, Cruz_Set, Brake_Light |
| 1411 | Controls_Fault | 1 | Controls_Fault, Motor_Controller_Fault, BPS_Fault, Pedals_Fault, CarCAN_Fault, Internal_Controls_Fault, OS_Fault, Lakshay_Fault |
| 1412 | Motor_Controller_Safe | 1 | Motor_Safe, Motor_Controller_Error |
| 545 | Motor_Drive_Command | 8 | Motor_Velocity_Setpoint, Motor_Current_Setpoint |
| 546 | Motor_Power_Command | 8 | Motor_Power_Setpoint |
| 1413 | Pedals_Raw_Voltage | 4 | Brake_Raw, Accel_Raw |

## Signals

| Name | Message | Start Bit | Length | Factor | Offset | Min | Max | Unit |
|------|---------|-----------|--------|--------|--------|-----|-----|------|
| Acceleration_Percentage | IO_State | 0 | 8 | 1 | 0 | 0 | 100 | Percentage |
| Brake_Percentage | IO_State | 8 | 8 | 1 | 0 | 0 | 100 | Percentage the brake is pressed |
| IGN_Array | IO_State | 16 | 1 | 1 | 0 | 0 | 1 | Array Ignition On |
| IGN_Motor | IO_State | 17 | 1 | 1 | 0 | 0 | 1 | Motor Ignition On |
| Regen_SW | IO_State | 18 | 1 | 1 | 0 | 0 | 1 | Regen Switched On |
| Forward_Gear | IO_State | 19 | 1 | 1 | 0 | 0 | 1 | Forward switch |
| Reverse_Gear | IO_State | 20 | 1 | 1 | 0 | 0 | 1 | Reverse switch |
| Cruz_EN | IO_State | 21 | 1 | 1 | 0 | 0 | 1 | Cruise Enable |
| Cruz_Set | IO_State | 22 | 1 | 1 | 0 | 0 | 1 | Cruise Set |
| Brake_Light | IO_State | 23 | 1 | 1 | 0 | 0 | 1 | Brakelight state |
| Controls_Fault | Controls_Fault | 0 | 1 | 1 | 0 | 0 | 1 |  |
| Motor_Controller_Fault | Controls_Fault | 1 | 1 | 1 | 0 | 0 | 1 |  |
| BPS_Fault | Controls_Fault | 2 | 1 | 1 | 0 | 0 | 1 |  |
| Pedals_Fault | Controls_Fault | 3 | 1 | 1 | 0 | None | None |  |
| CarCAN_Fault | Controls_Fault | 4 | 1 | 1 | 0 | 0 | 1 |  |
| Internal_Controls_Fault | Controls_Fault | 5 | 1 | 1 | 0 | 0 | 1 |  |
| OS_Fault | Controls_Fault | 6 | 1 | 1 | 0 | 0 | 1 |  |
| Lakshay_Fault | Controls_Fault | 7 | 1 | 1 | 0 | 0 | 1 |  |
| Motor_Safe | Motor_Controller_Safe | 0 | 1 | 1 | 0 | 0 | 1 |  |
| Motor_Controller_Error | Motor_Controller_Safe | 1 | 1 | 1 | 0 | 0 | 1 |  |
| Motor_Velocity_Setpoint | Motor_Drive_Command | 0 | 32 | 1 | 0 | None | None | rpm |
| Motor_Current_Setpoint | Motor_Drive_Command | 32 | 32 | 1 | 0 | None | None | % |
| Motor_Power_Setpoint | Motor_Power_Command | 32 | 32 | 1 | 0 | None | None | % |
| Brake_Raw | Pedals_Raw_Voltage | 0 | 15 | 1 | 0 | 0 | 3300 | mV |
| Accel_Raw | Pedals_Raw_Voltage | 16 | 15 | 1 | 0 | 0 | 3300 | mV |


# tpee_mppt[B].dbc

## Messages

| ID | Name | DLC | Signals |
|----|------|-----|---------|
| 529 | MPPT_33_0_Status | 5 | MPPT_Mode, MPPT_Fault, MPPT_Enabled, MPPT_AmbientTemperature, MPPT_HeatsinkTemperature |
| 528 | MPPT_33_0_Power | 8 | MPPT_Vin, MPPT_Iin, MPPT_Vout, MPPT_Iout |
| 537 | MPPT_33_0_SetMode | 1 | Boost_Enable |

## Signals

| Name | Message | Start Bit | Length | Factor | Offset | Min | Max | Unit |
|------|---------|-----------|--------|--------|--------|-----|-----|------|
| MPPT_Mode | MPPT_33_0_Status | 0 | 8 | 1 | 0 | None | None |  |
| MPPT_Fault | MPPT_33_0_Status | 8 | 8 | 1 | 0 | None | None |  |
| MPPT_Enabled | MPPT_33_0_Status | 16 | 8 | 1 | 0 | None | None |  |
| MPPT_AmbientTemperature | MPPT_33_0_Status | 31 | 8 | 1 | 0 | -128 | 127 | C |
| MPPT_HeatsinkTemperature | MPPT_33_0_Status | 39 | 8 | 1 | 0 | -128 | 127 | C |
| MPPT_Vin | MPPT_33_0_Power | 7 | 16 | 0.01 | 0 | -327.68 | 327.67 | V |
| MPPT_Iin | MPPT_33_0_Power | 23 | 16 | 0.0005 | 0 | -16.384 | 16.3835 | A |
| MPPT_Vout | MPPT_33_0_Power | 39 | 16 | 0.01 | 0 | -327.68 | 327.67 | V |
| MPPT_Iout | MPPT_33_0_Power | 55 | 16 | 0.0005 | 0 | None | None | A |
| Boost_Enable | MPPT_33_0_SetMode | 0 | 1 | 1 | 0 | 0 | 1 |  |


# bps (Daybreak).dbc

## Messages

| ID | Name | DLC | Signals |
|----|------|-----|---------|
| 2 | BPS_Trip | 1 | BPS_Trip |
| 257 | BPS_All_Clea | 1 | BPS_All_Clear |
| 258 | BPS_Contactor_State | 1 | HV_Contactor, Array_Contactor |
| 259 | BPS_Current | 4 | Current |
| 260 | BPS_Voltage_Array | 5 | Voltage_idx, Voltage_Value |
| 261 | BPS_Temperature_Array | 5 | Temperature_idx, Temperature_Value |
| 262 | BPS_SOC | 4 | SoC |
| 263 | BPS_WDog_Trigger | 1 | WDog_Trig |
| 264 | BPS_CAN_Error | 1 | BPS_CAN_Error |
| 265 | BPS_Command | 8 | BPS_Command |
| 267 | BPS_Supplemental_Voltage | 2 | Supplemental_Voltage |
| 268 | BPS_Charge_Enabled | 1 | Charge_Enabled |
| 269 | BPS_Voltage_Summary | 8 | Pack_Voltage, Voltage_Range, Voltage_Timestamp |
| 270 | BPS_Temperature_Summary | 8 | Average_Temp, Temperature_Range, Temperature_Timestamp |
| 271 | BPS_Fault_State | 1 | BPS_Fault_State |

## Signals

| Name | Message | Start Bit | Length | Factor | Offset | Min | Max | Unit |
|------|---------|-----------|--------|--------|--------|-----|-----|------|
| BPS_Trip | BPS_Trip | 0 | 1 | 1 | 0 | 0 | 1 |  |
| BPS_All_Clear | BPS_All_Clea | 0 | 1 | 1 | 0 | 0 | 1 |  |
| HV_Contactor | BPS_Contactor_State | 2 | 1 | 1 | 0 | 0 | 1 |  |
| Array_Contactor | BPS_Contactor_State | 0 | 1 | 1 | 0 | 0 | 1 |  |
| Current | BPS_Current | 0 | 32 | 1 | 0 | -20000 | 100000 | mA |
| Voltage_idx | BPS_Voltage_Array | 0 | 8 | 1 | 0 | 0 | 31 |  |
| Voltage_Value | BPS_Voltage_Array | 8 | 32 | 1 | 0 | 0 | 1000000 | mV |
| Temperature_idx | BPS_Temperature_Array | 0 | 8 | 1 | 0 | 0 | 31 |  |
| Temperature_Value | BPS_Temperature_Array | 8 | 32 | 1 | 0 | 0 | 1000000 | mC |
| SoC | BPS_SOC | 0 | 32 | 1 | 0 | 0 | 100 | % |
| WDog_Trig | BPS_WDog_Trigger | 0 | 1 | 1 | 0 | 0 | 1 |  |
| BPS_CAN_Error | BPS_CAN_Error | 0 | 1 | 1 | 0 | 0 | 1 |  |
| BPS_Command | BPS_Command | 0 | 32 | 1 | 0 | None | None |  |
| Supplemental_Voltage | BPS_Supplemental_Voltage | 0 | 16 | 1 | 0 | 0 | 65535 | mV |
| Charge_Enabled | BPS_Charge_Enabled | 0 | 1 | 1 | 0 | 0 | 1 |  |
| Pack_Voltage | BPS_Voltage_Summary | 0 | 24 | 1 | 0 | 0 | 16777215 | mV |
| Voltage_Range | BPS_Voltage_Summary | 24 | 24 | 1 | 0 | 0 | 16777215 | mV |
| Voltage_Timestamp | BPS_Voltage_Summary | 48 | 16 | 1 | 0 | 0 | 65535 | ms |
| Average_Temp | BPS_Temperature_Summary | 0 | 24 | 1 | 0 | None | None | mC |
| Temperature_Range | BPS_Temperature_Summary | 24 | 24 | 1 | 0 | None | None | mC |
| Temperature_Timestamp | BPS_Temperature_Summary | 48 | 16 | 1 | 0 | None | None | ms |
| BPS_Fault_State | BPS_Fault_State | 0 | 8 | 1 | 0 | 0 | 12 |  |


# contactor.dbc

## Messages

| ID | Name | DLC | Signals |
|----|------|-----|---------|
| 1025 | Precharge_Timeout | 1 | Motor_Precharge_Timeout, Array_Precharge_Timeout |
| 1024 | Contactor_Sense | 2 | Actual_Motor_Sense, Expected_Motor_Sense, Motor_Sense_Fault, Motor_Precharge_Sense, Expected_Motor_Precharge_Sense, Motor_Precharge_Sense_Fault, Array_Precharge_Sense, Expected_Array_Precharge_Sense, Array_Prechage_Sense_Fault |

## Signals

| Name | Message | Start Bit | Length | Factor | Offset | Min | Max | Unit |
|------|---------|-----------|--------|--------|--------|-----|-----|------|
| Motor_Precharge_Timeout | Precharge_Timeout | 0 | 1 | 1 | 0 | 0 | 1 |  |
| Array_Precharge_Timeout | Precharge_Timeout | 1 | 1 | 1 | 0 | None | None |  |
| Actual_Motor_Sense | Contactor_Sense | 0 | 1 | 1 | 0 | 0 | 1 |  |
| Expected_Motor_Sense | Contactor_Sense | 1 | 1 | 1 | 0 | None | None |  |
| Motor_Sense_Fault | Contactor_Sense | 2 | 1 | 1 | 0 | 0 | 1 |  |
| Motor_Precharge_Sense | Contactor_Sense | 3 | 1 | 1 | 0 | 0 | 1 |  |
| Expected_Motor_Precharge_Sense | Contactor_Sense | 4 | 1 | 1 | 0 | 0 | 1 |  |
| Motor_Precharge_Sense_Fault | Contactor_Sense | 5 | 1 | 1 | 0 | 0 | 1 |  |
| Array_Precharge_Sense | Contactor_Sense | 6 | 1 | 1 | 0 | 0 | 1 |  |
| Expected_Array_Precharge_Sense | Contactor_Sense | 7 | 1 | 1 | 0 | 0 | 1 |  |
| Array_Prechage_Sense_Fault | Contactor_Sense | 8 | 1 | 1 | 0 | 0 | 1 |  |


