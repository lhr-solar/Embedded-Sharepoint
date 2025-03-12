#define ID00CUI_status 0xc

#define Parse_UI_touchActive(d) ((d & 0x2) >> 1)
#define Parse_UI_audioActive(d) ((d & 0x8000000000000000) >> 63)
#define Parse_UI_bluetoothActive(d) ((d & 0x4000000000000000) >> 62)
#define Parse_UI_cellActive(d) ((d & 0x2000000000000000) >> 61)
#define Parse_UI_displayReady(d) ((d & 0x1000000000000000) >> 60)
#define Parse_UI_displayOn(d) ((d & 0x800000000000000) >> 59)
#define Parse_UI_wifiActive(d) ((d & 0x400000000000000) >> 58)
#define Parse_UI_wifiConnected(d) ((d & 0x200000000000000) >> 57)
#define Parse_UI_systemActive(d) ((d & 0x100000000000000) >> 56)
#define Parse_UI_readyForDrive(d) ((d & 0x80000000000000) >> 55)
#define Parse_UI_cellConnected(d) ((d & 0x40000000000000) >> 54)
#define Parse_UI_vpnActive(d) ((d & 0x20000000000000) >> 53)
#define Parse_UI_autopilotTrial(d) ((d & 0x30000000000000) >> 52)

enum UI_autopilotTrial_Values { 
  NONE = 0,
  START = 1,
  STOP = 2,
  ACTIVE = 3
 };

#define Parse_UI_factoryReset(d) ((d & 0xc000000000000) >> 50)

enum UI_factoryReset_Values { 
  NONE_SNA = 0,
  DEVELOPER = 1,
  DIAGNOSTIC = 2,
  CUSTOMER = 3
 };

#define Parse_UI_gpsActive(d) ((d & 0x1000000000000) >> 48)
#define Parse_UI_screenshotActive(d) ((d & 0x800000000000) >> 47)
#define Parse_UI_radioActive(d) ((d & 0x400000000000) >> 46)
#define Parse_UI_cellNetworkTechnology(d) ((d & 0x1e00000000000) >> 45)

enum UI_cellNetworkTechnology_Values { 
  CELL_NETWORK_NONE = 0,
  CELL_NETWORK_GPRS = 1,
  CELL_NETWORK_EDGE = 2,
  CELL_NETWORK_UMTS = 3,
  CELL_NETWORK_HSDPA = 4,
  CELL_NETWORK_HSUPA = 5,
  CELL_NETWORK_HSPA = 6,
  CELL_NETWORK_LTE = 7,
  CELL_NETWORK_GSM = 8,
  CELL_NETWORK_CDMA = 9,
  CELL_NETWORK_WCDMA = 10,
  CELL_NETWORK_SNA = 15
 };

#define Parse_UI_cellReceiverPower(d) ((d & 0xff0000000000) >> 40)
#define Parse_UI_falseTouchCounter(d) ((d & 0xff00000000) >> 32)
#define Parse_UI_developmentCar(d) ((d & 0x1000000) >> 24)
#define Parse_UI_cameraActive(d) ((d & 0x800000) >> 23)
#define Parse_UI_cellSignalBars(d) ((d & 0x1c00000) >> 22)

enum UI_cellSignalBars_Values { 
  ZERO = 0,
  ONE = 1,
  TWO = 2,
  THREE = 3,
  FOUR = 4,
  FIVE = 5,
  SNA = 7
 };

#define Parse_UI_pcbTemperature(d) ((d & 0xff0000) >> 16)
#define Parse_UI_cpuTemperature(d) ((d & 0xff00) >> 8)
#define ID353UI_status 0x353

#define Parse_UI_touchActive(d) ((d & 0x2) >> 1)
#define Parse_UI_audioActive(d) ((d & 0x8000000000000000) >> 63)
#define Parse_UI_bluetoothActive(d) ((d & 0x4000000000000000) >> 62)
#define Parse_UI_cellActive(d) ((d & 0x2000000000000000) >> 61)
#define Parse_UI_displayReady(d) ((d & 0x1000000000000000) >> 60)
#define Parse_UI_displayOn(d) ((d & 0x800000000000000) >> 59)
#define Parse_UI_wifiActive(d) ((d & 0x400000000000000) >> 58)
#define Parse_UI_wifiConnected(d) ((d & 0x200000000000000) >> 57)
#define Parse_UI_systemActive(d) ((d & 0x100000000000000) >> 56)
#define Parse_UI_readyForDrive(d) ((d & 0x80000000000000) >> 55)
#define Parse_UI_cellConnected(d) ((d & 0x40000000000000) >> 54)
#define Parse_UI_vpnActive(d) ((d & 0x20000000000000) >> 53)
#define Parse_UI_autopilotTrial(d) ((d & 0x30000000000000) >> 52)
#define Parse_UI_factoryReset(d) ((d & 0xc000000000000) >> 50)
#define Parse_UI_gpsActive(d) ((d & 0x1000000000000) >> 48)
#define Parse_UI_screenshotActive(d) ((d & 0x800000000000) >> 47)
#define Parse_UI_radioActive(d) ((d & 0x400000000000) >> 46)
#define Parse_UI_cellNetworkTechnology(d) ((d & 0x1e00000000000) >> 45)
#define Parse_UI_cellReceiverPower(d) ((d & 0xff0000000000) >> 40)
#define Parse_UI_falseTouchCounter(d) ((d & 0xff00000000) >> 32)
#define Parse_UI_developmentCar(d) ((d & 0x1000000) >> 24)
#define Parse_UI_cameraActive(d) ((d & 0x800000) >> 23)
#define Parse_UI_cellSignalBars(d) ((d & 0x1c00000) >> 22)
#define Parse_UI_pcbTemperature(d) ((d & 0xff0000) >> 16)
#define Parse_UI_cpuTemperature(d) ((d & 0xff00) >> 8)
#define ID016DI_bmsRequest 0x16

#define Parse_DI_bmsRequestInterfaceVersion(d) ((d & 0xf0) >> 4)
#define Parse_DI_bmsOpenContactorsRequest(d) ((d & 0x10) >> 4)
#define ID082UI_tripPlanning 0x82

#define Parse_UI_tripPlanningActive(d) ((d & 0x2) >> 1)
#define Parse_UI_navToSupercharger(d) ((d & 0x8000000000000000) >> 63)
#define Parse_UI_requestActiveBatteryHeating(d) ((d & 0x4000000000000000) >> 62)
#define Parse_UI_predictedEnergy(d) ((d & 0xffff000000000000) >> 48)

enum UI_predictedEnergy_Values { 
  TRIP_TOO_LONG = 32767,
  SNA = 32768
 };

#define Parse_UI_hindsightEnergy(d) ((d & 0xffff00000000) >> 32)

enum UI_hindsightEnergy_Values { 
  TRIP_TOO_LONG = 32767,
  SNA = 32768
 };

#define Parse_UI_energyAtDestination(d) ((d & 0xffff0000) >> 16)

enum UI_energyAtDestination_Values { 
  TRIP_TOO_LONG = 32767,
  SNA = 32768
 };

#define ID101RCM_inertial1 0x101

#define Parse_RCM_yawRate(d) ((d & 0xffff0000) >> 16)

enum RCM_yawRate_Values { 
  SNA = 32768
 };

#define Parse_RCM_pitchRate(d) ((d & 0x7fff000000000000) >> 48)

enum RCM_pitchRate_Values { 
  SNA = 16384
 };

#define Parse_RCM_rollRate(d) ((d & 0xfffe00000000) >> 33)

enum RCM_rollRate_Values { 
  SNA = 16384
 };

#define Parse_RCM_rollRateQF(d) ((d & 0xc0000) >> 18)

enum RCM_rollRateQF_Values { 
  INIT = 0,
  VALID = 1,
  TEMP_INVALID = 2,
  FAULTED = 3
 };

#define Parse_RCM_yawRateQF(d) ((d & 0x10000) >> 16)

enum RCM_yawRateQF_Values { 
  FAULTED = 0,
  NOT_FAULTED = 1
 };

#define Parse_RCM_pitchRateQF(d) ((d & 0xc000) >> 14)

enum RCM_pitchRateQF_Values { 
  INIT = 0,
  VALID = 1,
  TEMP_INVALID = 2,
  FAULTED = 3
 };

#define Parse_RCM_inertial1Counter(d) ((d & 0xf000) >> 12)
#define Parse_RCM_inertial1Checksum(d) ((d & 0xff00) >> 8)
#define ID111RCM_inertial2 0x111

#define Parse_RCM_longitudinalAccel(d) ((d & 0xffff0000) >> 16)

enum RCM_longitudinalAccel_Values { 
  SNA = 32768
 };

#define Parse_RCM_lateralAccel(d) ((d & 0xffff000000000000) >> 48)

enum RCM_lateralAccel_Values { 
  SNA = 32768
 };

#define Parse_RCM_verticalAccel(d) ((d & 0xffff00000000) >> 32)

enum RCM_verticalAccel_Values { 
  SNA = 32768
 };

#define Parse_RCM_longitudinalAccelQF(d) ((d & 0x10000) >> 16)

enum RCM_longitudinalAccelQF_Values { 
  FAULTED = 0,
  NOT_FAULTED = 1
 };

#define Parse_RCM_lateralAccelQF(d) ((d & 0x8000) >> 15)

enum RCM_lateralAccelQF_Values { 
  FAULTED = 0,
  NOT_FAULTED = 1
 };

#define Parse_RCM_verticalAccelQF(d) ((d & 0x4000) >> 14)

enum RCM_verticalAccelQF_Values { 
  FAULTED = 0,
  NOT_FAULTED = 1
 };

#define Parse_RCM_inertial2Counter(d) ((d & 0xf000) >> 12)
#define Parse_RCM_inertial2Checksum(d) ((d & 0xff00) >> 8)
#define RCM_inertial2New 0x116

#define Parse_RCM_longitudinalAccel(d) ((d & 0xffff0000) >> 16)
#define Parse_RCM_lateralAccel(d) ((d & 0xffff000000000000) >> 48)
#define Parse_RCM_verticalAccel(d) ((d & 0xffff00000000) >> 32)
#define Parse_RCM_longitudinalAccelQF(d) ((d & 0x10000) >> 16)
#define Parse_RCM_lateralAccelQF(d) ((d & 0x8000) >> 15)
#define Parse_RCM_verticalAccelQF(d) ((d & 0x4000) >> 14)
#define Parse_RCM_inertial2Counter(d) ((d & 0xf000) >> 12)
#define Parse_RCM_inertial2Checksum(d) ((d & 0xff00) >> 8)
#define ID102VCLEFT_doorStatus 0x102

#define Parse_VCLEFT_frontLatchStatus(d) ((d & 0xf0) >> 4)

enum VCLEFT_frontLatchStatus_Values { 
  LATCH_SNA = 0,
  LATCH_OPENED = 1,
  LATCH_CLOSED = 2,
  LATCH_CLOSING = 3,
  LATCH_OPENING = 4,
  LATCH_AJAR = 5,
  LATCH_TIMEOUT = 6,
  LATCH_DEFAULT = 7,
  LATCH_FAULT = 8
 };

#define Parse_VCLEFT_rearLatchStatus(d) ((d & 0xf000000000000000) >> 60)

enum VCLEFT_rearLatchStatus_Values { 
  LATCH_SNA = 0,
  LATCH_OPENED = 1,
  LATCH_CLOSED = 2,
  LATCH_CLOSING = 3,
  LATCH_OPENING = 4,
  LATCH_AJAR = 5,
  LATCH_TIMEOUT = 6,
  LATCH_DEFAULT = 7,
  LATCH_FAULT = 8
 };

#define Parse_VCLEFT_frontLatchSwitch(d) ((d & 0x100000000000000) >> 56)
#define Parse_VCLEFT_rearLatchSwitch(d) ((d & 0x80000000000000) >> 55)
#define Parse_VCLEFT_frontHandlePulled(d) ((d & 0x40000000000000) >> 54)
#define Parse_VCLEFT_rearHandlePulled(d) ((d & 0x20000000000000) >> 53)
#define Parse_VCLEFT_frontRelActuatorSwitch(d) ((d & 0x10000000000000) >> 52)
#define Parse_VCLEFT_rearRelActuatorSwitch(d) ((d & 0x8000000000000) >> 51)
#define Parse_VCLEFT_frontHandlePWM(d) ((d & 0x7f000000000000) >> 48)
#define Parse_VCLEFT_rearHandlePWM(d) ((d & 0x7f0000000000) >> 40)
#define Parse_VCLEFT_frontIntSwitchPressed(d) ((d & 0x200000000) >> 33)
#define Parse_VCLEFT_rearIntSwitchPressed(d) ((d & 0x100000000) >> 32)
#define Parse_VCLEFT_mirrorTiltXPosition(d) ((d & 0x7f80000000) >> 31)
#define Parse_VCLEFT_mirrorTiltYPosition(d) ((d & 0x7f800000) >> 23)
#define Parse_VCLEFT_mirrorState(d) ((d & 0x38000) >> 15)

enum VCLEFT_mirrorState_Values { 
  MIRROR_STATE_IDLE = 0,
  MIRROR_STATE_TILT_X = 1,
  MIRROR_STATE_TILT_Y = 2,
  MIRROR_STATE_FOLD_UNFOLD = 3,
  MIRROR_STATE_RECALL = 4
 };

#define Parse_VCLEFT_mirrorFoldState(d) ((d & 0x7000) >> 12)

enum VCLEFT_mirrorFoldState_Values { 
  MIRROR_FOLD_STATE_UNKNOWN = 0,
  MIRROR_FOLD_STATE_FOLDED = 1,
  MIRROR_FOLD_STATE_UNFOLDED = 2,
  MIRROR_FOLD_STATE_FOLDING = 3,
  MIRROR_FOLD_STATE_UNFOLDING = 4
 };

#define Parse_VCLEFT_mirrorRecallState(d) ((d & 0xe00) >> 9)

enum VCLEFT_mirrorRecallState_Values { 
  MIRROR_RECALL_STATE_INIT = 0,
  MIRROR_RECALL_STATE_RECALLING_AXIS_1 = 1,
  MIRROR_RECALL_STATE_RECALLING_AXIS_2 = 2,
  MIRROR_RECALL_STATE_RECALLING_COMPLETE = 3,
  MIRROR_RECALL_STATE_RECALLING_FAILED = 4,
  MIRROR_RECALL_STATE_RECALLING_STOPPED = 5
 };

#define Parse_VCLEFT_mirrorHeatState(d) ((d & 0x1c0) >> 6)

enum VCLEFT_mirrorHeatState_Values { 
  HEATER_STATE_SNA = 0,
  HEATER_STATE_ON = 1,
  HEATER_STATE_OFF = 2,
  HEATER_STATE_OFF_UNAVAILABLE = 3,
  HEATER_STATE_FAULT = 4
 };

#define Parse_VCLEFT_mirrorDipped(d) ((d & 0x8) >> 3)
#define Parse_VCLEFT_frontHandlePulledPersist(d) ((d & 0x4) >> 2)
#define ID103VCRIGHT_doorStatus 0x103

#define Parse_VCRIGHT_frontLatchStatus(d) ((d & 0xf0) >> 4)

enum VCRIGHT_frontLatchStatus_Values { 
  LATCH_SNA = 0,
  LATCH_OPENED = 1,
  LATCH_CLOSED = 2,
  LATCH_CLOSING = 3,
  LATCH_OPENING = 4,
  LATCH_AJAR = 5,
  LATCH_TIMEOUT = 6,
  LATCH_DEFAULT = 7,
  LATCH_FAULT = 8
 };

#define Parse_VCRIGHT_rearLatchStatus(d) ((d & 0xf000000000000000) >> 60)

enum VCRIGHT_rearLatchStatus_Values { 
  LATCH_SNA = 0,
  LATCH_OPENED = 1,
  LATCH_CLOSED = 2,
  LATCH_CLOSING = 3,
  LATCH_OPENING = 4,
  LATCH_AJAR = 5,
  LATCH_TIMEOUT = 6,
  LATCH_DEFAULT = 7,
  LATCH_FAULT = 8
 };

#define Parse_VCRIGHT_frontLatchSwitch(d) ((d & 0x100000000000000) >> 56)
#define Parse_VCRIGHT_rearLatchSwitch(d) ((d & 0x80000000000000) >> 55)
#define Parse_VCRIGHT_frontHandlePulled(d) ((d & 0x40000000000000) >> 54)
#define Parse_VCRIGHT_rearHandlePulled(d) ((d & 0x20000000000000) >> 53)
#define Parse_VCRIGHT_frontRelActuatorSwitch(d) ((d & 0x10000000000000) >> 52)
#define Parse_VCRIGHT_rearRelActuatorSwitch(d) ((d & 0x8000000000000) >> 51)
#define Parse_VCRIGHT_frontHandlePWM(d) ((d & 0x1fc000000000000) >> 50)
#define Parse_VCRIGHT_rearHandlePWM(d) ((d & 0x3f80000000000) >> 43)
#define Parse_VCRIGHT_reservedForBackCompat(d) ((d & 0x3000000000) >> 36)
#define Parse_VCRIGHT_frontHandlePulledPersist(d) ((d & 0x400000000) >> 34)
#define Parse_VCRIGHT_frontIntSwitchPressed(d) ((d & 0x200000000) >> 33)
#define Parse_VCRIGHT_rearIntSwitchPressed(d) ((d & 0x100000000) >> 32)
#define Parse_VCRIGHT_mirrorTiltXPosition(d) ((d & 0x7f80000000) >> 31)
#define Parse_VCRIGHT_mirrorTiltYPosition(d) ((d & 0x7f800000) >> 23)
#define Parse_VCRIGHT_mirrorState(d) ((d & 0x38000) >> 15)

enum VCRIGHT_mirrorState_Values { 
  MIRROR_STATE_IDLE = 0,
  MIRROR_STATE_TILT_X = 1,
  MIRROR_STATE_TILT_Y = 2,
  MIRROR_STATE_FOLD_UNFOLD = 3,
  MIRROR_STATE_RECALL = 4
 };

#define Parse_VCRIGHT_mirrorFoldState(d) ((d & 0x7000) >> 12)

enum VCRIGHT_mirrorFoldState_Values { 
  MIRROR_FOLD_STATE_UNKNOWN = 0,
  MIRROR_FOLD_STATE_FOLDED = 1,
  MIRROR_FOLD_STATE_UNFOLDED = 2,
  MIRROR_FOLD_STATE_FOLDING = 3,
  MIRROR_FOLD_STATE_UNFOLDING = 4
 };

#define Parse_VCRIGHT_trunkLatchStatus(d) ((d & 0xf00) >> 8)

enum VCRIGHT_trunkLatchStatus_Values { 
  LATCH_SNA = 0,
  LATCH_OPENED = 1,
  LATCH_CLOSED = 2,
  LATCH_CLOSING = 3,
  LATCH_OPENING = 4,
  LATCH_AJAR = 5,
  LATCH_TIMEOUT = 6,
  LATCH_DEFAULT = 7,
  LATCH_FAULT = 8
 };

#define Parse_VCRIGHT_mirrorRecallState(d) ((d & 0x70) >> 4)

enum VCRIGHT_mirrorRecallState_Values { 
  MIRROR_RECALL_STATE_INIT = 0,
  MIRROR_RECALL_STATE_RECALLING_AXIS_1 = 1,
  MIRROR_RECALL_STATE_RECALLING_AXIS_2 = 2,
  MIRROR_RECALL_STATE_RECALLING_COMPLETE = 3,
  MIRROR_RECALL_STATE_RECALLING_FAILED = 4,
  MIRROR_RECALL_STATE_RECALLING_STOPPED = 5
 };

#define Parse_VCRIGHT_mirrorDipped(d) ((d & 0x2) >> 1)
#define ID113GTW_bmpDebug 0x113

#define Parse_GTW_bmpState(d) ((d & 0x1fe00) >> 9)

enum GTW_bmpState_Values { 
  BMP_STATE_OFF = 0,
  BMP_STATE_ON = 1,
  BMP_STATE_ASLEEP = 2,
  BMP_STATE_MIA = 3,
  BMP_STATE_RESET = 4,
  BMP_STATE_POWER_CYCLE = 5,
  DUMMY = 255
 };

#define Parse_GTW_BMP_AWAKE_PIN(d) ((d & 0x10000) >> 16)
#define Parse_GTW_BMP_PGOOD_PIN(d) ((d & 0x8000) >> 15)
#define Parse_GTW_BMP_PERIPH_nRST_3V3_PIN(d) ((d & 0x4000) >> 14)
#define Parse_GTW_BMP_GTW_PMIC_THERMTRIP(d) ((d & 0x2000) >> 13)
#define Parse_GTW_BMP_GTW_SOC_PWROK(d) ((d & 0x1000) >> 12)
#define Parse_GTW_BMP_GTW_PMIC_ERROR(d) ((d & 0x800) >> 11)
#define Parse_GTW_BMP_GTW_nSUSPWR(d) ((d & 0x400) >> 10)
#define Parse_GTW_BMP_PMIC_PWR_ON(d) ((d & 0x200) >> 9)
#define Parse_GTW_BMP_GTW_PMIC_ON(d) ((d & 0x100) >> 8)
#define ID119VCSEC_windowRequests 0x119

#define Parse_VCSEC_windowRequestLF(d) ((d & 0x2) >> 1)
#define Parse_VCSEC_windowRequestLR(d) ((d & 0x8000) >> 15)
#define Parse_VCSEC_windowRequestRF(d) ((d & 0x4000) >> 14)
#define Parse_VCSEC_windowRequestRR(d) ((d & 0x2000) >> 13)
#define Parse_VCSEC_windowRequestType(d) ((d & 0x3000) >> 12)

enum VCSEC_windowRequestType_Values { 
  WINDOW_REQUEST_IDLE = 0,
  WINDOW_REQUEST_GOTO_PERCENT = 1,
  WINDOW_REQUEST_GOTO_CRACKED = 2,
  WINDOW_REQUEST_GOTO_CLOSED = 3
 };

#define Parse_VCSEC_windowRequestPercent(d) ((d & 0x7f00) >> 8)

enum VCSEC_windowRequestPercent_Values { 
  SNA = 127
 };

#define Parse_VCSEC_hvacRunScreenProtectOnly(d) ((d & 0x2) >> 1)
#define ID122VCLEFT_doorStatus2 0x122

#define Parse_VCLEFT_frontLatchRelDuty(d) ((d & 0xff00) >> 8)
#define Parse_VCLEFT_rearLatchRelDuty(d) ((d & 0xff0000000000) >> 40)
#define Parse_VCLEFT_vehicleInMotion(d) ((d & 0x100000000) >> 32)
#define Parse_VCLEFT_frontDoorState(d) ((d & 0x380000000) >> 31)

enum VCLEFT_frontDoorState_Values { 
  DOOR_STATE_UNKNOWN = 0,
  DOOR_STATE_CLOSED = 1,
  DOOR_STATE_WAIT_FOR_SHORT_DROP = 2,
  DOOR_STATE_RELEASING_LATCH = 3,
  DOOR_STATE_OPEN_OR_AJAR = 4
 };

#define Parse_VCLEFT_rearDoorState(d) ((d & 0x70000000) >> 28)

enum VCLEFT_rearDoorState_Values { 
  DOOR_STATE_UNKNOWN = 0,
  DOOR_STATE_CLOSED = 1,
  DOOR_STATE_WAIT_FOR_SHORT_DROP = 2,
  DOOR_STATE_RELEASING_LATCH = 3,
  DOOR_STATE_OPEN_OR_AJAR = 4
 };

#define Parse_VCLEFT_frontHandleRawStatus(d) ((d & 0x7000000) >> 24)

enum VCLEFT_frontHandleRawStatus_Values { 
  EXTERIOR_HANDLE_STATUS_SNA = 0,
  EXTERIOR_HANDLE_STATUS_INDETERMINATE = 1,
  EXTERIOR_HANDLE_STATUS_NOT_ACTIVE = 2,
  EXTERIOR_HANDLE_STATUS_ACTIVE = 3,
  EXTERIOR_HANDLE_STATUS_DISCONNECTED = 4,
  EXTERIOR_HANDLE_STATUS_FAULT = 5
 };

#define Parse_VCLEFT_rearHandleRawStatus(d) ((d & 0xe00000) >> 21)

enum VCLEFT_rearHandleRawStatus_Values { 
  EXTERIOR_HANDLE_STATUS_SNA = 0,
  EXTERIOR_HANDLE_STATUS_INDETERMINATE = 1,
  EXTERIOR_HANDLE_STATUS_NOT_ACTIVE = 2,
  EXTERIOR_HANDLE_STATUS_ACTIVE = 3,
  EXTERIOR_HANDLE_STATUS_DISCONNECTED = 4,
  EXTERIOR_HANDLE_STATUS_FAULT = 5
 };

#define Parse_VCLEFT_frontHandleDebounceStatus(d) ((d & 0x70000) >> 16)

enum VCLEFT_frontHandleDebounceStatus_Values { 
  EXTERIOR_HANDLE_STATUS_SNA = 0,
  EXTERIOR_HANDLE_STATUS_INDETERMINATE = 1,
  EXTERIOR_HANDLE_STATUS_NOT_ACTIVE = 2,
  EXTERIOR_HANDLE_STATUS_ACTIVE = 3,
  EXTERIOR_HANDLE_STATUS_DISCONNECTED = 4,
  EXTERIOR_HANDLE_STATUS_FAULT = 5
 };

#define Parse_VCLEFT_rearHandleDebounceStatus(d) ((d & 0xe000) >> 13)

enum VCLEFT_rearHandleDebounceStatus_Values { 
  EXTERIOR_HANDLE_STATUS_SNA = 0,
  EXTERIOR_HANDLE_STATUS_INDETERMINATE = 1,
  EXTERIOR_HANDLE_STATUS_NOT_ACTIVE = 2,
  EXTERIOR_HANDLE_STATUS_ACTIVE = 3,
  EXTERIOR_HANDLE_STATUS_DISCONNECTED = 4,
  EXTERIOR_HANDLE_STATUS_FAULT = 5
 };

#define Parse_VCLEFT_frontHandle5vEnable(d) ((d & 0x400) >> 10)
#define Parse_VCLEFT_rearHandle5vEnable(d) ((d & 0x200) >> 9)
#define Parse_VCLEFT_mirrorFoldMaxCurrent(d) ((d & 0x7f00) >> 8)
#define ID123UI_alertMatrix1 0x123

#define Parse_UI_a001_DriverDoorOpen(d) ((d & 0x2) >> 1)
#define Parse_UI_a002_DoorOpen(d) ((d & 0x8000000000000000) >> 63)
#define Parse_UI_a003_TrunkOpen(d) ((d & 0x4000000000000000) >> 62)
#define Parse_UI_a004_FrunkOpen(d) ((d & 0x2000000000000000) >> 61)
#define Parse_UI_a005_HeadlightsOnDoorOpen(d) ((d & 0x1000000000000000) >> 60)
#define Parse_UI_a006_RemoteServiceAlert(d) ((d & 0x800000000000000) >> 59)
#define Parse_UI_a007_SoftPackConfigMismatch(d) ((d & 0x400000000000000) >> 58)
#define Parse_UI_a008_TouchScreenError(d) ((d & 0x200000000000000) >> 57)
#define Parse_UI_a009_SquashfsError(d) ((d & 0x100000000000000) >> 56)
#define Parse_UI_a010_MapsMissing(d) ((d & 0x80000000000000) >> 55)
#define Parse_UI_a011_IncorrectMap(d) ((d & 0x40000000000000) >> 54)
#define Parse_UI_a012_NotOnPrivateProperty(d) ((d & 0x20000000000000) >> 53)
#define Parse_UI_a013_TPMSHardWarning(d) ((d & 0x10000000000000) >> 52)
#define Parse_UI_a014_TPMSSoftWarning(d) ((d & 0x8000000000000) >> 51)
#define Parse_UI_a015_TPMSOverPressureWarning(d) ((d & 0x4000000000000) >> 50)
#define Parse_UI_a016_TPMSTemperatureWarning(d) ((d & 0x2000000000000) >> 49)
#define Parse_UI_a017_TPMSSystemFault(d) ((d & 0x1000000000000) >> 48)
#define Parse_UI_a018_SlipStartOn(d) ((d & 0x800000000000) >> 47)
#define Parse_UI_a019_ParkBrakeFault(d) ((d & 0x400000000000) >> 46)
#define Parse_UI_a020_SteeringReduced(d) ((d & 0x200000000000) >> 45)
#define Parse_UI_a021_RearSeatbeltUnbuckled(d) ((d & 0x100000000000) >> 44)
#define Parse_UI_a022_ApeFusesEtc(d) ((d & 0x80000000000) >> 43)
#define Parse_UI_a023_CellInternetCheckFailed(d) ((d & 0x40000000000) >> 42)
#define Parse_UI_a024_WifiInternetCheckFailed(d) ((d & 0x20000000000) >> 41)
#define Parse_UI_a025_WifiOnlineCheckFailed(d) ((d & 0x10000000000) >> 40)
#define Parse_UI_a026_ModemResetLoopDetected(d) ((d & 0x8000000000) >> 39)
#define Parse_UI_a027_AutoSteerMIA(d) ((d & 0x4000000000) >> 38)
#define Parse_UI_a028_FrontTrunkPopupClosed(d) ((d & 0x2000000000) >> 37)
#define Parse_UI_a029_ModemMIA(d) ((d & 0x1000000000) >> 36)
#define Parse_UI_a030_ModemVMCrash(d) ((d & 0x800000000) >> 35)
#define Parse_UI_a031_BrakeFluidLow(d) ((d & 0x400000000) >> 34)
#define Parse_UI_a032_CellModemRecoveryResets(d) ((d & 0x200000000) >> 33)
#define Parse_UI_a033_ApTrialExpired(d) ((d & 0x100000000) >> 32)
#define Parse_UI_a034_WakeupProblem(d) ((d & 0x80000000) >> 31)
#define Parse_UI_a035_AudioWatchdogKernelError(d) ((d & 0x40000000) >> 30)
#define Parse_UI_a036_AudioWatchdogHfpError(d) ((d & 0x20000000) >> 29)
#define Parse_UI_a037_AudioWatchdogXrunStormEr(d) ((d & 0x10000000) >> 28)
#define Parse_UI_a038_AudioWatchdogA2bI2cLocku(d) ((d & 0x8000000) >> 27)
#define Parse_UI_a039_AudioA2bNeedRediscovery(d) ((d & 0x4000000) >> 26)
#define Parse_UI_a040_HomelinkTransmit(d) ((d & 0x2000000) >> 25)
#define Parse_UI_a041_AudioDmesgXrun(d) ((d & 0x1000000) >> 24)
#define Parse_UI_a042_AudioDmesgRtThrottling(d) ((d & 0x800000) >> 23)
#define Parse_UI_a043_InvalidMapDataOverride(d) ((d & 0x400000) >> 22)
#define Parse_UI_a044_AudioDmesgDspException(d) ((d & 0x200000) >> 21)
#define Parse_UI_a045_ECallNeedsService(d) ((d & 0x100000) >> 20)
#define Parse_UI_a046_BackupCameraStreamError(d) ((d & 0x80000) >> 19)
#define Parse_UI_a047_CellRoamingDisallowed(d) ((d & 0x40000) >> 18)
#define Parse_UI_a048_AudioPremiumAmpCheckFail(d) ((d & 0x20000) >> 17)
#define Parse_UI_a049_BrakeShiftRequired(d) ((d & 0x10000) >> 16)
#define Parse_UI_a050_BackupCameraIPUTimeout(d) ((d & 0x8000) >> 15)
#define Parse_UI_a051_BackupCameraFrameTimeout(d) ((d & 0x4000) >> 14)
#define Parse_UI_a052_KernelPanicReported(d) ((d & 0x2000) >> 13)
#define Parse_UI_a053_QtCarExitError(d) ((d & 0x1000) >> 12)
#define Parse_UI_a054_AudioBoostPowerBad(d) ((d & 0x800) >> 11)
#define Parse_UI_a055_ManualECallDisabled(d) ((d & 0x400) >> 10)
#define Parse_UI_a056_ManualECallButtonDisconn(d) ((d & 0x200) >> 9)
#define Parse_UI_a057_CellAntennaDisconnected(d) ((d & 0x100) >> 8)
#define Parse_UI_a058_GPSAntennaDisconnected(d) ((d & 0x80) >> 7)
#define Parse_UI_a059_ECallSpeakerDisconnected(d) ((d & 0x40) >> 6)
#define Parse_UI_a060_ECallMicDisconnected(d) ((d & 0x20) >> 5)
#define Parse_UI_a061_SIMTestFailed(d) ((d & 0x10) >> 4)
#define Parse_UI_a062_ENSTestFailed(d) ((d & 0x8) >> 3)
#define Parse_UI_a063_CellularTestFailed(d) ((d & 0x4) >> 2)
#define Parse_UI_a064_ModemFirmwareTestFailed(d) ((d & 0x2) >> 1)
#define ID142VCLEFT_liftgateStatus 0x142

#define Parse_VCLEFT_liftgateStatusIndex(d) ((d & 0xc) >> 2)

enum VCLEFT_liftgateStatusIndex_Values { 
  LIFTGATE_STATUS_INDEX_0 = 0,
  LIFTGATE_STATUS_INDEX_1 = 1,
  LIFTGATE_STATUS_INDEX_INVALID = 2
 };

#define Parse_VCLEFT_liftgateState(d) ((d & 0x1e) >> 1)

enum VCLEFT_liftgateState_Values { 
  PLG_STATE_INIT = 0,
  PLG_STATE_OFF = 1,
  PLG_STATE_BACKOFF = 2,
  PLG_STATE_OPENING = 3,
  PLG_STATE_CLOSING = 4,
  PLG_STATE_CLOSED = 5,
  PLG_STATE_LATCH_OPENING = 6,
  PLG_STATE_LATCH_CLOSING = 7,
  PLG_STATE_NOT_INSTALLED = 8,
  PLG_STATE_UNKNOWN = 9,
  PLG_STATE_LATCH_EXIT = 10,
  PLG_STATE_END_OF_TRAVEL = 11,
  PLG_STATE_LATCH_ENTRY = 12
 };

#define Parse_VCLEFT_liftgateStrutDutyCycle(d) ((d & 0x1fe0) >> 5)
#define Parse_VCLEFT_liftgateRequestSource(d) ((d & 0xe00000000000000) >> 57)

enum VCLEFT_liftgateRequestSource_Values { 
  PLG_REQUEST_SOURCE_NONE = 0,
  PLG_REQUEST_SOURCE_MCU_SWITCH = 1,
  PLG_REQUEST_SOURCE_EXTERIOR = 2,
  PLG_REQUEST_SOURCE_SHUTFACE = 3,
  PLG_REQUEST_SOURCE_KEY_TRUNK_BUTTON = 4,
  PLG_REQUEST_SOURCE_CLOSE_ALL = 5,
  PLG_REQUEST_SOURCE_MCU_CLOSE = 6,
  PLG_REQUEST_SOURCE_UDS = 7
 };

#define Parse_VCLEFT_liftgateStoppingCondition(d) ((d & 0x3c0000000000000) >> 54)

enum VCLEFT_liftgateStoppingCondition_Values { 
  PLG_STOPPING_CONDITION_NONE = 0,
  PLG_STOPPING_CONDITION_PINCH = 1,
  PLG_STOPPING_CONDITION_OBSTACLE_STALL = 2,
  PLG_STOPPING_CONDITION_LOW_12V = 3,
  PLG_STOPPING_CONDITION_STATE_TIMEOUT = 4,
  PLG_STOPPING_CONDITION_VEHICLE_AT_SPEED = 5,
  PLG_STOPPING_CONDITION_OBSTACLE_CURRENT = 6,
  PLG_STOPPING_CONDITION_OBSTACLE_TRAJ_POS = 7,
  PLG_STOPPING_CONDITION_OBSTACLE_TRAJ_VEL = 8,
  PLG_STOPPING_CONDITION_UNCALIBRATED = 9,
  PLG_STOPPING_CONDITION_LATCH_FAULT = 10,
  PLG_STOPPING_CONDITION_COUNT = 11
 };

#define Parse_VCLEFT_liftgateStrutCurrent(d) ((d & 0x7fe0000000000000) >> 53)
#define Parse_VCLEFT_liftgateMvmntNotAllowedCo(d) ((d & 0x1c000000000000) >> 50)

enum VCLEFT_liftgateMvmntNotAllowedCo_Values { 
  PLG_MVMT_NOT_ALLOWED_NONE = 0,
  PLG_MVMT_NOT_ALLOWED_LOW_12V = 1,
  PLG_MVMT_NOT_ALLOWED_VEHICLE_AT_SPEED = 2,
  PLG_MVMT_NOT_ALLOWED_UNCALIBRATED = 3,
  PLG_MVMT_NOT_ALLOWED_EXTERIOR_PRESS_AT_MAX_OPEN = 4,
  PLG_MVMT_NOT_ALLOWED_LOCKED = 5
 };

#define Parse_VCLEFT_liftgatePositionCalibrate(d) ((d & 0x800000000000) >> 47)
#define Parse_VCLEFT_liftgateUIChimeRequest(d) ((d & 0x1c00000000000) >> 46)

enum VCLEFT_liftgateUIChimeRequest_Values { 
  LIFTGATE_CHIME_REQUEST_NONE = 0,
  LIFTGATE_CHIME_REQUEST_ONE_SHORT = 1,
  LIFTGATE_CHIME_REQUEST_TWO_SHORT = 2,
  LIFTGATE_CHIME_REQUEST_THREE_SHORT = 3,
  LIFTGATE_CHIME_REQUEST_ONE_LONG = 4
 };

#define Parse_VCLEFT_liftgatePhysicalChimeRequ(d) ((d & 0x380000000000) >> 43)

enum VCLEFT_liftgatePhysicalChimeRequ_Values { 
  LIFTGATE_CHIME_REQUEST_NONE = 0,
  LIFTGATE_CHIME_REQUEST_ONE_SHORT = 1,
  LIFTGATE_CHIME_REQUEST_TWO_SHORT = 2,
  LIFTGATE_CHIME_REQUEST_THREE_SHORT = 3,
  LIFTGATE_CHIME_REQUEST_ONE_LONG = 4
 };

#define Parse_VCLEFT_liftgatePosition(d) ((d & 0x3f80000000000) >> 43)
#define Parse_VCLEFT_liftgateSpeed(d) ((d & 0x3ff000000000) >> 36)
#define Parse_VCLEFT_liftgateLatchRequest(d) ((d & 0x38) >> 3)

enum VCLEFT_liftgateLatchRequest_Values { 
  LATCH_REQUEST_NONE = 0,
  LATCH_REQUEST_CINCH = 1,
  LATCH_REQUEST_RELEASE = 2,
  LATCH_REQUEST_FORCE_RELEASE = 3,
  LATCH_REQUEST_RESET = 4
 };

#define ID145ESP_status 0x145

#define Parse_ESP_statusChecksum(d) ((d & 0xff00) >> 8)
#define Parse_ESP_statusCounter(d) ((d & 0xf00000000000000) >> 56)
#define Parse_ESP_espModeActive(d) ((d & 0x30000000000000) >> 52)

enum ESP_espModeActive_Values { 
  ESP_MODE_00_NORMAL = 0,
  ESP_MODE_01 = 1,
  ESP_MODE_02 = 2,
  ESP_MODE_03 = 3
 };

#define Parse_ESP_stabilityControlSts2(d) ((d & 0xc000000000000) >> 50)

enum ESP_stabilityControlSts2_Values { 
  INIT = 0,
  ON = 1,
  ENGAGED = 2,
  FAULTED = 3
 };

#define Parse_ESP_ebdFaultLamp(d) ((d & 0x1000000000000) >> 48)

enum ESP_ebdFaultLamp_Values { 
  EBD_FAULT_LAMP_OFF = 0,
  EBD_FAULT_LAMP_ON = 1
 };

#define Parse_ESP_absFaultLamp(d) ((d & 0x800000000000) >> 47)

enum ESP_absFaultLamp_Values { 
  ABS_FAULT_LAMP_OFF = 0,
  ABS_FAULT_LAMP_ON = 1
 };

#define Parse_ESP_espFaultLamp(d) ((d & 0x400000000000) >> 46)

enum ESP_espFaultLamp_Values { 
  ESP_FAULT_LAMP_OFF = 0,
  ESP_FAULT_LAMP_ON = 1
 };

#define Parse_ESP_hydraulicBoostEnabled(d) ((d & 0x200000000000) >> 45)
#define Parse_ESP_espLampFlash(d) ((d & 0x100000000000) >> 44)

enum ESP_espLampFlash_Values { 
  ESP_LAMP_OFF = 0,
  ESP_LAMP_FLASH = 1
 };

#define Parse_ESP_brakeLamp(d) ((d & 0x80000000000) >> 43)

enum ESP_brakeLamp_Values { 
  LAMP_OFF = 0,
  LAMP_ON = 1
 };

#define Parse_ESP_absBrakeEvent2(d) ((d & 0xc0000000000) >> 42)

enum ESP_absBrakeEvent2_Values { 
  ABS_EVENT_NOT_ACTIVE = 0,
  ABS_EVENT_ACTIVE_FRONT_REAR = 1,
  ABS_EVENT_ACTIVE_FRONT = 2,
  ABS_EVENT_ACTIVE_REAR = 3
 };

#define Parse_ESP_longitudinalAccelQF(d) ((d & 0x10000000000) >> 40)

enum ESP_longitudinalAccelQF_Values { 
  UNDEFINABLE_ACCURACY = 0,
  IN_SPEC = 1
 };

#define Parse_ESP_lateralAccelQF(d) ((d & 0x8000000000) >> 39)

enum ESP_lateralAccelQF_Values { 
  UNDEFINABLE_ACCURACY = 0,
  IN_SPEC = 1
 };

#define Parse_ESP_yawRateQF(d) ((d & 0x4000000000) >> 38)

enum ESP_yawRateQF_Values { 
  UNDEFINABLE_ACCURACY = 0,
  IN_SPEC = 1
 };

#define Parse_ESP_steeringAngleQF(d) ((d & 0x2000000000) >> 37)

enum ESP_steeringAngleQF_Values { 
  UNDEFINABLE_ACCURACY = 0,
  IN_SPEC = 1
 };

#define Parse_ESP_brakeDiscWipingActive(d) ((d & 0x1000000000) >> 36)

enum ESP_brakeDiscWipingActive_Values { 
  BDW_INACTIVE = 0,
  BDW_ACTIVE = 1
 };

#define Parse_ESP_driverBrakeApply(d) ((d & 0x1800000000) >> 35)

enum ESP_driverBrakeApply_Values { 
  NotInit_orOff = 0,
  Not_Applied = 1,
  Driver_applying_brakes = 2,
  Faulty_SNA = 3
 };

#define Parse_ESP_brakeApply(d) ((d & 0x200000000) >> 33)

enum ESP_brakeApply_Values { 
  BLS_INACTIVE = 0,
  BLS_ACTIVE = 1
 };

#define Parse_ESP_cdpStatus(d) ((d & 0xc0000000) >> 30)

enum ESP_cdpStatus_Values { 
  CDP_IS_NOT_AVAILABLE = 0,
  CDP_IS_AVAILABLE = 1,
  ACTUATING_EPB_CDP = 2,
  CDP_COMMAND_INVALID = 3
 };

#define Parse_ESP_ptcTargetState(d) ((d & 0x30000000) >> 28)

enum ESP_ptcTargetState_Values { 
  FAULT = 0,
  BACKUP = 1,
  ON = 2,
  SNA = 3
 };

#define Parse_ESP_btcTargetState(d) ((d & 0xc000000) >> 26)

enum ESP_btcTargetState_Values { 
  OFF = 0,
  BACKUP = 1,
  ON = 2,
  SNA = 3
 };

#define Parse_ESP_ebrStandstillSkid(d) ((d & 0x10000) >> 16)

enum ESP_ebrStandstillSkid_Values { 
  NO_STANDSTILL_SKID = 0,
  STANDSTILL_SKID_DETECTED = 1
 };

#define Parse_ESP_ebrStatus(d) ((d & 0x18000) >> 15)

enum ESP_ebrStatus_Values { 
  EBR_IS_NOT_AVAILABLE = 0,
  EBR_IS_AVAILABLE = 1,
  ACTUATING_DI_EBR = 2,
  EBR_COMMAND_INVALID = 3
 };

#define Parse_ESP_brakeTorqueTarget(d) ((d & 0x3ffe000) >> 13)

enum ESP_brakeTorqueTarget_Values { 
  SNA = 8191
 };

#define ID1D6DI_limits 0x1d6

#define Parse_DI_limitPCBTemp(d) ((d & 0x2) >> 1)
#define Parse_DI_limitInverterTemp(d) ((d & 0x8000000000) >> 39)
#define Parse_DI_limitStatorTemp(d) ((d & 0x4000000000) >> 38)
#define Parse_DI_limitPoleTemp(d) ((d & 0x2000000000) >> 37)
#define Parse_DI_limitLimpMode(d) ((d & 0x1000000000) >> 36)
#define Parse_DI_limitDeltaFluidTemp(d) ((d & 0x800000000) >> 35)
#define Parse_DI_limitDischargePower(d) ((d & 0x400000000) >> 34)
#define Parse_DI_limitRegenPower(d) ((d & 0x200000000) >> 33)
#define Parse_DI_limitMotorCurrent(d) ((d & 0x100000000) >> 32)
#define Parse_DI_limitMotorVoltage(d) ((d & 0x80000000) >> 31)
#define Parse_DI_limitDriveTorque(d) ((d & 0x40000000) >> 30)
#define Parse_DI_limitRegenTorque(d) ((d & 0x20000000) >> 29)
#define Parse_DI_limitTCDrive(d) ((d & 0x10000000) >> 28)
#define Parse_DI_limitTCRegen(d) ((d & 0x8000000) >> 27)
#define Parse_DI_limitShift(d) ((d & 0x4000000) >> 26)
#define Parse_DI_limitBaseSpeed(d) ((d & 0x2000000) >> 25)
#define Parse_DI_limitIBat(d) ((d & 0x1000000) >> 24)
#define Parse_DI_limitDcCapTemp(d) ((d & 0x800000) >> 23)
#define Parse_DI_limitVBatLow(d) ((d & 0x400000) >> 22)
#define Parse_DI_limitVBatHigh(d) ((d & 0x200000) >> 21)
#define Parse_DI_limitMotorSpeed(d) ((d & 0x100000) >> 20)
#define Parse_DI_limitVehicleSpeed(d) ((d & 0x80000) >> 19)
#define Parse_DI_limitDiff(d) ((d & 0x40000) >> 18)
#define Parse_DI_limitGracefulPowerOff(d) ((d & 0x20000) >> 17)
#define Parse_DI_limitIGBTJunctTemp(d) ((d & 0x10000) >> 16)
#define Parse_DI_limitShockTorque(d) ((d & 0x8000) >> 15)
#define Parse_DI_limitOilPumpFluidTemp(d) ((d & 0x4000) >> 14)
#define Parse_DI_limitStatorFrequency(d) ((d & 0x2000) >> 13)
#define Parse_DI_limitClutch(d) ((d & 0x1000) >> 12)
#define Parse_DI_limitObstacleDetection(d) ((d & 0x800) >> 11)
#define Parse_DI_limitphaseOutBusbarTemp(d) ((d & 0x400) >> 10)
#define Parse_DI_limitphaseOutBusBarWeldTemp(d) ((d & 0x200) >> 9)
#define Parse_DI_limitphaseOutLugTemp(d) ((d & 0x100) >> 8)
#define Parse_DI_limitdcLinkCapTemp(d) ((d & 0x80) >> 7)
#define Parse_DI_limitnegDcBusbarTemp(d) ((d & 0x40) >> 6)
#define Parse_DI_limitposDcBusbarTemp(d) ((d & 0x20) >> 5)
#define Parse_DI_limitRotorTemp(d) ((d & 0x10) >> 4)
#define Parse_DI_limithvDcCableTemp(d) ((d & 0x8) >> 3)
#define ID20AHVP_contactorState 0x20a

#define Parse_HVP_packContNegativeState(d) ((d & 0x38) >> 3)

enum HVP_packContNegativeState_Values { 
  CONTACTOR_STATE_SNA = 0,
  CONTACTOR_STATE_OPEN = 1,
  CONTACTOR_STATE_PRECHARGE = 2,
  CONTACTOR_STATE_BLOCKED = 3,
  CONTACTOR_STATE_PULLED_IN = 4,
  CONTACTOR_STATE_OPENING = 5,
  CONTACTOR_STATE_ECONOMIZED = 6,
  CONTACTOR_STATE_WELDED = 7
 };

#define Parse_HVP_packContPositiveState(d) ((d & 0xe00000000000) >> 45)

enum HVP_packContPositiveState_Values { 
  CONTACTOR_STATE_SNA = 0,
  CONTACTOR_STATE_OPEN = 1,
  CONTACTOR_STATE_PRECHARGE = 2,
  CONTACTOR_STATE_BLOCKED = 3,
  CONTACTOR_STATE_PULLED_IN = 4,
  CONTACTOR_STATE_OPENING = 5,
  CONTACTOR_STATE_ECONOMIZED = 6,
  CONTACTOR_STATE_WELDED = 7
 };

#define Parse_HVP_fcContPositiveAuxOpen(d) ((d & 0x40000000000) >> 42)
#define Parse_HVP_fcContNegativeAuxOpen(d) ((d & 0x20000000000) >> 41)
#define Parse_HVP_packContactorSetState(d) ((d & 0xf0000000000) >> 40)

enum HVP_packContactorSetState_Values { 
  CONTACTOR_SET_STATE_SNA = 0,
  CONTACTOR_SET_STATE_OPEN = 1,
  CONTACTOR_SET_STATE_CLOSING = 2,
  CONTACTOR_SET_STATE_BLOCKED = 3,
  CONTACTOR_SET_STATE_OPENING = 4,
  CONTACTOR_SET_STATE_CLOSED = 5,
  CONTACTOR_SET_STATE_PARTIAL_WELD = 6,
  CONTACTOR_SET_STATE_WELDED = 7,
  CONTACTOR_SET_STATE_POSITIVE_CLOSED = 8,
  CONTACTOR_SET_STATE_NEGATIVE_CLOSED = 9
 };

#define Parse_HVP_fcContNegativeState(d) ((d & 0x7000000000) >> 36)

enum HVP_fcContNegativeState_Values { 
  CONTACTOR_STATE_SNA = 0,
  CONTACTOR_STATE_OPEN = 1,
  CONTACTOR_STATE_PRECHARGE = 2,
  CONTACTOR_STATE_BLOCKED = 3,
  CONTACTOR_STATE_PULLED_IN = 4,
  CONTACTOR_STATE_OPENING = 5,
  CONTACTOR_STATE_ECONOMIZED = 6,
  CONTACTOR_STATE_WELDED = 7
 };

#define Parse_HVP_fcContPositiveState(d) ((d & 0x700000000) >> 32)

enum HVP_fcContPositiveState_Values { 
  CONTACTOR_STATE_SNA = 0,
  CONTACTOR_STATE_OPEN = 1,
  CONTACTOR_STATE_PRECHARGE = 2,
  CONTACTOR_STATE_BLOCKED = 3,
  CONTACTOR_STATE_PULLED_IN = 4,
  CONTACTOR_STATE_OPENING = 5,
  CONTACTOR_STATE_ECONOMIZED = 6,
  CONTACTOR_STATE_WELDED = 7
 };

#define Parse_HVP_fcContactorSetState(d) ((d & 0x1e0000000) >> 29)

enum HVP_fcContactorSetState_Values { 
  CONTACTOR_SET_STATE_SNA = 0,
  CONTACTOR_SET_STATE_OPEN = 1,
  CONTACTOR_SET_STATE_CLOSING = 2,
  CONTACTOR_SET_STATE_BLOCKED = 3,
  CONTACTOR_SET_STATE_OPENING = 4,
  CONTACTOR_SET_STATE_CLOSED = 5,
  CONTACTOR_SET_STATE_PARTIAL_WELD = 6,
  CONTACTOR_SET_STATE_WELDED = 7,
  CONTACTOR_SET_STATE_POSITIVE_CLOSED = 8,
  CONTACTOR_SET_STATE_NEGATIVE_CLOSED = 9
 };

#define Parse_HVP_fcCtrsRequestStatus(d) ((d & 0x3000000) >> 24)

enum HVP_fcCtrsRequestStatus_Values { 
  REQUEST_NOT_ACTIVE = 0,
  REQUEST_ACTIVE = 1,
  REQUEST_COMPLETED = 2
 };

#define Parse_HVP_fcCtrsResetRequestRequired(d) ((d & 0x400000) >> 22)
#define Parse_HVP_fcCtrsOpenNowRequested(d) ((d & 0x200000) >> 21)
#define Parse_HVP_fcCtrsOpenRequested(d) ((d & 0x100000) >> 20)
#define Parse_HVP_fcCtrsClosingAllowed(d) ((d & 0x80000) >> 19)
#define Parse_HVP_packCtrsRequestStatus(d) ((d & 0xc0000) >> 18)

enum HVP_packCtrsRequestStatus_Values { 
  REQUEST_NOT_ACTIVE = 0,
  REQUEST_ACTIVE = 1,
  REQUEST_COMPLETED = 2
 };

#define Parse_HVP_packCtrsResetRequestRequired(d) ((d & 0x10000) >> 16)
#define Parse_HVP_packCtrsOpenNowRequested(d) ((d & 0x8000) >> 15)
#define Parse_HVP_packCtrsOpenRequested(d) ((d & 0x4000) >> 14)
#define Parse_HVP_packCtrsClosingAllowed(d) ((d & 0x2000) >> 13)
#define Parse_HVP_dcLinkAllowedToEnergize(d) ((d & 0x1000) >> 12)
#define Parse_HVP_pyroTestInProgress(d) ((d & 0x800) >> 11)
#define Parse_HVP_hvilStatus(d) ((d & 0xf00) >> 8)

enum HVP_hvilStatus_Values { 
  UNKNOWN = 0,
  STATUS_OK = 1,
  CURRENT_SOURCE_FAULT = 2,
  INTERNAL_OPEN_FAULT = 3,
  VEHICLE_OPEN_FAULT = 4,
  PENTHOUSE_LID_OPEN_FAULT = 5,
  UNKNOWN_LOCATION_OPEN_FAULT = 6,
  VEHICLE_NODE_FAULT = 7,
  NO_12V_SUPPLY = 8,
  VEHICLE_OR_PENTHOUSE_LID_OPEN_FAULT = 9
 };

#define Parse_HVP_fcLinkAllowedToEnergize(d) ((d & 0x30) >> 4)

enum HVP_fcLinkAllowedToEnergize_Values { 
  FC_LINK_ENERGY_NONE = 0,
  FC_LINK_ENERGY_AC = 1,
  FC_LINK_ENERGY_DC = 2
 };

#define ID20EPARK_sdiFront 0x20e

#define Parse_PARK_sdiSensor1RawDistData(d) ((d & 0x3fe00) >> 9)

enum PARK_sdiSensor1RawDistData_Values { 
  BLOCKED = 0,
  NEAR_DETECTION = 1,
  NO_OBJECT_DETECTED = 500,
  SNA = 511
 };

#define Parse_PARK_sdiSensor2RawDistData(d) ((d & 0xff80000000000000) >> 55)

enum PARK_sdiSensor2RawDistData_Values { 
  BLOCKED = 0,
  NEAR_DETECTION = 1,
  NO_OBJECT_DETECTED = 500,
  SNA = 511
 };

#define Parse_PARK_sdiSensor3RawDistData(d) ((d & 0x7fc00000000000) >> 46)

enum PARK_sdiSensor3RawDistData_Values { 
  BLOCKED = 0,
  NEAR_DETECTION = 1,
  NO_OBJECT_DETECTED = 500,
  SNA = 511
 };

#define Parse_PARK_sdiSensor4RawDistData(d) ((d & 0x3fe000000000) >> 37)

enum PARK_sdiSensor4RawDistData_Values { 
  BLOCKED = 0,
  NEAR_DETECTION = 1,
  NO_OBJECT_DETECTED = 500,
  SNA = 511
 };

#define Parse_PARK_sdiSensor5RawDistData(d) ((d & 0x1ff0000000) >> 28)

enum PARK_sdiSensor5RawDistData_Values { 
  BLOCKED = 0,
  NEAR_DETECTION = 1,
  NO_OBJECT_DETECTED = 500,
  SNA = 511
 };

#define Parse_PARK_sdiSensor6RawDistData(d) ((d & 0xff80000) >> 19)

enum PARK_sdiSensor6RawDistData_Values { 
  BLOCKED = 0,
  NEAR_DETECTION = 1,
  NO_OBJECT_DETECTED = 500,
  SNA = 511
 };

#define Parse_PARK_sdiFrontCounter(d) ((d & 0xc00) >> 10)
#define Parse_PARK_sdiFrontChecksum(d) ((d & 0xff00) >> 8)
#define ID219VCSEC_TPMSData 0x219

#define Parse_VCSEC_TPMSDataIndex(d) ((d & 0xc) >> 2)

enum VCSEC_TPMSDataIndex_Values { 
  TPMS_DATA_SENSOR_0 = 0,
  TPMS_DATA_SENSOR_1 = 1,
  TPMS_DATA_SENSOR_2 = 2,
  TPMS_DATA_SENSOR_3 = 3
 };

#define Parse_VCSEC_TPMSPressure0(d) ((d & 0xff00000000) >> 32)

enum VCSEC_TPMSPressure0_Values { 
  SNA = 255
 };

#define Parse_VCSEC_TPMSPressure1(d) ((d & 0xff00000000) >> 32)

enum VCSEC_TPMSPressure1_Values { 
  SNA = 255
 };

#define Parse_VCSEC_TPMSPressure2(d) ((d & 0xff00000000) >> 32)

enum VCSEC_TPMSPressure2_Values { 
  SNA = 255
 };

#define Parse_VCSEC_TPMSPressure3(d) ((d & 0xff00000000) >> 32)

enum VCSEC_TPMSPressure3_Values { 
  SNA = 255
 };

#define Parse_VCSEC_TPMSTemperature0(d) ((d & 0xff000000) >> 24)

enum VCSEC_TPMSTemperature0_Values { 
  SNA = 255
 };

#define Parse_VCSEC_TPMSTemperature1(d) ((d & 0xff000000) >> 24)

enum VCSEC_TPMSTemperature1_Values { 
  SNA = 255
 };

#define Parse_VCSEC_TPMSTemperature2(d) ((d & 0xff000000) >> 24)

enum VCSEC_TPMSTemperature2_Values { 
  SNA = 255
 };

#define Parse_VCSEC_TPMSTemperature3(d) ((d & 0xff000000) >> 24)

enum VCSEC_TPMSTemperature3_Values { 
  SNA = 255
 };

#define Parse_VCSEC_TPMSBatVoltage0(d) ((d & 0xff0000) >> 16)

enum VCSEC_TPMSBatVoltage0_Values { 
  SNA = 255
 };

#define Parse_VCSEC_TPMSBatVoltage1(d) ((d & 0xff0000) >> 16)

enum VCSEC_TPMSBatVoltage1_Values { 
  SNA = 255
 };

#define Parse_VCSEC_TPMSBatVoltage2(d) ((d & 0xff0000) >> 16)

enum VCSEC_TPMSBatVoltage2_Values { 
  SNA = 255
 };

#define Parse_VCSEC_TPMSBatVoltage3(d) ((d & 0xff0000) >> 16)

enum VCSEC_TPMSBatVoltage3_Values { 
  SNA = 255
 };

#define Parse_VCSEC_TPMSLocation0(d) ((d & 0x700) >> 8)

enum VCSEC_TPMSLocation0_Values { 
  LOCATION_FL = 0,
  LOCATION_FR = 1,
  LOCATION_RL = 2,
  LOCATION_RR = 3,
  LOCATION_UNKNOWN = 4
 };

#define Parse_VCSEC_TPMSLocation1(d) ((d & 0x700) >> 8)

enum VCSEC_TPMSLocation1_Values { 
  LOCATION_FL = 0,
  LOCATION_FR = 1,
  LOCATION_RL = 2,
  LOCATION_RR = 3,
  LOCATION_UNKNOWN = 4
 };

#define Parse_VCSEC_TPMSLocation2(d) ((d & 0x700) >> 8)

enum VCSEC_TPMSLocation2_Values { 
  LOCATION_FL = 0,
  LOCATION_FR = 1,
  LOCATION_RL = 2,
  LOCATION_RR = 3,
  LOCATION_UNKNOWN = 4
 };

#define Parse_VCSEC_TPMSLocation3(d) ((d & 0x700) >> 8)

enum VCSEC_TPMSLocation3_Values { 
  LOCATION_FL = 0,
  LOCATION_FR = 1,
  LOCATION_RL = 2,
  LOCATION_RR = 3,
  LOCATION_UNKNOWN = 4
 };

#define ID204PCS_chgStatus 0x204

#define Parse_PCS_chgMainState(d) ((d & 0xf0) >> 4)

enum PCS_chgMainState_Values { 
  PCS_CHG_STATE_INIT = 0,
  PCS_CHG_STATE_IDLE = 1,
  PCS_CHG_STATE_STARTUP = 2,
  PCS_CHG_STATE_WAIT_FOR_LINE_VOLTAGE = 3,
  PCS_CHG_STATE_QUALIFY_LINE_CONFIG = 4,
  PCS_CHG_STATE_SYSTEM_CONFIG = 5,
  PCS_CHG_STATE_ENABLE = 6,
  PCS_CHG_STATE_SHUTDOWN = 7,
  PCS_CHG_STATE_FAULTED = 8,
  PCS_CHG_STATE_CLEAR_FAULTS = 9
 };

#define Parse_PCS_hvChargeStatus(d) ((d & 0x3000000000000000) >> 60)

enum PCS_hvChargeStatus_Values { 
  PCS_CHARGE_STANDBY = 0,
  PCS_CHARGE_BLOCKED = 1,
  PCS_CHARGE_ENABLED = 2,
  PCS_CHARGE_FAULTED = 3
 };

#define Parse_PCS_gridConfig(d) ((d & 0xc00000000000000) >> 58)

enum PCS_gridConfig_Values { 
  GRID_CONFIG_SNA = 0,
  GRID_CONFIG_SINGLE_PHASE = 1,
  GRID_CONFIG_THREE_PHASE = 2,
  GRID_CONFIG_THREE_PHASE_DELTA = 3
 };

#define Parse_PCS_chgPHAEnable(d) ((d & 0x100000000000000) >> 56)
#define Parse_PCS_chgPHBEnable(d) ((d & 0x80000000000000) >> 55)
#define Parse_PCS_chgPHCEnable(d) ((d & 0x40000000000000) >> 54)
#define Parse_PCS_chgInstantAcPowerAvailable(d) ((d & 0xff000000000000) >> 48)
#define Parse_PCS_chgMaxAcPowerAvailable(d) ((d & 0xff0000000000) >> 40)
#define Parse_PCS_chgPHALineCurrentRequest(d) ((d & 0xff00000000) >> 32)
#define Parse_PCS_chgPHBLineCurrentRequest(d) ((d & 0xff000000) >> 24)
#define Parse_PCS_chgPHCLineCurrentRequest(d) ((d & 0xff0000) >> 16)
#define Parse_PCS_chgPwmEnableLine(d) ((d & 0x100) >> 8)
#define Parse_PCS_chargeShutdownRequest(d) ((d & 0x180) >> 7)

enum PCS_chargeShutdownRequest_Values { 
  NO_SHUTDOWN_REQUESTED = 0,
  GRACEFUL_SHUTDOWN_REQUESTED = 1,
  EMERGENCY_SHUTDOWN_REQUESTED = 2
 };

#define Parse_PCS_hwVariantType(d) ((d & 0x60) >> 5)

enum PCS_hwVariantType_Values { 
  PCS_48A_SINGLE_PHASE_VARIANT = 0,
  PCS_32A_SINGLE_PHASE_VARIANT = 1,
  PCS_THREE_PHASES_VARIANT = 2,
  PCS_HW_VARIANT_TYPE_SNA = 3
 };

#define ID22AHVP_pcsControl 0x22a

#define Parse_HVP_dcLinkVoltageRequest(d) ((d & 0xffff0000) >> 16)
#define Parse_HVP_pcsControlRequest(d) ((d & 0x30000) >> 16)

enum HVP_pcsControlRequest_Values { 
  SHUTDOWN = 0,
  SUPPORT = 1,
  PRECHARGE = 2,
  DISCHARGE = 3
 };

#define Parse_HVP_pcsChargeHwEnabled(d) ((d & 0x4000) >> 14)
#define Parse_HVP_pcsDcdcHwEnabled(d) ((d & 0x2000) >> 13)
#define Parse_HVP_dcLinkVoltageFiltered(d) ((d & 0x7ff000) >> 12)

enum HVP_dcLinkVoltageFiltered_Values { 
  SNA = 550
 };

#define ID232BMS_contactorRequest 0x232

#define Parse_BMS_fcContactorRequest(d) ((d & 0x38) >> 3)

enum BMS_fcContactorRequest_Values { 
  SET_REQUEST_SNA = 0,
  SET_REQUEST_CLOSE = 1,
  SET_REQUEST_OPEN = 2,
  SET_REQUEST_OPEN_IMMEDIATELY = 3,
  SET_REQUEST_CLOSE_NEGATIVE_ONLY = 4,
  SET_REQUEST_CLOSE_POSITIVE_ONLY = 5
 };

#define Parse_BMS_packContactorRequest(d) ((d & 0xe000000000000000) >> 61)

enum BMS_packContactorRequest_Values { 
  SET_REQUEST_SNA = 0,
  SET_REQUEST_CLOSE = 1,
  SET_REQUEST_OPEN = 2,
  SET_REQUEST_OPEN_IMMEDIATELY = 3,
  SET_REQUEST_CLOSE_NEGATIVE_ONLY = 4,
  SET_REQUEST_CLOSE_POSITIVE_ONLY = 5
 };

#define Parse_BMS_gpoHasCompleted(d) ((d & 0x400000000000000) >> 58)
#define Parse_BMS_ensShouldBeActiveForDrive(d) ((d & 0x200000000000000) >> 57)
#define Parse_BMS_pcsPwmDisable(d) ((d & 0x100000000000000) >> 56)
#define Parse_BMS_internalHvilSenseV(d) ((d & 0xffff000000000000) >> 48)

enum BMS_internalHvilSenseV_Values { 
  SNA = 65535
 };

#define Parse_BMS_fcLinkOkToEnergizeRequest(d) ((d & 0x300000000) >> 32)

enum BMS_fcLinkOkToEnergizeRequest_Values { 
  FC_LINK_ENERGY_NONE = 0,
  FC_LINK_ENERGY_AC = 1,
  FC_LINK_ENERGY_DC = 2
 };

#define ID273UI_vehicleControl 0x273

#define Parse_UI_accessoryPowerRequest(d) ((d & 0x2) >> 1)
#define Parse_UI_frontFogSwitch(d) ((d & 0x2000000000000000) >> 61)
#define Parse_UI_summonActive(d) ((d & 0x1000000000000000) >> 60)
#define Parse_UI_frunkRequest(d) ((d & 0x800000000000000) >> 59)
#define Parse_UI_wiperMode(d) ((d & 0xc00000000000000) >> 58)

enum UI_wiperMode_Values { 
  WIPER_MODE_SNA = 0,
  WIPER_MODE_SERVICE = 1,
  WIPER_MODE_NORMAL = 2,
  WIPER_MODE_PARK = 3
 };

#define Parse_UI_steeringBacklightEnabled(d) ((d & 0x100000000000000) >> 56)

enum UI_steeringBacklightEnabled_Values { 
  STEERING_BACKLIGHT_DISABLED = 0,
  STEERING_BACKLIGHT_ENABLED = 1
 };

#define Parse_UI_steeringButtonMode(d) ((d & 0x380000000000000) >> 55)

enum UI_steeringButtonMode_Values { 
  STEERING_BUTTON_MODE_OFF = 0,
  STEERING_BUTTON_MODE_STEERING_COLUMN_ADJ = 1,
  STEERING_BUTTON_MODE_MIRROR_LEFT = 2,
  STEERING_BUTTON_MODE_MIRROR_RIGHT = 3,
  STEERING_BUTTON_MODE_HEADLIGHT_LEFT = 4,
  STEERING_BUTTON_MODE_HEADLIGHT_RIGHT = 5
 };

#define Parse_UI_walkUpUnlock(d) ((d & 0x10000000000000) >> 52)
#define Parse_UI_walkAwayLock(d) ((d & 0x8000000000000) >> 51)
#define Parse_UI_unlockOnPark(d) ((d & 0x4000000000000) >> 50)
#define Parse_UI_globalUnlockOn(d) ((d & 0x2000000000000) >> 49)
#define Parse_UI_childDoorLockOn(d) ((d & 0x1000000000000) >> 48)
#define Parse_UI_lockRequest(d) ((d & 0x3800000000000) >> 47)

enum UI_lockRequest_Values { 
  UI_LOCK_REQUEST_IDLE = 0,
  UI_LOCK_REQUEST_LOCK = 1,
  UI_LOCK_REQUEST_UNLOCK = 2,
  UI_LOCK_REQUEST_REMOTE_UNLOCK = 3,
  UI_LOCK_REQUEST_REMOTE_LOCK = 4,
  UI_LOCK_REQUEST_SNA = 7
 };

#define Parse_UI_alarmEnabled(d) ((d & 0x100000000000) >> 44)
#define Parse_UI_intrusionSensorOn(d) ((d & 0x80000000000) >> 43)
#define Parse_UI_stop12vSupport(d) ((d & 0x40000000000) >> 42)
#define Parse_UI_rearFogSwitch(d) ((d & 0x20000000000) >> 41)
#define Parse_UI_mirrorFoldRequest(d) ((d & 0x30000000000) >> 40)

enum UI_mirrorFoldRequest_Values { 
  MIRROR_FOLD_REQUEST_IDLE = 0,
  MIRROR_FOLD_REQUEST_RETRACT = 1,
  MIRROR_FOLD_REQUEST_PRESENT = 2,
  MIRROR_FOLD_REQUEST_SNA = 3
 };

#define Parse_UI_mirrorHeatRequest(d) ((d & 0x4000000000) >> 38)
#define Parse_UI_remoteStartRequest(d) ((d & 0xe000000000) >> 37)

enum UI_remoteStartRequest_Values { 
  UI_REMOTE_START_REQUEST_IDLE = 0,
  UI_REMOTE_START_REQUEST_START = 1,
  UI_REMOTE_START_REQUEST_SNA = 4
 };

#define Parse_UI_seeYouHomeLightingOn(d) ((d & 0x400000000) >> 34)
#define Parse_UI_powerOff(d) ((d & 0x200000000) >> 33)
#define Parse_UI_displayBrightnessLevel(d) ((d & 0xff00000000) >> 32)

enum UI_displayBrightnessLevel_Values { 
  SNA = 255
 };

#define Parse_UI_ambientLightingEnabled(d) ((d & 0x1000000) >> 24)
#define Parse_UI_autoHighBeamEnabled(d) ((d & 0x800000) >> 23)
#define Parse_UI_frontLeftSeatHeatReq(d) ((d & 0xc00000) >> 22)

enum UI_frontLeftSeatHeatReq_Values { 
  HEATER_REQUEST_OFF = 0,
  HEATER_REQUEST_LEVEL1 = 1,
  HEATER_REQUEST_LEVEL2 = 2,
  HEATER_REQUEST_LEVEL3 = 3
 };

#define Parse_UI_frontRightSeatHeatReq(d) ((d & 0x300000) >> 20)

enum UI_frontRightSeatHeatReq_Values { 
  HEATER_REQUEST_OFF = 0,
  HEATER_REQUEST_LEVEL1 = 1,
  HEATER_REQUEST_LEVEL2 = 2,
  HEATER_REQUEST_LEVEL3 = 3
 };

#define Parse_UI_rearLeftSeatHeatReq(d) ((d & 0xc0000) >> 18)

enum UI_rearLeftSeatHeatReq_Values { 
  HEATER_REQUEST_OFF = 0,
  HEATER_REQUEST_LEVEL1 = 1,
  HEATER_REQUEST_LEVEL2 = 2,
  HEATER_REQUEST_LEVEL3 = 3
 };

#define Parse_UI_rearCenterSeatHeatReq(d) ((d & 0x30000) >> 16)

enum UI_rearCenterSeatHeatReq_Values { 
  HEATER_REQUEST_OFF = 0,
  HEATER_REQUEST_LEVEL1 = 1,
  HEATER_REQUEST_LEVEL2 = 2,
  HEATER_REQUEST_LEVEL3 = 3
 };

#define Parse_UI_rearRightSeatHeatReq(d) ((d & 0xc000) >> 14)

enum UI_rearRightSeatHeatReq_Values { 
  HEATER_REQUEST_OFF = 0,
  HEATER_REQUEST_LEVEL1 = 1,
  HEATER_REQUEST_LEVEL2 = 2,
  HEATER_REQUEST_LEVEL3 = 3
 };

#define Parse_UI_autoFoldMirrorsOn(d) ((d & 0x1000) >> 12)
#define Parse_UI_mirrorDipOnReverse(d) ((d & 0x800) >> 11)
#define Parse_UI_remoteClosureRequest(d) ((d & 0xc00) >> 10)

enum UI_remoteClosureRequest_Values { 
  UI_REMOTE_CLOSURE_REQUEST_IDLE = 0,
  UI_REMOTE_CLOSURE_REQUEST_REAR_TRUNK_MOVE = 1,
  UI_REMOTE_CLOSURE_REQUEST_FRONT_TRUNK_MOVE = 2,
  UI_REMOTE_CLOSURE_REQUEST_SNA = 3
 };

#define Parse_UI_wiperRequest(d) ((d & 0x700) >> 8)

enum UI_wiperRequest_Values { 
  WIPER_REQUEST_SNA = 0,
  WIPER_REQUEST_OFF = 1,
  WIPER_REQUEST_AUTO = 2,
  WIPER_REQUEST_SLOW_INTERMITTENT = 3,
  WIPER_REQUEST_FAST_INTERMITTENT = 4,
  WIPER_REQUEST_SLOW_CONTINUOUS = 5,
  WIPER_REQUEST_FAST_CONTINUOUS = 6
 };

#define Parse_UI_domeLightSwitch(d) ((d & 0x60) >> 5)

enum UI_domeLightSwitch_Values { 
  DOME_LIGHT_SWITCH_OFF = 0,
  DOME_LIGHT_SWITCH_ON = 1,
  DOME_LIGHT_SWITCH_AUTO = 2
 };

#define Parse_UI_honkHorn(d) ((d & 0x8) >> 3)
#define Parse_UI_driveStateRequest(d) ((d & 0x4) >> 2)

enum UI_driveStateRequest_Values { 
  DRIVE_STATE_REQ_IDLE = 0,
  DRIVE_STATE_REQ_START = 1
 };

#define Parse_UI_rearWindowLockout(d) ((d & 0x2) >> 1)
#define ID27DCP_dcChargeLimits 0x27d

#define Parse_CP_evseMaxDcCurrentLimit(d) ((d & 0x3ffe000) >> 13)
#define Parse_CP_evseMinDcCurrentLimit(d) ((d & 0xfff8000000000000) >> 51)
#define Parse_CP_evseMaxDcVoltageLimit(d) ((d & 0x7ffc000000000) >> 38)
#define Parse_CP_evseMinDcVoltageLimit(d) ((d & 0x3ffe000000) >> 25)
#define Parse_CP_evseInstantDcCurrentLimit(d) ((d & 0xfff000) >> 12)
#define ID2BDCP_dcPowerLimits 0x2bd

#define Parse_CP_evseInstantDcPowerLimit(d) ((d & 0x3ffe000) >> 13)
#define Parse_CP_evseMaxDcPowerLimit(d) ((d & 0x1fff0000) >> 16)
#define ID42AVCSEC_TPMSConnectionData 0x42a

#define Parse_VCSEC_TPMSSensorState0(d) ((d & 0x38) >> 3)

enum VCSEC_TPMSSensorState0_Values { 
  SENSOR_NOT_PAIRED = 0,
  SENSOR_WAIT_FOR_ADV = 1,
  SENSOR_WAIT_FOR_CONN = 2,
  SENSOR_CONNECTED = 3,
  SENSOR_DISCONNECTING = 4
 };

#define Parse_VCSEC_TPMSRSSI0(d) ((d & 0x1fe0) >> 5)
#define Parse_VCSEC_TPMSConnectionTypeCurrent0(d) ((d & 0x60000000000000) >> 53)

enum VCSEC_TPMSConnectionTypeCurrent0_Values { 
  CONNECTIONTYPE_FAST = 0,
  CONNECTIONTYPE_SLOW = 1,
  CONNECTIONTYPE_UNKNOWN = 2
 };

#define Parse_VCSEC_TPMSConnectionTypeDesired0(d) ((d & 0x18000000000000) >> 51)

enum VCSEC_TPMSConnectionTypeDesired0_Values { 
  CONNECTIONTYPE_FAST = 0,
  CONNECTIONTYPE_SLOW = 1,
  CONNECTIONTYPE_UNKNOWN = 2
 };

#define Parse_VCSEC_TPMSSensorState1(d) ((d & 0xe000000000000) >> 49)

enum VCSEC_TPMSSensorState1_Values { 
  SENSOR_NOT_PAIRED = 0,
  SENSOR_WAIT_FOR_ADV = 1,
  SENSOR_WAIT_FOR_CONN = 2,
  SENSOR_CONNECTED = 3,
  SENSOR_DISCONNECTING = 4
 };

#define Parse_VCSEC_TPMSRSSI1(d) ((d & 0x3fc00000000000) >> 46)
#define Parse_VCSEC_TPMSConnectionTypeCurrent1(d) ((d & 0xc000000000) >> 38)

enum VCSEC_TPMSConnectionTypeCurrent1_Values { 
  CONNECTIONTYPE_FAST = 0,
  CONNECTIONTYPE_SLOW = 1,
  CONNECTIONTYPE_UNKNOWN = 2
 };

#define Parse_VCSEC_TPMSConnectionTypeDesired1(d) ((d & 0x3000000000) >> 36)

enum VCSEC_TPMSConnectionTypeDesired1_Values { 
  CONNECTIONTYPE_FAST = 0,
  CONNECTIONTYPE_SLOW = 1,
  CONNECTIONTYPE_UNKNOWN = 2
 };

#define Parse_VCSEC_TPMSSensorState2(d) ((d & 0x1c00000000) >> 34)

enum VCSEC_TPMSSensorState2_Values { 
  SENSOR_NOT_PAIRED = 0,
  SENSOR_WAIT_FOR_ADV = 1,
  SENSOR_WAIT_FOR_CONN = 2,
  SENSOR_CONNECTED = 3,
  SENSOR_DISCONNECTING = 4
 };

#define Parse_VCSEC_TPMSRSSI2(d) ((d & 0x7f80000000) >> 31)
#define Parse_VCSEC_TPMSConnectionTypeCurrent2(d) ((d & 0x1800000) >> 23)

enum VCSEC_TPMSConnectionTypeCurrent2_Values { 
  CONNECTIONTYPE_FAST = 0,
  CONNECTIONTYPE_SLOW = 1,
  CONNECTIONTYPE_UNKNOWN = 2
 };

#define Parse_VCSEC_TPMSConnectionTypeDesired2(d) ((d & 0x600000) >> 21)

enum VCSEC_TPMSConnectionTypeDesired2_Values { 
  CONNECTIONTYPE_FAST = 0,
  CONNECTIONTYPE_SLOW = 1,
  CONNECTIONTYPE_UNKNOWN = 2
 };

#define Parse_VCSEC_TPMSSensorState3(d) ((d & 0x380000) >> 19)

enum VCSEC_TPMSSensorState3_Values { 
  SENSOR_NOT_PAIRED = 0,
  SENSOR_WAIT_FOR_ADV = 1,
  SENSOR_WAIT_FOR_CONN = 2,
  SENSOR_CONNECTED = 3,
  SENSOR_DISCONNECTING = 4
 };

#define Parse_VCSEC_TPMSRSSI3(d) ((d & 0xff0000) >> 16)
#define Parse_VCSEC_TPMSConnectionTypeCurrent3(d) ((d & 0x300) >> 8)

enum VCSEC_TPMSConnectionTypeCurrent3_Values { 
  CONNECTIONTYPE_FAST = 0,
  CONNECTIONTYPE_SLOW = 1,
  CONNECTIONTYPE_UNKNOWN = 2
 };

#define Parse_VCSEC_TPMSConnectionTypeDesired3(d) ((d & 0xc0) >> 6)

enum VCSEC_TPMSConnectionTypeDesired3_Values { 
  CONNECTIONTYPE_FAST = 0,
  CONNECTIONTYPE_SLOW = 1,
  CONNECTIONTYPE_UNKNOWN = 2
 };

#define ID22EPARK_sdiRear 0x22e

#define Parse_PARK_sdiSensor7RawDistData(d) ((d & 0x3fe00) >> 9)

enum PARK_sdiSensor7RawDistData_Values { 
  BLOCKED = 0,
  NEAR_DETECTION = 1,
  NO_OBJECT_DETECTED = 500,
  SNA = 511
 };

#define Parse_PARK_sdiSensor8RawDistData(d) ((d & 0xff80000000000000) >> 55)

enum PARK_sdiSensor8RawDistData_Values { 
  BLOCKED = 0,
  NEAR_DETECTION = 1,
  NO_OBJECT_DETECTED = 500,
  SNA = 511
 };

#define Parse_PARK_sdiSensor9RawDistData(d) ((d & 0x7fc00000000000) >> 46)

enum PARK_sdiSensor9RawDistData_Values { 
  BLOCKED = 0,
  NEAR_DETECTION = 1,
  NO_OBJECT_DETECTED = 500,
  SNA = 511
 };

#define Parse_PARK_sdiSensor10RawDistData(d) ((d & 0x3fe000000000) >> 37)

enum PARK_sdiSensor10RawDistData_Values { 
  BLOCKED = 0,
  NEAR_DETECTION = 1,
  NO_OBJECT_DETECTED = 500,
  SNA = 511
 };

#define Parse_PARK_sdiSensor11RawDistData(d) ((d & 0x1ff0000000) >> 28)

enum PARK_sdiSensor11RawDistData_Values { 
  BLOCKED = 0,
  NEAR_DETECTION = 1,
  NO_OBJECT_DETECTED = 500,
  SNA = 511
 };

#define Parse_PARK_sdiSensor12RawDistData(d) ((d & 0xff80000) >> 19)

enum PARK_sdiSensor12RawDistData_Values { 
  BLOCKED = 0,
  NEAR_DETECTION = 1,
  NO_OBJECT_DETECTED = 500,
  SNA = 511
 };

#define Parse_PARK_sdiRearCounter(d) ((d & 0xc00) >> 10)
#define Parse_PARK_sdiRearChecksum(d) ((d & 0xff00) >> 8)
#define ID238UI_driverAssistMapData 0x238

#define Parse_UI_mapSpeedLimitDependency(d) ((d & 0x38) >> 3)

enum UI_mapSpeedLimitDependency_Values { 
  NONE = 0,
  SCHOOL = 1,
  RAIN = 2,
  SNOW = 3,
  TIME = 4,
  SEASON = 5,
  LANE = 6,
  SNA = 7
 };

#define Parse_UI_roadClass(d) ((d & 0xe000000000000000) >> 61)

enum UI_roadClass_Values { 
  UNKNOWN_INVALID_SNA = 0,
  CLASS_1_MAJOR = 1,
  CLASS_2 = 2,
  CLASS_3 = 3,
  CLASS_4 = 4,
  CLASS_5 = 5,
  CLASS_6_MINOR = 6
 };

#define Parse_UI_inSuperchargerGeofence(d) ((d & 0x400000000000000) >> 58)
#define Parse_UI_mapSpeedUnits(d) ((d & 0x200000000000000) >> 57)

enum UI_mapSpeedUnits_Values { 
  MPH = 0,
  KPH = 1
 };

#define Parse_UI_mapSpeedLimit(d) ((d & 0x1f00000000000000) >> 56)

enum UI_mapSpeedLimit_Values { 
  UNKNOWN = 0,
  LESS_OR_EQ_5 = 1,
  LESS_OR_EQ_7 = 2,
  LESS_OR_EQ_10 = 3,
  LESS_OR_EQ_15 = 4,
  LESS_OR_EQ_20 = 5,
  LESS_OR_EQ_25 = 6,
  LESS_OR_EQ_30 = 7,
  LESS_OR_EQ_35 = 8,
  LESS_OR_EQ_40 = 9,
  LESS_OR_EQ_45 = 10,
  LESS_OR_EQ_50 = 11,
  LESS_OR_EQ_55 = 12,
  LESS_OR_EQ_60 = 13,
  LESS_OR_EQ_65 = 14,
  LESS_OR_EQ_70 = 15,
  LESS_OR_EQ_75 = 16,
  LESS_OR_EQ_80 = 17,
  LESS_OR_EQ_85 = 18,
  LESS_OR_EQ_90 = 19,
  LESS_OR_EQ_95 = 20,
  LESS_OR_EQ_100 = 21,
  LESS_OR_EQ_105 = 22,
  LESS_OR_EQ_110 = 23,
  LESS_OR_EQ_115 = 24,
  LESS_OR_EQ_120 = 25,
  LESS_OR_EQ_130 = 26,
  LESS_OR_EQ_140 = 27,
  LESS_OR_EQ_150 = 28,
  LESS_OR_EQ_160 = 29,
  UNLIMITED = 30,
  SNA = 31
 };

#define Parse_UI_mapSpeedLimitType(d) ((d & 0x38000000000000) >> 51)

enum UI_mapSpeedLimitType_Values { 
  REGULAR = 1,
  ADVISORY = 2,
  DEPENDENT = 3,
  BUMPS = 4,
  UNKNOWN_SNA = 7
 };

#define Parse_UI_countryCode(d) ((d & 0x3ff000000000000) >> 48)

enum UI_countryCode_Values { 
  UNKNOWN = 0,
  SNA = 1023
 };

#define Parse_UI_streetCount(d) ((d & 0xc000000000) >> 38)
#define Parse_UI_gpsRoadMatch(d) ((d & 0x1000000000) >> 36)
#define Parse_UI_navRouteActive(d) ((d & 0x800000000) >> 35)
#define Parse_UI_parallelAutoparkEnabled(d) ((d & 0x400000000) >> 34)
#define Parse_UI_perpendicularAutoparkEnabled(d) ((d & 0x200000000) >> 33)
#define Parse_UI_nextBranchDist(d) ((d & 0x1f00000000) >> 32)

enum UI_nextBranchDist_Values { 
  SNA = 31
 };

#define Parse_UI_controlledAccess(d) ((d & 0x8000000) >> 27)
#define Parse_UI_nextBranchLeftOffRamp(d) ((d & 0x4000000) >> 26)
#define Parse_UI_nextBranchRightOffRamp(d) ((d & 0x2000000) >> 25)
#define Parse_UI_rejectLeftLane(d) ((d & 0x1000000) >> 24)
#define Parse_UI_rejectRightLane(d) ((d & 0x800000) >> 23)
#define Parse_UI_rejectHPP(d) ((d & 0x400000) >> 22)
#define Parse_UI_rejectNav(d) ((d & 0x200000) >> 21)
#define Parse_UI_rejectLeftFreeSpace(d) ((d & 0x100000) >> 20)
#define Parse_UI_rejectRightFreeSpace(d) ((d & 0x80000) >> 19)
#define Parse_UI_rejectAutosteer(d) ((d & 0x40000) >> 18)
#define Parse_UI_rejectHandsOn(d) ((d & 0x20000) >> 17)
#define Parse_UI_acceptBottsDots(d) ((d & 0x10000) >> 16)
#define Parse_UI_autosteerRestricted(d) ((d & 0x8000) >> 15)
#define Parse_UI_pmmEnabled(d) ((d & 0x4000) >> 14)
#define Parse_UI_scaEnabled(d) ((d & 0x2000) >> 13)
#define Parse_UI_mapDataCounter(d) ((d & 0xf000) >> 12)
#define Parse_UI_mapDataChecksum(d) ((d & 0xff00) >> 8)
#define ID239DAS_lanes 0x239

#define Parse_DAS_leftLaneExists(d) ((d & 0x2) >> 1)
#define Parse_DAS_rightLaneExists(d) ((d & 0x8000000000000000) >> 63)
#define Parse_DAS_virtualLaneWidth(d) ((d & 0xf000000000000000) >> 60)
#define Parse_DAS_virtualLaneViewRange(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DAS_virtualLaneC0(d) ((d & 0xff000000000000) >> 48)
#define Parse_DAS_virtualLaneC1(d) ((d & 0xff0000000000) >> 40)
#define Parse_DAS_virtualLaneC2(d) ((d & 0xff00000000) >> 32)
#define Parse_DAS_virtualLaneC3(d) ((d & 0xff000000) >> 24)
#define Parse_DAS_leftLineUsage(d) ((d & 0x30000) >> 16)

enum DAS_leftLineUsage_Values { 
  REJECTED_UNAVAILABLE = 0,
  AVAILABLE = 1,
  FUSED = 2,
  BLACKLISTED = 3
 };

#define Parse_DAS_rightLineUsage(d) ((d & 0xc000) >> 14)

enum DAS_rightLineUsage_Values { 
  REJECTED_UNAVAILABLE = 0,
  AVAILABLE = 1,
  FUSED = 2,
  BLACKLISTED = 3
 };

#define Parse_DAS_leftFork(d) ((d & 0x3000) >> 12)

enum DAS_leftFork_Values { 
  LEFT_FORK_NONE = 0,
  LEFT_FORK_AVAILABLE = 1,
  LEFT_FORK_SELECTED = 2,
  LEFT_FORK_UNAVAILABLE = 3
 };

#define Parse_DAS_rightFork(d) ((d & 0xc00) >> 10)

enum DAS_rightFork_Values { 
  RIGHT_FORK_NONE = 0,
  RIGHT_FORK_AVAILABLE = 1,
  RIGHT_FORK_SELECTED = 2,
  RIGHT_FORK_UNAVAILABLE = 3
 };

#define Parse_DAS_lanesCounter(d) ((d & 0xf0) >> 4)
#define ID24ADAS_visualDebug 0x24a

#define Parse_DAS_autosteerVehiclesUsage(d) ((d & 0x3000000000000000) >> 60)

enum DAS_autosteerVehiclesUsage_Values { 
  REJECTED_UNAVAILABLE = 0,
  AVAILABLE = 1,
  FUSED = 2,
  BLACKLISTED = 3
 };

#define Parse_DAS_autosteerHPPUsage(d) ((d & 0xc00000000000000) >> 58)

enum DAS_autosteerHPPUsage_Values { 
  REJECTED_UNAVAILABLE = 0,
  AVAILABLE = 1,
  FUSED = 2,
  BLACKLISTED = 3
 };

#define Parse_DAS_autosteerNavigationUsage(d) ((d & 0x300000000000000) >> 56)

enum DAS_autosteerNavigationUsage_Values { 
  REJECTED_UNAVAILABLE = 0,
  AVAILABLE = 1,
  FUSED = 2,
  BLACKLISTED = 3
 };

#define Parse_DAS_autosteerModelUsage(d) ((d & 0xc0000000000000) >> 54)

enum DAS_autosteerModelUsage_Values { 
  REJECTED_UNAVAILABLE = 0,
  AVAILABLE = 1,
  FUSED = 2,
  BLACKLISTED = 3
 };

#define Parse_DAS_autosteerBottsDotsUsage(d) ((d & 0x30000000000000) >> 52)

enum DAS_autosteerBottsDotsUsage_Values { 
  REJECTED_UNAVAILABLE = 0,
  AVAILABLE = 1,
  FUSED = 2,
  BLACKLISTED = 3
 };

#define Parse_DAS_offsetSide(d) ((d & 0xc000000000000) >> 50)

enum DAS_offsetSide_Values { 
  NO_OFFSET = 0,
  OFFSET_RIGHT_OBJECT = 1,
  OFFSET_LEFT_OBJECT = 2,
  OFFSET_BOTH_OBJECTS = 3
 };

#define Parse_DAS_roadSurfaceType(d) ((d & 0x3000000000000) >> 48)

enum DAS_roadSurfaceType_Values { 
  ROAD_SURFACE_SNA = 0,
  ROAD_SURFACE_NORMAL = 1,
  ROAD_SURFACE_ENHANCED = 2
 };

#define Parse_DAS_autosteerHealthAnomalyLevel(d) ((d & 0x1c00000000000) >> 46)
#define Parse_DAS_autosteerHealthState(d) ((d & 0x380000000000) >> 43)

enum DAS_autosteerHealthState_Values { 
  HEALTH_UNAVAILABLE = 0,
  HEALTH_NOMINAL = 1,
  HEALTH_DEGRADED = 2,
  HEALTH_SEVERELY_DEGRADED = 3,
  HEALTH_ABORTING = 4,
  HEALTH_FAULT = 5
 };

#define Parse_DAS_lastLinePreferenceReason(d) ((d & 0xf0000000000) >> 40)

enum DAS_lastLinePreferenceReason_Values { 
  OTHER_LANE_DISAGREES_WITH_MODEL = 0,
  AGREEMENT_WITH_NEIGHBOR_LANES = 1,
  NEIGHBOR_LANE_PROBABILIY = 2,
  NAVIGATION_BRANCH = 3,
  AVOID_ONCOMING_LANES = 4,
  COUNTRY_DRIVING_SIDE = 5,
  NONE = 15
 };

#define Parse_DAS_plannerState(d) ((d & 0xf000000000) >> 36)

enum DAS_plannerState_Values { 
  TP_EXTSTATE_DISABLED = 0,
  TP_EXTSTATE_VL = 1,
  TP_EXTSTATE_FOLLOW = 2,
  TP_EXTSTATE_LANECHANGE_REQUESTED = 3,
  TP_EXTSTATE_LANECHANGE_IN_PROGRESS = 4,
  TP_EXTSTATE_LANECHANGE_WAIT_FOR_SIDE_OBSTACLE = 5,
  TP_EXTSTATE_LANECHANGE_WAIT_FOR_FWD_OBSTACLE = 6,
  TP_EXTSTATE_LANECHANGE_ABORT = 7
 };

#define Parse_DAS_lastAutosteerAbortReason(d) ((d & 0x3f00000000) >> 32)

enum DAS_lastAutosteerAbortReason_Values { 
  UI_ABORT_REASON_HM_LANE_VIEW_RANGE = 0,
  UI_ABORT_REASON_HM_VIRTUAL_LANE_NO_INPUTS = 1,
  UI_ABORT_REASON_HM_STEERING_ERROR = 2,
  UI_ABORT_REASON_APP_ME_STATE_NOT_VISION = 14,
  UI_ABORT_REASON_ME_MAIN_STATE_NOT_VISION = 15,
  UI_ABORT_REASON_CAM_MSG_MIA = 16,
  UI_ABORT_REASON_CAM_WATCHDOG = 17,
  UI_ABORT_REASON_TRAILER_MODE = 18,
  UI_ABORT_REASON_SIDE_COLLISION_IMMINENT = 19,
  UI_ABORT_REASON_EPAS_EAC_DENIED = 20,
  UI_ABORT_REASON_COMPONENT_MIA = 21,
  UI_ABORT_REASON_CRUISE_FAULT = 22,
  UI_ABORT_REASON_CID_SWITCH_DISABLED = 23,
  UI_ABORT_REASON_DRIVING_OFF_NAV = 24,
  UI_ABORT_REASON_VEHICLE_SPEED_ABOVE_MAX = 25,
  UI_ABORT_REASON_FOLLOWER_OUTPUT_INVALID = 26,
  UI_ABORT_REASON_PLANNER_OUTPUT_INVALID = 27,
  UI_ABORT_REASON_EPAS_ERROR_CODE = 28,
  UI_ABORT_REASON_ACC_CANCEL = 29,
  UI_ABORT_REASON_CAMERA_FAILSAFES = 30,
  UI_ABORT_REASON_NO_ABORT = 31,
  UI_ABORT_REASON_AEB = 32,
  UI_ABORT_REASON_SEATBELT_UNBUCKLED = 33,
  UI_ABORT_REASON_USER_OVERRIDE_STRIKEOUT = 34
 };

#define Parse_DAS_devAppInterfaceEnabled(d) ((d & 0x4000000) >> 26)
#define Parse_DAS_navAvailable(d) ((d & 0x2000000) >> 25)

enum DAS_navAvailable_Values { 
  DAS_NAV_UNAVAILABLE = 0,
  DAS_NAV_AVAILABLE = 1
 };

#define Parse_DAS_navDistance(d) ((d & 0xff000000) >> 24)
#define Parse_DAS_accSmartSpeedActive(d) ((d & 0x10000) >> 16)
#define Parse_DAS_accSmartSpeedState(d) ((d & 0x38000) >> 15)

enum DAS_accSmartSpeedState_Values { 
  NOT_ACTIVE = 0,
  ACTIVE_OFFRAMP = 1,
  ACTIVE_INTEGRATING = 2,
  ACTIVE_ONRAMP = 3,
  SET_SPEED_SET_REQUESTED = 4,
  OFFRAMP_DELAY = 5,
  SNA = 7
 };

#define Parse_DAS_ulcInProgress(d) ((d & 0x1000) >> 12)

enum DAS_ulcInProgress_Values { 
  ULC_INACTIVE = 0,
  ULC_ACTIVE = 1
 };

#define Parse_DAS_behaviorType(d) ((d & 0x300) >> 8)

enum DAS_behaviorType_Values { 
  DAS_BEHAVIOR_INVALID = 0,
  DAS_BEHAVIOR_IN_LANE = 1,
  DAS_BEHAVIOR_LANE_CHANGE_LEFT = 2,
  DAS_BEHAVIOR_LANE_CHANGE_RIGHT = 3
 };

#define Parse_DAS_ulcType(d) ((d & 0xc0) >> 6)

enum DAS_ulcType_Values { 
  ULC_TYPE_NONE = 0,
  ULC_TYPE_NAV = 1,
  ULC_TYPE_SPEED = 2
 };

#define Parse_DAS_rearVehDetectedThisCycle(d) ((d & 0x10) >> 4)

enum DAS_rearVehDetectedThisCycle_Values { 
  VEHICLE_NOT_DETECTED = 0,
  VEHICLE_DETECTED = 1
 };

#define Parse_DAS_rearLeftVehDetectedCurrent(d) ((d & 0x8) >> 3)

enum DAS_rearLeftVehDetectedCurrent_Values { 
  VEHICLE_NOT_DETECTED = 0,
  VEHICLE_DETECTED = 1
 };

#define Parse_DAS_rearRightVehDetectedTrip(d) ((d & 0x4) >> 2)

enum DAS_rearRightVehDetectedTrip_Values { 
  VEHICLE_NOT_DETECTED = 0,
  VEHICLE_DETECTED = 1
 };

#define Parse_DAS_rearLeftVehDetectedTrip(d) ((d & 0x2) >> 1)

enum DAS_rearLeftVehDetectedTrip_Values { 
  VEHICLE_NOT_DETECTED = 0,
  VEHICLE_DETECTED = 1
 };

#define ID25BAPP_environment 0x25b

#define Parse_APP_environmentRainy(d) ((d & 0x2) >> 1)
#define Parse_APP_environmentSnowy(d) ((d & 0x80) >> 7)
#define ID25DCP_status 0x25d

#define Parse_CP_type(d) ((d & 0xc) >> 2)

enum CP_type_Values { 
  CP_TYPE_US_TESLA = 0,
  CP_TYPE_EURO_IEC = 1,
  CP_TYPE_GB = 2,
  CP_TYPE_IEC_CCS = 3
 };

#define Parse_CP_insertEnableLine(d) ((d & 0x4000000000000000) >> 62)
#define Parse_CP_chargeCablePresent(d) ((d & 0x2000000000000000) >> 61)

enum CP_chargeCablePresent_Values { 
  CABLE_NOT_PRESENT = 0,
  CABLE_PRESENT = 1
 };

#define Parse_CP_chargeCableSecured(d) ((d & 0x1000000000000000) >> 60)
#define Parse_CP_latchState(d) ((d & 0x1800000000000000) >> 59)

enum CP_latchState_Values { 
  CP_LATCH_SNA = 0,
  CP_LATCH_DISENGAGED = 1,
  CP_LATCH_ENGAGED = 2,
  CP_LATCH_BLOCKING = 3
 };

#define Parse_CP_permanentPowerRequest(d) ((d & 0x200000000000000) >> 57)
#define Parse_CP_latch2State(d) ((d & 0x300000000000000) >> 56)

enum CP_latch2State_Values { 
  CP_LATCH_SNA = 0,
  CP_LATCH_DISENGAGED = 1,
  CP_LATCH_ENGAGED = 2,
  CP_LATCH_BLOCKING = 3
 };

#define Parse_CP_chargeDoorOpen(d) ((d & 0x40000000000000) >> 54)
#define Parse_CP_doorControlState(d) ((d & 0xe0000000000000) >> 53)

enum CP_doorControlState_Values { 
  CP_doorInit = 0,
  CP_doorIdle = 1,
  CP_doorOpenRequested = 2,
  CP_doorOpening = 3,
  CP_doorSenseOpen = 4,
  CP_doorClosing = 5,
  CP_doorSenseClosed = 6
 };

#define Parse_CP_chargeCableState(d) ((d & 0xc000000000000) >> 50)

enum CP_chargeCableState_Values { 
  CHARGE_CABLE_UNKNOWN_SNA = 0,
  CHARGE_CABLE_NOT_CONNECTED = 1,
  CHARGE_CABLE_CONNECTED = 2
 };

#define Parse_CP_latchControlState(d) ((d & 0x7000000000000) >> 48)

enum CP_latchControlState_Values { 
  CP_latchInit = 0,
  CP_latchIdle = 1,
  CP_latchDisengageRequested = 2,
  CP_latchDisengaging = 3,
  CP_latchDisengaged = 4,
  CP_latchEngaging = 5
 };

#define Parse_CP_latch2ControlState(d) ((d & 0xe00000000000) >> 45)

enum CP_latch2ControlState_Values { 
  CP_latchInit = 0,
  CP_latchIdle = 1,
  CP_latchDisengageRequested = 2,
  CP_latchDisengaging = 3,
  CP_latchDisengaged = 4,
  CP_latchEngaging = 5
 };

#define Parse_CP_apsVoltage(d) ((d & 0xff0000000000) >> 40)
#define Parse_CP_doorButtonPressed(d) ((d & 0x100000000) >> 32)
#define Parse_CP_ledColor(d) ((d & 0x780000000) >> 31)

enum CP_ledColor_Values { 
  CP_LEDS_OFF = 0,
  CP_LEDS_RED = 1,
  CP_LEDS_GREEN = 2,
  CP_LEDS_BLUE = 3,
  CP_LEDS_WHITE = 4,
  CP_LEDS_FLASHING_GREEN = 5,
  CP_LEDS_FLASHING_AMBER = 6,
  CP_LEDS_AMBER = 7,
  CP_LEDS_RAVE = 8,
  CP_LEDS_DEBUG = 9,
  CP_LEDS_FLASHING_BLUE = 10
 };

#define Parse_CP_swcanRelayClosed(d) ((d & 0x8000000) >> 27)
#define Parse_CP_UHF_controlState(d) ((d & 0x3c000000) >> 26)

enum CP_UHF_controlState_Values { 
  CP_UHF_INIT = 0,
  CP_UHF_CONFIG = 1,
  CP_UHF_IDLE = 2,
  CP_UHF_CALIBRATE = 3,
  CP_UHF_PREPARE_RX = 4,
  CP_UHF_RX = 5,
  CP_UHF_CHECK_RX = 6,
  CP_UHF_READ_RXFIFO = 7,
  CP_UHF_HANDLE_FOUND = 8,
  CP_UHF_SLEEP = 9,
  CP_UHF_FAULT = 10
 };

#define Parse_CP_UHF_handleFound(d) ((d & 0x400000) >> 22)
#define Parse_CP_doorOpenRequested(d) ((d & 0x200000) >> 21)
#define Parse_CP_faultLineSensed(d) ((d & 0x100000) >> 20)

enum CP_faultLineSensed_Values { 
  FAULT_LINE_CLEARED = 0,
  FAULT_LINE_SET = 1
 };

#define Parse_CP_inductiveDoorState(d) ((d & 0x380000) >> 19)

enum CP_inductiveDoorState_Values { 
  CP_INDUCTIVE_DOOR_INIT = 0,
  CP_INDUCTIVE_DOOR_INIT_FROM_CHARGE = 1,
  CP_INDUCTIVE_DOOR_INIT_FROM_DRIVE = 2,
  CP_INDUCTIVE_DOOR_PRESENT = 3,
  CP_INDUCTIVE_DOOR_NOT_PRESENT = 4,
  CP_INDUCTIVE_DOOR_OFF_DRIVE = 5,
  CP_INDUCTIVE_DOOR_OFF_CHARGE = 6,
  CP_INDUCTIVE_DOOR_FAULT = 7
 };

#define Parse_CP_inductiveSensorState(d) ((d & 0x70000) >> 16)

enum CP_inductiveSensorState_Values { 
  CP_INDUCTIVE_SENSOR_INIT = 0,
  CP_INDUCTIVE_SENSOR_POLL = 1,
  CP_INDUCTIVE_SENSOR_SHUTDOWN = 2,
  CP_INDUCTIVE_SENSOR_PAUSE = 3,
  CP_INDUCTIVE_SENSOR_WAIT_FOR_INIT = 4,
  CP_INDUCTIVE_SENSOR_FAULT = 5,
  CP_INDUCTIVE_SENSOR_RESET = 6,
  CP_INDUCTIVE_SENSOR_CONFIG = 7
 };

#define Parse_CP_chargeDoorOpenUI(d) ((d & 0x2000) >> 13)
#define Parse_CP_vehicleUnlockRequest(d) ((d & 0x1000) >> 12)
#define Parse_CP_numAlertsSet(d) ((d & 0x3f800) >> 11)
#define Parse_CP_coldWeatherMode(d) ((d & 0x10) >> 4)

enum CP_coldWeatherMode_Values { 
  CP_COLD_WEATHER_NONE = 0,
  CP_COLD_WEATHER_LATCH_MITIGATION = 1
 };

#define Parse_CP_hvInletExposed(d) ((d & 0x8) >> 3)
#define Parse_CP_latchEngaged(d) ((d & 0x4) >> 2)
#define Parse_CP_coverClosed(d) ((d & 0x2) >> 1)
#define ID29DCP_dcChargeStatus 0x29d

#define Parse_CP_evseOutputDcCurrent(d) ((d & 0x3fff8000) >> 15)
#define Parse_CP_evseOutputDcVoltage(d) ((d & 0x1fff0000) >> 16)
#define Parse_CP_evseOutputDcCurrentStale(d) ((d & 0x8) >> 3)
#define ID2B4PCS_dcdcRailStatus 0x2b4

#define Parse_PCS_dcdcLvBusVolt(d) ((d & 0xffc00) >> 10)
#define Parse_PCS_dcdcHvBusVolt(d) ((d & 0x3ffc) >> 2)
#define Parse_PCS_dcdcLvOutputCurrent(d) ((d & 0xfff0000) >> 16)
#define ID2B9DAS_control 0x2b9

#define Parse_DAS_setSpeed(d) ((d & 0xfff000) >> 12)

enum DAS_setSpeed_Values { 
  SNA = 4095
 };

#define Parse_DAS_accState(d) ((d & 0xf0000000000000) >> 52)

enum DAS_accState_Values { 
  ACC_CANCEL_GENERIC = 0,
  ACC_CANCEL_CAMERA_BLIND = 1,
  ACC_CANCEL_RADAR_BLIND = 2,
  ACC_HOLD = 3,
  ACC_ON = 4,
  APC_BACKWARD = 5,
  APC_FORWARD = 6,
  APC_COMPLETE = 7,
  APC_ABORT = 8,
  APC_PAUSE = 9,
  APC_UNPARK_COMPLETE = 10,
  APC_SELFPARK_START = 11,
  ACC_CANCEL_PATH_NOT_CLEAR = 12,
  ACC_CANCEL_GENERIC_SILENT = 13,
  ACC_CANCEL_OUT_OF_CALIBRATION = 14,
  FAULT_SNA = 15
 };

#define Parse_DAS_aebEvent(d) ((d & 0x3000000000000) >> 48)

enum DAS_aebEvent_Values { 
  AEB_NOT_ACTIVE = 0,
  AEB_ACTIVE = 1,
  AEB_FAULT = 2,
  AEB_SNA = 3
 };

#define Parse_DAS_jerkMin(d) ((d & 0x7fc00000000000) >> 46)

enum DAS_jerkMin_Values { 
  SNA = 511
 };

#define Parse_DAS_jerkMax(d) ((d & 0x1fe000000000) >> 37)

enum DAS_jerkMax_Values { 
  SNA = 255
 };

#define Parse_DAS_accelMin(d) ((d & 0x3fe0000000) >> 29)

enum DAS_accelMin_Values { 
  SNA = 511
 };

#define Parse_DAS_accelMax(d) ((d & 0x1ff00000) >> 20)

enum DAS_accelMax_Values { 
  SNA = 511
 };

#define Parse_DAS_controlCounter(d) ((d & 0x3800) >> 11)
#define Parse_DAS_controlChecksum(d) ((d & 0xff00) >> 8)
#define ID2D3UI_solarData 0x2d3

#define Parse_UI_solarAzimuthAngle(d) ((d & 0xffff0000) >> 16)

enum UI_solarAzimuthAngle_Values { 
  SNA = 32768
 };

#define Parse_UI_solarAzimuthAngleCarRef(d) ((d & 0x1ff000000000000) >> 48)

enum UI_solarAzimuthAngleCarRef_Values { 
  SNA = 255
 };

#define Parse_UI_isSunUp(d) ((d & 0x18000000000) >> 39)

enum UI_isSunUp_Values { 
  SUN_DOWN = 0,
  SUN_UP = 1,
  SUN_SNA = 3
 };

#define Parse_UI_solarElevationAngle(d) ((d & 0xff00000000) >> 32)

enum UI_solarElevationAngle_Values { 
  SNA = 127
 };

#define Parse_UI_screenPCBTemperature(d) ((d & 0xff000000) >> 24)
#define Parse_UI_minsToSunset(d) ((d & 0xff0000) >> 16)
#define Parse_UI_minsToSunrise(d) ((d & 0xff00) >> 8)
#define ID309DAS_object 0x309

#define Parse_DAS_objectId(d) ((d & 0x38) >> 3)

enum DAS_objectId_Values { 
  LEAD_VEHICLES = 0,
  LEFT_VEHICLES = 1,
  RIGHT_VEHICLES = 2,
  CUTIN_VEHICLE = 3,
  ROAD_SIGN = 4,
  VEHICLE_HEADINGS = 5
 };

#define Parse_DAS_cutinVehType(d) ((d & 0xe000000000000000) >> 61)

enum DAS_cutinVehType_Values { 
  UNKNOWN = 0,
  TRUCK = 1,
  CAR = 2,
  MOTORCYCLE = 3,
  BICYCLE = 4,
  PEDESTRIAN = 5
 };

#define Parse_DAS_leadVehType(d) ((d & 0xe000000000000000) >> 61)

enum DAS_leadVehType_Values { 
  UNKNOWN = 0,
  TRUCK = 1,
  CAR = 2,
  MOTORCYCLE = 3,
  BICYCLE = 4,
  PEDESTRIAN = 5,
  IPSO = 6
 };

#define Parse_DAS_leftVehType(d) ((d & 0xe000000000000000) >> 61)

enum DAS_leftVehType_Values { 
  UNKNOWN = 0,
  TRUCK = 1,
  CAR = 2,
  MOTORCYCLE = 3,
  BICYCLE = 4,
  PEDESTRIAN = 5
 };

#define Parse_DAS_rightVehType(d) ((d & 0xe000000000000000) >> 61)

enum DAS_rightVehType_Values { 
  UNKNOWN = 0,
  TRUCK = 1,
  CAR = 2,
  MOTORCYCLE = 3,
  BICYCLE = 4,
  PEDESTRIAN = 5
 };

#define Parse_DAS_roadSignColor(d) ((d & 0xe000000000000000) >> 61)

enum DAS_roadSignColor_Values { 
  NONE = 0,
  RED = 1,
  YELLOW = 2,
  GREEN = 3,
  RED_YELLOW = 4
 };

#define Parse_DAS_roadSignId(d) ((d & 0x3fc) >> 2)

enum DAS_roadSignId_Values { 
  STOP_SIGN = 0,
  TRAFFIC_LIGHT = 1,
  SNA = 255
 };

#define Parse_DAS_cutinVehRelevantForControl(d) ((d & 0x200000000000000) >> 57)
#define Parse_DAS_leadVehRelevantForControl(d) ((d & 0x200000000000000) >> 57)
#define Parse_DAS_leftVehRelevantForControl(d) ((d & 0x200000000000000) >> 57)
#define Parse_DAS_rightVehRelevantForControl(d) ((d & 0x200000000000000) >> 57)
#define Parse_DAS_cutinVehDx(d) ((d & 0xff00000000000000) >> 56)

enum DAS_cutinVehDx_Values { 
  SNA = 255
 };

#define Parse_DAS_leadVehDx(d) ((d & 0xff00000000000000) >> 56)

enum DAS_leadVehDx_Values { 
  SNA = 255
 };

#define Parse_DAS_leadVehHeading(d) ((d & 0xff00000000000000) >> 56)

enum DAS_leadVehHeading_Values { 
  SNA = 255
 };

#define Parse_DAS_leftVehDx(d) ((d & 0xff00000000000000) >> 56)

enum DAS_leftVehDx_Values { 
  SNA = 255
 };

#define Parse_DAS_rightVehDx(d) ((d & 0xff00000000000000) >> 56)

enum DAS_rightVehDx_Values { 
  SNA = 255
 };

#define Parse_DAS_roadSignStopLineDist(d) ((d & 0xffc000000000000) >> 50)

enum DAS_roadSignStopLineDist_Values { 
  SNA = 1023
 };

#define Parse_DAS_cutinVehVxRel(d) ((d & 0xf000000000000) >> 48)

enum DAS_cutinVehVxRel_Values { 
  SNA = 15
 };

#define Parse_DAS_leadVeh2Heading(d) ((d & 0xff000000000000) >> 48)

enum DAS_leadVeh2Heading_Values { 
  SNA = 255
 };

#define Parse_DAS_leadVehVxRel(d) ((d & 0xf000000000000) >> 48)

enum DAS_leadVehVxRel_Values { 
  SNA = 15
 };

#define Parse_DAS_leftVehVxRel(d) ((d & 0xf000000000000) >> 48)

enum DAS_leftVehVxRel_Values { 
  SNA = 15
 };

#define Parse_DAS_rightVehVxRel(d) ((d & 0xf000000000000) >> 48)

enum DAS_rightVehVxRel_Values { 
  SNA = 15
 };

#define Parse_DAS_cutinVehDy(d) ((d & 0x7f00000000000) >> 44)
#define Parse_DAS_leadVehDy(d) ((d & 0x7f00000000000) >> 44)
#define Parse_DAS_leftVehDy(d) ((d & 0x7f00000000000) >> 44)
#define Parse_DAS_rightVehDy(d) ((d & 0x7f00000000000) >> 44)
#define Parse_DAS_leftVehHeading(d) ((d & 0xff0000000000) >> 40)

enum DAS_leftVehHeading_Values { 
  SNA = 255
 };

#define Parse_DAS_roadSignControlActive(d) ((d & 0x10000000000) >> 40)
#define Parse_DAS_roadSignSource(d) ((d & 0x18000000000) >> 39)

enum DAS_roadSignSource_Values { 
  NONE = 0,
  NAV = 1,
  VISION = 2
 };

#define Parse_DAS_cutinVehId(d) ((d & 0xfe000000000) >> 37)

enum DAS_cutinVehId_Values { 
  SNA = 127
 };

#define Parse_DAS_leadVehId(d) ((d & 0xfe000000000) >> 37)

enum DAS_leadVehId_Values { 
  SNA = 127
 };

#define Parse_DAS_leftVehId(d) ((d & 0xfe000000000) >> 37)

enum DAS_leftVehId_Values { 
  SNA = 127
 };

#define Parse_DAS_rightVehId(d) ((d & 0xfe000000000) >> 37)

enum DAS_rightVehId_Values { 
  SNA = 127
 };

#define Parse_DAS_roadSignArrow(d) ((d & 0xe000000000) >> 37)

enum DAS_roadSignArrow_Values { 
  CIRCLE = 0,
  LEFT = 1,
  RIGHT = 2,
  STRAIGHT = 3,
  UNKNOWN = 4
 };

#define Parse_DAS_roadSignOrientation(d) ((d & 0xc00000000) >> 34)

enum DAS_roadSignOrientation_Values { 
  UNKNOWN = 0,
  VERTICAL_3_LIGHT = 1,
  HORIZONTAL_3_LIGHT = 2
 };

#define Parse_DAS_leftVeh2Heading(d) ((d & 0xff00000000) >> 32)

enum DAS_leftVeh2Heading_Values { 
  SNA = 255
 };

#define Parse_DAS_leadVeh2Type(d) ((d & 0x1c0000000) >> 30)

enum DAS_leadVeh2Type_Values { 
  UNKNOWN = 0,
  TRUCK = 1,
  CAR = 2,
  MOTORCYCLE = 3,
  BICYCLE = 4,
  PEDESTRIAN = 5
 };

#define Parse_DAS_leftVeh2Type(d) ((d & 0x1c0000000) >> 30)

enum DAS_leftVeh2Type_Values { 
  UNKNOWN = 0,
  TRUCK = 1,
  CAR = 2,
  MOTORCYCLE = 3,
  BICYCLE = 4,
  PEDESTRIAN = 5
 };

#define Parse_DAS_rightVeh2Type(d) ((d & 0x1c0000000) >> 30)

enum DAS_rightVeh2Type_Values { 
  UNKNOWN = 0,
  TRUCK = 1,
  CAR = 2,
  MOTORCYCLE = 3,
  BICYCLE = 4,
  PEDESTRIAN = 5
 };

#define Parse_DAS_leadVeh2RelevantForControl(d) ((d & 0x4000000) >> 26)
#define Parse_DAS_leftVeh2RelevantForControl(d) ((d & 0x4000000) >> 26)
#define Parse_DAS_rightVeh2RelevantForControl(d) ((d & 0x4000000) >> 26)
#define Parse_DAS_leadVeh2Dx(d) ((d & 0x1fe000000) >> 25)

enum DAS_leadVeh2Dx_Values { 
  SNA = 255
 };

#define Parse_DAS_leftVeh2Dx(d) ((d & 0x1fe000000) >> 25)

enum DAS_leftVeh2Dx_Values { 
  SNA = 255
 };

#define Parse_DAS_rightVeh2Dx(d) ((d & 0x1fe000000) >> 25)

enum DAS_rightVeh2Dx_Values { 
  SNA = 255
 };

#define Parse_DAS_rightVehHeading(d) ((d & 0xff000000) >> 24)

enum DAS_rightVehHeading_Values { 
  SNA = 255
 };

#define Parse_DAS_leadVeh2VxRel(d) ((d & 0x1e0000) >> 17)

enum DAS_leadVeh2VxRel_Values { 
  SNA = 15
 };

#define Parse_DAS_leftVeh2VxRel(d) ((d & 0x1e0000) >> 17)

enum DAS_leftVeh2VxRel_Values { 
  SNA = 15
 };

#define Parse_DAS_rightVeh2VxRel(d) ((d & 0x1e0000) >> 17)

enum DAS_rightVeh2VxRel_Values { 
  SNA = 15
 };

#define Parse_DAS_rightVeh2Heading(d) ((d & 0xff0000) >> 16)

enum DAS_rightVeh2Heading_Values { 
  SNA = 255
 };

#define Parse_DAS_leadVeh2Dy(d) ((d & 0xfe000) >> 13)
#define Parse_DAS_leftVeh2Dy(d) ((d & 0xfe000) >> 13)
#define Parse_DAS_rightVeh2Dy(d) ((d & 0xfe000) >> 13)
#define Parse_DAS_cutinVehHeading(d) ((d & 0xff00) >> 8)

enum DAS_cutinVehHeading_Values { 
  SNA = 255
 };

#define Parse_DAS_leadVeh2Id(d) ((d & 0xfc0) >> 6)

enum DAS_leadVeh2Id_Values { 
  SNA = 0
 };

#define Parse_DAS_leftVeh2Id(d) ((d & 0xfc0) >> 6)

enum DAS_leftVeh2Id_Values { 
  SNA = 0
 };

#define Parse_DAS_rightVeh2Id(d) ((d & 0xfc0) >> 6)

enum DAS_rightVeh2Id_Values { 
  SNA = 0
 };

#define ID389DAS_status2 0x389

#define Parse_DAS_accSpeedLimit(d) ((d & 0xffc00) >> 10)

enum DAS_accSpeedLimit_Values { 
  NONE = 0,
  SNA = 1023
 };

#define Parse_DAS_pmmObstacleSeverity(d) ((d & 0x1c0000000000000) >> 54)

enum DAS_pmmObstacleSeverity_Values { 
  PMM_NONE = 0,
  PMM_IMMINENT_REAR = 1,
  PMM_IMMINENT_FRONT = 2,
  PMM_BRAKE_REQUEST = 3,
  PMM_CRASH_REAR = 4,
  PMM_CRASH_FRONT = 5,
  PMM_ACCEL_LIMIT = 6,
  PMM_SNA = 7
 };

#define Parse_DAS_pmmLoggingRequest(d) ((d & 0x8000000000000) >> 51)

enum DAS_pmmLoggingRequest_Values { 
  FALSE = 0,
  TRUE = 1
 };

#define Parse_DAS_activationFailureStatus(d) ((d & 0xc000000000000) >> 50)

enum DAS_activationFailureStatus_Values { 
  LC_ACTIVATION_IDLE = 0,
  LC_ACTIVATION_FAILED_1 = 1,
  LC_ACTIVATION_FAILED_2 = 2
 };

#define Parse_DAS_pmmUltrasonicsFaultReason(d) ((d & 0x7000000000000) >> 48)

enum DAS_pmmUltrasonicsFaultReason_Values { 
  PMM_ULTRASONICS_NO_FAULT = 0,
  PMM_ULTRASONICS_BLOCKED_FRONT = 1,
  PMM_ULTRASONICS_BLOCKED_REAR = 2,
  PMM_ULTRASONICS_BLOCKED_BOTH = 3,
  PMM_ULTRASONICS_INVALID_MIA = 4
 };

#define Parse_DAS_pmmRadarFaultReason(d) ((d & 0x600000000000) >> 45)

enum DAS_pmmRadarFaultReason_Values { 
  PMM_RADAR_NO_FAULT = 0,
  PMM_RADAR_BLOCKED_FRONT = 1,
  PMM_RADAR_INVALID_MIA = 2
 };

#define Parse_DAS_pmmSysFaultReason(d) ((d & 0x380000000000) >> 43)

enum DAS_pmmSysFaultReason_Values { 
  PMM_FAULT_NONE = 0,
  PMM_FAULT_DAS_DISABLED = 1,
  PMM_FAULT_SPEED = 2,
  PMM_FAULT_DI_FAULT = 3,
  PMM_FAULT_STEERING_ANGLE_RATE = 4,
  PMM_FAULT_DISABLED_BY_USER = 5,
  PMM_FAULT_ROAD_TYPE = 6,
  PMM_FAULT_BRAKE_PEDAL_INHIBIT = 7
 };

#define Parse_DAS_pmmCameraFaultReason(d) ((d & 0x30000000000) >> 40)

enum DAS_pmmCameraFaultReason_Values { 
  PMM_CAMERA_NO_FAULT = 0,
  PMM_CAMERA_BLOCKED_FRONT = 1,
  PMM_CAMERA_INVALID_MIA = 2
 };

#define Parse_DAS_ACC_report(d) ((d & 0x7c000000000) >> 38)

enum DAS_ACC_report_Values { 
  ACC_REPORT_TARGET_NONE = 0,
  ACC_REPORT_TARGET_CIPV = 1,
  ACC_REPORT_TARGET_IN_FRONT_OF_CIPV = 2,
  ACC_REPORT_TARGET_MCVL = 3,
  ACC_REPORT_TARGET_MCVR = 4,
  ACC_REPORT_TARGET_CUTIN = 5,
  ACC_REPORT_TARGET_TYPE_STOP_SIGN = 6,
  ACC_REPORT_TARGET_TYPE_TRAFFIC_LIGHT = 7,
  ACC_REPORT_TARGET_TYPE_IPSO = 8,
  ACC_REPORT_TARGET_TYPE_FAULT = 9,
  ACC_REPORT_CSA = 10,
  ACC_REPORT_LC_HANDS_ON_REQD_STRUCK_OUT = 11,
  ACC_REPORT_LC_EXTERNAL_STATE_ABORTING = 12,
  ACC_REPORT_LC_EXTERNAL_STATE_ABORTED = 13,
  ACC_REPORT_LC_EXTERNAL_STATE_ACTIVE_RESTRICTED = 14,
  ACC_REPORT_RADAR_OBJ_ONE = 15,
  ACC_REPORT_RADAR_OBJ_TWO = 16,
  ACC_REPORT_TARGET_MCP = 17,
  ACC_REPORT_FLEET_SPEEDS = 18,
  ACC_REPORT_MCVLR_DPP = 19,
  ACC_REPORT_MCVLR_IN_PATH = 20,
  ACC_REPORT_CIPV_CUTTING_OUT = 21,
  ACC_REPORT_RADAR_OBJ_FIVE = 22,
  ACC_REPORT_CAMERA_ONLY = 23,
  ACC_REPORT_BEHAVIOR_REPORT = 24
 };

#define Parse_DAS_relaxCruiseLimits(d) ((d & 0x200000000) >> 33)
#define Parse_DAS_csaState(d) ((d & 0x300000000) >> 32)

enum DAS_csaState_Values { 
  CSA_EXTERNAL_STATE_UNAVAILABLE = 0,
  CSA_EXTERNAL_STATE_AVAILABLE = 1,
  CSA_EXTERNAL_STATE_ENABLE = 2,
  CSA_EXTERNAL_STATE_HOLD = 3
 };

#define Parse_DAS_radarTelemetry(d) ((d & 0xc0000000) >> 30)

enum DAS_radarTelemetry_Values { 
  RADAR_TELEMETRY_IDLE = 0,
  RADAR_TELEMETRY_NORMAL = 1,
  RADAR_TELEMETRY_URGENT = 2
 };

#define Parse_DAS_robState(d) ((d & 0x30000000) >> 28)

enum DAS_robState_Values { 
  ROB_STATE_INHIBITED = 0,
  ROB_STATE_MEASURE = 1,
  ROB_STATE_ACTIVE = 2,
  ROB_STATE_MAPLESS = 3
 };

#define Parse_DAS_driverInteractionLevel(d) ((d & 0xc000000) >> 26)

enum DAS_driverInteractionLevel_Values { 
  DRIVER_INTERACTING = 0,
  DRIVER_NOT_INTERACTING = 1,
  CONTINUED_DRIVER_NOT_INTERACTING = 2
 };

#define Parse_DAS_ppOffsetDesiredRamp(d) ((d & 0xff000000) >> 24)

enum DAS_ppOffsetDesiredRamp_Values { 
  PP_NO_OFFSET = 128
 };

#define Parse_DAS_longCollisionWarning(d) ((d & 0xf0000) >> 16)

enum DAS_longCollisionWarning_Values { 
  FCM_LONG_COLLISION_WARNING_NONE = 0,
  FCM_LONG_COLLISION_WARNING_VEHICLE_UNKNOWN = 1,
  FCM_LONG_COLLISION_WARNING_PEDESTRIAN = 2,
  FCM_LONG_COLLISION_WARNING_IPSO = 3,
  FCM_LONG_COLLISION_WARNING_STOPSIGN_STOPLINE = 4,
  FCM_LONG_COLLISION_WARNING_TFL_STOPLINE = 5,
  FCM_LONG_COLLISION_WARNING_VEHICLE_CIPV = 6,
  FCM_LONG_COLLISION_WARNING_VEHICLE_CUTIN = 7,
  FCM_LONG_COLLISION_WARNING_VEHICLE_MCVL = 8,
  FCM_LONG_COLLISION_WARNING_VEHICLE_MCVL2 = 9,
  FCM_LONG_COLLISION_WARNING_VEHICLE_MCVR = 10,
  FCM_LONG_COLLISION_WARNING_VEHICLE_MCVR2 = 11,
  FCM_LONG_COLLISION_WARNING_VEHICLE_CIPV2 = 12,
  FCM_LONG_COLLISION_WARNING_SNA = 15
 };

#define Parse_DAS_status2Counter(d) ((d & 0xf000) >> 12)
#define Parse_DAS_status2Checksum(d) ((d & 0xff00) >> 8)
#define ID399DAS_status 0x399

#define Parse_DAS_autopilotState(d) ((d & 0xf0) >> 4)

enum DAS_autopilotState_Values { 
  DISABLED = 0,
  UNAVAILABLE = 1,
  AVAILABLE = 2,
  ACTIVE_NOMINAL = 3,
  ACTIVE_RESTRICTED = 4,
  ACTIVE_NAV = 5,
  ABORTING = 8,
  ABORTED = 9,
  FAULT = 14,
  SNA = 15
 };

#define Parse_DAS_blindSpotRearLeft(d) ((d & 0x3000000000000000) >> 60)

enum DAS_blindSpotRearLeft_Values { 
  NO_WARNING = 0,
  WARNING_LEVEL_1 = 1,
  WARNING_LEVEL_2 = 2,
  SNA = 3
 };

#define Parse_DAS_blindSpotRearRight(d) ((d & 0xc00000000000000) >> 58)

enum DAS_blindSpotRearRight_Values { 
  NO_WARNING = 0,
  WARNING_LEVEL_1 = 1,
  WARNING_LEVEL_2 = 2,
  SNA = 3
 };

#define Parse_DAS_fusedSpeedLimit(d) ((d & 0x1f00000000000000) >> 56)

enum DAS_fusedSpeedLimit_Values { 
  UNKNOWN_SNA = 0,
  NONE = 31
 };

#define Parse_DAS_suppressSpeedWarning(d) ((d & 0x8000000000000) >> 51)

enum DAS_suppressSpeedWarning_Values { 
  Do_Not_Suppress = 0,
  Suppress_Speed_Warning = 1
 };

#define Parse_DAS_summonObstacle(d) ((d & 0x4000000000000) >> 50)
#define Parse_DAS_summonClearedGate(d) ((d & 0x2000000000000) >> 49)
#define Parse_DAS_visionOnlySpeedLimit(d) ((d & 0x1f000000000000) >> 48)

enum DAS_visionOnlySpeedLimit_Values { 
  UNKNOWN_SNA = 0,
  NONE = 31
 };

#define Parse_DAS_heaterState(d) ((d & 0x80000000000) >> 43)

enum DAS_heaterState_Values { 
  HEATER_OFF_SNA = 0,
  HEATER_ON = 1
 };

#define Parse_DAS_forwardCollisionWarning(d) ((d & 0xc0000000000) >> 42)

enum DAS_forwardCollisionWarning_Values { 
  NONE = 0,
  FORWARD_COLLISION_WARNING = 1,
  SNA = 3
 };

#define Parse_DAS_autoparkReady(d) ((d & 0x10000000000) >> 40)

enum DAS_autoparkReady_Values { 
  AUTOPARK_UNAVAILABLE = 0,
  AUTOPARK_READY = 1
 };

#define Parse_DAS_autoParked(d) ((d & 0x8000000000) >> 39)
#define Parse_DAS_autoparkWaitingForBrake(d) ((d & 0x4000000000) >> 38)
#define Parse_DAS_summonFwdLeashReached(d) ((d & 0x2000000000) >> 37)
#define Parse_DAS_summonRvsLeashReached(d) ((d & 0x1000000000) >> 36)
#define Parse_DAS_lssState(d) ((d & 0x3800000000) >> 35)

enum DAS_lssState_Values { 
  LSS_STATE_FAULT = 0,
  LSS_STATE_LDW = 1,
  LSS_STATE_LKA = 2,
  LSS_STATE_ELK = 3,
  LSS_STATE_MONITOR = 4,
  LSS_STATE_BLINDSPOT = 5,
  LSS_STATE_ABORT = 6,
  LSS_STATE_OFF = 7
 };

#define Parse_DAS_sideCollisionAvoid(d) ((d & 0x300000000) >> 32)

enum DAS_sideCollisionAvoid_Values { 
  NONE = 0,
  AVOID_LEFT = 1,
  AVOID_RIGHT = 2,
  SNA = 3
 };

#define Parse_DAS_sideCollisionWarning(d) ((d & 0xc0000000) >> 30)

enum DAS_sideCollisionWarning_Values { 
  NONE = 0,
  WARN_LEFT = 1,
  WARN_RIGHT = 2,
  WARN_LEFT_RIGHT = 3
 };

#define Parse_DAS_sideCollisionInhibit(d) ((d & 0x10000000) >> 28)

enum DAS_sideCollisionInhibit_Values { 
  NO_INHIBIT = 0,
  INHIBIT = 1
 };

#define Parse_DAS_laneDepartureWarning(d) ((d & 0x38000000) >> 27)

enum DAS_laneDepartureWarning_Values { 
  NONE = 0,
  LEFT_WARNING = 1,
  RIGHT_WARNING = 2,
  LEFT_WARNING_SEVERE = 3,
  RIGHT_WARNING_SEVERE = 4,
  SNA = 5
 };

#define Parse_DAS_fleetSpeedState(d) ((d & 0x3000000) >> 24)

enum DAS_fleetSpeedState_Values { 
  FLEETSPEED_UNAVAILABLE = 0,
  FLEETSPEED_AVAILABLE = 1,
  FLEETSPEED_ACTIVE = 2,
  FLEETSPEED_HOLD = 3
 };

#define Parse_DAS_autopilotHandsOnState(d) ((d & 0x3c00000) >> 22)

enum DAS_autopilotHandsOnState_Values { 
  LC_HANDS_ON_NOT_REQD = 0,
  LC_HANDS_ON_REQD_DETECTED = 1,
  LC_HANDS_ON_REQD_NOT_DETECTED = 2,
  LC_HANDS_ON_REQD_VISUAL = 3,
  LC_HANDS_ON_REQD_CHIME_1 = 4,
  LC_HANDS_ON_REQD_CHIME_2 = 5,
  LC_HANDS_ON_REQD_SLOWING = 6,
  LC_HANDS_ON_REQD_STRUCK_OUT = 7,
  LC_HANDS_ON_SUSPENDED = 8,
  LC_HANDS_ON_REQD_ESCALATED_CHIME_1 = 9,
  LC_HANDS_ON_REQD_ESCALATED_CHIME_2 = 10,
  LC_HANDS_ON_SNA = 15
 };

#define Parse_DAS_autoLaneChangeState(d) ((d & 0x7c0000) >> 18)

enum DAS_autoLaneChangeState_Values { 
  ALC_UNAVAILABLE_DISABLED = 0,
  ALC_UNAVAILABLE_NO_LANES = 1,
  ALC_UNAVAILABLE_SONICS_INVALID = 2,
  ALC_UNAVAILABLE_TP_FOLLOW = 3,
  ALC_UNAVAILABLE_EXITING_HIGHWAY = 4,
  ALC_UNAVAILABLE_VEHICLE_SPEED = 5,
  ALC_AVAILABLE_ONLY_L = 6,
  ALC_AVAILABLE_ONLY_R = 7,
  ALC_AVAILABLE_BOTH = 8,
  ALC_IN_PROGRESS_L = 9,
  ALC_IN_PROGRESS_R = 10,
  ALC_WAITING_FOR_SIDE_OBST_TO_PASS_L = 11,
  ALC_WAITING_FOR_SIDE_OBST_TO_PASS_R = 12,
  ALC_WAITING_FOR_FWD_OBST_TO_PASS_L = 13,
  ALC_WAITING_FOR_FWD_OBST_TO_PASS_R = 14,
  ALC_ABORT_SIDE_OBSTACLE_PRESENT_L = 15,
  ALC_ABORT_SIDE_OBSTACLE_PRESENT_R = 16,
  ALC_ABORT_POOR_VIEW_RANGE = 17,
  ALC_ABORT_LC_HEALTH_BAD = 18,
  ALC_ABORT_BLINKER_TURNED_OFF = 19,
  ALC_ABORT_OTHER_REASON = 20,
  ALC_UNAVAILABLE_SOLID_LANE_LINE = 21,
  ALC_BLOCKED_VEH_TTC_L = 22,
  ALC_BLOCKED_VEH_TTC_AND_USS_L = 23,
  ALC_BLOCKED_VEH_TTC_R = 24,
  ALC_BLOCKED_VEH_TTC_AND_USS_R = 25,
  ALC_BLOCKED_LANE_TYPE_L = 26,
  ALC_BLOCKED_LANE_TYPE_R = 27,
  ALC_WAITING_HANDS_ON = 28,
  ALC_ABORT_TIMEOUT = 29,
  ALC_ABORT_MISSION_PLAN_INVALID = 30,
  ALC_SNA = 31
 };

#define Parse_DAS_summonAvailable(d) ((d & 0x2000) >> 13)
#define Parse_DAS_statusCounter(d) ((d & 0xf000) >> 12)
#define Parse_DAS_statusChecksum(d) ((d & 0xff00) >> 8)
#define ID39DIBST_status 0x39d

#define Parse_IBST_statusChecksum(d) ((d & 0xff00) >> 8)
#define Parse_IBST_statusCounter(d) ((d & 0xf00000000) >> 32)
#define Parse_IBST_iBoosterStatus(d) ((d & 0x70000000) >> 28)

enum IBST_iBoosterStatus_Values { 
  IBOOSTER_OFF = 0,
  IBOOSTER_INIT = 1,
  IBOOSTER_FAILURE = 2,
  IBOOSTER_DIAGNOSTIC = 3,
  IBOOSTER_ACTIVE_GOOD_CHECK = 4,
  IBOOSTER_READY = 5,
  IBOOSTER_ACTUATION = 6
 };

#define Parse_IBST_driverBrakeApply(d) ((d & 0x3000000) >> 24)

enum IBST_driverBrakeApply_Values { 
  NOT_INIT_OR_OFF = 0,
  BRAKES_NOT_APPLIED = 1,
  DRIVER_APPLYING_BRAKES = 2,
  FAULT = 3
 };

#define Parse_IBST_internalState(d) ((d & 0x1c00000) >> 22)

enum IBST_internalState_Values { 
  NO_MODE_ACTIVE = 0,
  PRE_DRIVE_CHECK = 1,
  LOCAL_BRAKE_REQUEST = 2,
  EXTERNAL_BRAKE_REQUEST = 3,
  DIAGNOSTIC = 4,
  TRANSITION_TO_IDLE = 5,
  POST_DRIVE_CHECK = 6
 };

#define Parse_IBST_sInputRodDriver(d) ((d & 0x7ff80000) >> 19)
#define ID3A1VCFRONT_vehicleStatus 0x3a1

#define Parse_VCFRONT_bmsHvChargeEnable(d) ((d & 0x2) >> 1)
#define Parse_VCFRONT_preconditionRequest(d) ((d & 0x8000000000000000) >> 63)
#define Parse_VCFRONT_APGlassHeaterState(d) ((d & 0xe) >> 1)

enum VCFRONT_APGlassHeaterState_Values { 
  HEATER_STATE_SNA = 0,
  HEATER_STATE_ON = 1,
  HEATER_STATE_OFF = 2,
  HEATER_STATE_OFF_UNAVAILABLE = 3,
  HEATER_STATE_FAULT = 4
 };

#define Parse_VCFRONT_thermalSystemType(d) ((d & 0x800000000000000) >> 59)

enum VCFRONT_thermalSystemType_Values { 
  LEGACY_THERMAL_SYSTEM = 0,
  HEAT_PUMP_THERMAL_SYSTEM = 1
 };

#define Parse_VCFRONT_standbySupplySupported(d) ((d & 0x400000000000000) >> 58)
#define Parse_VCFRONT_driverPresent(d) ((d & 0x200000000000000) >> 57)
#define Parse_VCFRONT_passengerPresent(d) ((d & 0x100000000000000) >> 56)
#define Parse_VCFRONT_accPlusAvailable(d) ((d & 0x80000000000000) >> 55)
#define Parse_VCFRONT_diPowerOnState(d) ((d & 0x1c0000000000000) >> 54)

enum VCFRONT_diPowerOnState_Values { 
  DI_POWERED_OFF = 0,
  DI_POWERED_ON_FOR_SUMMON = 1,
  DI_POWERED_ON_FOR_STATIONARY_HEAT = 2,
  DI_POWERED_ON_FOR_DRIVE = 3,
  DI_POWER_GOING_DOWN = 4
 };

#define Parse_VCFRONT_driverIsLeavingAnySpeed(d) ((d & 0x8000000000000) >> 51)
#define Parse_VCFRONT_12vStatusForDrive(d) ((d & 0xc000000000000) >> 50)

enum VCFRONT_12vStatusForDrive_Values { 
  NOT_READY_FOR_DRIVE_12V = 0,
  READY_FOR_DRIVE_12V = 1,
  EXIT_DRIVE_REQUESTED_12V = 2
 };

#define Parse_VCFRONT_pcs12vVoltageTarget(d) ((d & 0x7ff000000000000) >> 48)
#define Parse_VCFRONT_batterySupportRequest(d) ((d & 0x2000000000) >> 37)
#define Parse_VCFRONT_driverIsLeaving(d) ((d & 0x1000000000) >> 36)
#define Parse_VCFRONT_ota12VSupportRequest(d) ((d & 0x800000000) >> 35)
#define Parse_VCFRONT_driverBuckleStatus(d) ((d & 0x400000000) >> 34)

enum VCFRONT_driverBuckleStatus_Values { 
  UNBUCKLED = 0,
  BUCKLED = 1
 };

#define Parse_VCFRONT_driverDoorStatus(d) ((d & 0x200000000) >> 33)

enum VCFRONT_driverDoorStatus_Values { 
  DOOR_OPEN = 0,
  DOOR_CLOSED = 1
 };

#define Parse_VCFRONT_driverUnbuckled(d) ((d & 0x300000000) >> 32)

enum VCFRONT_driverUnbuckled_Values { 
  CHIME_NONE = 0,
  CHIME_OCCUPIED_AND_UNBUCKLED = 1,
  CHIME_SNA = 2
 };

#define Parse_VCFRONT_passengerUnbuckled(d) ((d & 0xc0000000) >> 30)

enum VCFRONT_passengerUnbuckled_Values { 
  CHIME_NONE = 0,
  CHIME_OCCUPIED_AND_UNBUCKLED = 1,
  CHIME_SNA = 2
 };

#define Parse_VCFRONT_2RowLeftUnbuckled(d) ((d & 0x30000000) >> 28)

enum VCFRONT_2RowLeftUnbuckled_Values { 
  CHIME_NONE = 0,
  CHIME_OCCUPIED_AND_UNBUCKLED = 1,
  CHIME_SNA = 2
 };

#define Parse_VCFRONT_2RowCenterUnbuckled(d) ((d & 0xc000000) >> 26)

enum VCFRONT_2RowCenterUnbuckled_Values { 
  CHIME_NONE = 0,
  CHIME_OCCUPIED_AND_UNBUCKLED = 1,
  CHIME_SNA = 2
 };

#define Parse_VCFRONT_2RowRightUnbuckled(d) ((d & 0x3000000) >> 24)

enum VCFRONT_2RowRightUnbuckled_Values { 
  CHIME_NONE = 0,
  CHIME_OCCUPIED_AND_UNBUCKLED = 1,
  CHIME_SNA = 2
 };

#define Parse_VCFRONT_pcsEFuseVoltage(d) ((d & 0xffc00000) >> 22)

enum VCFRONT_pcsEFuseVoltage_Values { 
  SNA = 1023
 };

#define Parse_VCFRONT_vehicleStatusCounter(d) ((d & 0xf000) >> 12)
#define Parse_VCFRONT_vehicleStatusChecksum(d) ((d & 0xff00) >> 8)
#define ID3D9UI_gpsVehicleSpeed 0x3d9

#define Parse_UI_gpsHDOP(d) ((d & 0xff00) >> 8)
#define Parse_UI_gpsVehicleHeading(d) ((d & 0xffff00) >> 8)
#define Parse_UI_gpsVehicleSpeed(d) ((d & 0xffff0000000000) >> 40)
#define Parse_UI_userSpeedOffset(d) ((d & 0x3f000000) >> 24)
#define Parse_UI_mapSpeedLimitUnits(d) ((d & 0x40000) >> 18)

enum UI_mapSpeedLimitUnits_Values { 
  MPH = 0,
  KPH = 1
 };

#define Parse_UI_userSpeedOffsetUnits(d) ((d & 0x20000) >> 17)

enum UI_userSpeedOffsetUnits_Values { 
  MPH = 0,
  KPH = 1
 };

#define Parse_UI_mppSpeedLimit(d) ((d & 0x1f0000) >> 16)
#define Parse_UI_gpsNmeaMIA(d) ((d & 0x800) >> 11)
#define Parse_UI_gpsAntennaDisconnected(d) ((d & 0x400) >> 10)
#define Parse_UI_conditionalLimitActive(d) ((d & 0x200) >> 9)
#define Parse_UI_conditionalSpeedLimit(d) ((d & 0x1f00) >> 8)

enum UI_conditionalSpeedLimit_Values { 
  SNA = 31
 };

#define ID3E2VCLEFT_lightStatus 0x3e2

#define Parse_VCLEFT_brakeLightStatus(d) ((d & 0xc) >> 2)

enum VCLEFT_brakeLightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCLEFT_tailLightStatus(d) ((d & 0xc0000000000000) >> 54)

enum VCLEFT_tailLightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCLEFT_turnSignalStatus(d) ((d & 0x30000000000000) >> 52)

enum VCLEFT_turnSignalStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCLEFT_FLMapLightStatus(d) ((d & 0xc000000000000) >> 50)

enum VCLEFT_FLMapLightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCLEFT_FRMapLightStatus(d) ((d & 0x3000000000000) >> 48)

enum VCLEFT_FRMapLightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCLEFT_RLMapLightStatus(d) ((d & 0xc00000000000) >> 46)

enum VCLEFT_RLMapLightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCLEFT_RRMapLightStatus(d) ((d & 0x300000000000) >> 44)

enum VCLEFT_RRMapLightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCLEFT_FLMapLightSwitchPressed(d) ((d & 0x40000000000) >> 42)
#define Parse_VCLEFT_FRMapLightSwitchPressed(d) ((d & 0x20000000000) >> 41)
#define Parse_VCLEFT_RLMapLightSwitchPressed(d) ((d & 0x10000000000) >> 40)
#define Parse_VCLEFT_RRMapLightSwitchPressed(d) ((d & 0x8000000000) >> 39)
#define Parse_VCLEFT_leftTurnTrailerLightStatu(d) ((d & 0xc000000000) >> 38)

enum VCLEFT_leftTurnTrailerLightStatu_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCLEFT_rightTrnTrailerLightStatu(d) ((d & 0x3000000000) >> 36)

enum VCLEFT_rightTrnTrailerLightStatu_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCLEFT_brakeTrailerLightStatus(d) ((d & 0xc00000000) >> 34)

enum VCLEFT_brakeTrailerLightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCLEFT_tailTrailerLightStatus(d) ((d & 0x300000000) >> 32)

enum VCLEFT_tailTrailerLightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCLEFT_fogTrailerLightStatus(d) ((d & 0xc0000000) >> 30)

enum VCLEFT_fogTrailerLightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCLEFT_frontRideHeight(d) ((d & 0xff0000000) >> 28)

enum VCLEFT_frontRideHeight_Values { 
  SNA = 128
 };

#define Parse_VCLEFT_rearRideHeight(d) ((d & 0xff00000) >> 20)

enum VCLEFT_rearRideHeight_Values { 
  SNA = 128
 };

#define Parse_VCLEFT_dynamicBrakeLightStatus(d) ((d & 0x3000) >> 12)

enum VCLEFT_dynamicBrakeLightStatus_Values { 
  DYNAMIC_BRAKE_LIGHT_OFF = 0,
  DYNAMIC_BRAKE_LIGHT_ACTIVE_LOW = 1,
  DYNAMIC_BRAKE_LIGHT_ACTIVE_HIGH = 2
 };

#define Parse_VCLEFT_trailerDetected(d) ((d & 0xc00) >> 10)

enum VCLEFT_trailerDetected_Values { 
  TRAILER_LIGHT_DETECTION_SNA = 0,
  TRAILER_LIGHT_DETECTION_FAULT = 1,
  TRAILER_LIGHT_DETECTION_DETECTED = 2,
  TRAILER_LIGHT_DETECTION_NOT_DETECTED = 3
 };

#define Parse_VCLEFT_rideHeightSensorFault(d) ((d & 0x100) >> 8)
#define Parse_VCLEFT_reverseTrailerLightStatus(d) ((d & 0x180) >> 7)

enum VCLEFT_reverseTrailerLightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCLEFT_tailLightOutageStatus(d) ((d & 0x20) >> 5)
#define ID3E9DAS_bodyControls 0x3e9

#define Parse_DAS_headlightRequest(d) ((d & 0xc) >> 2)

enum DAS_headlightRequest_Values { 
  DAS_HEADLIGHT_REQUEST_OFF = 0,
  DAS_HEADLIGHT_REQUEST_ON = 1,
  DAS_HEADLIGHT_REQUEST_INVALID = 3
 };

#define Parse_DAS_hazardLightRequest(d) ((d & 0xc000000000000000) >> 62)

enum DAS_hazardLightRequest_Values { 
  DAS_REQUEST_HAZARDS_OFF = 0,
  DAS_REQUEST_HAZARDS_ON = 1,
  DAS_REQUEST_HAZARDS_UNUSED = 2,
  DAS_REQUEST_HAZARDS_SNA = 3
 };

#define Parse_DAS_wiperSpeed(d) ((d & 0xf000000000000000) >> 60)

enum DAS_wiperSpeed_Values { 
  DAS_WIPER_SPEED_OFF = 0,
  DAS_WIPER_SPEED_1 = 1,
  DAS_WIPER_SPEED_2 = 2,
  DAS_WIPER_SPEED_3 = 3,
  DAS_WIPER_SPEED_4 = 4,
  DAS_WIPER_SPEED_5 = 5,
  DAS_WIPER_SPEED_6 = 6,
  DAS_WIPER_SPEED_7 = 7,
  DAS_WIPER_SPEED_8 = 8,
  DAS_WIPER_SPEED_9 = 9,
  DAS_WIPER_SPEED_10 = 10,
  DAS_WIPER_SPEED_11 = 11,
  DAS_WIPER_SPEED_12 = 12,
  DAS_WIPER_SPEED_13 = 13,
  DAS_WIPER_SPEED_14 = 14,
  DAS_WIPER_SPEED_INVALID = 15
 };

#define Parse_DAS_turnIndicatorRequest(d) ((d & 0x700000000000000) >> 56)

enum DAS_turnIndicatorRequest_Values { 
  DAS_TURN_INDICATOR_NONE = 0,
  DAS_TURN_INDICATOR_LEFT = 1,
  DAS_TURN_INDICATOR_RIGHT = 2,
  DAS_TURN_INDICATOR_CANCEL = 3,
  DAS_TURN_INDICATOR_DEFER = 4
 };

#define Parse_DAS_highLowBeamDecision(d) ((d & 0x60000000000000) >> 53)

enum DAS_highLowBeamDecision_Values { 
  DAS_HIGH_BEAM_UNDECIDED = 0,
  DAS_HIGH_BEAM_OFF = 1,
  DAS_HIGH_BEAM_ON = 2,
  DAS_HIGH_BEAM_SNA = 3
 };

#define Parse_DAS_heaterRequest(d) ((d & 0x18000000000000) >> 51)

enum DAS_heaterRequest_Values { 
  DAS_HEATER_SNA = 0,
  DAS_HEATER_OFF = 1,
  DAS_HEATER_ON = 2
 };

#define Parse_DAS_highLowBeamOffReason(d) ((d & 0xe000000000000) >> 49)

enum DAS_highLowBeamOffReason_Values { 
  HIGH_BEAM_ON = 0,
  HIGH_BEAM_OFF_REASON_MOVING_VISION_TARGET = 1,
  HIGH_BEAM_OFF_REASON_MOVING_RADAR_TARGET = 2,
  HIGH_BEAM_OFF_REASON_AMBIENT_LIGHT = 3,
  HIGH_BEAM_OFF_REASON_HEAD_LIGHT = 4,
  HIGH_BEAM_OFF_REASON_SNA = 5
 };

#define Parse_DAS_turnIndicatorRequestReason(d) ((d & 0x3c00000000000) >> 46)

enum DAS_turnIndicatorRequestReason_Values { 
  DAS_NONE = 0,
  DAS_ACTIVE_NAV_LANE_CHANGE = 1,
  DAS_ACTIVE_SPEED_LANE_CHANGE = 2,
  DAS_ACTIVE_FORK = 3,
  DAS_CANCEL_LANE_CHANGE = 4,
  DAS_CANCEL_FORK = 5,
  DAS_ACTIVE_MERGE = 6,
  DAS_CANCEL_MERGE = 7,
  DAS_ACTIVE_COMMANDED_LANE_CHANGE = 8,
  DAS_ACTIVE_INTERSECTION = 9,
  DAS_CANCEL_INTERSECTION = 10,
  DAS_ACTIVE_SUMMMON = 11,
  DAS_CANCEL_SUMMMON = 12
 };

#define Parse_DAS_dynamicBrakeLightRequest(d) ((d & 0x40000000000) >> 42)
#define Parse_DAS_radarHeaterRequest(d) ((d & 0x20000000000) >> 41)
#define Parse_DAS_ahlbOverride(d) ((d & 0x10000000000) >> 40)
#define Parse_DAS_mirrorFoldRequest(d) ((d & 0x18000000000) >> 39)

enum DAS_mirrorFoldRequest_Values { 
  NONE = 0,
  FOLD = 1,
  UNFOLD = 2,
  SNA = 3
 };

#define Parse_DAS_bodyControlsCounter(d) ((d & 0xf000) >> 12)
#define Parse_DAS_bodyControlsChecksum(d) ((d & 0xff00) >> 8)
#define ID3F3UI_odo 0x3f3

#define Parse_UI_odometer(d) ((d & 0xffffff000000) >> 24)

enum UI_odometer_Values { 
  SNA = 16777215
 };

#define ID3F5VCFRONT_lighting 0x3f5

#define Parse_VCFRONT_indicatorLeftRequest(d) ((d & 0xc) >> 2)

enum VCFRONT_indicatorLeftRequest_Values { 
  TURN_SIGNAL_OFF = 0,
  TURN_SIGNAL_ACTIVE_LOW = 1,
  TURN_SIGNAL_ACTIVE_HIGH = 2
 };

#define Parse_VCFRONT_indicatorRightRequest(d) ((d & 0xc000000000000000) >> 62)

enum VCFRONT_indicatorRightRequest_Values { 
  TURN_SIGNAL_OFF = 0,
  TURN_SIGNAL_ACTIVE_LOW = 1,
  TURN_SIGNAL_ACTIVE_HIGH = 2
 };

#define Parse_VCFRONT_hazardLightRequest(d) ((d & 0xf000000000000000) >> 60)

enum VCFRONT_hazardLightRequest_Values { 
  HAZARD_REQUEST_NONE = 0,
  HAZARD_REQUEST_BUTTON = 1,
  HAZARD_REQUEST_LOCK = 2,
  HAZARD_REQUEST_UNLOCK = 3,
  HAZARD_REQUEST_MISLOCK = 4,
  HAZARD_REQUEST_CRASH = 5,
  HAZARD_REQUEST_CAR_ALARM = 6,
  HAZARD_REQUEST_DAS = 7,
  HAZARD_REQUEST_UDS = 8
 };

#define Parse_VCFRONT_ambientLightingBrightnes(d) ((d & 0xff00000000000000) >> 56)

enum VCFRONT_ambientLightingBrightnes_Values { 
  SNA = 255
 };

#define Parse_VCFRONT_switchLightingBrightness(d) ((d & 0xff000000000000) >> 48)

enum VCFRONT_switchLightingBrightness_Values { 
  SNA = 255
 };

#define Parse_VCFRONT_courtesyLightingRequest(d) ((d & 0x10000000000) >> 40)
#define Parse_VCFRONT_approachLightingRequest(d) ((d & 0x8000000000) >> 39)
#define Parse_VCFRONT_seeYouHomeLightingReq(d) ((d & 0x4000000000) >> 38)
#define Parse_VCFRONT_hazardSwitchBacklight(d) ((d & 0x2000000000) >> 37)
#define Parse_VCFRONT_lowBeamLeftStatus(d) ((d & 0x3000000000) >> 36)

enum VCFRONT_lowBeamLeftStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCFRONT_lowBeamRightStatus(d) ((d & 0xc00000000) >> 34)

enum VCFRONT_lowBeamRightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCFRONT_highBeamLeftStatus(d) ((d & 0x300000000) >> 32)

enum VCFRONT_highBeamLeftStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCFRONT_highBeamRightStatus(d) ((d & 0xc0000000) >> 30)

enum VCFRONT_highBeamRightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCFRONT_DRLLeftStatus(d) ((d & 0x30000000) >> 28)

enum VCFRONT_DRLLeftStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCFRONT_DRLRightStatus(d) ((d & 0xc000000) >> 26)

enum VCFRONT_DRLRightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCFRONT_fogLeftStatus(d) ((d & 0x3000000) >> 24)

enum VCFRONT_fogLeftStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCFRONT_fogRightStatus(d) ((d & 0xc00000) >> 22)

enum VCFRONT_fogRightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCFRONT_sideMarkersStatus(d) ((d & 0x300000) >> 20)

enum VCFRONT_sideMarkersStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCFRONT_sideRepeaterLeftStatus(d) ((d & 0xc0000) >> 18)

enum VCFRONT_sideRepeaterLeftStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCFRONT_sideRepeaterRightStatus(d) ((d & 0x30000) >> 16)

enum VCFRONT_sideRepeaterRightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCFRONT_turnSignalLeftStatus(d) ((d & 0xc000) >> 14)

enum VCFRONT_turnSignalLeftStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCFRONT_turnSignalRightStatus(d) ((d & 0x3000) >> 12)

enum VCFRONT_turnSignalRightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCFRONT_parkLeftStatus(d) ((d & 0xc00) >> 10)

enum VCFRONT_parkLeftStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCFRONT_parkRightStatus(d) ((d & 0x300) >> 8)

enum VCFRONT_parkRightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCFRONT_highBeamSwitchActive(d) ((d & 0x40) >> 6)
#define Parse_VCFRONT_simLatchingStalk(d) ((d & 0x60) >> 5)

enum VCFRONT_simLatchingStalk_Values { 
  SIMULATED_LATCHING_STALK_IDLE = 0,
  SIMULATED_LATCHING_STALK_LEFT = 1,
  SIMULATED_LATCHING_STALK_RIGHT = 2,
  SIMULATED_LATCHING_STALK_SNA = 3
 };

#define Parse_VCFRONT_lowBeamsOnForDRL(d) ((d & 0x8) >> 3)
#define Parse_VCFRONT_lowBeamsCalibrated(d) ((d & 0x4) >> 2)
#define ID3F8UI_driverAssistControl 0x3f8

#define Parse_UI_autopilotControlRequest(d) ((d & 0x2) >> 1)

enum UI_autopilotControlRequest_Values { 
  LEGACY_LAT_CTRL = 0,
  NEXT_GEN_CTRL = 1
 };

#define Parse_UI_ulcStalkConfirm(d) ((d & 0x8000000000000000) >> 63)
#define Parse_UI_summonHeartbeat(d) ((d & 0xc000000000000000) >> 62)
#define Parse_UI_curvSpeedAdaptDisable(d) ((d & 0x1000000000000000) >> 60)

enum UI_curvSpeedAdaptDisable_Values { 
  CSA_ON = 0,
  CSA_OFF = 1
 };

#define Parse_UI_dasDeveloper(d) ((d & 0x800000000000000) >> 59)
#define Parse_UI_enableVinAssociation(d) ((d & 0x400000000000000) >> 58)
#define Parse_UI_lssLkaEnabled(d) ((d & 0x200000000000000) >> 57)

enum UI_lssLkaEnabled_Values { 
  LKA_OFF = 0,
  LKA_ON = 1
 };

#define Parse_UI_lssLdwEnabled(d) ((d & 0x100000000000000) >> 56)

enum UI_lssLdwEnabled_Values { 
  LDW_OFF = 0,
  LDW_ON = 1
 };

#define Parse_UI_coastToCoast(d) ((d & 0x80000000000000) >> 55)
#define Parse_UI_autoSummonEnable(d) ((d & 0x40000000000000) >> 54)
#define Parse_UI_exceptionListEnable(d) ((d & 0x20000000000000) >> 53)

enum UI_exceptionListEnable_Values { 
  EXCEPTION_LIST_OFF = 0,
  EXCEPTION_LIST_ON = 1
 };

#define Parse_UI_roadCheckDisable(d) ((d & 0x10000000000000) >> 52)

enum UI_roadCheckDisable_Values { 
  RC_ON = 0,
  RC_OFF = 1
 };

#define Parse_UI_driveOnMapsEnable(d) ((d & 0x8000000000000) >> 51)

enum UI_driveOnMapsEnable_Values { 
  DOM_OFF = 0,
  DOM_ON = 1
 };

#define Parse_UI_handsOnRequirementDisable(d) ((d & 0x4000000000000) >> 50)

enum UI_handsOnRequirementDisable_Values { 
  HANDS_ON_REQ_ON = 0,
  HANDS_ON_REQ_OFF = 1
 };

#define Parse_UI_ulcOffHighway(d) ((d & 0x2000000000000) >> 49)
#define Parse_UI_fuseLanesDisable(d) ((d & 0x1000000000000) >> 48)

enum UI_fuseLanesDisable_Values { 
  FUSE_LANES_ON = 0,
  FUSE_LANES_OFF = 1
 };

#define Parse_UI_fuseHPPDisable(d) ((d & 0x800000000000) >> 47)

enum UI_fuseHPPDisable_Values { 
  FUSE_HPP_ON = 0,
  FUSE_HPP_OFF = 1
 };

#define Parse_UI_fuseVehiclesDisable(d) ((d & 0x400000000000) >> 46)

enum UI_fuseVehiclesDisable_Values { 
  FUSE_VEH_ON = 0,
  FUSE_VEH_OFF = 1
 };

#define Parse_UI_visionSpeedType(d) ((d & 0x300000000000) >> 44)

enum UI_visionSpeedType_Values { 
  VISION_SPEED_DISABLED = 0,
  VISION_SPEED_ONE_SECOND = 1,
  VISION_SPEED_TWO_SECOND = 2,
  VISION_SPEED_OPTIMIZED = 3
 };

#define Parse_UI_curvatureDatabaseOnly(d) ((d & 0x40000000000) >> 42)

enum UI_curvatureDatabaseOnly_Values { 
  OFF = 0,
  ON = 1
 };

#define Parse_UI_lssElkEnabled(d) ((d & 0x20000000000) >> 41)

enum UI_lssElkEnabled_Values { 
  ELK_OFF = 0,
  ELK_ON = 1
 };

#define Parse_UI_summonExitType(d) ((d & 0x30000000000) >> 40)

enum UI_summonExitType_Values { 
  STRAIGHT = 0,
  TURN_RIGHT = 1,
  TURN_LEFT = 2,
  SNA = 3
 };

#define Parse_UI_summonEntryType(d) ((d & 0xc000000000) >> 38)

enum UI_summonEntryType_Values { 
  STRAIGHT = 0,
  TURN_RIGHT = 1,
  TURN_LEFT = 2,
  SNA = 3
 };

#define Parse_UI_selfParkRequest(d) ((d & 0xf000000000) >> 36)

enum UI_selfParkRequest_Values { 
  NONE = 0,
  SELF_PARK_FORWARD = 1,
  SELF_PARK_REVERSE = 2,
  ABORT = 3,
  PRIME = 4,
  PAUSE = 5,
  RESUME = 6,
  AUTO_SUMMON_FORWARD = 7,
  AUTO_SUMMON_REVERSE = 8,
  AUTO_SUMMON_CANCEL = 9,
  AUTO_SUMMON_PRIMED = 10,
  SMART_SUMMON = 11,
  SMART_SUMMON_NO_OP = 12,
  SNA = 15
 };

#define Parse_UI_summonReverseDist(d) ((d & 0x3f00000000) >> 32)

enum UI_summonReverseDist_Values { 
  SNA = 63
 };

#define Parse_UI_undertakeAssistEnable(d) ((d & 0x4000000) >> 26)
#define Parse_UI_adaptiveSetSpeedEnable(d) ((d & 0x2000000) >> 25)
#define Parse_UI_drivingSide(d) ((d & 0x3000000) >> 24)

enum UI_drivingSide_Values { 
  DRIVING_SIDE_LEFT = 0,
  DRIVING_SIDE_RIGHT = 1,
  DRIVING_SIDE_UNKNOWN = 2
 };

#define Parse_UI_enableClipTelemetry(d) ((d & 0x400000) >> 22)
#define Parse_UI_enableTripTelemetry(d) ((d & 0x200000) >> 21)
#define Parse_UI_enableRoadSegmentTelemetry(d) ((d & 0x100000) >> 20)
#define Parse_UI_accFollowDistanceSetting(d) ((d & 0x380000) >> 19)

enum UI_accFollowDistanceSetting_Values { 
  DISTANCE_SETTING_1 = 0,
  DISTANCE_SETTING_2 = 1,
  DISTANCE_SETTING_3 = 2,
  DISTANCE_SETTING_4 = 3,
  DISTANCE_SETTING_5 = 4,
  DISTANCE_SETTING_6 = 5,
  DISTANCE_SETTING_7 = 6,
  DISTANCE_SETTING_SNA = 7
 };

#define Parse_UI_hasDriveOnNav(d) ((d & 0x10000) >> 16)
#define Parse_UI_followNavRouteEnable(d) ((d & 0x8000) >> 15)

enum UI_followNavRouteEnable_Values { 
  NAV_ROUTE_OFF = 0,
  NAV_ROUTE_ON = 1
 };

#define Parse_UI_ulcSpeedConfig(d) ((d & 0xc000) >> 14)

enum UI_ulcSpeedConfig_Values { 
  SPEED_BASED_ULC_DISABLED = 0,
  SPEED_BASED_ULC_MILD = 1,
  SPEED_BASED_ULC_AVERAGE = 2,
  SPEED_BASED_ULC_MAD_MAX = 3
 };

#define Parse_UI_ulcBlindSpotConfig(d) ((d & 0x3000) >> 12)

enum UI_ulcBlindSpotConfig_Values { 
  STANDARD = 0,
  AGGRESSIVE = 1,
  MAD_MAX = 2
 };

#define Parse_UI_alcOffHighwayEnable(d) ((d & 0x100) >> 8)
#define Parse_UI_validationLoop(d) ((d & 0x80) >> 7)
#define Parse_UI_smartSummonType(d) ((d & 0xc0) >> 6)

enum UI_smartSummonType_Values { 
  PIN_DROP = 0,
  FIND_ME = 1,
  SMART_AUTOPARK = 2
 };

#define Parse_UI_enableVisionOnlyStops(d) ((d & 0x10) >> 4)
#define Parse_UI_source3D(d) ((d & 0x18) >> 3)

enum UI_source3D_Values { 
  Z_FROM_MAP = 0,
  Z_FROM_PATH_PREDICTION = 1,
  XYZ_PREDICTION = 2
 };

#define Parse_UI_enableBrakeLightPulse(d) ((d & 0x2) >> 1)
#define ID3FDUI_autopilotControl 0x3fd

#define Parse_UI_autopilotControlIndex(d) ((d & 0x38) >> 3)

enum UI_autopilotControlIndex_Values { 
  AUTOPILOT_CONTROL_0 = 0,
  AUTOPILOT_CONTROL_1 = 1,
  AUTOPILOT_CONTROL_2 = 2,
  AUTOPILOT_CONTROL_3 = 3,
  AUTOPILOT_CONTROL_4 = 4,
  AUTOPILOT_CONTROL_5 = 5,
  AUTOPILOT_CONTROL_6 = 6,
  AUTOPILOT_CONTROL_7 = 7
 };

#define Parse_UI_hovEnabled(d) ((d & 0x2000000000000000) >> 61)

enum UI_hovEnabled_Values { 
  HOV_OFF = 0,
  HOV_ON = 1
 };

#define Parse_UI_donDisableAutoWiperDuration(d) ((d & 0x7000000000000000) >> 60)

enum UI_donDisableAutoWiperDuration_Values { 
  DON_DISABLE_AUTO_WIPER_DURATION_DEFAULT = 0,
  DON_DISABLE_AUTO_WIPER_DURATION_5_S = 1,
  DON_DISABLE_AUTO_WIPER_DURATION_15_S = 2,
  DON_DISABLE_AUTO_WIPER_DURATION_30_S = 3,
  DON_DISABLE_AUTO_WIPER_DURATION_60_S = 4,
  DON_DISABLE_AUTO_WIPER_DURATION_120_S = 5,
  DON_DISABLE_AUTO_WIPER_DURATION_OFF = 6
 };

#define Parse_UI_donDisableOnAutoWiperSpeed(d) ((d & 0x1e00000000000000) >> 57)

enum UI_donDisableOnAutoWiperSpeed_Values { 
  DAS_WIPER_SPEED_OFF = 0,
  DAS_WIPER_SPEED_1 = 1,
  DAS_WIPER_SPEED_2 = 2,
  DAS_WIPER_SPEED_3 = 3,
  DAS_WIPER_SPEED_4 = 4,
  DAS_WIPER_SPEED_5 = 5,
  DAS_WIPER_SPEED_6 = 6,
  DAS_WIPER_SPEED_7 = 7,
  DAS_WIPER_SPEED_8 = 8,
  DAS_WIPER_SPEED_9 = 9,
  DAS_WIPER_SPEED_10 = 10,
  DAS_WIPER_SPEED_11 = 11,
  DAS_WIPER_SPEED_12 = 12,
  DAS_WIPER_SPEED_13 = 13,
  DAS_WIPER_SPEED_14 = 14,
  DAS_WIPER_SPEED_INVALID = 15
 };

#define Parse_UI_blindspotMinSpeed(d) ((d & 0x1e0000000000000) >> 53)

enum UI_blindspotMinSpeed_Values { 
  BLINDSPOT_MIN_SPEED_DEFAULT = 0,
  BLINDSPOT_MIN_SPEED_5_KPH = 1,
  BLINDSPOT_MIN_SPEED_10_KPH = 2,
  BLINDSPOT_MIN_SPEED_15_KPH = 3,
  BLINDSPOT_MIN_SPEED_20_KPH = 4,
  BLINDSPOT_MIN_SPEED_25_KPH = 5,
  BLINDSPOT_MIN_SPEED_30_KPH = 6,
  BLINDSPOT_MIN_SPEED_35_KPH = 7,
  BLINDSPOT_MIN_SPEED_40_KPH = 8,
  BLINDSPOT_MIN_SPEED_45_KPH = 9,
  BLINDSPOT_MIN_SPEED_OFF = 10
 };

#define Parse_UI_blindspotDistance(d) ((d & 0xe000000000000) >> 49)

enum UI_blindspotDistance_Values { 
  BLINDSPOT_DISTANCE_DEFAULT = 0,
  BLINDSPOT_DISTANCE_0P5_M = 1,
  BLINDSPOT_DISTANCE_1_M = 2,
  BLINDSPOT_DISTANCE_2_M = 3,
  BLINDSPOT_DISTANCE_4_M = 4,
  BLINDSPOT_DISTANCE_OFF = 5
 };

#define Parse_UI_driverMonitorConfirmation(d) ((d & 0x800000000000) >> 47)
#define Parse_UI_blindspotTTC(d) ((d & 0x1c00000000000) >> 46)

enum UI_blindspotTTC_Values { 
  BLINDSPOT_TTC_DEFAULT = 0,
  BLINDSPOT_TTC_0P5_S = 1,
  BLINDSPOT_TTC_1_S = 2,
  BLINDSPOT_TTC_2_S = 3,
  BLINDSPOT_TTC_4_S = 4,
  BLINDSPOT_TTC_3_S = 5,
  BLINDSPOT_TTC_5_S = 6,
  BLINDSPOT_TTC_OFF = 7
 };

#define Parse_UI_applyEceR79(d) ((d & 0x200000000000) >> 45)
#define Parse_UI_enableMapStops(d) ((d & 0x100000000000) >> 44)
#define Parse_UI_donStopEndOfRampBuffer(d) ((d & 0x380000000000) >> 43)

enum UI_donStopEndOfRampBuffer_Values { 
  DON_STOP_END_OF_RAMP_BUFFER_DEFAULT = 0,
  DON_STOP_END_OF_RAMP_BUFFER_15_M = 1,
  DON_STOP_END_OF_RAMP_BUFFER_30_M = 2,
  DON_STOP_END_OF_RAMP_BUFFER_45_M = 3,
  DON_STOP_END_OF_RAMP_BUFFER_OFF = 4
 };

#define Parse_UI_disableMain(d) ((d & 0x20000000000) >> 41)
#define Parse_UI_disableNarrow(d) ((d & 0x10000000000) >> 40)
#define Parse_UI_donDisableCutin(d) ((d & 0x10000000000) >> 40)

enum UI_donDisableCutin_Values { 
  DON_DISABLE_CUTIN_OFF = 0,
  DON_DISABLE_CUTIN_ON = 1
 };

#define Parse_UI_disableFisheye(d) ((d & 0x8000000000) >> 39)
#define Parse_UI_donMinGoreWidthForAbortMap(d) ((d & 0x78000000000) >> 39)

enum UI_donMinGoreWidthForAbortMap_Values { 
  DON_MIN_GORE_WIDTH_FOR_ABORT_DEFAULT = 0,
  DON_MIN_GORE_WIDTH_FOR_ABORT_0_M = 1,
  DON_MIN_GORE_WIDTH_FOR_ABORT_0P5_M = 2,
  DON_MIN_GORE_WIDTH_FOR_ABORT_1_M = 3,
  DON_MIN_GORE_WIDTH_FOR_ABORT_1P5_M = 4,
  DON_MIN_GORE_WIDTH_FOR_ABORT_2_M = 5,
  DON_MIN_GORE_WIDTH_FOR_ABORT_2P5_M = 6,
  DON_MIN_GORE_WIDTH_FOR_ABORT_3_M = 7,
  DON_MIN_GORE_WIDTH_FOR_ABORT_3P5_M = 8,
  DON_MIN_GORE_WIDTH_FOR_ABORT_4_M = 9,
  DON_MIN_GORE_WIDTH_FOR_ABORT_4P5_M = 10,
  DON_MIN_GORE_WIDTH_FOR_ABORT_5_M = 11
 };

#define Parse_UI_disableLeftPillar(d) ((d & 0x4000000000) >> 38)
#define Parse_UI_disableRightPillar(d) ((d & 0x2000000000) >> 37)
#define Parse_UI_disableLeftRepeater(d) ((d & 0x1000000000) >> 36)
#define Parse_UI_disableRightRepeater(d) ((d & 0x800000000) >> 35)
#define Parse_UI_donAlcProgGoreAbortThres(d) ((d & 0x7800000000) >> 35)

enum UI_donAlcProgGoreAbortThres_Values { 
  DON_ALC_PROGRESS_GORE_ABORT_THRESHOLD_DEFAULT = 0,
  DON_ALC_PROGRESS_GORE_ABORT_THRESHOLD_0P00 = 1,
  DON_ALC_PROGRESS_GORE_ABORT_THRESHOLD_0P05 = 2,
  DON_ALC_PROGRESS_GORE_ABORT_THRESHOLD_0P10 = 3,
  DON_ALC_PROGRESS_GORE_ABORT_THRESHOLD_0P15 = 4,
  DON_ALC_PROGRESS_GORE_ABORT_THRESHOLD_0P20 = 5,
  DON_ALC_PROGRESS_GORE_ABORT_THRESHOLD_0P25 = 6,
  DON_ALC_PROGRESS_GORE_ABORT_THRESHOLD_0P30 = 7,
  DON_ALC_PROGRESS_GORE_ABORT_THRESHOLD_0P35 = 8,
  DON_ALC_PROGRESS_GORE_ABORT_THRESHOLD_0P40 = 9
 };

#define Parse_UI_disableBackup(d) ((d & 0x400000000) >> 34)
#define Parse_UI_disableRadar(d) ((d & 0x200000000) >> 33)
#define Parse_UI_noStalkConfirmAlertHaptic(d) ((d & 0x100000000) >> 32)
#define Parse_UI_donMinGoreWidthForAbortNotMap(d) ((d & 0x780000000) >> 31)

enum UI_donMinGoreWidthForAbortNotMap_Values { 
  DON_MIN_GORE_WIDTH_FOR_ABORT_DEFAULT = 0,
  DON_MIN_GORE_WIDTH_FOR_ABORT_0_M = 1,
  DON_MIN_GORE_WIDTH_FOR_ABORT_0P5_M = 2,
  DON_MIN_GORE_WIDTH_FOR_ABORT_1_M = 3,
  DON_MIN_GORE_WIDTH_FOR_ABORT_1P5_M = 4,
  DON_MIN_GORE_WIDTH_FOR_ABORT_2_M = 5,
  DON_MIN_GORE_WIDTH_FOR_ABORT_2P5_M = 6,
  DON_MIN_GORE_WIDTH_FOR_ABORT_3_M = 7,
  DON_MIN_GORE_WIDTH_FOR_ABORT_3P5_M = 8,
  DON_MIN_GORE_WIDTH_FOR_ABORT_4_M = 9,
  DON_MIN_GORE_WIDTH_FOR_ABORT_4P5_M = 10,
  DON_MIN_GORE_WIDTH_FOR_ABORT_5_M = 11
 };

#define Parse_UI_ulcSnooze(d) ((d & 0x10000000) >> 28)
#define Parse_UI_fsdVisualizationEnabled(d) ((d & 0x8000000) >> 27)
#define Parse_UI_noStalkConfirmAlertChime(d) ((d & 0x8000000) >> 27)
#define Parse_UI_fsdStopsControlEnabled(d) ((d & 0x4000000) >> 26)
#define Parse_UI_factorySummonEnable(d) ((d & 0x2000000) >> 25)
#define Parse_UI_apmv3Branch(d) ((d & 0x7000000) >> 24)

enum UI_apmv3Branch_Values { 
  LIVE = 0,
  STAGE = 1,
  DEV = 2,
  STAGE2 = 3,
  EAP = 4,
  DEMO = 5
 };

#define Parse_UI_enableCabinCamera(d) ((d & 0x200000) >> 21)
#define Parse_UI_enableAutopilotStopWarning(d) ((d & 0x100000) >> 20)
#define Parse_UI_homelinkNearby(d) ((d & 0x80000) >> 19)

enum UI_homelinkNearby_Values { 
  HOMELINK_NOT_NEARBY = 0,
  HOMELINK_NEARBY = 1
 };

#define Parse_UI_showLaneGraph(d) ((d & 0x80000) >> 19)
#define Parse_UI_showTrackLabels(d) ((d & 0x40000) >> 18)
#define Parse_UI_hardCoreSummon(d) ((d & 0x20000) >> 17)
#define Parse_UI_enableCabinCameraTelemetry(d) ((d & 0x10000) >> 16)
#define ID267DI_vehicleEstimates 0x267

#define Parse_DI_mass(d) ((d & 0xffc00) >> 10)
#define Parse_DI_tireFitment(d) ((d & 0xc0000000000000) >> 54)

enum DI_tireFitment_Values { 
  FITMENT_SQUARE = 0,
  FITMENT_STAGGERED = 1,
  FITMENT_SNA = 3
 };

#define Parse_DI_trailerDetected(d) ((d & 0x10000000000000) >> 52)

enum DI_trailerDetected_Values { 
  TRAILER_NOT_DETECTED = 0,
  TRAILER_DETECTED = 1
 };

#define Parse_DI_vehicleEstimatesCounter(d) ((d & 0x38000000000000) >> 51)
#define Parse_DI_relativeTireTreadDepth(d) ((d & 0x3f000000000000) >> 48)

enum DI_relativeTireTreadDepth_Values { 
  SNA = 32
 };

#define Parse_DI_massConfidence(d) ((d & 0x100000000) >> 32)

enum DI_massConfidence_Values { 
  MASS_NOT_CONFIDED = 0,
  MASS_CONFIDED = 1
 };

#define Parse_DI_gradeEst(d) ((d & 0x3f80000000) >> 31)
#define Parse_DI_vehicleEstimatesChecksum(d) ((d & 0xff000000) >> 24)
#define Parse_DI_gradeEstInternal(d) ((d & 0x7f0000) >> 16)
#define Parse_DI_steeringAngleOffset(d) ((d & 0xff00) >> 8)
#define ID282VCLEFT_hvacBlowerFeedback 0x282

#define Parse_VCLEFT_blowerIndex(d) ((d & 0xc) >> 2)

enum VCLEFT_blowerIndex_Values { 
  HVAC_FEEDBACK_SIGNALS = 0,
  HVAC_VARS = 1
 };

#define Parse_VCLEFT_hvacBlowerEnabled(d) ((d & 0x4000000000000000) >> 62)
#define Parse_VCLEFT_hvacBlowerRs(d) ((d & 0x3fc0) >> 6)

enum VCLEFT_hvacBlowerRs_Values { 
  SNA = 255
 };

#define Parse_VCLEFT_hvacBlowerOutputDuty(d) ((d & 0x7f0) >> 4)

enum VCLEFT_hvacBlowerOutputDuty_Values { 
  SNA = 127
 };

#define Parse_VCLEFT_hvacBlowerIPhase0(d) ((d & 0x3fc0000000000000) >> 54)

enum VCLEFT_hvacBlowerIPhase0_Values { 
  SNA = 255
 };

#define Parse_VCLEFT_hvacBlowerRPMTarget(d) ((d & 0xffc0000000000000) >> 54)

enum VCLEFT_hvacBlowerRPMTarget_Values { 
  SNA = 1023
 };

#define Parse_VCLEFT_hvacBlowerIPhase1(d) ((d & 0x3fc00000000000) >> 46)

enum VCLEFT_hvacBlowerIPhase1_Values { 
  SNA = 255
 };

#define Parse_VCLEFT_hvacBlowerRPMActual(d) ((d & 0x3ff00000000000) >> 44)
#define Parse_VCLEFT_hvacBlowerIPhase2(d) ((d & 0x3fc000000000) >> 38)

enum VCLEFT_hvacBlowerIPhase2_Values { 
  SNA = 255
 };

#define Parse_VCLEFT_hvacBlowerTorque(d) ((d & 0xffc0000000) >> 30)

enum VCLEFT_hvacBlowerTorque_Values { 
  SNA = 1023
 };

#define Parse_VCLEFT_hvacBlower_IO_CBC_HEAD(d) ((d & 0x3c0000000) >> 30)
#define Parse_VCLEFT_hvacBlower_IO_CBC_TAIL(d) ((d & 0x3c000000) >> 26)
#define Parse_VCLEFT_hvacBlower_IO_CBC_TAIL_va(d) ((d & 0x400000) >> 22)
#define Parse_VCLEFT_hvacBlower_IO_CBC_Status(d) ((d & 0x200000) >> 21)

enum VCLEFT_hvacBlower_IO_CBC_Status_Values { 
  IDLE = 0,
  RX = 1
 };

#define Parse_VCLEFT_hvacBlowerFETTemp(d) ((d & 0x7f00000) >> 20)

enum VCLEFT_hvacBlowerFETTemp_Values { 
  SNA = 127
 };

#define Parse_VCLEFT_hvacBlower_IO_CBC_numUart(d) ((d & 0x3fc0000) >> 18)
#define Parse_VCLEFT_hvacBlowerITerm(d) ((d & 0xfe000) >> 13)

enum VCLEFT_hvacBlowerITerm_Values { 
  SNA = 127
 };

#define Parse_VCLEFT_hvacBlowerCBCEstState(d) ((d & 0x3c00) >> 10)

enum VCLEFT_hvacBlowerCBCEstState_Values { 
  ERROR = 0,
  IDLE = 1,
  ROVERL = 2,
  RS = 3,
  RAMPUP = 4,
  IDRATED = 5,
  RATEDFLUX_OL = 6,
  RATEDFLUX = 7,
  RAMPDOWN = 8,
  LOCKROTOR = 9,
  INDUCTANCE_EST = 10,
  ROTOR_RESISTANCE = 11,
  MOTOR_IDENTIFIED = 12,
  ONLINE = 13,
  COUNT = 14
 };

#define Parse_VCLEFT_hvacBlowerCBCCtrlState(d) ((d & 0x1c0) >> 6)

enum VCLEFT_hvacBlowerCBCCtrlState_Values { 
  ERROR = 0,
  IDLE = 1,
  OFFLINE = 2,
  ONLINE = 3,
  COUNT = 4
 };

#define Parse_VCLEFT_hvacBlowerInitd(d) ((d & 0x40) >> 6)
#define Parse_VCLEFT_hvacBlowerFault(d) ((d & 0x20) >> 5)
#define Parse_VCLEFT_hvacBlowerLimitFETTemps(d) ((d & 0x10) >> 4)
#define Parse_VCLEFT_hvacBlowerPowerOn(d) ((d & 0x8) >> 3)
#define Parse_VCLEFT_hvacBlowerRsOnlineActive(d) ((d & 0x8) >> 3)
#define Parse_VCLEFT_hvacBlowerSpiError(d) ((d & 0x4) >> 2)
#define ID2F3UI_hvacRequest 0x2f3

#define Parse_UI_hvacReqTempSetpointLeft(d) ((d & 0x3e0) >> 5)

enum UI_hvacReqTempSetpointLeft_Values { 
  LO = 0,
  HI = 26
 };

#define Parse_UI_hvacReqTempSetpointRight(d) ((d & 0x1f00000000) >> 32)

enum UI_hvacReqTempSetpointRight_Values { 
  LO = 0,
  HI = 26
 };

#define Parse_UI_hvacReqAirDistributionMode(d) ((d & 0x38000000) >> 27)

enum UI_hvacReqAirDistributionMode_Values { 
  AUTO = 0,
  MANUAL_FLOOR = 1,
  MANUAL_PANEL = 2,
  MANUAL_PANEL_FLOOR = 3,
  MANUAL_DEFROST = 4,
  MANUAL_DEFROST_FLOOR = 5,
  MANUAL_DEFROST_PANEL = 6,
  MANUAL_DEFROST_PANEL_FLOOR = 7
 };

#define Parse_UI_hvacReqBlowerSegment(d) ((d & 0xf000000) >> 24)

enum UI_hvacReqBlowerSegment_Values { 
  OFF = 0,
  1 = 1,
  2 = 2,
  3 = 3,
  4 = 4,
  5 = 5,
  6 = 6,
  7 = 7,
  8 = 8,
  9 = 9,
  10 = 10,
  AUTO = 11
 };

#define Parse_UI_hvacReqRecirc(d) ((d & 0x300000) >> 20)

enum UI_hvacReqRecirc_Values { 
  AUTO = 0,
  RECIRC = 1,
  FRESH = 2
 };

#define Parse_UI_hvacReqACDisable(d) ((d & 0xc0000) >> 18)

enum UI_hvacReqACDisable_Values { 
  AUTO = 0,
  OFF = 1,
  ON = 2
 };

#define Parse_UI_hvacReqManualDefogState(d) ((d & 0x30000) >> 16)

enum UI_hvacReqManualDefogState_Values { 
  NONE = 0,
  DEFOG = 1,
  DEFROST = 2
 };

#define Parse_UI_hvacReqUserPowerState(d) ((d & 0x1c000) >> 14)

enum UI_hvacReqUserPowerState_Values { 
  OFF = 0,
  ON = 1,
  PRECONDITION = 2,
  OVERHEAT_PROTECT_FANONLY = 3,
  OVERHEAT_PROTECT = 4
 };

#define Parse_UI_hvacReqSecondRowState(d) ((d & 0x3800) >> 11)

enum UI_hvacReqSecondRowState_Values { 
  AUTO = 0,
  OFF = 1,
  LOW = 2,
  MED = 3,
  HIGH = 4
 };

#define Parse_UI_hvacUseModeledDuctTemp(d) ((d & 0x100) >> 8)
#define Parse_UI_hvacReqKeepClimateOn(d) ((d & 0x180) >> 7)

enum UI_hvacReqKeepClimateOn_Values { 
  KEEP_CLIMATE_ON_REQ_OFF = 0,
  KEEP_CLIMATE_ON_REQ_ON = 1,
  KEEP_CLIMATE_ON_REQ_DOG = 2,
  KEEP_CLIMATE_ON_REQ_PARTY = 3
 };

#define ID313UI_trackModeSettings 0x313

#define Parse_UI_trackModeRequest(d) ((d & 0xc) >> 2)

enum UI_trackModeRequest_Values { 
  TRACK_MODE_REQUEST_IDLE = 0,
  TRACK_MODE_REQUEST_ON = 1,
  TRACK_MODE_REQUEST_OFF = 2
 };

#define Parse_UI_trackRotationTendency(d) ((d & 0xff00000000000000) >> 56)
#define Parse_UI_trackStabilityAssist(d) ((d & 0xff000000000000) >> 48)
#define Parse_UI_trackPostCooling(d) ((d & 0x10000000000) >> 40)

enum UI_trackPostCooling_Values { 
  TRACK_MODE_POST_COOLING_OFF = 0,
  TRACK_MODE_POST_COOLING_ON = 1
 };

#define Parse_UI_trackCmpOverclock(d) ((d & 0x8000000000) >> 39)

enum UI_trackCmpOverclock_Values { 
  TRACK_MODE_CMP_OVERCLOCK_OFF = 0,
  TRACK_MODE_CMP_OVERCLOCK_ON = 1
 };

#define Parse_UI_trackModeSettingsCounter(d) ((d & 0xf000) >> 12)
#define Parse_UI_trackModeSettingsChecksum(d) ((d & 0xff00) >> 8)
#define ID335RearDIinfo 0x335

#define Parse_DIR_infoIndex(d) ((d & 0xff00) >> 8)

enum DIR_infoIndex_Values { 
  DI_INFO_DEPRECATED_0 = 0,
  DI_INFO_DEPRECATED_1 = 1,
  DI_INFO_DEPRECATED_2 = 2,
  DI_INFO_DEPRECATED_3 = 3,
  DI_INFO_DEPRECATED_4 = 4,
  DI_INFO_DEPRECATED_5 = 5,
  DI_INFO_DEPRECATED_6 = 6,
  DI_INFO_DEPRECATED_7 = 7,
  DI_INFO_DEPRECATED_8 = 8,
  DI_INFO_DEPRECATED_9 = 9,
  DI_INFO_BUILD_HWID_COMPONENTID = 10,
  DI_INFO_PCBAID_ASSYID_USAGEID = 11,
  DI_INFO_APP_CRC = 13,
  DI_INFO_BOOTLOADER_SVN = 14,
  DI_INFO_BOOTLOADER_CRC = 15,
  DI_INFO_SUBCOMPONENT = 16,
  DI_INFO_APP_GITHASH = 17,
  DI_INFO_BOOTLOADER_GITHASH = 18,
  DI_INFO_VERSION_DEPRECATED = 19,
  DI_INFO_UDS_PROTOCOL_BOOTCRC = 20,
  DI_INFO_SUBCOMPONENT2 = 23,
  DI_INFO_SUBCOMPONENT_GITHASH = 31,
  DI_INFO_END = 255
 };

#define Parse_DIR_appGitHash(d) ((d & 0xffffffffffffff000000000000) >> 48)
#define Parse_DIR_bootGitHash(d) ((d & 0xffffffffffffff000000000000) >> 48)
#define Parse_DIR_buildType(d) ((d & 0x700000000000000) >> 56)

enum DIR_buildType_Values { 
  INFO_UNKNOWN_BUILD = 0,
  INFO_PLATFORM_BUILD = 1,
  INFO_LOCAL_BUILD = 2,
  INFO_TRACEABLE_CI_BUILD = 3,
  INFO_MFG_BUILD = 4
 };

#define Parse_DIR_infoBootLdUdsProtocolVersion(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_oilPumpBuildType(d) ((d & 0x700000000000000) >> 56)

enum DIR_oilPumpBuildType_Values { 
  INFO_UNKNOWN_BUILD = 0,
  INFO_PLATFORM_BUILD = 1,
  INFO_LOCAL_BUILD = 2,
  INFO_TRACEABLE_CI_BUILD = 3,
  INFO_MFG_BUILD = 4
 };

#define Parse_DIR_platformTyp(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_subcomponentGitHash(d) ((d & 0xffffffffffffff000000000000) >> 48)
#define Parse_DIR_buildConfigurationId(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIR_oilPumpAppCrc(d) ((d & 0xffffffff0000) >> 16)
#define Parse_DIR_pcbaId(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIR_assemblyId(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIR_applicationCrc(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_DIR_bootloaderCrc(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_DIR_hardwareId(d) ((d & 0xff00000000) >> 32)

enum DIR_hardwareId_Values { 
  CONFIGURABLE_HWID_PLACEHOLDER = 252
 };

#define Parse_DIR_usageId(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIR_FPGA_version(d) ((d & 0xff0000) >> 16)

enum DIR_FPGA_version_Values { 
  FPGA_VERSION_LOCAL_BUILD = 254,
  FPGA_VERSION_SNA = 255
 };

#define Parse_DIR_componentId(d) ((d & 0xffff0000) >> 16)
#define Parse_DIR_subUsageId(d) ((d & 0xffff0000) >> 16)
#define ID383VCRIGHT_thsStatus 0x383

#define Parse_VCRIGHT_thsActive(d) ((d & 0x2) >> 1)
#define Parse_VCRIGHT_thsTemperature(d) ((d & 0x7f80) >> 7)

enum VCRIGHT_thsTemperature_Values { 
  SNA = 128
 };

#define Parse_VCRIGHT_thsHumidity(d) ((d & 0x7f800000000000) >> 47)

enum VCRIGHT_thsHumidity_Values { 
  SNA = 255
 };

#define Parse_VCRIGHT_estimatedVehicleSituatio(d) ((d & 0x600000000) >> 33)

enum VCRIGHT_estimatedVehicleSituatio_Values { 
  VEHICLE_SITUATION_UNKNOWN = 0,
  VEHICLE_SITUATION_INDOOR = 1,
  VEHICLE_SITUATION_OUTDOOR = 2
 };

#define Parse_VCRIGHT_thsSolarLoadInfrared(d) ((d & 0x1ff80000000) >> 31)

enum VCRIGHT_thsSolarLoadInfrared_Values { 
  SNA = 1023
 };

#define Parse_VCRIGHT_thsSolarLoadVisible(d) ((d & 0x7fe00000) >> 21)

enum VCRIGHT_thsSolarLoadVisible_Values { 
  SNA = 1023
 };

#define Parse_VCRIGHT_estimatedThsSolarLoad(d) ((d & 0x1ff800) >> 11)

enum VCRIGHT_estimatedThsSolarLoad_Values { 
  SNA = 1023
 };

#define ID3B3UI_vehicleControl2 0x3b3

#define Parse_UI_gloveboxRequest(d) ((d & 0x2) >> 1)
#define Parse_UI_trunkRequest(d) ((d & 0x80000000) >> 31)
#define Parse_UI_UMCUpdateInhibit(d) ((d & 0x40000000) >> 30)
#define Parse_UI_WCUpdateInhibit(d) ((d & 0x20000000) >> 29)
#define Parse_UI_soundHornOnLock(d) ((d & 0x10000000) >> 28)
#define Parse_UI_locksPanelActive(d) ((d & 0x8000000) >> 27)
#define Parse_UI_PINToDriveEnabled(d) ((d & 0x4000000) >> 26)
#define Parse_UI_PINToDrivePassed(d) ((d & 0x2000000) >> 25)
#define Parse_UI_lightSwitch(d) ((d & 0x3800000) >> 23)

enum UI_lightSwitch_Values { 
  LIGHT_SWITCH_AUTO = 0,
  LIGHT_SWITCH_ON = 1,
  LIGHT_SWITCH_PARKING = 2,
  LIGHT_SWITCH_OFF = 3,
  LIGHT_SWITCH_SNA = 4
 };

#define Parse_UI_readyToAddKey(d) ((d & 0x100000) >> 20)
#define Parse_UI_alarmTriggerRequest(d) ((d & 0x80000) >> 19)
#define Parse_UI_VCSECFeature1(d) ((d & 0x40000) >> 18)
#define Parse_UI_VCLEFTFeature1(d) ((d & 0x20000) >> 17)
#define Parse_UI_summonState(d) ((d & 0x30000) >> 16)

enum UI_summonState_Values { 
  SNA = 0,
  IDLE = 1,
  PRE_PRIMED = 2,
  ACTIVE = 3
 };

#define Parse_UI_userPresent(d) ((d & 0x4000) >> 14)
#define Parse_UI_freeRollModeRequest(d) ((d & 0x2000) >> 13)
#define Parse_UI_windowRequest(d) ((d & 0x7000) >> 12)

enum UI_windowRequest_Values { 
  WINDOW_REQUEST_IDLE = 0,
  WINDOW_REQUEST_GOTO_PERCENT = 1,
  WINDOW_REQUEST_GOTO_VENT = 2,
  WINDOW_REQUEST_GOTO_CLOSED = 3,
  WINDOW_REQUEST_GOTO_OPEN = 4
 };

#define Parse_UI_batteryPreconditioningRequest(d) ((d & 0x200) >> 9)
#define Parse_UI_coastDownMode(d) ((d & 0x100) >> 8)
#define Parse_UI_autopilotPowerStateRequest(d) ((d & 0x180) >> 7)

enum UI_autopilotPowerStateRequest_Values { 
  AUTOPILOT_NOMINAL = 0,
  AUTOPILOT_SENTRY = 1,
  AUTOPILOT_SUSPEND = 2
 };

#define Parse_UI_shorted12VCellTestMode(d) ((d & 0x60) >> 5)

enum UI_shorted12VCellTestMode_Values { 
  SHORTED_CELL_TEST_MODE_DISABLED = 0,
  SHORTED_CELL_TEST_MODE_SHADOW = 1,
  SHORTED_CELL_TEST_MODE_ACTIVE = 2
 };

#define Parse_UI_autoRollWindowsOnLockEnable(d) ((d & 0x8) >> 3)
#define Parse_UI_efuseMXResistanceEstArmed(d) ((d & 0x4) >> 2)
#define Parse_UI_conditionalLoggingEnabledVCSE(d) ((d & 0x2) >> 1)
#define ID3C3VCRIGHT_switchStatus 0x3c3

#define Parse_VCRIGHT_frontSeatTrackBack(d) ((d & 0xc) >> 2)

enum VCRIGHT_frontSeatTrackBack_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_frontSeatTrackForward(d) ((d & 0xc0000000000000) >> 54)

enum VCRIGHT_frontSeatTrackForward_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_frontSeatTiltDown(d) ((d & 0x30000000000000) >> 52)

enum VCRIGHT_frontSeatTiltDown_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_frontSeatTiltUp(d) ((d & 0xc000000000000) >> 50)

enum VCRIGHT_frontSeatTiltUp_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_frontSeatLiftDown(d) ((d & 0x3000000000000) >> 48)

enum VCRIGHT_frontSeatLiftDown_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_frontSeatLiftUp(d) ((d & 0xc00000000000) >> 46)

enum VCRIGHT_frontSeatLiftUp_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_frontSeatBackrestBack(d) ((d & 0x300000000000) >> 44)

enum VCRIGHT_frontSeatBackrestBack_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_frontSeatBackrestForward(d) ((d & 0xc0000000000) >> 42)

enum VCRIGHT_frontSeatBackrestForward_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_frontSeatLumbarDown(d) ((d & 0x30000000000) >> 40)

enum VCRIGHT_frontSeatLumbarDown_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_frontSeatLumbarUp(d) ((d & 0xc000000000) >> 38)

enum VCRIGHT_frontSeatLumbarUp_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_frontSeatLumbarIn(d) ((d & 0x3000000000) >> 36)

enum VCRIGHT_frontSeatLumbarIn_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_frontSeatLumbarOut(d) ((d & 0xc00000000) >> 34)

enum VCRIGHT_frontSeatLumbarOut_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_btnWindowUpRF(d) ((d & 0x100000000) >> 32)
#define Parse_VCRIGHT_btnWindowAutoUpRF(d) ((d & 0x80000000) >> 31)
#define Parse_VCRIGHT_btnWindowDownRF(d) ((d & 0x40000000) >> 30)
#define Parse_VCRIGHT_btnWindowAutoDownRF(d) ((d & 0x20000000) >> 29)
#define Parse_VCRIGHT_btnWindowUpRR(d) ((d & 0x10000000) >> 28)
#define Parse_VCRIGHT_btnWindowAutoUpRR(d) ((d & 0x8000000) >> 27)
#define Parse_VCRIGHT_btnWindowDownRR(d) ((d & 0x4000000) >> 26)
#define Parse_VCRIGHT_btnWindowAutoDownRR(d) ((d & 0x2000000) >> 25)
#define Parse_VCRIGHT_btnWindowSwPackUpLF(d) ((d & 0x1000000) >> 24)
#define Parse_VCRIGHT_btnWindowSwPackAutoUpLF(d) ((d & 0x800000) >> 23)
#define Parse_VCRIGHT_btnWindowSwPackDownLF(d) ((d & 0x400000) >> 22)
#define Parse_VCRIGHT_btnWindowSwPackAutoDwnLF(d) ((d & 0x200000) >> 21)
#define Parse_VCRIGHT_btnWindowSwPackUpLR(d) ((d & 0x100000) >> 20)
#define Parse_VCRIGHT_btnWindowSwPackAutoUpLR(d) ((d & 0x80000) >> 19)
#define Parse_VCRIGHT_btnWindowSwPackDownLR(d) ((d & 0x40000) >> 18)
#define Parse_VCRIGHT_btnWindowSwPackAutoDwnLR(d) ((d & 0x20000) >> 17)
#define Parse_VCRIGHT_frontBuckleSwitch(d) ((d & 0x30000) >> 16)

enum VCRIGHT_frontBuckleSwitch_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_frontOccupancySwitch(d) ((d & 0xc000) >> 14)

enum VCRIGHT_frontOccupancySwitch_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_rearCenterBuckleSwitch(d) ((d & 0x3000) >> 12)

enum VCRIGHT_rearCenterBuckleSwitch_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_rearRightBuckleSwitch(d) ((d & 0xc00) >> 10)

enum VCRIGHT_rearRightBuckleSwitch_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCRIGHT_trunkExtReleasePressed(d) ((d & 0x100) >> 8)
#define Parse_VCRIGHT_btnWindowSwPackUpRR(d) ((d & 0x80) >> 7)
#define Parse_VCRIGHT_btnWindowSwPackAutoUpRR(d) ((d & 0x40) >> 6)
#define Parse_VCRIGHT_btnWindowSwPackDownRR(d) ((d & 0x20) >> 5)
#define Parse_VCRIGHT_btnWindowSwPackAutoDwnRR(d) ((d & 0x10) >> 4)
#define Parse_VCRIGHT_liftgateShutfaceSwitchPr(d) ((d & 0x8) >> 3)
#define Parse_VCRIGHT_2RowSeatReclineSwitch(d) ((d & 0x4) >> 2)
#define ID3E3VCRIGHT_lightStatus 0x3e3

#define Parse_VCRIGHT_brakeLightStatus(d) ((d & 0xc) >> 2)

enum VCRIGHT_brakeLightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCRIGHT_tailLightStatus(d) ((d & 0xc000) >> 14)

enum VCRIGHT_tailLightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCRIGHT_turnSignalStatus(d) ((d & 0x3000) >> 12)

enum VCRIGHT_turnSignalStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCRIGHT_reverseLightStatus(d) ((d & 0xc00) >> 10)

enum VCRIGHT_reverseLightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCRIGHT_rearFogLightStatus(d) ((d & 0x300) >> 8)

enum VCRIGHT_rearFogLightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define Parse_VCRIGHT_interiorTrunkLightStatus(d) ((d & 0xc0) >> 6)

enum VCRIGHT_interiorTrunkLightStatus_Values { 
  LIGHT_OFF = 0,
  LIGHT_ON = 1,
  LIGHT_FAULT = 2,
  LIGHT_SNA = 3
 };

#define ID656FrontDIinfo 0x656

#define Parse_DIF_infoIndex(d) ((d & 0xff00) >> 8)

enum DIF_infoIndex_Values { 
  DI_INFO_DEPRECATED_0 = 0,
  DI_INFO_DEPRECATED_1 = 1,
  DI_INFO_DEPRECATED_2 = 2,
  DI_INFO_DEPRECATED_3 = 3,
  DI_INFO_DEPRECATED_4 = 4,
  DI_INFO_DEPRECATED_5 = 5,
  DI_INFO_DEPRECATED_6 = 6,
  DI_INFO_DEPRECATED_7 = 7,
  DI_INFO_DEPRECATED_8 = 8,
  DI_INFO_DEPRECATED_9 = 9,
  DI_INFO_BUILD_HWID_COMPONENTID = 10,
  DI_INFO_PCBAID_ASSYID_USAGEID = 11,
  DI_INFO_APP_CRC = 13,
  DI_INFO_BOOTLOADER_SVN = 14,
  DI_INFO_BOOTLOADER_CRC = 15,
  DI_INFO_SUBCOMPONENT = 16,
  DI_INFO_APP_GITHASH = 17,
  DI_INFO_BOOTLOADER_GITHASH = 18,
  DI_INFO_VERSION_DEPRECATED = 19,
  DI_INFO_UDS_PROTOCOL_BOOTCRC = 20,
  DI_INFO_SUBCOMPONENT2 = 23,
  DI_INFO_SUBCOMPONENT_GITHASH = 31,
  DI_INFO_END = 255
 };

#define Parse_DIF_appGitHash(d) ((d & 0xffffffffffffff000000000000) >> 48)
#define Parse_DIF_bootGitHash(d) ((d & 0xffffffffffffff000000000000) >> 48)
#define Parse_DIF_buildType(d) ((d & 0x700000000000000) >> 56)

enum DIF_buildType_Values { 
  INFO_UNKNOWN_BUILD = 0,
  INFO_PLATFORM_BUILD = 1,
  INFO_LOCAL_BUILD = 2,
  INFO_TRACEABLE_CI_BUILD = 3,
  INFO_MFG_BUILD = 4
 };

#define Parse_DIF_infoBootLdUdsProtocolVersion(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_oilPumpBuildType(d) ((d & 0x700000000000000) >> 56)

enum DIF_oilPumpBuildType_Values { 
  INFO_UNKNOWN_BUILD = 0,
  INFO_PLATFORM_BUILD = 1,
  INFO_LOCAL_BUILD = 2,
  INFO_TRACEABLE_CI_BUILD = 3,
  INFO_MFG_BUILD = 4
 };

#define Parse_DIF_platformTyp(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_subcomponentGitHash(d) ((d & 0xffffffffffffff000000000000) >> 48)
#define Parse_DIF_buildConfigurationId(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIF_oilPumpAppCrc(d) ((d & 0xffffffff0000) >> 16)
#define Parse_DIF_pcbaId(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIF_assemblyId(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIF_applicationCrc(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_DIF_bootloaderCrc(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_DIF_hardwareId(d) ((d & 0xff00000000) >> 32)

enum DIF_hardwareId_Values { 
  CONFIGURABLE_HWID_PLACEHOLDER = 252
 };

#define Parse_DIF_usageId(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIF_FPGA_version(d) ((d & 0xff0000) >> 16)

enum DIF_FPGA_version_Values { 
  FPGA_VERSION_LOCAL_BUILD = 254,
  FPGA_VERSION_SNA = 255
 };

#define Parse_DIF_componentId(d) ((d & 0xffff0000) >> 16)
#define Parse_DIF_subUsageId(d) ((d & 0xffff0000) >> 16)
#define ID300BMS_info 0x300

#define Parse_BMS_infoIndex(d) ((d & 0xff00) >> 8)

enum BMS_infoIndex_Values { 
  INFO_DEPRECATED_0 = 0,
  INFO_DEPRECATED_1 = 1,
  INFO_DEPRECATED_2 = 2,
  INFO_DEPRECATED_3 = 3,
  INFO_DEPRECATED_4 = 4,
  INFO_DEPRECATED_5 = 5,
  INFO_DEPRECATED_6 = 6,
  INFO_DEPRECATED_7 = 7,
  INFO_DEPRECATED_8 = 8,
  INFO_DEPRECATED_9 = 9,
  INFO_BUILD_HWID_COMPONENTID = 10,
  INFO_PCBAID_ASSYID_USAGEID = 11,
  INFO_APP_CRC = 13,
  INFO_BOOTLOADER_SVN = 14,
  INFO_BOOTLOADER_CRC = 15,
  INFO_SUBCOMPONENT = 16,
  INFO_APP_GITHASH = 17,
  INFO_BOOTLOADER_GITHASH = 18,
  INFO_VERSION_DEPRECATED = 19,
  INFO_UDS_PROTOCOL_BOOTCRC = 20,
  INFO_VARIANTCRC = 22,
  INFO_PACKAGE_PN_1_7 = 25,
  INFO_PACKAGE_PN_8_14 = 26,
  INFO_PACKAGE_PN_15_20 = 27,
  INFO_PACKAGE_SN_1_7 = 29,
  INFO_PACKAGE_SN_8_14 = 30,
  INFO_SUBCOMPONENT_GITHASH = 31,
  INFO_END = 255
 };

#define Parse_BMS_appGitHash(d) ((d & 0xffffffffffffff000000000000) >> 48)
#define Parse_BMS_bootGitHash(d) ((d & 0xffffffffffffff000000000000) >> 48)
#define Parse_BMS_bootUdsProtoVersion(d) ((d & 0xff00000000000000) >> 56)
#define Parse_BMS_buildType(d) ((d & 0x700000000000000) >> 56)

enum BMS_buildType_Values { 
  INFO_UNKNOWN_BUILD = 0,
  INFO_PLATFORM_BUILD = 1,
  INFO_LOCAL_BUILD = 2,
  INFO_TRACEABLE_CI_BUILD = 3,
  INFO_MFG_BUILD = 4
 };

#define Parse_BMS_platformType(d) ((d & 0xff00000000000000) >> 56)
#define Parse_BMS_buildConfigId(d) ((d & 0xffff000000000000) >> 48)
#define Parse_BMS_pcbaId(d) ((d & 0xff000000000000) >> 48)
#define Parse_BMS_assemblyId(d) ((d & 0xff0000000000) >> 40)
#define Parse_BMS_appCrc(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_BMS_bootCrc(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_BMS_hardwareId(d) ((d & 0xffff00000000) >> 32)
#define Parse_BMS_usageId(d) ((d & 0xffff00000000) >> 32)
#define Parse_BMS_componentId(d) ((d & 0xffff0000) >> 16)
#define Parse_BMS_subUsageId(d) ((d & 0xffff0000) >> 16)
#define ID212BMS_status 0x212

#define Parse_BMS_hvacPowerRequest(d) ((d & 0x2) >> 1)
#define Parse_BMS_notEnoughPowerForDrive(d) ((d & 0x8000000000000000) >> 63)
#define Parse_BMS_notEnoughPowerForSupport(d) ((d & 0x4000000000000000) >> 62)
#define Parse_BMS_preconditionAllowed(d) ((d & 0x2000000000000000) >> 61)
#define Parse_BMS_updateAllowed(d) ((d & 0x1000000000000000) >> 60)
#define Parse_BMS_activeHeatingWorthwhile(d) ((d & 0x800000000000000) >> 59)
#define Parse_BMS_cpMiaOnHvs(d) ((d & 0x400000000000000) >> 58)
#define Parse_BMS_pcsPwmEnabled(d) ((d & 0x200000000000000) >> 57)
#define Parse_BMS_contactorState(d) ((d & 0x700000000000000) >> 56)

enum BMS_contactorState_Values { 
  BMS_CTRSET_SNA = 0,
  BMS_CTRSET_OPEN = 1,
  BMS_CTRSET_OPENING = 2,
  BMS_CTRSET_CLOSING = 3,
  BMS_CTRSET_CLOSED = 4,
  BMS_CTRSET_WELDED = 5,
  BMS_CTRSET_BLOCKED = 6
 };

#define Parse_BMS_uiChargeStatus(d) ((d & 0xe0000000000000) >> 53)

enum BMS_uiChargeStatus_Values { 
  BMS_DISCONNECTED = 0,
  BMS_NO_POWER = 1,
  BMS_ABOUT_TO_CHARGE = 2,
  BMS_CHARGING = 3,
  BMS_CHARGE_COMPLETE = 4,
  BMS_CHARGE_STOPPED = 5
 };

#define Parse_BMS_ecuLogUploadRequest(d) ((d & 0xc000000000000) >> 50)

enum BMS_ecuLogUploadRequest_Values { 
  REQUEST_PRIORITY_NONE = 0,
  REQUEST_PRIORITY_1 = 1,
  REQUEST_PRIORITY_2 = 2,
  REQUEST_PRIORITY_3 = 3
 };

#define Parse_BMS_hvState(d) ((d & 0x7000000000000) >> 48)

enum BMS_hvState_Values { 
  HV_DOWN = 0,
  HV_COMING_UP = 1,
  HV_GOING_DOWN = 2,
  HV_UP_FOR_DRIVE = 3,
  HV_UP_FOR_CHARGE = 4,
  HV_UP_FOR_DC_CHARGE = 5,
  HV_UP = 6
 };

#define Parse_BMS_isolationResistance(d) ((d & 0x7fe00000000000) >> 45)

enum BMS_isolationResistance_Values { 
  SNA = 1023
 };

#define Parse_BMS_chargeRequest(d) ((d & 0x800000000) >> 35)
#define Parse_BMS_keepWarmRequest(d) ((d & 0x400000000) >> 34)
#define Parse_BMS_state(d) ((d & 0xf00000000) >> 32)

enum BMS_state_Values { 
  BMS_STANDBY = 0,
  BMS_DRIVE = 1,
  BMS_SUPPORT = 2,
  BMS_CHARGE = 3,
  BMS_FEIM = 4,
  BMS_CLEAR_FAULT = 5,
  BMS_FAULT = 6,
  BMS_WELD = 7,
  BMS_TEST = 8,
  BMS_SNA = 9,
  BMS_DIAG = 10
 };

#define Parse_BMS_diLimpRequest(d) ((d & 0x10000000) >> 28)

enum BMS_diLimpRequest_Values { 
  LIMP_REQUEST_NONE = 0,
  LIMP_REQUEST_WELDED = 1
 };

#define Parse_BMS_okToShipByAir(d) ((d & 0x8000000) >> 27)
#define Parse_BMS_okToShipByLand(d) ((d & 0x4000000) >> 26)
#define Parse_BMS_chgPowerAvailable(d) ((d & 0x7ff000000) >> 24)

enum BMS_chgPowerAvailable_Values { 
  SNA = 2047
 };

#define Parse_BMS_chargeRetryCount(d) ((d & 0xe000) >> 13)
#define Parse_BMS_smStateRequest(d) ((d & 0xf00) >> 8)

enum BMS_smStateRequest_Values { 
  BMS_STANDBY = 0,
  BMS_DRIVE = 1,
  BMS_SUPPORT = 2,
  BMS_CHARGE = 3,
  BMS_FEIM = 4,
  BMS_CLEAR_FAULT = 5,
  BMS_FAULT = 6,
  BMS_WELD = 7,
  BMS_TEST = 8,
  BMS_SNA = 9,
  BMS_DIAG = 10
 };

#define ID31CCC_chgStatus 0x31c

#define Parse_CC_currentLimit(d) ((d & 0xff00) >> 8)

enum CC_currentLimit_Values { 
  SNA = 255
 };

#define Parse_CC_pilotState(d) ((d & 0x300000000000000) >> 56)

enum CC_pilotState_Values { 
  CC_PILOT_STATE_READY = 0,
  CC_PILOT_STATE_IDLE = 1,
  CC_PILOT_STATE_FAULTED = 2,
  CC_PILOT_STATE_SNA = 3
 };

#define Parse_CC_numPhases(d) ((d & 0xc0000000000000) >> 54)

enum CC_numPhases_Values { 
  SNA = 0
 };

#define Parse_CC_line1Voltage(d) ((d & 0x1ff000000000000) >> 48)

enum CC_line1Voltage_Values { 
  SNA = 511
 };

#define Parse_CC_gridGrounding(d) ((d & 0xc000000000) >> 38)

enum CC_gridGrounding_Values { 
  CC_GRID_GROUNDING_TN_TT = 0,
  CC_GRID_GROUNDING_IT_SplitPhase = 1,
  CC_GRID_GROUNDING_SNA = 2
 };

#define Parse_CC_deltaTransformer(d) ((d & 0x3000000000) >> 36)
#define Parse_CC_numVehCharging(d) ((d & 0x1c00000000) >> 34)
#define Parse_CC_line2Voltage(d) ((d & 0xff80000000) >> 31)

enum CC_line2Voltage_Values { 
  SNA = 511
 };

#define Parse_CC_line3Voltage(d) ((d & 0x7fc00000) >> 22)

enum CC_line3Voltage_Values { 
  SNA = 511
 };

#define ID23DCP_chargeStatus 0x23d

#define Parse_CP_hvChargeStatus(d) ((d & 0x38) >> 3)

enum CP_hvChargeStatus_Values { 
  CP_CHARGE_INACTIVE = 0,
  CP_CHARGE_CONNECTED = 1,
  CP_CHARGE_STANDBY = 2,
  CP_EXT_EVSE_TEST_ACTIVE = 3,
  CP_EVSE_TEST_PASSED = 4,
  CP_CHARGE_ENABLED = 5,
  CP_CHARGE_FAULTED = 6
 };

#define Parse_CP_chargeShutdownRequest(d) ((d & 0x60000000) >> 29)

enum CP_chargeShutdownRequest_Values { 
  NO_SHUTDOWN_REQUESTED = 0,
  GRACEFUL_SHUTDOWN_REQUESTED = 1,
  EMERGENCY_SHUTDOWN_REQUESTED = 2
 };

#define Parse_CP_acChargeCurrentLimit(d) ((d & 0xff000000) >> 24)
#define Parse_CP_internalMaxCurrentLimit(d) ((d & 0x1fff0000) >> 16)
#define Parse_CP_vehicleIsoCheckRequired(d) ((d & 0x8) >> 3)
#define Parse_CP_vehiclePrechargeRequired(d) ((d & 0x4) >> 2)
#define ID13DCP_chargeStatus 0x13d

#define Parse_CP_hvChargeStatus(d) ((d & 0x38) >> 3)

enum CP_hvChargeStatus_Values { 
  CP_CHARGE_INACTIVE = 0,
  CP_CHARGE_CONNECTED = 1,
  CP_CHARGE_STANDBY = 2,
  CP_EXT_EVSE_TEST_ACTIVE = 3,
  CP_EVSE_TEST_PASSED = 4,
  CP_CHARGE_ENABLED = 5,
  CP_CHARGE_FAULTED = 6
 };

#define Parse_CP_chargeShutdownRequest(d) ((d & 0x600000000000) >> 45)

enum CP_chargeShutdownRequest_Values { 
  NO_SHUTDOWN_REQUESTED = 0,
  GRACEFUL_SHUTDOWN_REQUESTED = 1,
  EMERGENCY_SHUTDOWN_REQUESTED = 2
 };

#define Parse_CP_acChargeCurrentLimit(d) ((d & 0xff0000000000) >> 40)
#define Parse_CP_internalMaxDcCurrentLimit(d) ((d & 0x1fff00000000) >> 32)
#define Parse_CP_vehicleIsoCheckRequired(d) ((d & 0x80000) >> 19)
#define Parse_CP_vehiclePrechargeRequired(d) ((d & 0x40000) >> 18)
#define Parse_CP_internalMaxAcCurrentLimit(d) ((d & 0xff0000) >> 16)
#define Parse_CP_evseChargeType(d) ((d & 0x300) >> 8)

enum CP_evseChargeType_Values { 
  NO_CHARGER_PRESENT = 0,
  DC_CHARGER_PRESENT = 1,
  AC_CHARGER_PRESENT = 2
 };

#define ID43DCP_chargeStatusLog 0x43d

#define Parse_CP_hvChargeStatus_log(d) ((d & 0x38) >> 3)

enum CP_hvChargeStatus_log_Values { 
  CP_CHARGE_INACTIVE = 0,
  CP_CHARGE_CONNECTED = 1,
  CP_CHARGE_STANDBY = 2,
  CP_EXT_EVSE_TEST_ACTIVE = 3,
  CP_EVSE_TEST_PASSED = 4,
  CP_CHARGE_ENABLED = 5,
  CP_CHARGE_FAULTED = 6
 };

#define Parse_CP_chargeShutdownRequest_log(d) ((d & 0x600000000000) >> 45)

enum CP_chargeShutdownRequest_log_Values { 
  NO_SHUTDOWN_REQUESTED = 0,
  GRACEFUL_SHUTDOWN_REQUESTED = 1,
  EMERGENCY_SHUTDOWN_REQUESTED = 2
 };

#define Parse_CP_acChargeCurrentLimit_log(d) ((d & 0xff0000000000) >> 40)
#define Parse_CP_internalMaxDcCurrentLimit_log(d) ((d & 0x1fff00000000) >> 32)
#define Parse_CP_vehicleIsoCheckRequired_log(d) ((d & 0x80000) >> 19)
#define Parse_CP_vehiclePrechargeRequired_log(d) ((d & 0x40000) >> 18)
#define Parse_CP_internalMaxAcCurrentLimit_log(d) ((d & 0xff0000) >> 16)
#define Parse_CP_evseChargeType_log(d) ((d & 0x300) >> 8)

enum CP_evseChargeType_log_Values { 
  NO_CHARGER_PRESENT = 0,
  DC_CHARGER_PRESENT = 1,
  AC_CHARGER_PRESENT = 2
 };

#define ID21DCP_evseStatus 0x21d

#define Parse_CP_evseAccept(d) ((d & 0x2) >> 1)
#define Parse_CP_evseRequest(d) ((d & 0x8000000000000000) >> 63)
#define Parse_CP_proximity(d) ((d & 0xc000000000000000) >> 62)

enum CP_proximity_Values { 
  CHG_PROXIMITY_SNA = 0,
  CHG_PROXIMITY_DISCONNECTED = 1,
  CHG_PROXIMITY_UNLATCHED = 2,
  CHG_PROXIMITY_LATCHED = 3
 };

#define Parse_CP_pilot(d) ((d & 0x7000000000000000) >> 60)

enum CP_pilot_Values { 
  CHG_PILOT_NONE = 0,
  CHG_PILOT_FAULTED = 1,
  CHG_PILOT_LINE_CHARGE = 2,
  CHG_PILOT_FAST_CHARGE = 3,
  CHG_PILOT_IDLE = 4,
  CHG_PILOT_INVALID = 5,
  CHG_PILOT_UNUSED_6 = 6,
  CHG_PILOT_SNA = 7
 };

#define Parse_CP_pilotCurrent(d) ((d & 0xff00000000000000) >> 56)
#define Parse_CP_cableType(d) ((d & 0x7000000000000) >> 48)

enum CP_cableType_Values { 
  CHG_CABLE_TYPE_IEC = 0,
  CHG_CABLE_TYPE_SAE = 1,
  CHG_CABLE_TYPE_GB_AC = 2,
  CHG_CABLE_TYPE_GB_DC = 3,
  CHG_CABLE_TYPE_SNA = 4
 };

#define Parse_CP_cableCurrentLimit(d) ((d & 0x7f0000000000) >> 40)
#define Parse_CP_digitalCommsAttempts(d) ((d & 0x300000000) >> 32)
#define Parse_CP_teslaSwcanState(d) ((d & 0x1c0000000) >> 30)

enum CP_teslaSwcanState_Values { 
  TESLA_SWCAN_INACTIVE = 0,
  TESLA_SWCAN_ACCEPT = 1,
  TESLA_SWCAN_RECEIVE = 2,
  TESLA_SWCAN_ESTABLISHED = 3,
  TESLA_SWCAN_FAULT = 4,
  TESLA_SWCAN_GO_TO_SLEEP = 5,
  TESLA_SWCAN_OFFBOARD_UPDATE_IN_PROGRESS = 6
 };

#define Parse_CP_digitalCommsEstablished(d) ((d & 0x8000000) >> 27)
#define Parse_CP_evseChargeType_UI(d) ((d & 0xc000000) >> 26)

enum CP_evseChargeType_UI_Values { 
  NO_CHARGER_PRESENT = 0,
  DC_CHARGER_PRESENT = 1,
  AC_CHARGER_PRESENT = 2
 };

#define Parse_CP_acNumRetries(d) ((d & 0x3000000) >> 24)
#define Parse_CP_gbState(d) ((d & 0x3c00000) >> 22)

enum CP_gbState_Values { 
  GBDC_INACTIVE = 0,
  GBDC_WAIT_FOR_COMMS = 1,
  GBDC_COMMS_RECEIVED = 2,
  GBDC_HANDSHAKING_EXT_ISO = 3,
  GBDC_RECOGNITION = 4,
  GBDC_CHARGE_PARAM_CONFIG = 5,
  GBDC_VEH_PACK_PRECHARGE = 6,
  GBDC_READY_TO_CHARGE = 7,
  GBDC_CHARGING = 8,
  GBDC_STOP_CHARGE_REQUESTED = 9,
  GBDC_CHARGE_DISABLING = 10,
  GBDC_END_OF_CHARGE = 11,
  GBDC_ERROR_HANDLING = 12,
  GBDC_RETRY_CHARGE = 13,
  GBDC_FAULTED = 14,
  GBDC_TESTER_PRESENT = 15
 };

#define Parse_CP_gbdcStopChargeReason(d) ((d & 0x1c0000) >> 18)

enum CP_gbdcStopChargeReason_Values { 
  GBDC_STOP_REASON_NONE = 0,
  GBDC_VEH_REQUESTED = 1,
  GBDC_EVSE_REQUESTED = 2,
  GBDC_COMMS_TIMEOUT = 3,
  GBDC_EVSE_FAULT = 4,
  GBDC_EVSE_CRITICAL_FAULT = 5,
  GBDC_LIVE_DISCONNECT = 6,
  GBDC_SUPERCHARGER_COMMS_TIMEOUT = 7
 };

#define Parse_CP_gbdcFailureReason(d) ((d & 0x18000) >> 15)

enum CP_gbdcFailureReason_Values { 
  GBDC_FAILURE_NONE = 0,
  GBDC_ATTEMPTS_EXPIRED = 1,
  GBDC_SHUTDOWN_FAILURE = 2,
  GBDC_CRITICAL_FAILURE = 3
 };

#define Parse_CP_gbdcChargeAttempts(d) ((d & 0x6000) >> 13)
#define Parse_CP_acChargeState(d) ((d & 0x3800) >> 11)

enum CP_acChargeState_Values { 
  AC_CHARGE_INACTIVE = 0,
  AC_CHARGE_CONNECTED_CHARGE_BLOCKED = 1,
  AC_CHARGE_STANDBY = 2,
  AC_CHARGE_ENABLED = 3,
  AC_CHARGE_ONBOARD_CHARGER_SHUTDOWN = 4,
  AC_CHARGE_VEH_SHUTDOWN = 5,
  AC_CHARGE_FAULT = 6
 };

#define Parse_CP_teslaDcState(d) ((d & 0xf00) >> 8)

enum CP_teslaDcState_Values { 
  TESLA_DC_INACTIVE = 0,
  TESLA_DC_CONNECTED_CHARGE_BLOCKED = 1,
  TESLA_DC_STANDBY = 2,
  TESLA_DC_EXT_TESTS_ENABLED = 3,
  TESLA_DC_EXT_TEST_ACTIVE = 4,
  TESLA_DC_EXT_PRECHARGE_ACTIVE = 5,
  TESLA_DC_ENABLED = 6,
  TESLA_DC_EVSE_SHUTDOWN = 7,
  TESLA_DC_VEH_SHUTDOWN = 8,
  TESLA_DC_EMERGENCY_SHUTDOWN = 9,
  TESLA_DC_FAULT = 10
 };

#define Parse_CP_iecComboState(d) ((d & 0xf0) >> 4)

enum CP_iecComboState_Values { 
  IEC_COMBO_INACTIVE = 0,
  IEC_COMBO_CONNECTED = 1,
  IEC_COMBO_V2G_SESSION_SETUP = 2,
  IEC_COMBO_SERVICE_DISCOVERY = 3,
  IEC_COMBO_PAYMENT_SELECTION = 4,
  IEC_COMBO_CHARGE_PARAM_DISCOVERY = 5,
  IEC_COMBO_CABLE_CHECK = 6,
  IEC_COMBO_PRECHARGE = 7,
  IEC_COMBO_ENABLED = 8,
  IEC_COMBO_SHUTDOWN = 9,
  IEC_COMBO_END_OF_CHARGE = 10,
  IEC_COMBO_FAULT = 11,
  IEC_COMBO_WAIT_RESTART = 12
 };

#define ID743VCRIGHT_recallStatus 0x743

#define Parse_VCRIGHT_systemRecallStatus(d) ((d & 0xc) >> 2)

enum VCRIGHT_systemRecallStatus_Values { 
  RECALL_SNA = 0,
  RECALL_IN_PROGRESS = 1,
  RECALL_COMPLETE = 2,
  RECALL_INTERRUPTED = 3
 };

#define Parse_VCRIGHT_seatRecallStatus(d) ((d & 0xc0) >> 6)

enum VCRIGHT_seatRecallStatus_Values { 
  RECALL_SNA = 0,
  RECALL_IN_PROGRESS = 1,
  RECALL_COMPLETE = 2,
  RECALL_INTERRUPTED = 3
 };

#define Parse_VCRIGHT_mirrorRecallStatus(d) ((d & 0x30) >> 4)

enum VCRIGHT_mirrorRecallStatus_Values { 
  RECALL_SNA = 0,
  RECALL_IN_PROGRESS = 1,
  RECALL_COMPLETE = 2,
  RECALL_INTERRUPTED = 3
 };

#define ID75DCP_sensorData 0x75d

#define Parse_CP_sensorDataSelect(d) ((d & 0xf0) >> 4)

enum CP_sensorDataSelect_Values { 
  CP_SENSOR_DOOR_COUNTS = 0,
  CP_SENSOR_PIN_TEMP = 1,
  CP_SENSOR_DOOR = 2,
  CP_SENSOR_LATCH = 3,
  CP_SENSOR_INDUCTIVE_DOOR = 4,
  CP_SENSOR_SAFETY = 5,
  CP_SENSOR_UHF = 6,
  CP_SENSOR_RAILS = 7,
  CP_SENSOR_PROX = 8,
  CP_SENSOR_PILOT = 9,
  CP_SENSOR_PILOT2 = 10,
  CP_SENSOR_PROX_GB = 11,
  CP_SENSOR_INLET_HARNESS_ID = 12
 };

#define Parse_CP_UHF_chipState(d) ((d & 0x7000000000000000) >> 60)
#define Parse_CP_doorPot(d) ((d & 0xfff00) >> 8)
#define Parse_CP_faultLineV(d) ((d & 0xfff00) >> 8)
#define Parse_CP_inductiveSensor_raw(d) ((d & 0xfffffff000000) >> 24)
#define Parse_CP_inlet1HarnessIdState(d) ((d & 0x3000000000000000) >> 60)

enum CP_inlet1HarnessIdState_Values { 
  HARNESS_PEDIGREE_UNKNOWN_SNA = 0,
  HARNESS_PEDIGREE_INVALID = 1,
  HARNESS_PEDIGREE_VALID = 2
 };

#define Parse_CP_latchI(d) ((d & 0xfff00) >> 8)
#define Parse_CP_pilotLowValue_intervalMax10s(d) ((d & 0xfff00) >> 8)
#define Parse_CP_pilotPulseWidth(d) ((d & 0xfff00) >> 8)
#define Parse_CP_UHF_rssi(d) ((d & 0xff00000000000000) >> 56)
#define Parse_CP_doorCountsFiltered(d) ((d & 0xffff00) >> 8)
#define Parse_CP_inlet1HarnessIdValue(d) ((d & 0x700000000000000) >> 56)
#define Parse_CP_pinTemperature1(d) ((d & 0xff00000000000000) >> 56)
#define Parse_CP_proximityV(d) ((d & 0xffff00) >> 8)
#define Parse_CP_proximityV_GBCC1(d) ((d & 0xfffc0) >> 6)
#define Parse_CP_refVoltage(d) ((d & 0xff00000000000000) >> 56)
#define Parse_CP_UHF_rxOverflow(d) ((d & 0x1000000000000) >> 48)
#define Parse_CP_backCoverCounts(d) ((d & 0xffff000000000000) >> 48)
#define Parse_CP_doorI(d) ((d & 0xfff000000000000) >> 48)
#define Parse_CP_inlet1HarnessV(d) ((d & 0x3fff000000000000) >> 48)
#define Parse_CP_latch2I(d) ((d & 0xfff000000000000) >> 48)
#define Parse_CP_pilotLowValue_intervalMin10s(d) ((d & 0xfff000000000000) >> 48)
#define Parse_CP_pilotPeriod(d) ((d & 0xfff000000000000) >> 48)
#define Parse_CP_pinTemperature2(d) ((d & 0xff000000000000) >> 48)
#define Parse_CP_UHF_rxNumBytes(d) ((d & 0xff0000000000) >> 40)
#define Parse_CP_doorCountsDebounced(d) ((d & 0xffff0000000000) >> 40)
#define Parse_CP_pinTemperature3(d) ((d & 0xff0000000000) >> 40)
#define Parse_CP_proximityV_GBCC2(d) ((d & 0x3fff0000000000) >> 40)
#define Parse_CP_proximityV_intervalMin10s(d) ((d & 0xffff0000000000) >> 40)
#define Parse_CP_doorLastRequestMaxI(d) ((d & 0xfff000000000) >> 36)
#define Parse_CP_pilotHighValue_intervalMax10s(d) ((d & 0xfff000000000) >> 36)
#define Parse_CP_pilotLowValue(d) ((d & 0xfff000000000) >> 36)
#define Parse_CP_inlet2HarnessIdState(d) ((d & 0xc00000000) >> 34)

enum CP_inlet2HarnessIdState_Values { 
  HARNESS_PEDIGREE_UNKNOWN_SNA = 0,
  HARNESS_PEDIGREE_INVALID = 1,
  HARNESS_PEDIGREE_VALID = 2
 };

#define Parse_CP_UHF_fifoData(d) ((d & 0xff00000000) >> 32)
#define Parse_CP_backCover2Counts(d) ((d & 0xffff00000000) >> 32)
#define Parse_CP_boardTemperature(d) ((d & 0xff00000000) >> 32)
#define Parse_CP_inlet2HarnessIdValue(d) ((d & 0x700000000) >> 32)
#define Parse_CP_UHF_selfTestRssi(d) ((d & 0xff000000) >> 24)
#define Parse_CP_inlet2HarnessV(d) ((d & 0x3fff000000) >> 24)
#define Parse_CP_pilotHighValue(d) ((d & 0xfff000000) >> 24)
#define Parse_CP_pilotHighValue_intervalMin10s(d) ((d & 0xfff000000) >> 24)
#define Parse_CP_proximityV_intervalMax10s(d) ((d & 0xffff000000) >> 24)
#define Parse_CP_proxConn1Sense(d) ((d & 0x100) >> 8)
#define Parse_CP_proxEn(d) ((d & 0x80) >> 7)
#define ID287PTCcabinHeatSensorStatus 0x287

#define Parse_PTC_leftTempIGBT(d) ((d & 0xff00) >> 8)
#define Parse_PTC_tempOCP(d) ((d & 0xff00000000000000) >> 56)
#define Parse_PTC_rightTempIGBT(d) ((d & 0xff000000000000) >> 48)
#define Parse_PTC_tempPCB(d) ((d & 0xff0000000000) >> 40)
#define Parse_PTC_voltageHV(d) ((d & 0x3ff00000000) >> 32)
#define Parse_PTC_leftCurrentHV(d) ((d & 0xff0000) >> 16)
#define Parse_PTC_rightCurrentHV(d) ((d & 0xff00) >> 8)
#define ID333UI_chargeRequest 0x333

#define Parse_UI_openChargePortDoorRequest(d) ((d & 0x2) >> 1)
#define Parse_UI_closeChargePortDoorRequest(d) ((d & 0x8000000000) >> 39)
#define Parse_UI_chargeEnableRequest(d) ((d & 0x4000000000) >> 38)
#define Parse_UI_brickVLoggingRequest(d) ((d & 0x2000000000) >> 37)

enum UI_brickVLoggingRequest_Values { 
  FALSE = 0,
  TRUE = 1
 };

#define Parse_UI_brickBalancingDisabled(d) ((d & 0x1000000000) >> 36)

enum UI_brickBalancingDisabled_Values { 
  FALSE = 0,
  TRUE = 1
 };

#define Parse_UI_acChargeCurrentLimit(d) ((d & 0x7f00000000) >> 32)

enum UI_acChargeCurrentLimit_Values { 
  SNA = 127
 };

#define Parse_UI_chargeTerminationPct(d) ((d & 0x3ff000000) >> 24)
#define Parse_UI_smartAcChargingEnabled(d) ((d & 0x4000) >> 14)
#define Parse_UI_scheduledDepartureEnabled(d) ((d & 0x2000) >> 13)
#define Parse_UI_socSnapshotExpirationTime(d) ((d & 0xf000) >> 12)
#define Parse_UI_cpInletHeaterRequest(d) ((d & 0x100) >> 8)
#define ID334UI_powertrainControl 0x334

#define Parse_UI_systemPowerLimit(d) ((d & 0x3e0) >> 5)

enum UI_systemPowerLimit_Values { 
  SNA = 31
 };

#define Parse_UI_pedalMap(d) ((d & 0x1800000000000000) >> 59)

enum UI_pedalMap_Values { 
  CHILL = 0,
  SPORT = 1,
  PERFORMANCE = 2
 };

#define Parse_UI_systemTorqueLimit(d) ((d & 0x3f00000000000000) >> 56)

enum UI_systemTorqueLimit_Values { 
  SNA = 63
 };

#define Parse_UI_closureConfirmed(d) ((d & 0xc000000000000) >> 50)

enum UI_closureConfirmed_Values { 
  CONFIRMED_NONE = 0,
  CONFIRMED_FRUNK = 1,
  CONFIRMED_PROX = 2
 };

#define Parse_UI_speedLimit(d) ((d & 0xff000000000000) >> 48)

enum UI_speedLimit_Values { 
  SNA = 255
 };

#define Parse_UI_regenTorqueMax(d) ((d & 0xff0000000000) >> 40)
#define Parse_UI_limitMode(d) ((d & 0x300000000) >> 32)

enum UI_limitMode_Values { 
  LIMIT_NORMAL = 0,
  LIMIT_VALET = 1,
  LIMIT_FACTORY = 2,
  LIMIT_SERVICE = 3
 };

#define Parse_UI_motorOnMode(d) ((d & 0xc0000000) >> 30)

enum UI_motorOnMode_Values { 
  MOTORONMODE_NORMAL = 0,
  MOTORONMODE_FRONT_ONLY = 1,
  MOTORONMODE_REAR_ONLY = 2
 };

#define Parse_UI_wasteMode(d) ((d & 0x30000000) >> 28)

enum UI_wasteMode_Values { 
  WASTE_TYPE_NONE = 0,
  WASTE_TYPE_PARTIAL = 1,
  WASTE_TYPE_FULL = 2
 };

#define Parse_UI_wasteModeRegenLimit(d) ((d & 0xc000000) >> 26)

enum UI_wasteModeRegenLimit_Values { 
  MAX_REGEN_CURRENT_MAX = 0,
  MAX_REGEN_CURRENT_30A = 1,
  MAX_REGEN_CURRENT_10A = 2,
  MAX_REGEN_CURRENT_0A = 3
 };

#define Parse_UI_stoppingMode(d) ((d & 0x3000000) >> 24)

enum UI_stoppingMode_Values { 
  STANDARD = 0,
  CREEP = 1,
  HOLD = 2
 };

#define Parse_UI_DIAppSliderDebug(d) ((d & 0xc00000) >> 22)
#define Parse_UI_powertrainControlCounter(d) ((d & 0xf000) >> 12)
#define Parse_UI_powertrainControlChecksum(d) ((d & 0xff00) >> 8)
#define ID33AUI_rangeSOC 0x33a

#define Parse_UI_Range(d) ((d & 0xffc00) >> 10)
#define Parse_UI_idealRange(d) ((d & 0x3ff000000000000) >> 48)
#define Parse_UI_ratedWHpM(d) ((d & 0x3ff00000000) >> 32)
#define Parse_UI_SOC(d) ((d & 0x7f0000) >> 16)
#define Parse_UI_uSOE(d) ((d & 0x7f00) >> 8)
#define ID241VCFRONT_coolant 0x241

#define Parse_VCFRONT_coolantFlowBatActual(d) ((d & 0x3fe00) >> 9)
#define Parse_VCFRONT_coolantFlowBatTarget(d) ((d & 0xff800000000000) >> 47)
#define Parse_VCFRONT_coolantFlowBatReason(d) ((d & 0x3c000000000) >> 38)

enum VCFRONT_coolantFlowBatReason_Values { 
  NONE = 0,
  COOLANT_AIR_PURGE = 1,
  NO_FLOW_REQ = 2,
  OVERRIDE_BATT = 3,
  ACTIVE_MANAGER_BATT = 4,
  PASSIVE_MANAGER_BATT = 5,
  BMS_FLOW_REQ = 6,
  DAS_FLOW_REQ = 7,
  OVERRIDE_PT = 8,
  ACTIVE_MANAGER_PT = 9,
  PASSIVE_MANAGER_PT = 10,
  PCS_FLOW_REQ = 11,
  DI_FLOW_REQ = 12,
  DIS_FLOW_REQ = 13,
  HP_FLOW_REQ = 14
 };

#define Parse_VCFRONT_coolantFlowPTActual(d) ((d & 0x7fc00000000) >> 34)
#define Parse_VCFRONT_coolantFlowPTTarget(d) ((d & 0x3fe000000) >> 25)
#define Parse_VCFRONT_coolantFlowPTReason(d) ((d & 0xf0000) >> 16)

enum VCFRONT_coolantFlowPTReason_Values { 
  NONE = 0,
  COOLANT_AIR_PURGE = 1,
  NO_FLOW_REQ = 2,
  OVERRIDE_BATT = 3,
  ACTIVE_MANAGER_BATT = 4,
  PASSIVE_MANAGER_BATT = 5,
  BMS_FLOW_REQ = 6,
  DAS_FLOW_REQ = 7,
  OVERRIDE_PT = 8,
  ACTIVE_MANAGER_PT = 9,
  PASSIVE_MANAGER_PT = 10,
  PCS_FLOW_REQ = 11,
  DI_FLOW_REQ = 12,
  DIS_FLOW_REQ = 13,
  HP_FLOW_REQ = 14
 };

#define Parse_VCFRONT_wasteHeatRequestType(d) ((d & 0x3000) >> 12)

enum VCFRONT_wasteHeatRequestType_Values { 
  WASTE_TYPE_NONE = 0,
  WASTE_TYPE_PARTIAL = 1,
  WASTE_TYPE_FULL = 2
 };

#define Parse_VCFRONT_coolantHasBeenFilled(d) ((d & 0x400) >> 10)
#define Parse_VCFRONT_radiatorIneffective(d) ((d & 0x200) >> 9)
#define Parse_VCFRONT_coolantAirPurgeBatState(d) ((d & 0x700) >> 8)

enum VCFRONT_coolantAirPurgeBatState_Values { 
  AIR_PURGE_STATE_INACTIVE = 0,
  AIR_PURGE_STATE_ACTIVE = 1,
  AIR_PURGE_STATE_COMPLETE = 2,
  AIR_PURGE_STATE_INTERRUPTED = 3,
  AIR_PURGE_STATE_PENDING = 4
 };

#define ID3BBUI_power 0x3bb

#define Parse_UI_powerExpected(d) ((d & 0xff00) >> 8)
#define Parse_UI_powerIdeal(d) ((d & 0xff00) >> 8)
#define ID5D5RearDItemps 0x5d5

#define Parse_DI_ph1Temp(d) ((d & 0xff00) >> 8)
#define Parse_DI_ph2Temp(d) ((d & 0xff00000000) >> 32)
#define Parse_DI_ph3Temp(d) ((d & 0xff000000) >> 24)
#define Parse_DI_pcbTemp2(d) ((d & 0xff0000) >> 16)
#define Parse_DI_IGBTJunctTemp(d) ((d & 0xff00) >> 8)

enum DI_IGBTJunctTemp_Values { 
  SNA = 255
 };

#define ID556FrontDItemps 0x556

#define Parse_DIF_ph1Temp(d) ((d & 0xff00) >> 8)
#define Parse_DIF_ph2Temp(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIF_ph3Temp(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIF_pcbTemp2(d) ((d & 0xff00000000) >> 32)
#define Parse_DIF_IGBTJunctTemp(d) ((d & 0xff000000) >> 24)

enum DIF_IGBTJunctTemp_Values { 
  SNA = 255
 };

#define Parse_DIF_lashAngle(d) ((d & 0x3ff0000) >> 16)
#define Parse_DIF_lashCheckCount(d) ((d & 0xfc0) >> 6)
#define ID557FrontThermalControl 0x557

#define Parse_DIS_passiveInletTempReq(d) ((d & 0xff00) >> 8)
#define Parse_DIS_activeInletTempReq(d) ((d & 0xff000000) >> 24)
#define Parse_DIS_coolantFlowReq(d) ((d & 0xff0000) >> 16)
#define Parse_DIS_oilFlowReq(d) ((d & 0xff00) >> 8)
#define ID5D7RearThermalControl 0x5d7

#define Parse_DI_passiveInletTempReq(d) ((d & 0xff00) >> 8)
#define Parse_DI_activeInletTempReq(d) ((d & 0xff000000) >> 24)
#define Parse_DI_coolantFlowReq(d) ((d & 0xff0000) >> 16)
#define Parse_DI_oilFlowReq(d) ((d & 0xff00) >> 8)
#define ID7D5DIR_debug 0x7d5

#define Parse_DIR_debugSelector(d) ((d & 0xff00) >> 8)
#define Parse_DIR_cpu10HzMin(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_cpu1kHzMin(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_dcCapTemp(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_gainScale(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_gateDriveSupplyState(d) ((d & 0x300000000000000) >> 56)

enum DIR_gateDriveSupplyState_Values { 
  PSTG_GD_SUPPLY_DOWN = 0,
  PSTG_GD_SUPPLY_RISING = 1,
  PSTG_GD_SUPPLY_UP = 2,
  PSTG_GD_SUPPLY_FALLING = 3
 };

#define Parse_DIR_hwFaultCount(d) ((d & 0xf00000000000000) >> 56)
#define Parse_DIR_lmScale(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_motorType(d) ((d & 0x3f00000000000000) >> 56)

enum DIR_motorType_Values { 
  DI_MOTOR_SNA = 0,
  DI_MOTOR_ROADSTER_BASE = 1,
  DI_MOTOR_ROADSTER_SPORT = 2,
  DI_MOTOR_M7M3 = 3,
  DI_MOTOR_M7M4 = 4,
  DI_MOTOR_M7M5 = 5,
  DI_MOTOR_M8A = 6,
  DI_MOTOR_M7M6 = 7,
  DI_MOTOR_F1A = 8,
  DI_MOTOR_SSR1A = 9,
  DI_MOTOR_F1AC = 10,
  DI_MOTOR_F2AB = 11,
  DI_MOTOR_F2AC = 12,
  DI_MOTOR_F2AD = 13,
  DI_MOTOR_F2AE = 14,
  DI_MOTOR_F2APMSRM = 15,
  DI_MOTOR_PM216A = 16,
  DI_MOTOR_IM100A = 17,
  DI_MOTOR_PM216B = 18,
  DI_MOTOR_IM100B = 19,
  DI_MOTOR_IM216A = 20,
  DI_MOTOR_PM216C = 21,
  DI_MOTOR_IM130C = 22,
  DI_MOTOR_PM216D = 23,
  DI_MOTOR_IM130D = 24,
  DI_MOTOR_IM130D_AL = 25,
  DI_MOTOR_F2AE_AL = 26,
  DI_MOTOR_IM130D_AL_POSCO = 27,
  DI_MOTOR_PM275B = 28,
  DI_MOTOR_PM350B = 29,
  DI_MOTOR_PM216CSR = 30,
  DI_MOTOR_PM216CSR_N42 = 31,
  DI_MOTOR_PM228B = 32
 };

#define Parse_DIR_offsetA(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_offsetB(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_oilPumpMotorSpeed(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_phaseOutBusbarTemp(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_pwrSatChargeCurrent(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_pwrSatDischargeCurrent(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_resolverOffsetCos(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_rotorOffsetEst(d) ((d & 0xfff0) >> 4)
#define Parse_DIR_rotorOffsetLearningState(d) ((d & 0xf00000000000000) >> 56)

enum DIR_rotorOffsetLearningState_Values { 
  ROL_STATE_INIT = 0,
  ROL_STATE_WAIT = 1,
  ROL_STATE_ACCELERATE = 2,
  ROL_STATE_SHIFT = 3,
  ROL_STATE_MEASURE = 4,
  ROL_STATE_CORRECT = 5,
  ROL_STATE_VERIFY = 6,
  ROL_STATE_WRITE = 7,
  ROL_STATE_DONE = 8,
  ROL_NUM_STATES = 9
 };

#define Parse_DIR_rsScale(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_soptTimeToTrip(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_ssmState(d) ((d & 0xf00000000000000) >> 56)

enum DIR_ssmState_Values { 
  SSM_STATE_START = 0,
  SSM_STATE_UNAVAILABLE = 1,
  SSM_STATE_IDLE = 2,
  SSM_STATE_STANDBY = 3,
  SSM_STATE_ENABLE = 4,
  SSM_STATE_ABORT = 5,
  SSM_STATE_WAIT_FOR_RETRY = 6,
  SSM_STATE_RETRY = 7,
  SSM_STATE_FAULT = 8
 };

#define Parse_DIR_statorEndWindingTemp(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_systemStack(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_tqSatThermal(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_tqScaleDifferential(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_veMassInvRaw(d) ((d & 0xfff0) >> 4)
#define Parse_DIR_gateDriveState(d) ((d & 0x1c0000000000000) >> 54)

enum DIR_gateDriveState_Values { 
  PSTG_GD_STATE_INIT = 0,
  PSTG_GD_STATE_SELFTEST = 1,
  PSTG_GD_STATE_CONFIGURING = 2,
  PSTG_GD_STATE_CONFIGURED = 3,
  PSTG_GD_STATE_NOT_CONFIGURED = 4
 };

#define Parse_DIR_usmState(d) ((d & 0x70000000000000) >> 52)

enum DIR_usmState_Values { 
  USM_STATE_START = 0,
  USM_STATE_STANDBY = 1,
  USM_STATE_RETRY = 2,
  USM_STATE_ABORT = 3,
  USM_STATE_ENABLE = 4,
  USM_STATE_FAULT = 5,
  USM_STATE_UNAVAILABLE = 6,
  USM_STATE_WAIT_FOR_RETRY = 7
 };

#define Parse_DIR_powerStageSafeState(d) ((d & 0x18000000000000) >> 51)

enum DIR_powerStageSafeState_Values { 
  PSTG_SAFESTATE_NONE = 0,
  PSTG_SAFESTATE_ALL_OFF = 1,
  PSTG_SAFESTATE_3PS_HIGH = 2,
  PSTG_SAFESTATE_3PS_LOW = 3
 };

#define Parse_DIR_busbarTemp(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIR_cpu10HzAvg(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIR_cpu1HzMin(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIR_cpu1kHzAvg(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIR_cpuIDWord0(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIR_cpuIDWord1(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIR_dcCableCurrentEst(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIR_fluxState(d) ((d & 0xf000000000000) >> 48)

enum DIR_fluxState_Values { 
  DI_FLUXSTATE_START = 0,
  DI_FLUXSTATE_TEST = 1,
  DI_FLUXSTATE_STANDBY = 2,
  DI_FLUXSTATE_FLUX_UP = 3,
  DI_FLUXSTATE_FLUX_DOWN = 4,
  DI_FLUXSTATE_ENABLED = 5,
  DI_FLUXSTATE_ICONTROL = 6,
  DI_FLUXSTATE_VCONTROL = 7,
  DI_FLUXSTATE_FAULT = 9,
  DI_FLUXSTATE_STATIONARY_WASTE = 10
 };

#define Parse_DIR_idleStack(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIR_motorIA(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIR_oilPumpPhaseVoltage(d) ((d & 0xff000000000000) >> 48)

enum DIR_oilPumpPhaseVoltage_Values { 
  SNA = 255
 };

#define Parse_DIR_peakIQref(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIR_phaseOutBusbarWeldTemp(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIR_pwrSatMaxDischargePower(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIR_resolverCosRmsSquared(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIR_resolverOffsetSin(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIR_rotorFlux(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIR_rotorMaxMagnetTemp(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIR_soptTripDelay(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIR_statorFluxRef(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIR_statorIDref(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIR_statorIQref(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIR_statorVDFiltered(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIR_statorVQ(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIR_sysHeatPowerOptimal(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIR_tcMaxRequest(d) ((d & 0xff000000000000) >> 48)

enum DIR_tcMaxRequest_Values { 
  SNA = 255
 };

#define Parse_DIR_rotorOffsetMean(d) ((d & 0xfff00000000000) >> 44)
#define Parse_DIR_veResForce(d) ((d & 0xfff00000000000) >> 44)
#define Parse_DIR_cpu100HzMin(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIR_cpu1HzAvg(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIR_cpu20kHzMin(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIR_decodeHardwareStack(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIR_llsScale(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIR_oilPumpPressureEstimateMax(d) ((d & 0xff0000000000) >> 40)

enum DIR_oilPumpPressureEstimateMax_Values { 
  SNA = 255
 };

#define Parse_DIR_pcsTemp(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIR_phaseOutLugTemp(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIR_pwrSatMaxRegenPower(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIR_resolverPhaseOffset(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIR_resolverSinRmsSquared(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIR_soptTimeToOff(d) ((d & 0x7ff0000000000) >> 40)
#define Parse_DIR_tcMinRequest(d) ((d & 0xff0000000000) >> 40)

enum DIR_tcMinRequest_Values { 
  SNA = 255
 };

#define Parse_DIR_cpu100HzAvg(d) ((d & 0xff00000000) >> 32)
#define Parse_DIR_cpu20kHzAvg(d) ((d & 0xff00000000) >> 32)
#define Parse_DIR_cpuIDWord2(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIR_crc(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_DIR_currentLimit(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIR_dcCableHeat(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIR_dcLinkCapTemp(d) ((d & 0xff00000000) >> 32)
#define Parse_DIR_driveUnitOdometer(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_DIR_eepromStack(d) ((d & 0xff00000000) >> 32)
#define Parse_DIR_llrScale(d) ((d & 0xff00000000) >> 32)
#define Parse_DIR_loadAngleMargin(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIR_motorIAavg(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIR_motorIB(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIR_oilPumpPressureExpectedMin(d) ((d & 0xff00000000) >> 32)

enum DIR_oilPumpPressureExpectedMin_Values { 
  SNA = 255
 };

#define Parse_DIR_resolverCommonGain(d) ((d & 0xff00000000) >> 32)
#define Parse_DIR_resolverCosFiltered(d) ((d & 0xff00000000) >> 32)
#define Parse_DIR_statorFluxFdb(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIR_statorIDfdb(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIR_statorIQfdb(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIR_statorTemp1(d) ((d & 0xff00000000) >> 32)
#define Parse_DIR_statorVD(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIR_statorVQFiltered(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIR_tqSatMotorVoltage(d) ((d & 0xff00000000) >> 32)
#define Parse_DIR_tqSatUiDriveTorque(d) ((d & 0xff00000000) >> 32)
#define Parse_DIR_controlStack(d) ((d & 0xff000000) >> 24)
#define Parse_DIR_cpu10msMin(d) ((d & 0xff000000) >> 24)
#define Parse_DIR_hvDcCableTemp(d) ((d & 0xff000000) >> 24)
#define Parse_DIR_loadAngle(d) ((d & 0xffff000000) >> 24)
#define Parse_DIR_resolverReady(d) ((d & 0x1000000) >> 24)
#define Parse_DIR_resolverSinFiltered(d) ((d & 0xff000000) >> 24)
#define Parse_DIR_soptMaxCurrentMagSqrd(d) ((d & 0xffff000000) >> 24)
#define Parse_DIR_statorTemp2(d) ((d & 0xff000000) >> 24)
#define Parse_DIR_tqSatMotorCurrent(d) ((d & 0xff000000) >> 24)
#define Parse_DIR_tqSatUiRegenTorque(d) ((d & 0xff000000) >> 24)
#define Parse_DIR_udsStack(d) ((d & 0xff000000) >> 24)
#define Parse_DIR_resolverNoCarrier(d) ((d & 0x800000) >> 23)
#define Parse_DIR_resolverNoPhaseLock(d) ((d & 0x400000) >> 22)
#define Parse_DIR_resolverClaMIA(d) ((d & 0x200000) >> 21)
#define Parse_DIR_cpuIDWord3(d) ((d & 0xffff0000) >> 16)
#define Parse_DIR_internalAngleFilt(d) ((d & 0xffff0000) >> 16)
#define Parse_DIR_magnetTempEst(d) ((d & 0xff0000) >> 16)

enum DIR_magnetTempEst_Values { 
  SNA = 0
 };

#define Parse_DIR_module10HzStack(d) ((d & 0xff0000) >> 16)
#define Parse_DIR_motorIBavg(d) ((d & 0xffff0000) >> 16)
#define Parse_DIR_motorIC(d) ((d & 0xffff0000) >> 16)
#define Parse_DIR_motorV(d) ((d & 0xffff0000) >> 16)
#define Parse_DIR_negDcBusbarTemp(d) ((d & 0xff0000) >> 16)
#define Parse_DIR_peakFlux(d) ((d & 0xffff0000) >> 16)
#define Parse_DIR_pwmState(d) ((d & 0x30000) >> 16)

enum DIR_pwmState_Values { 
  PWMSTATE_SVPWM = 0,
  PWMSTATE_DPWM2 = 1,
  PWMSTATE_OPWM1 = 2,
  PWMSTATE_OPWM2 = 3
 };

#define Parse_DIR_pwrSatMaxBusVoltage(d) ((d & 0xff0000) >> 16)
#define Parse_DIR_resolverErrorRmsSquared(d) ((d & 0xff0000) >> 16)
#define Parse_DIR_systemTorqueCommand(d) ((d & 0xffff0000) >> 16)
#define Parse_DIR_torquePerAmp(d) ((d & 0xffff0000) >> 16)
#define Parse_DIR_tqScaleMaxMotorSpeed(d) ((d & 0xff0000) >> 16)
#define Parse_DIR_wasteCurrentLimit(d) ((d & 0xffff0000) >> 16)
#define Parse_DIR_xcpStack(d) ((d & 0xff0000) >> 16)
#define Parse_DIR_immobilizerStack(d) ((d & 0xff00) >> 8)
#define Parse_DIR_posDcBusbarTemp(d) ((d & 0xff00) >> 8)
#define Parse_DIR_pwrSatMinBusVoltage(d) ((d & 0xff00) >> 8)
#define Parse_DIR_tqScaleShift(d) ((d & 0xff00) >> 8)
#define Parse_DIR_brakeSwitchNO(d) ((d & 0x4) >> 2)
#define Parse_DIR_brakeSwitchNC(d) ((d & 0x2) >> 1)
#define ID757DIF_debug 0x757

#define Parse_DIF_debugSelector(d) ((d & 0xff00) >> 8)
#define Parse_DIF_cpu10HzMin(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_cpu1kHzMin(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_dcCapTemp(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_gainScale(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_gateDriveSupplyState(d) ((d & 0x300000000000000) >> 56)

enum DIF_gateDriveSupplyState_Values { 
  PSTG_GD_SUPPLY_DOWN = 0,
  PSTG_GD_SUPPLY_RISING = 1,
  PSTG_GD_SUPPLY_UP = 2,
  PSTG_GD_SUPPLY_FALLING = 3
 };

#define Parse_DIF_hwFaultCount(d) ((d & 0xf00000000000000) >> 56)
#define Parse_DIF_lmScale(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_motorType(d) ((d & 0x3f00000000000000) >> 56)

enum DIF_motorType_Values { 
  DI_MOTOR_SNA = 0,
  DI_MOTOR_ROADSTER_BASE = 1,
  DI_MOTOR_ROADSTER_SPORT = 2,
  DI_MOTOR_M7M3 = 3,
  DI_MOTOR_M7M4 = 4,
  DI_MOTOR_M7M5 = 5,
  DI_MOTOR_M8A = 6,
  DI_MOTOR_M7M6 = 7,
  DI_MOTOR_F1A = 8,
  DI_MOTOR_SSR1A = 9,
  DI_MOTOR_F1AC = 10,
  DI_MOTOR_F2AB = 11,
  DI_MOTOR_F2AC = 12,
  DI_MOTOR_F2AD = 13,
  DI_MOTOR_F2AE = 14,
  DI_MOTOR_F2APMSRM = 15,
  DI_MOTOR_PM216A = 16,
  DI_MOTOR_IM100A = 17,
  DI_MOTOR_PM216B = 18,
  DI_MOTOR_IM100B = 19,
  DI_MOTOR_IM216A = 20,
  DI_MOTOR_PM216C = 21,
  DI_MOTOR_IM130C = 22,
  DI_MOTOR_PM216D = 23,
  DI_MOTOR_IM130D = 24,
  DI_MOTOR_IM130D_AL = 25,
  DI_MOTOR_F2AE_AL = 26,
  DI_MOTOR_IM130D_AL_POSCO = 27,
  DI_MOTOR_PM275B = 28,
  DI_MOTOR_PM350B = 29,
  DI_MOTOR_PM216CSR = 30,
  DI_MOTOR_PM216CSR_N42 = 31,
  DI_MOTOR_PM228B = 32
 };

#define Parse_DIF_offsetA(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_offsetB(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_oilPumpMotorSpeed(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_phaseOutBusbarTemp(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_pwrSatChargeCurrent(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_pwrSatDischargeCurrent(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_resolverOffsetCos(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_rotorOffsetEst(d) ((d & 0xfff0) >> 4)
#define Parse_DIF_rotorOffsetLearningState(d) ((d & 0xf00000000000000) >> 56)

enum DIF_rotorOffsetLearningState_Values { 
  ROL_STATE_INIT = 0,
  ROL_STATE_WAIT = 1,
  ROL_STATE_ACCELERATE = 2,
  ROL_STATE_SHIFT = 3,
  ROL_STATE_MEASURE = 4,
  ROL_STATE_CORRECT = 5,
  ROL_STATE_VERIFY = 6,
  ROL_STATE_WRITE = 7,
  ROL_STATE_DONE = 8,
  ROL_NUM_STATES = 9
 };

#define Parse_DIF_rsScale(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_soptTimeToTrip(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_ssmState(d) ((d & 0xf00000000000000) >> 56)

enum DIF_ssmState_Values { 
  SSM_STATE_START = 0,
  SSM_STATE_UNAVAILABLE = 1,
  SSM_STATE_IDLE = 2,
  SSM_STATE_STANDBY = 3,
  SSM_STATE_ENABLE = 4,
  SSM_STATE_ABORT = 5,
  SSM_STATE_WAIT_FOR_RETRY = 6,
  SSM_STATE_RETRY = 7,
  SSM_STATE_FAULT = 8
 };

#define Parse_DIF_statorEndWindingTemp(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_systemStack(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_tqSatThermal(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_tqScaleDifferential(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIF_veMassInvRaw(d) ((d & 0xfff0) >> 4)
#define Parse_DIF_gateDriveState(d) ((d & 0x1c0000000000000) >> 54)

enum DIF_gateDriveState_Values { 
  PSTG_GD_STATE_INIT = 0,
  PSTG_GD_STATE_SELFTEST = 1,
  PSTG_GD_STATE_CONFIGURING = 2,
  PSTG_GD_STATE_CONFIGURED = 3,
  PSTG_GD_STATE_NOT_CONFIGURED = 4
 };

#define Parse_DIF_usmState(d) ((d & 0x70000000000000) >> 52)

enum DIF_usmState_Values { 
  USM_STATE_START = 0,
  USM_STATE_STANDBY = 1,
  USM_STATE_RETRY = 2,
  USM_STATE_ABORT = 3,
  USM_STATE_ENABLE = 4,
  USM_STATE_FAULT = 5,
  USM_STATE_UNAVAILABLE = 6,
  USM_STATE_WAIT_FOR_RETRY = 7
 };

#define Parse_DIF_powerStageSafeState(d) ((d & 0x18000000000000) >> 51)

enum DIF_powerStageSafeState_Values { 
  PSTG_SAFESTATE_NONE = 0,
  PSTG_SAFESTATE_ALL_OFF = 1,
  PSTG_SAFESTATE_3PS_HIGH = 2,
  PSTG_SAFESTATE_3PS_LOW = 3
 };

#define Parse_DIF_busbarTemp(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIF_cpu10HzAvg(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIF_cpu1HzMin(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIF_cpu1kHzAvg(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIF_cpuIDWord0(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIF_cpuIDWord1(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIF_dcCableCurrentEst(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIF_fluxState(d) ((d & 0xf000000000000) >> 48)

enum DIF_fluxState_Values { 
  DI_FLUXSTATE_START = 0,
  DI_FLUXSTATE_TEST = 1,
  DI_FLUXSTATE_STANDBY = 2,
  DI_FLUXSTATE_FLUX_UP = 3,
  DI_FLUXSTATE_FLUX_DOWN = 4,
  DI_FLUXSTATE_ENABLED = 5,
  DI_FLUXSTATE_ICONTROL = 6,
  DI_FLUXSTATE_VCONTROL = 7,
  DI_FLUXSTATE_FAULT = 9,
  DI_FLUXSTATE_STATIONARY_WASTE = 10
 };

#define Parse_DIF_idleStack(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIF_motorIA(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIF_oilPumpPhaseVoltage(d) ((d & 0xff000000000000) >> 48)

enum DIF_oilPumpPhaseVoltage_Values { 
  SNA = 255
 };

#define Parse_DIF_peakIQref(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIF_phaseOutBusbarWeldTemp(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIF_pwrSatMaxDischargePower(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIF_resolverCosRmsSquared(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIF_resolverOffsetSin(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIF_rotorFlux(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIF_rotorMaxMagnetTemp(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIF_soptTripDelay(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIF_statorFluxRef(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIF_statorIDref(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIF_statorIQref(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIF_statorVDFiltered(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIF_statorVQ(d) ((d & 0xffff000000000000) >> 48)
#define Parse_DIF_sysHeatPowerOptimal(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIF_tcMaxRequest(d) ((d & 0xff000000000000) >> 48)

enum DIF_tcMaxRequest_Values { 
  SNA = 255
 };

#define Parse_DIF_rotorOffsetMean(d) ((d & 0xfff00000000000) >> 44)
#define Parse_DIF_veResForce(d) ((d & 0xfff00000000000) >> 44)
#define Parse_DIF_cpu100HzMin(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIF_cpu1HzAvg(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIF_cpu20kHzMin(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIF_decodeHardwareStack(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIF_llsScale(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIF_oilPumpPressureEstimateMax(d) ((d & 0xff0000000000) >> 40)

enum DIF_oilPumpPressureEstimateMax_Values { 
  SNA = 255
 };

#define Parse_DIF_pcsTemp(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIF_phaseOutLugTemp(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIF_pwrSatMaxRegenPower(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIF_resolverPhaseOffset(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIF_resolverSinRmsSquared(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIF_soptTimeToOff(d) ((d & 0x7ff0000000000) >> 40)
#define Parse_DIF_tcMinRequest(d) ((d & 0xff0000000000) >> 40)

enum DIF_tcMinRequest_Values { 
  SNA = 255
 };

#define Parse_DIF_cpu100HzAvg(d) ((d & 0xff00000000) >> 32)
#define Parse_DIF_cpu20kHzAvg(d) ((d & 0xff00000000) >> 32)
#define Parse_DIF_cpuIDWord2(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIF_crc(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_DIF_currentLimit(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIF_dcCableHeat(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIF_dcLinkCapTemp(d) ((d & 0xff00000000) >> 32)
#define Parse_DIF_driveUnitOdometer(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_DIF_eepromStack(d) ((d & 0xff00000000) >> 32)
#define Parse_DIF_llrScale(d) ((d & 0xff00000000) >> 32)
#define Parse_DIF_loadAngleMargin(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIF_motorIAavg(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIF_motorIB(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIF_oilPumpPressureExpectedMin(d) ((d & 0xff00000000) >> 32)

enum DIF_oilPumpPressureExpectedMin_Values { 
  SNA = 255
 };

#define Parse_DIF_resolverCommonGain(d) ((d & 0xff00000000) >> 32)
#define Parse_DIF_resolverCosFiltered(d) ((d & 0xff00000000) >> 32)
#define Parse_DIF_statorFluxFdb(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIF_statorIDfdb(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIF_statorIQfdb(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIF_statorTemp1(d) ((d & 0xff00000000) >> 32)
#define Parse_DIF_statorVD(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIF_statorVQFiltered(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIF_tqSatMotorVoltage(d) ((d & 0xff00000000) >> 32)
#define Parse_DIF_tqSatUiDriveTorque(d) ((d & 0xff00000000) >> 32)
#define Parse_DIF_controlStack(d) ((d & 0xff000000) >> 24)
#define Parse_DIF_cpu10msMin(d) ((d & 0xff000000) >> 24)
#define Parse_DIF_hvDcCableTemp(d) ((d & 0xff000000) >> 24)
#define Parse_DIF_loadAngle(d) ((d & 0xffff000000) >> 24)
#define Parse_DIF_resolverReady(d) ((d & 0x1000000) >> 24)
#define Parse_DIF_resolverSinFiltered(d) ((d & 0xff000000) >> 24)
#define Parse_DIF_soptMaxCurrentMagSqrd(d) ((d & 0xffff000000) >> 24)
#define Parse_DIF_statorTemp2(d) ((d & 0xff000000) >> 24)
#define Parse_DIF_tqSatMotorCurrent(d) ((d & 0xff000000) >> 24)
#define Parse_DIF_tqSatUiRegenTorque(d) ((d & 0xff000000) >> 24)
#define Parse_DIF_udsStack(d) ((d & 0xff000000) >> 24)
#define Parse_DIF_resolverNoCarrier(d) ((d & 0x800000) >> 23)
#define Parse_DIF_resolverNoPhaseLock(d) ((d & 0x400000) >> 22)
#define Parse_DIF_resolverClaMIA(d) ((d & 0x200000) >> 21)
#define Parse_DIF_cpuIDWord3(d) ((d & 0xffff0000) >> 16)
#define Parse_DIF_internalAngleFilt(d) ((d & 0xffff0000) >> 16)
#define Parse_DIF_magnetTempEst(d) ((d & 0xff0000) >> 16)

enum DIF_magnetTempEst_Values { 
  SNA = 0
 };

#define Parse_DIF_module10HzStack(d) ((d & 0xff0000) >> 16)
#define Parse_DIF_motorIBavg(d) ((d & 0xffff0000) >> 16)
#define Parse_DIF_motorIC(d) ((d & 0xffff0000) >> 16)
#define Parse_DIF_motorV(d) ((d & 0xffff0000) >> 16)
#define Parse_DIF_negDcBusbarTemp(d) ((d & 0xff0000) >> 16)
#define Parse_DIF_peakFlux(d) ((d & 0xffff0000) >> 16)
#define Parse_DIF_pwmState(d) ((d & 0x30000) >> 16)

enum DIF_pwmState_Values { 
  PWMSTATE_SVPWM = 0,
  PWMSTATE_DPWM2 = 1,
  PWMSTATE_OPWM1 = 2,
  PWMSTATE_OPWM2 = 3
 };

#define Parse_DIF_pwrSatMaxBusVoltage(d) ((d & 0xff0000) >> 16)
#define Parse_DIF_resolverErrorRmsSquared(d) ((d & 0xff0000) >> 16)
#define Parse_DIF_systemTorqueCommand(d) ((d & 0xffff0000) >> 16)
#define Parse_DIF_torquePerAmp(d) ((d & 0xffff0000) >> 16)
#define Parse_DIF_tqScaleMaxMotorSpeed(d) ((d & 0xff0000) >> 16)
#define Parse_DIF_wasteCurrentLimit(d) ((d & 0xffff0000) >> 16)
#define Parse_DIF_xcpStack(d) ((d & 0xff0000) >> 16)
#define Parse_DIF_immobilizerStack(d) ((d & 0xff00) >> 8)
#define Parse_DIF_posDcBusbarTemp(d) ((d & 0xff00) >> 8)
#define Parse_DIF_pwrSatMinBusVoltage(d) ((d & 0xff00) >> 8)
#define Parse_DIF_tqScaleShift(d) ((d & 0xff00) >> 8)
#define Parse_DIF_brakeSwitchNO(d) ((d & 0x4) >> 2)
#define Parse_DIF_brakeSwitchNC(d) ((d & 0x2) >> 1)
#define ID2B6DI_chassisControlStatus 0x2b6

#define Parse_DI_vdcTelltaleFlash(d) ((d & 0x2) >> 1)
#define Parse_DI_vdcTelltaleOn(d) ((d & 0x8000) >> 15)
#define Parse_DI_tcTelltaleFlash(d) ((d & 0x4000) >> 14)
#define Parse_DI_tcTelltaleOn(d) ((d & 0x2000) >> 13)
#define Parse_DI_tractionControlModeUI(d) ((d & 0x7000) >> 12)

enum DI_tractionControlModeUI_Values { 
  NORMAL = 0,
  SLIP_START = 1,
  DEV_MODE_1 = 2,
  DEV_MODE_2 = 3,
  ROLLS_MODE = 4,
  DYNO_MODE = 5,
  OFFROAD_ASSIST = 6
 };

#define Parse_DI_ptcStateUI(d) ((d & 0x600) >> 9)

enum DI_ptcStateUI_Values { 
  FAULTED = 0,
  BACKUP = 1,
  ON = 2,
  SNA = 3
 };

#define Parse_DI_btcStateUI(d) ((d & 0x80) >> 7)
#define Parse_DI_vehicleHoldTelltaleOn(d) ((d & 0x40) >> 6)
#define ID284UIvehicleModes 0x284

#define Parse_UIfactoryMode284(d) ((d & 0x2) >> 1)
#define Parse_UItransportMode284(d) ((d & 0x8000000000) >> 39)
#define Parse_UIshowroomMode284(d) ((d & 0x4000000000) >> 38)
#define Parse_UIserviceMode284(d) ((d & 0x2000000000) >> 37)
#define Parse_UIisDelivered284(d) ((d & 0x1000000000) >> 36)
#define Parse_UIsentryMode284(d) ((d & 0x800000000) >> 35)
#define Parse_UIhomelinkV2Command0284(d) ((d & 0xff00000000) >> 32)
#define Parse_UIhomelinkV2Command1284(d) ((d & 0xff000000) >> 24)
#define Parse_UIhomelinkV2Command2284(d) ((d & 0xff0000) >> 16)
#define Parse_UIcarWashModeRequest284(d) ((d & 0x100) >> 8)
#define Parse_UIvaletMode284(d) ((d & 0x80) >> 7)
#define Parse_UIgameMode284(d) ((d & 0x40) >> 6)
#define ID221VCFRONT_LVPowerState 0x221

#define Parse_VCFRONT_LVPowerStateIndex(d) ((d & 0x3e0) >> 5)

enum VCFRONT_LVPowerStateIndex_Values { 
  Mux0 = 0,
  Mux1 = 1
 };

#define Parse_VCFRONT_cpLVRequest(d) ((d & 0x300000000000000) >> 56)

enum VCFRONT_cpLVRequest_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_parkLVState(d) ((d & 0x300000000000000) >> 56)

enum VCFRONT_parkLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_epasLVState(d) ((d & 0xc0000000000000) >> 54)

enum VCFRONT_epasLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_espLVState(d) ((d & 0xc0000000000000) >> 54)

enum VCFRONT_espLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_hvcLVRequest(d) ((d & 0x30000000000000) >> 52)

enum VCFRONT_hvcLVRequest_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_radcLVState(d) ((d & 0x30000000000000) >> 52)

enum VCFRONT_radcLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_hvacCompLVState(d) ((d & 0xc000000000000) >> 50)

enum VCFRONT_hvacCompLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_tasLVState(d) ((d & 0xc000000000000) >> 50)

enum VCFRONT_tasLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_pcsLVState(d) ((d & 0x3000000000000) >> 48)

enum VCFRONT_pcsLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_ptcLVRequest(d) ((d & 0x3000000000000) >> 48)

enum VCFRONT_ptcLVRequest_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_CMPDLVState(d) ((d & 0xc00000000000) >> 46)

enum VCFRONT_CMPDLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_sccmLVRequest(d) ((d & 0xc00000000000) >> 46)

enum VCFRONT_sccmLVRequest_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_tpmsLVRequest(d) ((d & 0x300000000000) >> 44)

enum VCFRONT_tpmsLVRequest_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_rcmLVRequest(d) ((d & 0xc0000000000) >> 42)

enum VCFRONT_rcmLVRequest_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_iBoosterLVState(d) ((d & 0x30000000000) >> 40)

enum VCFRONT_iBoosterLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_tunerLVRequest(d) ((d & 0xc000000000) >> 38)

enum VCFRONT_tunerLVRequest_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_amplifierLVRequest(d) ((d & 0x3000000000) >> 36)

enum VCFRONT_amplifierLVRequest_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_das1HighCurrentLVState(d) ((d & 0xc00000000) >> 34)

enum VCFRONT_das1HighCurrentLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_das2HighCurrentLVState(d) ((d & 0x300000000) >> 32)

enum VCFRONT_das2HighCurrentLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_dirLVRequest(d) ((d & 0xc0000000) >> 30)

enum VCFRONT_dirLVRequest_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_difLVState(d) ((d & 0x30000000) >> 28)

enum VCFRONT_difLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_oilPumpFrontLVState(d) ((d & 0xc000000) >> 26)

enum VCFRONT_oilPumpFrontLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_oilPumpRearLVRequest(d) ((d & 0x3000000) >> 24)

enum VCFRONT_oilPumpRearLVRequest_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_ocsLVRequest(d) ((d & 0xc00000) >> 22)

enum VCFRONT_ocsLVRequest_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_vcleftHiCurrentLVState(d) ((d & 0x300000) >> 20)

enum VCFRONT_vcleftHiCurrentLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_vcrightHiCurrentLVState(d) ((d & 0xc0000) >> 18)

enum VCFRONT_vcrightHiCurrentLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCFRONT_uiHiCurrentLVState(d) ((d & 0x30000) >> 16)

enum VCFRONT_uiHiCurrentLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define ID225VCRIGHT_LVPowerState 0x225

#define Parse_VCRIGHT_ptcLVState(d) ((d & 0xc) >> 2)

enum VCRIGHT_ptcLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCRIGHT_ocsLVState(d) ((d & 0xc00000) >> 22)

enum VCRIGHT_ocsLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCRIGHT_amplifierLVState(d) ((d & 0x300000) >> 20)

enum VCRIGHT_amplifierLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCRIGHT_rearOilPumpLVState(d) ((d & 0xc0000) >> 18)

enum VCRIGHT_rearOilPumpLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCRIGHT_tunerLVState(d) ((d & 0x30000) >> 16)

enum VCRIGHT_tunerLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCRIGHT_hvcLVState(d) ((d & 0xc000) >> 14)

enum VCRIGHT_hvcLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCRIGHT_rcmLVState(d) ((d & 0x3000) >> 12)

enum VCRIGHT_rcmLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCRIGHT_lumbarLVState(d) ((d & 0xc00) >> 10)

enum VCRIGHT_lumbarLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCRIGHT_cntctrPwrState(d) ((d & 0x80) >> 7)
#define Parse_VCRIGHT_eFuseLockoutStatus(d) ((d & 0xc0) >> 6)

enum VCRIGHT_eFuseLockoutStatus_Values { 
  EFUSE_LOCKOUT_STATUS_IDLE = 0,
  EFUSE_LOCKOUT_STATUS_PENDING = 1,
  EFUSE_LOCKOUT_STATUS_ACTIVE = 2
 };

#define Parse_VCRIGHT_swEnStatus(d) ((d & 0x10) >> 4)
#define Parse_VCRIGHT_vehiclePowerStateDBG(d) ((d & 0x18) >> 3)

enum VCRIGHT_vehiclePowerStateDBG_Values { 
  VEHICLE_POWER_STATE_OFF = 0,
  VEHICLE_POWER_STATE_CONDITIONING = 1,
  VEHICLE_POWER_STATE_ACCESSORY = 2,
  VEHICLE_POWER_STATE_DRIVE = 3
 };

#define ID2F1VCFRONT_eFuseDebugStatus 0x2f1

#define Parse_VCFRONT_eFuseDebugStatusIndex(d) ((d & 0x3e0) >> 5)

enum VCFRONT_eFuseDebugStatusIndex_Values { 
  VCF_DBG_STS_VCRIGHT = 0,
  VCF_DBG_STS_VCLEFT = 1,
  VCF_DBG_STS_PCS = 2,
  VCF_DBG_STS_IBOOSTER = 3,
  VCF_DBG_STS_EPAS3P = 4,
  VCF_DBG_STS_EPAS3S = 5,
  VCF_DBG_STS_ESP_MOTOR = 6,
  VCF_DBG_STS_ESP_VALVE = 7,
  VCF_DBG_STS_AUTOPILOT_1 = 8,
  VCF_DBG_STS_AUTOPILOT_2 = 9,
  VCF_DBG_STS_SLEEP_BYPASS = 10,
  VCF_DBG_STS_UI = 11,
  VCF_DBG_STS_HEADLAMPS = 12,
  VCF_DBG_STS_VBAT_FUSED_HIGH_CURRENT = 13,
  VCF_DBG_STS_PUMPS = 14,
  VCF_DBG_STS_RAILS_A_B = 15,
  VCF_DBG_STS_MISC_RAILS = 16,
  VCF_DBG_STS_LV_BATTERY_DEBUG = 17,
  VCF_DBG_STS_INVALID = 18
 };

#define Parse_VCFRONT_pump1AndFanState(d) ((d & 0x800000000000000) >> 59)
#define Parse_VCFRONT_uiAudioState(d) ((d & 0x800000000000000) >> 59)
#define Parse_VCFRONT_pump1AndFanCurrent(d) ((d & 0x3fffc00) >> 10)
#define Parse_VCFRONT_uiAudioFault(d) ((d & 0x400000000000000) >> 58)
#define Parse_VCFRONT_EPAS3PState(d) ((d & 0x300000000000000) >> 56)

enum VCFRONT_EPAS3PState_Values { 
  EFUSE_STATE_OFF = 0,
  EFUSE_STATE_ON = 1,
  EFUSE_STATE_LOCKED_OUT = 2
 };

#define Parse_VCFRONT_EPAS3SState(d) ((d & 0x300000000000000) >> 56)

enum VCFRONT_EPAS3SState_Values { 
  EFUSE_STATE_OFF = 0,
  EFUSE_STATE_ON = 1,
  EFUSE_STATE_LOCKED_OUT = 2
 };

#define Parse_VCFRONT_ESPMotorState(d) ((d & 0x300000000000000) >> 56)

enum VCFRONT_ESPMotorState_Values { 
  EFUSE_STATE_OFF = 0,
  EFUSE_STATE_ON = 1,
  EFUSE_STATE_LOCKED_OUT = 2
 };

#define Parse_VCFRONT_ESPValveState(d) ((d & 0x300000000000000) >> 56)

enum VCFRONT_ESPValveState_Values { 
  EFUSE_STATE_OFF = 0,
  EFUSE_STATE_ON = 1,
  EFUSE_STATE_LOCKED_OUT = 2
 };

#define Parse_VCFRONT_PCSState(d) ((d & 0x300000000000000) >> 56)

enum VCFRONT_PCSState_Values { 
  EFUSE_STATE_OFF = 0,
  EFUSE_STATE_ON = 1,
  EFUSE_STATE_LOCKED_OUT = 2
 };

#define Parse_VCFRONT_autopilot1State(d) ((d & 0x300000000000000) >> 56)

enum VCFRONT_autopilot1State_Values { 
  EFUSE_STATE_OFF = 0,
  EFUSE_STATE_ON = 1,
  EFUSE_STATE_LOCKED_OUT = 2
 };

#define Parse_VCFRONT_autopilot2State(d) ((d & 0x300000000000000) >> 56)

enum VCFRONT_autopilot2State_Values { 
  EFUSE_STATE_OFF = 0,
  EFUSE_STATE_ON = 1,
  EFUSE_STATE_LOCKED_OUT = 2
 };

#define Parse_VCFRONT_chargedIBSAmpHours(d) ((d & 0xffff00) >> 8)
#define Parse_VCFRONT_eFuseLockoutVoltage(d) ((d & 0xfff0) >> 4)
#define Parse_VCFRONT_headlampLeftState(d) ((d & 0x100000000000000) >> 56)
#define Parse_VCFRONT_iBoosterState(d) ((d & 0x300000000000000) >> 56)

enum VCFRONT_iBoosterState_Values { 
  EFUSE_STATE_OFF = 0,
  EFUSE_STATE_ON = 1,
  EFUSE_STATE_LOCKED_OUT = 2
 };

#define Parse_VCFRONT_leftControllerState(d) ((d & 0x300000000000000) >> 56)

enum VCFRONT_leftControllerState_Values { 
  EFUSE_STATE_OFF = 0,
  EFUSE_STATE_ON = 1,
  EFUSE_STATE_LOCKED_OUT = 2
 };

#define Parse_VCFRONT_railA_12v(d) ((d & 0xfff0) >> 4)
#define Parse_VCFRONT_rightControllerState(d) ((d & 0x300000000000000) >> 56)

enum VCFRONT_rightControllerState_Values { 
  EFUSE_STATE_OFF = 0,
  EFUSE_STATE_ON = 1,
  EFUSE_STATE_LOCKED_OUT = 2
 };

#define Parse_VCFRONT_sleepBypassState(d) ((d & 0x100000000000000) >> 56)
#define Parse_VCFRONT_uiAudioCurrent(d) ((d & 0xffff00) >> 8)
#define Parse_VCFRONT_vbatFusedHighState(d) ((d & 0x100000000000000) >> 56)
#define Parse_VCFRONT_headlampLeftFault(d) ((d & 0x80000000000000) >> 55)
#define Parse_VCFRONT_sleepBypassFault(d) ((d & 0x80000000000000) >> 55)
#define Parse_VCFRONT_vbatFusedHighFault(d) ((d & 0x80000000000000) >> 55)
#define Parse_VCFRONT_EPAS3PFault(d) ((d & 0x40000000000000) >> 54)
#define Parse_VCFRONT_EPAS3SFault(d) ((d & 0x40000000000000) >> 54)
#define Parse_VCFRONT_ESPMotorFault(d) ((d & 0x40000000000000) >> 54)
#define Parse_VCFRONT_ESPValveFault(d) ((d & 0x40000000000000) >> 54)
#define Parse_VCFRONT_PCSFault(d) ((d & 0x40000000000000) >> 54)
#define Parse_VCFRONT_autopilot1Fault(d) ((d & 0x40000000000000) >> 54)
#define Parse_VCFRONT_autopilot2Fault(d) ((d & 0x40000000000000) >> 54)
#define Parse_VCFRONT_headlampLeftCurrent(d) ((d & 0xffc0000000000000) >> 54)
#define Parse_VCFRONT_iBoosterFault(d) ((d & 0x40000000000000) >> 54)
#define Parse_VCFRONT_leftControllerFault(d) ((d & 0x40000000000000) >> 54)
#define Parse_VCFRONT_rightControllerFault(d) ((d & 0x40000000000000) >> 54)
#define Parse_VCFRONT_sleepBypassVoltage(d) ((d & 0x3fffc0) >> 6)
#define Parse_VCFRONT_vbatFusedSelfTestResult(d) ((d & 0x3c0000000000000) >> 54)

enum VCFRONT_vbatFusedSelfTestResult_Values { 
  EFUSE_SELF_TEST_EFUSE_RESULT_NOT_RUN = 0,
  EFUSE_SELF_TEST_EFUSE_RESULT_RUNNING = 1,
  EFUSE_SELF_TEST_EFUSE_RESULT_PASSED = 2,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_RAILS_UNSTABLE = 3,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_EFUSE_OUTPUT_SHORT = 4,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_STUCK_ON = 5,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_LOW_MALFUNCTION = 6,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_CHANNEL_OPEN = 7,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_HIGH_MALFUNCTION = 8,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_TURN_OFF_PATH_TOO_SLOW = 9,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_NOT_LATCHED = 10,
  EFUSE_SELF_TEST_EFUSE_RESULT_SKIPPED = 11
 };

#define Parse_VCFRONT_EPAS3PSelfTestResult(d) ((d & 0x1e0000000000000) >> 53)

enum VCFRONT_EPAS3PSelfTestResult_Values { 
  EFUSE_SELF_TEST_EFUSE_RESULT_NOT_RUN = 0,
  EFUSE_SELF_TEST_EFUSE_RESULT_RUNNING = 1,
  EFUSE_SELF_TEST_EFUSE_RESULT_PASSED = 2,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_RAILS_UNSTABLE = 3,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_EFUSE_OUTPUT_SHORT = 4,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_STUCK_ON = 5,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_LOW_MALFUNCTION = 6,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_CHANNEL_OPEN = 7,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_HIGH_MALFUNCTION = 8,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_TURN_OFF_PATH_TOO_SLOW = 9,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_NOT_LATCHED = 10,
  EFUSE_SELF_TEST_EFUSE_RESULT_SKIPPED = 11
 };

#define Parse_VCFRONT_EPAS3SSelfTestResult(d) ((d & 0x1e0000000000000) >> 53)

enum VCFRONT_EPAS3SSelfTestResult_Values { 
  EFUSE_SELF_TEST_EFUSE_RESULT_NOT_RUN = 0,
  EFUSE_SELF_TEST_EFUSE_RESULT_RUNNING = 1,
  EFUSE_SELF_TEST_EFUSE_RESULT_PASSED = 2,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_RAILS_UNSTABLE = 3,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_EFUSE_OUTPUT_SHORT = 4,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_STUCK_ON = 5,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_LOW_MALFUNCTION = 6,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_CHANNEL_OPEN = 7,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_HIGH_MALFUNCTION = 8,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_TURN_OFF_PATH_TOO_SLOW = 9,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_NOT_LATCHED = 10,
  EFUSE_SELF_TEST_EFUSE_RESULT_SKIPPED = 11
 };

#define Parse_VCFRONT_ESPMotorSelfTestResult(d) ((d & 0x1e0000000000000) >> 53)

enum VCFRONT_ESPMotorSelfTestResult_Values { 
  EFUSE_SELF_TEST_EFUSE_RESULT_NOT_RUN = 0,
  EFUSE_SELF_TEST_EFUSE_RESULT_RUNNING = 1,
  EFUSE_SELF_TEST_EFUSE_RESULT_PASSED = 2,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_RAILS_UNSTABLE = 3,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_EFUSE_OUTPUT_SHORT = 4,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_STUCK_ON = 5,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_LOW_MALFUNCTION = 6,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_CHANNEL_OPEN = 7,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_HIGH_MALFUNCTION = 8,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_TURN_OFF_PATH_TOO_SLOW = 9,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_NOT_LATCHED = 10,
  EFUSE_SELF_TEST_EFUSE_RESULT_SKIPPED = 11
 };

#define Parse_VCFRONT_autopilot1Temp(d) ((d & 0xffe0000000000000) >> 53)
#define Parse_VCFRONT_autopilot2Temp(d) ((d & 0xffe0000000000000) >> 53)
#define Parse_VCFRONT_iBoosterSelfTestResult(d) ((d & 0x1e0000000000000) >> 53)

enum VCFRONT_iBoosterSelfTestResult_Values { 
  EFUSE_SELF_TEST_EFUSE_RESULT_NOT_RUN = 0,
  EFUSE_SELF_TEST_EFUSE_RESULT_RUNNING = 1,
  EFUSE_SELF_TEST_EFUSE_RESULT_PASSED = 2,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_RAILS_UNSTABLE = 3,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_EFUSE_OUTPUT_SHORT = 4,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_STUCK_ON = 5,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_LOW_MALFUNCTION = 6,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_CHANNEL_OPEN = 7,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_HIGH_MALFUNCTION = 8,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_TURN_OFF_PATH_TOO_SLOW = 9,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_NOT_LATCHED = 10,
  EFUSE_SELF_TEST_EFUSE_RESULT_SKIPPED = 11
 };

#define Parse_VCFRONT_pcsSelfTestResult(d) ((d & 0x1e0000000000000) >> 53)

enum VCFRONT_pcsSelfTestResult_Values { 
  EFUSE_SELF_TEST_EFUSE_RESULT_NOT_RUN = 0,
  EFUSE_SELF_TEST_EFUSE_RESULT_RUNNING = 1,
  EFUSE_SELF_TEST_EFUSE_RESULT_PASSED = 2,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_RAILS_UNSTABLE = 3,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_EFUSE_OUTPUT_SHORT = 4,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_STUCK_ON = 5,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_LOW_MALFUNCTION = 6,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_CHANNEL_OPEN = 7,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_HIGH_MALFUNCTION = 8,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_TURN_OFF_PATH_TOO_SLOW = 9,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_NOT_LATCHED = 10,
  EFUSE_SELF_TEST_EFUSE_RESULT_SKIPPED = 11
 };

#define Parse_VCFRONT_vcleftSelfTestResult(d) ((d & 0x1e0000000000000) >> 53)

enum VCFRONT_vcleftSelfTestResult_Values { 
  EFUSE_SELF_TEST_EFUSE_RESULT_NOT_RUN = 0,
  EFUSE_SELF_TEST_EFUSE_RESULT_RUNNING = 1,
  EFUSE_SELF_TEST_EFUSE_RESULT_PASSED = 2,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_RAILS_UNSTABLE = 3,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_EFUSE_OUTPUT_SHORT = 4,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_STUCK_ON = 5,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_LOW_MALFUNCTION = 6,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_CHANNEL_OPEN = 7,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_HIGH_MALFUNCTION = 8,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_TURN_OFF_PATH_TOO_SLOW = 9,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_NOT_LATCHED = 10,
  EFUSE_SELF_TEST_EFUSE_RESULT_SKIPPED = 11
 };

#define Parse_VCFRONT_vcrightSelfTestResult(d) ((d & 0x1e0000000000000) >> 53)

enum VCFRONT_vcrightSelfTestResult_Values { 
  EFUSE_SELF_TEST_EFUSE_RESULT_NOT_RUN = 0,
  EFUSE_SELF_TEST_EFUSE_RESULT_RUNNING = 1,
  EFUSE_SELF_TEST_EFUSE_RESULT_PASSED = 2,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_RAILS_UNSTABLE = 3,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_EFUSE_OUTPUT_SHORT = 4,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_STUCK_ON = 5,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_LOW_MALFUNCTION = 6,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_POWER_FET_CHANNEL_OPEN = 7,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_ENABLE_HIGH_MALFUNCTION = 8,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_TURN_OFF_PATH_TOO_SLOW = 9,
  EFUSE_SELF_TEST_EFUSE_RESULT_FAILED_NOT_LATCHED = 10,
  EFUSE_SELF_TEST_EFUSE_RESULT_SKIPPED = 11
 };

#define Parse_VCFRONT_ESPValveTemp(d) ((d & 0x7ff0000000000000) >> 52)
#define Parse_VCFRONT_vbatFusedHighTemp(d) ((d & 0x1ffc000000000000) >> 50)
#define Parse_VCFRONT_EPAS3PTemp(d) ((d & 0xffe000000000000) >> 49)
#define Parse_VCFRONT_EPAS3STemp(d) ((d & 0xffe000000000000) >> 49)
#define Parse_VCFRONT_ESPMotorTemp(d) ((d & 0xffe000000000000) >> 49)
#define Parse_VCFRONT_PCSTemp(d) ((d & 0xffe000000000000) >> 49)
#define Parse_VCFRONT_iBoosterTemp(d) ((d & 0xffe000000000000) >> 49)
#define Parse_VCFRONT_leftControllerTemp(d) ((d & 0xffe000000000000) >> 49)
#define Parse_VCFRONT_rightControllerTemp(d) ((d & 0xffe000000000000) >> 49)
#define Parse_VCFRONT_headlampLeftVoltage(d) ((d & 0xff00000000000) >> 44)

enum VCFRONT_headlampLeftVoltage_Values { 
  SNA = 255
 };

#define Parse_VCFRONT_railB_12v(d) ((d & 0xfff00000000000) >> 44)
#define Parse_VCFRONT_pump1AndFanVoltage(d) ((d & 0x3ffc0000000000) >> 42)
#define Parse_VCFRONT_ChargePumpVoltage(d) ((d & 0xffff0000000000) >> 40)
#define Parse_VCFRONT_dischargedIBSAmpHours(d) ((d & 0xffff0000000000) >> 40)
#define Parse_VCFRONT_uiState(d) ((d & 0x10000000000) >> 40)
#define Parse_VCFRONT_uiFault(d) ((d & 0x8000000000) >> 39)
#define Parse_VCFRONT_sleepBypassCurrent(d) ((d & 0x3fffc000000000) >> 38)
#define Parse_VCFRONT_autopilot1Voltage(d) ((d & 0x1fffe000000000) >> 37)
#define Parse_VCFRONT_autopilot2Voltage(d) ((d & 0x1fffe000000000) >> 37)
#define Parse_VCFRONT_ESPValveVoltage(d) ((d & 0xffff000000000) >> 36)
#define Parse_VCFRONT_headlampRightState(d) ((d & 0x1000000000) >> 36)
#define Parse_VCFRONT_headlampRightFault(d) ((d & 0x800000000) >> 35)
#define Parse_VCFRONT_headlampRightCurrent(d) ((d & 0xffc00000000) >> 34)
#define Parse_VCFRONT_vbatFusedHighVoltage(d) ((d & 0x3fffc00000000) >> 34)
#define Parse_VCFRONT_EPAS3PVoltage(d) ((d & 0x1fffe00000000) >> 33)
#define Parse_VCFRONT_EPAS3SVoltage(d) ((d & 0x1fffe00000000) >> 33)
#define Parse_VCFRONT_ESPMotorVoltage(d) ((d & 0x1fffe00000000) >> 33)
#define Parse_VCFRONT_PCSVoltage(d) ((d & 0x1fffe00000000) >> 33)
#define Parse_VCFRONT_iBoosterVoltage(d) ((d & 0x1fffe00000000) >> 33)
#define Parse_VCFRONT_leftControllerVoltage(d) ((d & 0x1fffe00000000) >> 33)
#define Parse_VCFRONT_rightControllerVoltage(d) ((d & 0x1fffe00000000) >> 33)
#define Parse_VCFRONT_railA_5v(d) ((d & 0xfff00000000) >> 32)
#define Parse_VCFRONT_uiCurrent(d) ((d & 0xffff00000000) >> 32)
#define Parse_VCFRONT_pump2AndAirCompState(d) ((d & 0x40000000) >> 30)
#define Parse_VCFRONT_pump2AndAirCompCurrent(d) ((d & 0x1fffe0000000) >> 29)
#define Parse_VCFRONT_IBSUnfilteredTemperature(d) ((d & 0xffff000000) >> 24)
#define Parse_VCFRONT_headlampRightVoltage(d) ((d & 0xff000000) >> 24)

enum VCFRONT_headlampRightVoltage_Values { 
  SNA = 255
 };

#define Parse_VCFRONT_autopilot1Current(d) ((d & 0x1fffe00000) >> 21)
#define Parse_VCFRONT_autopilot2Current(d) ((d & 0x1fffe00000) >> 21)
#define Parse_VCFRONT_ESPValveCurrent(d) ((d & 0xffff00000) >> 20)
#define Parse_VCFRONT_railB_5v(d) ((d & 0xfff00000) >> 20)
#define Parse_VCFRONT_vbatFusedHighCurrent(d) ((d & 0x3fffc0000) >> 18)
#define Parse_VCFRONT_EPAS3PCurrent(d) ((d & 0x1fffe0000) >> 17)
#define Parse_VCFRONT_EPAS3SCurrent(d) ((d & 0x1fffe0000) >> 17)
#define Parse_VCFRONT_ESPMotorCurrent(d) ((d & 0x1fffe0000) >> 17)
#define Parse_VCFRONT_PCSCurrent(d) ((d & 0x1fffe0000) >> 17)
#define Parse_VCFRONT_iBoosterCurrent(d) ((d & 0x1fffe0000) >> 17)
#define Parse_VCFRONT_leftControllerCurrent(d) ((d & 0x1fffe0000) >> 17)
#define Parse_VCFRONT_rightControllerCurrent(d) ((d & 0x1fffe0000) >> 17)
#define Parse_VCFRONT_headlampLeftTemperature(d) ((d & 0xff0000) >> 16)

enum VCFRONT_headlampLeftTemperature_Values { 
  SNA = 128
 };

#define Parse_VCFRONT_pump2AndAirCompVoltage(d) ((d & 0x1ffe000) >> 13)
#define Parse_VCFRONT_headlampRightTemperature(d) ((d & 0xff00) >> 8)

enum VCFRONT_headlampRightTemperature_Values { 
  SNA = 128
 };

#define ID242VCLEFT_LVPowerState 0x242

#define Parse_VCLEFT_rcmLVState(d) ((d & 0xc) >> 2)

enum VCLEFT_rcmLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCLEFT_tpmsLVState(d) ((d & 0xc000) >> 14)

enum VCLEFT_tpmsLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCLEFT_sccmLVState(d) ((d & 0x3000) >> 12)

enum VCLEFT_sccmLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCLEFT_diLVState(d) ((d & 0xc00) >> 10)

enum VCLEFT_diLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCLEFT_cpLVState(d) ((d & 0x300) >> 8)

enum VCLEFT_cpLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCLEFT_swcLVState(d) ((d & 0xc0) >> 6)

enum VCLEFT_swcLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCLEFT_lumbarLVState(d) ((d & 0x30) >> 4)

enum VCLEFT_lumbarLVState_Values { 
  LV_OFF = 0,
  LV_ON = 1,
  LV_GOING_DOWN = 2,
  LV_FAULT = 3
 };

#define Parse_VCLEFT_vehiclePowerStateDBG(d) ((d & 0xc) >> 2)

enum VCLEFT_vehiclePowerStateDBG_Values { 
  VEHICLE_POWER_STATE_OFF = 0,
  VEHICLE_POWER_STATE_CONDITIONING = 1,
  VEHICLE_POWER_STATE_ACCESSORY = 2,
  VEHICLE_POWER_STATE_DRIVE = 3
 };

#define ID243VCRIGHT_hvacStatus 0x243

#define Parse_VCRIGHT_hvacStatusIndex(d) ((d & 0xc) >> 2)

enum VCRIGHT_hvacStatusIndex_Values { 
  STATUS_UI = 0,
  STATUS_VCFRONT = 1,
  STATUS_VCFRONT2 = 2,
  END = 3
 };

#define Parse_VCRIGHT_hvacMassflowRefrigSystem(d) ((d & 0x3fc0) >> 6)
#define Parse_VCRIGHT_hvacQdotLeft(d) ((d & 0x3fff000) >> 12)
#define Parse_VCRIGHT_tempDuctLeftUpper(d) ((d & 0x3fc0) >> 6)

enum VCRIGHT_tempDuctLeftUpper_Values { 
  SNA = 255
 };

#define Parse_VCRIGHT_hvacRecircDoorPercent(d) ((d & 0xfc0000000000000) >> 54)
#define Parse_VCRIGHT_tempDuctRightUpper(d) ((d & 0x3fc0000000000000) >> 54)

enum VCRIGHT_tempDuctRightUpper_Values { 
  SNA = 255
 };

#define Parse_VCRIGHT_hvacQdotRight(d) ((d & 0x3fff000000000000) >> 48)
#define Parse_VCRIGHT_tempDuctLeft(d) ((d & 0xff000000000000) >> 48)

enum VCRIGHT_tempDuctLeft_Values { 
  SNA = 255
 };

#define Parse_VCRIGHT_hvacEvapInletTempEstimat(d) ((d & 0x7fe00000000000) >> 45)

enum VCRIGHT_hvacEvapInletTempEstimat_Values { 
  SNA = 1023
 };

#define Parse_VCRIGHT_tempDuctLeftLower(d) ((d & 0xff0000000000) >> 40)

enum VCRIGHT_tempDuctLeftLower_Values { 
  SNA = 255
 };

#define Parse_VCRIGHT_hvacCabinTempEst(d) ((d & 0x1ffc00000000) >> 34)
#define Parse_VCRIGHT_tempDuctRight(d) ((d & 0xff00000000) >> 32)

enum VCRIGHT_tempDuctRight_Values { 
  SNA = 255
 };

#define Parse_VCRIGHT_tempDuctRightLower(d) ((d & 0xff000000) >> 24)

enum VCRIGHT_tempDuctRightLower_Values { 
  SNA = 255
 };

#define Parse_VCRIGHT_hvacAirDistributionMode(d) ((d & 0x3800000) >> 23)

enum VCRIGHT_hvacAirDistributionMode_Values { 
  NONE = 0,
  FLOOR = 1,
  PANEL = 2,
  PANEL_FLOOR = 3,
  DEFROST = 4,
  DEFROST_FLOOR = 5,
  DEFROST_PANEL = 6,
  DEFROST_PANEL_FLOOR = 7
 };

#define Parse_VCRIGHT_hvacBlowerSegment(d) ((d & 0xf00000) >> 20)

enum VCRIGHT_hvacBlowerSegment_Values { 
  OFF = 0,
  1 = 1,
  2 = 2,
  3 = 3,
  4 = 4,
  5 = 5,
  6 = 6,
  7 = 7,
  8 = 8,
  9 = 9,
  10 = 10,
  11 = 11
 };

#define Parse_VCRIGHT_hvacDuctTargetLeft(d) ((d & 0xff0000) >> 16)

enum VCRIGHT_hvacDuctTargetLeft_Values { 
  SNA = 255
 };

#define Parse_VCRIGHT_hvacRecirc(d) ((d & 0x30000) >> 16)

enum VCRIGHT_hvacRecirc_Values { 
  AUTO = 0,
  RECIRC = 1,
  FRESH = 2
 };

#define Parse_VCRIGHT_hvacACRunning(d) ((d & 0x4000) >> 14)

enum VCRIGHT_hvacACRunning_Values { 
  OFF = 0,
  ON = 1
 };

#define Parse_VCRIGHT_hvacPowerState(d) ((d & 0xe000) >> 13)

enum VCRIGHT_hvacPowerState_Values { 
  OFF = 0,
  ON = 1,
  PRECONDITION = 2,
  OVERHEAT_PROTECT_FANONLY = 3,
  OVERHEAT_PROTECT = 4
 };

#define Parse_VCRIGHT_hvacVentStatus(d) ((d & 0xc00) >> 10)

enum VCRIGHT_hvacVentStatus_Values { 
  BOTH = 0,
  LEFT = 1,
  RIGHT = 2
 };

#define Parse_VCRIGHT_hvacDuctTargetRight(d) ((d & 0xff00) >> 8)

enum VCRIGHT_hvacDuctTargetRight_Values { 
  SNA = 255
 };

#define Parse_VCRIGHT_hvacSecondRowState(d) ((d & 0x700) >> 8)

enum VCRIGHT_hvacSecondRowState_Values { 
  AUTO = 0,
  OFF = 1,
  LOW = 2,
  MED = 3,
  HIGH = 4
 };

#define Parse_VCRIGHT_hvacSystemNominal(d) ((d & 0x20) >> 5)
#define Parse_VCRIGHT_hvacModelInitStatus(d) ((d & 0x70) >> 4)

enum VCRIGHT_hvacModelInitStatus_Values { 
  NOT_INIT_WAIT_FOR_SENSORS = 0,
  NOT_INIT_WAIT_FOR_GTW = 1,
  INIT_FROM_SENSORS = 2,
  INIT_FROM_SENSORS_PREDICTION_ERROR = 3,
  INIT_FORWARD_CALC = 4,
  INIT_WAITING_FOR_SENSORS = 5
 };

#define Parse_VCRIGHT_hvacOverheatProtActive(d) ((d & 0x2) >> 1)
#define ID20CVCRIGHT_hvacRequest 0x20c

#define Parse_VCRIGHT_wattsDemandEvap(d) ((d & 0x3ff800) >> 11)
#define Parse_VCRIGHT_hvacEvapEnabled(d) ((d & 0x20000000000000) >> 53)
#define Parse_VCRIGHT_conditioningRequest(d) ((d & 0x10000000000000) >> 52)
#define Parse_VCRIGHT_tempEvaporator(d) ((d & 0x3ff8000000000000) >> 51)

enum VCRIGHT_tempEvaporator_Values { 
  SNA = 2047
 };

#define Parse_VCRIGHT_tempEvaporatorTarget(d) ((d & 0xff0000000000) >> 40)

enum VCRIGHT_tempEvaporatorTarget_Values { 
  SNA = 255
 };

#define Parse_VCRIGHT_hvacBlowerSpeedRPMReq(d) ((d & 0x3ff00000000) >> 32)
#define Parse_VCRIGHT_hvacPerfTestRunning(d) ((d & 0x400000) >> 22)
#define Parse_VCRIGHT_evapPerformanceLow(d) ((d & 0x200000) >> 21)
#define Parse_VCRIGHT_tempAmbientRaw(d) ((d & 0xff00000) >> 20)

enum VCRIGHT_tempAmbientRaw_Values { 
  SNA = 0
 };

#define Parse_VCRIGHT_hvacHeatingEnabledLeft(d) ((d & 0x1000) >> 12)
#define Parse_VCRIGHT_hvacHeatingEnabledRight(d) ((d & 0x800) >> 11)
#define Parse_VCRIGHT_hvacPerfTestState(d) ((d & 0xc00) >> 10)

enum VCRIGHT_hvacPerfTestState_Values { 
  STOPPED = 0,
  WAITING = 1,
  BLOWING = 2
 };

#define Parse_VCRIGHT_hvacUnavailable(d) ((d & 0x100) >> 8)
#define ID2E1VCFRONT_status 0x2e1

#define Parse_VCFRONT_statusIndex(d) ((d & 0x38) >> 3)

enum VCFRONT_statusIndex_Values { 
  VCF_STS_IDX_BODY_CONTROLS = 0,
  VCF_STS_IDX_VEHICLE_STATE = 1,
  VCF_STS_IDX_HOMELINK = 2,
  VCF_STS_IDX_REFRIGERANT_SYSTEM = 3,
  VCF_STS_IDX_LV_BATTERY = 4,
  VCF_STS_IDX_SYSTEM_HEALTH = 5,
  VCF_STS_IDX_INVALID = 6
 };

#define Parse_VCFRONT_5VARailStable(d) ((d & 0x2000000000000000) >> 61)
#define Parse_VCFRONT_frunkLatchStatus(d) ((d & 0x1e) >> 1)

enum VCFRONT_frunkLatchStatus_Values { 
  LATCH_SNA = 0,
  LATCH_OPENED = 1,
  LATCH_CLOSED = 2,
  LATCH_CLOSING = 3,
  LATCH_OPENING = 4,
  LATCH_AJAR = 5,
  LATCH_TIMEOUT = 6,
  LATCH_DEFAULT = 7,
  LATCH_FAULT = 8
 };

#define Parse_VCFRONT_iBoosterWakeLine(d) ((d & 0x2000000000000000) >> 61)
#define Parse_VCFRONT_tempCompTargetVoltage(d) ((d & 0x1ff80) >> 7)
#define Parse_VCFRONT_5VBRailStable(d) ((d & 0x1000000000000000) >> 60)
#define Parse_VCFRONT_epasWakeLine(d) ((d & 0x1000000000000000) >> 60)
#define Parse_VCFRONT_12VARailStable(d) ((d & 0x800000000000000) >> 59)
#define Parse_VCFRONT_iBoosterStateDBG(d) ((d & 0x3800000000000000) >> 59)

enum VCFRONT_iBoosterStateDBG_Values { 
  IBOOSTER_OFF = 0,
  IBOOSTER_ON = 1,
  IBOOSTER_GOING_DOWN = 2,
  IBOOSTER_WRITING_DATA_SHUTDOWN = 3,
  IBOOSTER_FORCE_OFF = 4
 };

#define Parse_VCFRONT_12VBRailStable(d) ((d & 0x400000000000000) >> 58)
#define Parse_VCFRONT_railAState(d) ((d & 0x200000000000000) >> 57)
#define Parse_VCFRONT_homelinkV2Response0(d) ((d & 0xff00000000000000) >> 56)
#define Parse_VCFRONT_maxEvapHeatRejection(d) ((d & 0xff00000000000000) >> 56)
#define Parse_VCFRONT_railBState(d) ((d & 0x100000000000000) >> 56)
#define Parse_VCFRONT_vehicleStatusDBG(d) ((d & 0x1f00000000000000) >> 56)

enum VCFRONT_vehicleStatusDBG_Values { 
  VEHICLE_STATUS_INIT = 0,
  VEHICLE_STATUS_LOW_POWER_STANDBY = 1,
  VEHICLE_STATUS_SILENT_WAKE = 2,
  VEHICLE_STATUS_BATTERY_POST_WAKE = 3,
  VEHICLE_STATUS_SYSTEM_CHECKS = 4,
  VEHICLE_STATUS_SLEEP_SHUTDOWN = 5,
  VEHICLE_STATUS_SLEEP_STANDBY = 6,
  VEHICLE_STATUS_LV_SHUTDOWN = 7,
  VEHICLE_STATUS_LV_AWAKE = 8,
  VEHICLE_STATUS_HV_UP_STANDBY = 9,
  VEHICLE_STATUS_ACCESSORY = 10,
  VEHICLE_STATUS_ACCESSORY_PLUS = 11,
  VEHICLE_STATUS_CONDITIONING = 12,
  VEHICLE_STATUS_DRIVE = 13,
  VEHICLE_STATUS_CRASH = 14,
  VEHICLE_STATUS_OTA = 15,
  VEHICLE_STATUS_TURN_ON_RAILS = 16,
  VEHICLE_STATUS_RESET = 17
 };

#define Parse_VCFRONT_wiperSpeed(d) ((d & 0xf00000000000000) >> 56)

enum VCFRONT_wiperSpeed_Values { 
  WIPER_SPEED_SNA = 0,
  WIPER_SPEED_OFF = 1,
  WIPER_SPEED_1 = 2,
  WIPER_SPEED_2 = 3,
  WIPER_SPEED_3 = 4,
  WIPER_SPEED_4 = 5,
  WIPER_SPEED_5 = 6,
  WIPER_SPEED_LOW = 7,
  WIPER_SPEED_HIGH = 8
 };

#define Parse_VCFRONT_ChargePumpVoltageStable(d) ((d & 0x80000000000000) >> 55)
#define Parse_VCFRONT_PEResetLineState(d) ((d & 0x40000000000000) >> 54)
#define Parse_VCFRONT_HSDInitCompleteU13(d) ((d & 0x20000000000000) >> 53)
#define Parse_VCFRONT_HSDInitCompleteU16(d) ((d & 0x10000000000000) >> 52)
#define Parse_VCFRONT_wiperPosition(d) ((d & 0x70000000000000) >> 52)

enum VCFRONT_wiperPosition_Values { 
  WIPER_POSITION_SNA = 0,
  WIPER_POSITION_SERVICE = 1,
  WIPER_POSITION_DEPRESSED_PARK = 2,
  WIPER_POSITION_DELAYED_REST = 3,
  WIPER_POSITION_WIPING = 4
 };

#define Parse_VCFRONT_chargeNeeded(d) ((d & 0x8000000000000) >> 51)
#define Parse_VCFRONT_PCSMia(d) ((d & 0x4000000000000) >> 50)
#define Parse_VCFRONT_IBSFault(d) ((d & 0x2000000000000) >> 49)
#define Parse_VCFRONT_12VOverchargeCounter(d) ((d & 0xf000000000000) >> 48)
#define Parse_VCFRONT_batterySMState(d) ((d & 0xf000000000000) >> 48)

enum VCFRONT_batterySMState_Values { 
  BATTERY_SM_STATE_INIT = 0,
  BATTERY_SM_STATE_CHARGE = 1,
  BATTERY_SM_STATE_DISCHARGE = 2,
  BATTERY_SM_STATE_STANDBY = 3,
  BATTERY_SM_STATE_RESISTANCE_ESTIMATION = 4,
  BATTERY_SM_STATE_OTA_STANDBY = 5,
  BATTERY_SM_STATE_DISCONNECTED_BATTERY_TEST = 6,
  BATTERY_SM_STATE_SHORTED_CELL_TEST = 7,
  BATTERY_SM_STATE_FAULT = 8
 };

#define Parse_VCFRONT_homelinkV2Response1(d) ((d & 0xff000000000000) >> 48)
#define Parse_VCFRONT_minEvapHeatRejection(d) ((d & 0xff000000000000) >> 48)
#define Parse_VCFRONT_vbatMonitorVoltage(d) ((d & 0xfff000000000000) >> 48)

enum VCFRONT_vbatMonitorVoltage_Values { 
  SNA = 4095
 };

#define Parse_VCFRONT_wiperState(d) ((d & 0xf000000000000) >> 48)

enum VCFRONT_wiperState_Values { 
  WIPER_STATE_SNA = 0,
  WIPER_STATE_SERVICE = 1,
  WIPER_STATE_FAULT = 2,
  WIPER_STATE_DELAYED_REST = 3,
  WIPER_STATE_PARK = 4,
  WIPER_STATE_WASH = 5,
  WIPER_STATE_MOMENTARY_WIPE = 6,
  WIPER_STATE_INTERMITTENT_HIGH = 7,
  WIPER_STATE_INTERMITTENT_LOW = 8,
  WIPER_STATE_CONT_FAST = 9,
  WIPER_STATE_CONT_SLOW = 10,
  WIPER_STATE_INT_AUTO_LOW = 11,
  WIPER_STATE_INT_AUTO_HIGH = 12
 };

#define Parse_VCFRONT_crashDetectedType(d) ((d & 0x300000000000) >> 44)

enum VCFRONT_crashDetectedType_Values { 
  CRASH_DETECTED_TYPE_NONE = 0,
  CRASH_DETECTED_TYPE_MINOR_1 = 1,
  CRASH_DETECTED_TYPE_MINOR_2 = 2,
  CRASH_DETECTED_TYPE_SEVERE = 3
 };

#define Parse_VCFRONT_voltageDropCounter(d) ((d & 0xf00000000000) >> 44)
#define Parse_VCFRONT_crashState(d) ((d & 0xc0000000000) >> 42)

enum VCFRONT_crashState_Values { 
  CRASH_STATE_IDLE = 0,
  CRASH_STATE_MINOR_1 = 1,
  CRASH_STATE_MINOR_2 = 2,
  CRASH_STATE_SEVERE = 3
 };

#define Parse_VCFRONT_crashUnlockOverrideSet(d) ((d & 0x10000000000) >> 40)
#define Parse_VCFRONT_freezeEvapITerm(d) ((d & 0x10000000000) >> 40)
#define Parse_VCFRONT_homelinkV2Response2(d) ((d & 0xff0000000000) >> 40)
#define Parse_VCFRONT_timeSpentSleeping(d) ((d & 0xff0000000000) >> 40)
#define Parse_VCFRONT_voltageFloorReachedCount(d) ((d & 0xf0000000000) >> 40)
#define Parse_VCFRONT_airCompressorStatus(d) ((d & 0x38000000000) >> 39)

enum VCFRONT_airCompressorStatus_Values { 
  VCFRONT_AIR_COMPRESSOR_STATUS_OFF = 0,
  VCFRONT_AIR_COMPRESSOR_STATUS_ON = 1,
  VCFRONT_AIR_COMPRESSOR_STATUS_TURNING_OFF = 2,
  VCFRONT_AIR_COMPRESSOR_STATUS_TURNING_ON = 3,
  VCFRONT_AIR_COMPRESSOR_STATUS_FAULT = 4,
  VCFRONT_AIR_COMPRESSOR_STATUS_RETRY_AVAILABLE = 5,
  VCFRONT_AIR_COMPRESSOR_STATUS_SNA = 7
 };

#define Parse_VCFRONT_isEvapOperationAllowed(d) ((d & 0x8000000000) >> 39)
#define Parse_VCFRONT_chillerDemandActive(d) ((d & 0x4000000000) >> 38)
#define Parse_VCFRONT_compPerfRecoveryLimited(d) ((d & 0x2000000000) >> 37)
#define Parse_VCFRONT_AS8510Voltage(d) ((d & 0xfff000000000) >> 36)

enum VCFRONT_AS8510Voltage_Values { 
  SNA = 4095
 };

#define Parse_VCFRONT_headlightLeftVPosition(d) ((d & 0x3ff000000000) >> 36)
#define Parse_VCFRONT_hvacModeNotAttainable(d) ((d & 0x1000000000) >> 36)
#define Parse_VCFRONT_voltageProfile(d) ((d & 0x3000000000) >> 36)

enum VCFRONT_voltageProfile_Values { 
  VOLTAGE_PROFILE_CHARGE = 0,
  VOLTAGE_PROFILE_FLOAT = 1,
  VOLTAGE_PROFILE_REDUCED_FLOAT = 2,
  VOLTAGE_PROFILE_ALWAYS_CLOSED_CONTACTORS = 3
 };

#define Parse_VCFRONT_hasLowRefrigerant(d) ((d & 0x800000000) >> 35)
#define Parse_VCFRONT_isColdStartRunning(d) ((d & 0x400000000) >> 34)
#define Parse_VCFRONT_reverseBatteryFault(d) ((d & 0x400000000) >> 34)
#define Parse_VCFRONT_isHeatPumpOilPurgeActive(d) ((d & 0x200000000) >> 33)
#define Parse_VCFRONT_homelinkV2Response3(d) ((d & 0xff00000000) >> 32)
#define Parse_VCFRONT_pressureRefrigSuction(d) ((d & 0x7f00000000) >> 32)

enum VCFRONT_pressureRefrigSuction_Values { 
  SNA = 127
 };

#define Parse_VCFRONT_silentWakeIBSCurrent(d) ((d & 0xfff00000000) >> 32)
#define Parse_VCFRONT_sleepCurrent(d) ((d & 0xfff00000000) >> 32)
#define Parse_VCFRONT_headlightRightVPosition(d) ((d & 0xffc000000) >> 26)
#define Parse_VCFRONT_homelinkV2Response4(d) ((d & 0xff000000) >> 24)
#define Parse_VCFRONT_pressureRefrigDischarge(d) ((d & 0xff000000) >> 24)

enum VCFRONT_pressureRefrigDischarge_Values { 
  SNA = 255
 };

#define Parse_VCFRONT_vbatProt(d) ((d & 0xfff000000) >> 24)
#define Parse_VCFRONT_shortedCellFaultCounter(d) ((d & 0x700000) >> 20)
#define Parse_VCFRONT_frunkInteriorRelSwitch(d) ((d & 0x10000) >> 16)
#define Parse_VCFRONT_homelinkCommStatus(d) ((d & 0x30000) >> 16)

enum VCFRONT_homelinkCommStatus_Values { 
  HOMELINK_COMM_STATUS_SNA = 0,
  HOMELINK_COMM_STATUS_OFF = 1,
  HOMELINK_COMM_STATUS_ON = 2,
  HOMELINK_COMM_STATUS_FAULT = 3
 };

#define Parse_VCFRONT_hvacPerfTestCommand(d) ((d & 0x30000) >> 16)

enum VCFRONT_hvacPerfTestCommand_Values { 
  NOT_STARTED = 0,
  INIT = 1,
  BLOW = 2,
  STOP = 3
 };

#define Parse_VCFRONT_anyClosureOpen(d) ((d & 0x8000) >> 15)
#define Parse_VCFRONT_anyDoorOpen(d) ((d & 0x4000) >> 14)
#define Parse_VCFRONT_coolantFillRoutineStatus(d) ((d & 0xc000) >> 14)

enum VCFRONT_coolantFillRoutineStatus_Values { 
  NOT_READY = 0,
  MOVING_TO_FILL_POSITION = 1,
  READY_TO_FILL = 2,
  FAULTED = 3
 };

#define Parse_VCFRONT_hornOn(d) ((d & 0x2000) >> 13)
#define Parse_VCFRONT_radarHeaterState(d) ((d & 0x7000) >> 12)

enum VCFRONT_radarHeaterState_Values { 
  HEATER_STATE_SNA = 0,
  HEATER_STATE_ON = 1,
  HEATER_STATE_OFF = 2,
  HEATER_STATE_OFF_UNAVAILABLE = 3,
  HEATER_STATE_FAULT = 4
 };

#define Parse_VCFRONT_refrigFillRoutineStatus(d) ((d & 0x3000) >> 12)

enum VCFRONT_refrigFillRoutineStatus_Values { 
  NOT_READY = 0,
  MOVING_TO_FILL_POSITION = 1,
  READY_TO_FILL = 2,
  FAULTED = 3
 };

#define Parse_VCFRONT_passengerBuckleStatus(d) ((d & 0x80) >> 7)

enum VCFRONT_passengerBuckleStatus_Values { 
  UNBUCKLED = 0,
  BUCKLED = 1
 };

#define Parse_VCFRONT_frunkLatchType(d) ((d & 0xc0) >> 6)

enum VCFRONT_frunkLatchType_Values { 
  FRUNK_LATCH_TYPE_UNKNOWN = 0,
  FRUNK_LATCH_TYPE_DOUBLE_ACTUATOR = 1,
  FRUNK_LATCH_TYPE_DOUBLE_PULL = 2
 };

#define Parse_VCFRONT_headlampLeftFanStatus(d) ((d & 0x10) >> 4)
#define Parse_VCFRONT_headlampRightFanStatus(d) ((d & 0x8) >> 3)
#define Parse_VCFRONT_frunkAccessPost(d) ((d & 0x4) >> 2)
#define Parse_VCFRONT_isActiveHeatingBattery(d) ((d & 0x2) >> 1)
#define ID381VCFRONT_logging1Hz 0x381

#define Parse_VCFRONT_logging1HzIndex(d) ((d & 0x3e0) >> 5)

enum VCFRONT_logging1HzIndex_Values { 
  COOLANT = 0,
  FAN_DEMAND_CONDENSER_AND_FET_TEMPS = 1,
  COOLANT_VALVE = 2,
  HCML_LED_TEMPS = 3,
  HCMR_LED_TEMPS = 4,
  HOMELINK = 5,
  HEADLAMP_AIM = 6,
  HP_EXV_RANGE = 7,
  HP_DATA_AND_ACCUMULATORS = 8,
  HP_CONTROL_LOOP_AND_STATE = 9,
  HP_CYCLE_MODEL = 10,
  HP_EXV_CALIBRATION = 11,
  HP_DISSIPATION_AND_POWER = 12,
  HP_TEMPS_AND_DEMANDS = 13,
  HP_PRESSURE_CONTROL = 14,
  HP_ARBITRATION = 15,
  HP_MODE_SELECT_AND_ESTIMATES = 16,
  HP_MODE_OPTIONS_AND_ESTIMATES = 17,
  BODY_CONTROL = 18,
  COOLANT_2 = 19,
  END = 20
 };

#define Parse_VCFRONT_calibratedPositionHCML(d) ((d & 0x7fe0) >> 5)
#define Parse_VCFRONT_chillerExvRange(d) ((d & 0x1ff0) >> 4)
#define Parse_VCFRONT_coolantValveRecalReason(d) ((d & 0x3800000000000000) >> 59)

enum VCFRONT_coolantValveRecalReason_Values { 
  UNDEFINED = 0,
  MAX_TRAVEL = 1,
  GENERAL_FAULT = 2,
  CALIBRATION_FAULT_NO_TRAVEL = 3,
  SELF_TEST = 4,
  MOTOR_FEEDBACK_INTERRUPTED = 5,
  NVRAM_LOSS = 6,
  SYSTEM_LEVEL_FAULT_RESPONSE = 7
 };

#define Parse_VCFRONT_drlMode(d) ((d & 0x1800000000000000) >> 59)

enum VCFRONT_drlMode_Values { 
  DRL_MODE_OFF = 0,
  DRL_MODE_POSITION = 1,
  DRL_MODE_DRL = 2
 };

#define Parse_VCFRONT_estPressureLiq(d) ((d & 0x1fc) >> 2)
#define Parse_VCFRONT_exteriorQuietModeEnabled(d) ((d & 0x800000000000000) >> 59)
#define Parse_VCFRONT_hpForceScavenge(d) ((d & 0x800000000000000) >> 59)
#define Parse_VCFRONT_hpGeneral(d) ((d & 0x800000000000000) >> 59)
#define Parse_VCFRONT_passiveSeriesRegOn(d) ((d & 0x800000000000000) >> 59)
#define Parse_VCFRONT_radPassiveRejectEstimate(d) ((d & 0x7ffe00) >> 9)
#define Parse_VCFRONT_subcoolActual(d) ((d & 0x1fc) >> 2)
#define Parse_VCFRONT_battOverStagUpperLimit(d) ((d & 0x400000000000000) >> 58)
#define Parse_VCFRONT_exteriorQuietModeAllowed(d) ((d & 0x400000000000000) >> 58)
#define Parse_VCFRONT_hpAmbientSource(d) ((d & 0x400000000000000) >> 58)
#define Parse_VCFRONT_CCQdotFdFrwrdTarget(d) ((d & 0xfe00000000000000) >> 57)
#define Parse_VCFRONT_battUnderStagUpperLimit(d) ((d & 0x200000000000000) >> 57)
#define Parse_VCFRONT_hpCabinHeatScavengeOnly(d) ((d & 0x200000000000000) >> 57)
#define Parse_VCFRONT_HCML_lowBeamSpotTemp(d) ((d & 0xff00000000000000) >> 56)

enum VCFRONT_HCML_lowBeamSpotTemp_Values { 
  SNA = 128
 };

#define Parse_VCFRONT_HCMR_lowBeamSpotTemp(d) ((d & 0xff00000000000000) >> 56)

enum VCFRONT_HCMR_lowBeamSpotTemp_Values { 
  SNA = 128
 };

#define Parse_VCFRONT_ambientColderThanBatt(d) ((d & 0x100000000000000) >> 56)
#define Parse_VCFRONT_battDissipation(d) ((d & 0xff00000000000000) >> 56)
#define Parse_VCFRONT_chillerExvCalibOffset(d) ((d & 0xff00000000000000) >> 56)
#define Parse_VCFRONT_condenserPressureLimit(d) ((d & 0x3f00000000000000) >> 56)
#define Parse_VCFRONT_coolantValveCountRange(d) ((d & 0xffc) >> 2)

enum VCFRONT_coolantValveCountRange_Values { 
  SNA = 1023
 };

#define Parse_VCFRONT_dischargePressureTarget(d) ((d & 0x7f00000000000000) >> 56)
#define Parse_VCFRONT_feedFwdMDotEvaporator(d) ((d & 0xff00000000000000) >> 56)
#define Parse_VCFRONT_homelinkRegionCode(d) ((d & 0xf00000000000000) >> 56)

enum VCFRONT_homelinkRegionCode_Values { 
  HOMELINK_REGION_CODE_UNKNOWN = 0,
  HOMELINK_REGION_CODE_EUROPE = 1,
  HOMELINK_REGION_CODE_AMERICAS = 5,
  HOMELINK_REGION_CODE_REST_OF_WORLD = 8,
  HOMELINK_REGION_CODE_CHINA = 9
 };

#define Parse_VCFRONT_hpCabinHeatAmbientSource(d) ((d & 0x100000000000000) >> 56)
#define Parse_VCFRONT_modeTransitionID(d) ((d & 0x3f00000000000000) >> 56)

enum VCFRONT_modeTransitionID_Values { 
  PARALLEL_F1_noFlowRequest = 0,
  SERIES_F2_faultPumps = 1,
  SERIES_F3_faultTempSensors = 2,
  SERIES_1_drive_batteryWantsCool = 3,
  SERIES_2_drive_batteryNeedsHeat = 4,
  SERIES_3_drive_batteryWantsHeat = 5,
  PARALLEL_2_drive_batteryWantsHeat = 6,
  PARALLEL_3_drive_batteryWantsCool = 7,
  PARALLEL_4_drive_batteryNeedsCool = 8,
  SERIES_4_charge_batteryNeedsHeat = 9,
  SERIES_5_charge_batteryWantsHeat = 10,
  PARALLEL_5_charge_batteryWantsHeat = 11,
  PARALLEL_6_charge_batteryWantsCool = 12,
  SERIES_6_fastCharge_batteryNeedsHeat = 13,
  SERIES_7_fastCharge_batteryWantsCool = 14,
  PARALLEL_7_fastCharge_batteryWantsCool = 15,
  PARALLEL_8_fastCharge_batteryWantsHeat = 16,
  SERIES_8_preConditioning_batteryNeedsHeat = 17,
  SERIES_9_drive_driveUnitThermalLimiting = 18,
  PARALLEL_9_drive_batteryThermalLimiting = 19,
  INIT = 20,
  OVERRIDE = 21,
  UNDEFINED = 22,
  ENTER_AMBIENTSOURCE = 23,
  EXIT_AMBIENTSOURCE = 24,
  SER_1_drive_battNeedsActiveCooling_evapEnabled = 25,
  SER_2_drive_battBelowHotStagnationTemp = 26,
  SER_3_drive_chillerPassivelyCools = 27,
  SER_4_drive_radPassivelyCoolsBatt = 28,
  SER_5_FC_battHeatingNeeded = 29,
  SER_6_FC_battNeedsActiveCooling_evapDisabled = 30,
  SER_7_FC_battNeedsActiveCooling_evapEnabled = 31,
  SER_8_charge_battBelowPassiveTarget = 32,
  PAR_1_drive_battNeedsActiveCooling_evapDisabled = 33,
  PAR_2_drive_ptNeedsActiveCooling = 34,
  PAR_3_drive_chillerPassivelyCoolsBatt = 35,
  PAR_4_drive_cannotPassivelyCoolBatt = 36,
  PAR_5_drive_battAboveHotStagnationTemp = 37,
  PAR_6_FC_battNeedsActiveCooling_evapDisabled = 38,
  PAR_7_FC_battNeedsActiveCooling_evapEnabled = 39,
  PAR_8_charge_battAbovePassiveTarget = 40
 };

#define Parse_VCFRONT_passiveDemandRadBypass(d) ((d & 0x7f00000000000000) >> 56)
#define Parse_VCFRONT_ambientSourcingAvailable(d) ((d & 0x80000000000000) >> 55)
#define Parse_VCFRONT_hpCabinHeatReheatScaveng(d) ((d & 0x80000000000000) >> 55)
#define Parse_VCFRONT_hpCabinHeatReheatAmbient(d) ((d & 0x40000000000000) >> 54)
#define Parse_VCFRONT_hpMode(d) ((d & 0x7c0000000000000) >> 54)

enum VCFRONT_hpMode_Values { 
  NONE = 0,
  GENERAL = 1,
  AMBIENT_SOURCE = 2,
  CABIN_HEAT_SCAVENGE_ONLY = 3,
  CABIN_HEAT_AMBIENT_SOURCE = 4,
  CABIN_HEAT_REHEAT_SCAVENGE = 5,
  CABIN_HEAT_REHEAT_AMBIENT_SOURCE = 6,
  CABIN_HEAT_BLEND = 7,
  CABIN_HEAT_COP1 = 8,
  CABIN_HEAT_BATTERY_HEAT_REHEAT_AMBIENT_SOURCE = 9,
  CABIN_HEAT_BATTERY_COOL_REHEAT = 10,
  CABIN_COOL_EVAPORATOR = 11,
  CABIN_COOL_EVAPORATOR_REHEAT = 12,
  BATTERY_HEAT_AMBIENT_SOURCE = 13,
  BATTERY_HEAT_COP1 = 14,
  BATTERY_COOL = 15,
  BATTERY_COOL_CABIN_CONDENSER_REHEAT = 16,
  BATTERY_COOL_CABIN_CONDENSER = 17,
  BATTERY_COOL_CABIN_REHEAT = 18,
  BATTERY_COOL_EVAPORATOR = 19
 };

#define Parse_VCFRONT_hpCabinHeatBlend(d) ((d & 0x20000000000000) >> 53)
#define Parse_VCFRONT_estPressureSuct(d) ((d & 0x7f0000000000000) >> 52)
#define Parse_VCFRONT_hpCabinHeatCOP1(d) ((d & 0x10000000000000) >> 52)
#define Parse_VCFRONT_hpSubcoolTarget(d) ((d & 0x1f0000000000000) >> 52)
#define Parse_VCFRONT_hpCabinHeatBatteryHeatRe(d) ((d & 0x8000000000000) >> 51)
#define Parse_VCFRONT_CCQdotFdbk(d) ((d & 0x1fc000000000000) >> 50)
#define Parse_VCFRONT_evapExvRange(d) ((d & 0x7fc000000000000) >> 50)
#define Parse_VCFRONT_fanDemandCondenser(d) ((d & 0x1fc000000000000) >> 50)
#define Parse_VCFRONT_hpCabinHeatBatteryCoolRe(d) ((d & 0x4000000000000) >> 50)
#define Parse_VCFRONT_modeDesired(d) ((d & 0xc000000000000) >> 50)

enum VCFRONT_modeDesired_Values { 
  SERIES = 0,
  PARALLEL = 1,
  BLEND = 2,
  AMBIENT_SOURCE = 3
 };

#define Parse_VCFRONT_battLoopWorthCooling(d) ((d & 0x2000000000000) >> 49)
#define Parse_VCFRONT_evapDisabledLowPsCutout(d) ((d & 0x2000000000000) >> 49)
#define Parse_VCFRONT_hpCabinCoolEvaporator(d) ((d & 0x2000000000000) >> 49)
#define Parse_VCFRONT_HCML_highBeamTemp(d) ((d & 0xff000000000000) >> 48)

enum VCFRONT_HCML_highBeamTemp_Values { 
  SNA = 128
 };

#define Parse_VCFRONT_HCMR_highBeamTemp(d) ((d & 0xff000000000000) >> 48)

enum VCFRONT_HCMR_highBeamTemp_Values { 
  SNA = 128
 };

#define Parse_VCFRONT_calibratedPositionHCMR(d) ((d & 0x3ff000000000000) >> 48)
#define Parse_VCFRONT_diDissipation(d) ((d & 0xff000000000000) >> 48)
#define Parse_VCFRONT_evapExvCalibOffset(d) ((d & 0xff000000000000) >> 48)
#define Parse_VCFRONT_feedFwdMDotCabinCondense(d) ((d & 0xff000000000000) >> 48)
#define Parse_VCFRONT_hpCabinCoolEvaporatorReh(d) ((d & 0x1000000000000) >> 48)
#define Parse_VCFRONT_lccActiveCoolTarget(d) ((d & 0x7f000000000000) >> 48)
#define Parse_VCFRONT_minFlowPDCont(d) ((d & 0xff000000000000) >> 48)
#define Parse_VCFRONT_ptLoopWorthCooling(d) ((d & 0x1000000000000) >> 48)
#define Parse_VCFRONT_targetPTActiveCool(d) ((d & 0xff000000000000) >> 48)
#define Parse_VCFRONT_CMPDischargeSuperheat(d) ((d & 0xf800000000000) >> 47)
#define Parse_VCFRONT_estCompRefrigMassflow(d) ((d & 0x1f800000000000) >> 47)
#define Parse_VCFRONT_hpBatteryHeatAmbientSour(d) ((d & 0x800000000000) >> 47)
#define Parse_VCFRONT_coolantValveAngleDrift(d) ((d & 0xffc00000000000) >> 46)
#define Parse_VCFRONT_hpBatteryHeatCOP1(d) ((d & 0x400000000000) >> 46)
#define Parse_VCFRONT_estPressureDisch(d) ((d & 0xfe00000000000) >> 45)
#define Parse_VCFRONT_hpBatteryCool(d) ((d & 0x200000000000) >> 45)
#define Parse_VCFRONT_lccInletTempEstimate(d) ((d & 0x1fe00000000000) >> 45)
#define Parse_VCFRONT_hpBatteryCoolCabinConden(d) ((d & 0x100000000000) >> 44)
#define Parse_VCFRONT_CCQdotActual(d) ((d & 0x3f80000000000) >> 43)
#define Parse_VCFRONT_fanDemandRadiator(d) ((d & 0x3f80000000000) >> 43)
#define Parse_VCFRONT_hpBatteryCoolCabinReheat(d) ((d & 0x80000000000) >> 43)
#define Parse_VCFRONT_hpBatteryCoolEvaporator(d) ((d & 0x40000000000) >> 42)
#define Parse_VCFRONT_hpCOP(d) ((d & 0xfc0000000000) >> 42)
#define Parse_VCFRONT_recircExvRange(d) ((d & 0x3fe0000000000) >> 41)
#define Parse_VCFRONT_HCML_turnTemp(d) ((d & 0xff0000000000) >> 40)

enum VCFRONT_HCML_turnTemp_Values { 
  SNA = 128
 };

#define Parse_VCFRONT_HCMR_turnTemp(d) ((d & 0xff0000000000) >> 40)

enum VCFRONT_HCMR_turnTemp_Values { 
  SNA = 128
 };

#define Parse_VCFRONT_PDischargeControllerOutp(d) ((d & 0xff0000000000) >> 40)
#define Parse_VCFRONT_chillerPassiveCoolPower(d) ((d & 0xff0000000000) >> 40)
#define Parse_VCFRONT_coolantValveOdometer(d) ((d & 0x3ff0000000000) >> 40)
#define Parse_VCFRONT_feedBackEvapTempControll(d) ((d & 0x7f0000000000) >> 40)
#define Parse_VCFRONT_pressureRefrigDischEst(d) ((d & 0x1f0000000000) >> 40)
#define Parse_VCFRONT_recircExvCalibOffset(d) ((d & 0xff0000000000) >> 40)
#define Parse_VCFRONT_suctionSuperheatEstTsSNA(d) ((d & 0xf0000000000) >> 40)
#define Parse_VCFRONT_targetPTPassive(d) ((d & 0xff0000000000) >> 40)
#define Parse_VCFRONT_currentPositionHCML(d) ((d & 0xffc000000000) >> 38)
#define Parse_VCFRONT_estTempLiq(d) ((d & 0x3fc000000000) >> 38)
#define Parse_VCFRONT_chillerInletTempEstimate(d) ((d & 0x1fe000000000) >> 37)
#define Parse_VCFRONT_coolantValveRecalCount(d) ((d & 0xffff000000000) >> 36)
#define Parse_VCFRONT_evapFdFrwrdTarget(d) ((d & 0x7f000000000) >> 36)
#define Parse_VCFRONT_hpBattStagTarget(d) ((d & 0x7f000000000) >> 36)
#define Parse_VCFRONT_tempRefrigSuction(d) ((d & 0xff000000000) >> 36)

enum VCFRONT_tempRefrigSuction_Values { 
  SNA = 255
 };

#define Parse_VCFRONT_tempRefrigDischargeEst(d) ((d & 0x1f800000000) >> 35)
#define Parse_VCFRONT_HCML_bladeTemp(d) ((d & 0xff00000000) >> 32)

enum VCFRONT_HCML_bladeTemp_Values { 
  SNA = 128
 };

#define Parse_VCFRONT_HCMR_bladeTemp(d) ((d & 0xff00000000) >> 32)

enum VCFRONT_HCMR_bladeTemp_Values { 
  SNA = 128
 };

#define Parse_VCFRONT_feedBackDuctTempControll(d) ((d & 0xff00000000) >> 32)
#define Parse_VCFRONT_lccExvCalibOffset(d) ((d & 0xff00000000) >> 32)
#define Parse_VCFRONT_lccExvRange(d) ((d & 0x1ff00000000) >> 32)
#define Parse_VCFRONT_lccPassiveHeatPower(d) ((d & 0xff00000000) >> 32)
#define Parse_VCFRONT_suctionPressureTarget(d) ((d & 0x3f00000000) >> 32)
#define Parse_VCFRONT_targetBatActiveCool(d) ((d & 0x7f00000000) >> 32)
#define Parse_VCFRONT_tempRefrigSuctionEst(d) ((d & 0x3f00000000) >> 32)
#define Parse_VCFRONT_coolantValveDailyAngleTr(d) ((d & 0xffc0000000) >> 30)
#define Parse_VCFRONT_estTempSuct(d) ((d & 0xfc0000000) >> 30)
#define Parse_VCFRONT_compEnergyDuringDrive(d) ((d & 0xfe0000000) >> 29)
#define Parse_VCFRONT_estCompPower(d) ((d & 0x1fe0000000) >> 29)
#define Parse_VCFRONT_evapFdbk(d) ((d & 0xfe0000000) >> 29)
#define Parse_VCFRONT_feedFwdFanDemand(d) ((d & 0xfe0000000) >> 29)
#define Parse_VCFRONT_currentPositionHCMR(d) ((d & 0x3ff0000000) >> 28)
#define Parse_VCFRONT_pumpBatteryFETTemp(d) ((d & 0xff0000000) >> 28)

enum VCFRONT_pumpBatteryFETTemp_Values { 
  SNA = 255
 };

#define Parse_VCFRONT_ambientSourcingDisabled(d) ((d & 0x4000000) >> 26)
#define Parse_VCFRONT_hpBattOverTempHvacDisabl(d) ((d & 0x4000000) >> 26)
#define Parse_VCFRONT_chillerLiftDisabledLowPs(d) ((d & 0x2000000) >> 25)
#define Parse_VCFRONT_hpDiagLouverCalib(d) ((d & 0x2000000) >> 25)
#define Parse_VCFRONT_HCML_diffuseTemp(d) ((d & 0xff000000) >> 24)

enum VCFRONT_HCML_diffuseTemp_Values { 
  SNA = 128
 };

#define Parse_VCFRONT_HCMR_diffuseTemp(d) ((d & 0xff000000) >> 24)

enum VCFRONT_HCMR_diffuseTemp_Values { 
  SNA = 128
 };

#define Parse_VCFRONT_PSuctionControllerOutput(d) ((d & 0xff000000) >> 24)
#define Parse_VCFRONT_ccLeftExvCalibOffset(d) ((d & 0xff000000) >> 24)
#define Parse_VCFRONT_estTempDisch(d) ((d & 0xff000000) >> 24)
#define Parse_VCFRONT_maxAllowedEvapPowerInSer(d) ((d & 0xff000000) >> 24)
#define Parse_VCFRONT_maxChillerCoolingPower(d) ((d & 0xff000000) >> 24)
#define Parse_VCFRONT_targetBatPassive(d) ((d & 0x7f000000) >> 24)
#define Parse_VCFRONT_ccLeftExvRange(d) ((d & 0xff800000) >> 23)
#define Parse_VCFRONT_DIQdotA(d) ((d & 0x1fc00000) >> 22)
#define Parse_VCFRONT_cabinHeatEnergyDuringDri(d) ((d & 0x1fc00000) >> 22)
#define Parse_VCFRONT_feedFwdPumpDemand(d) ((d & 0x1fc00000) >> 22)
#define Parse_VCFRONT_hpCompPowerIndex(d) ((d & 0xfe00000) >> 21)
#define Parse_VCFRONT_coolantValveWindupEstL(d) ((d & 0x3f00000) >> 20)
#define Parse_VCFRONT_pumpPowertrainFETTemp(d) ((d & 0xff00000) >> 20)

enum VCFRONT_pumpPowertrainFETTemp_Values { 
  SNA = 255
 };

#define Parse_VCFRONT_ccRightExvCalibOffset(d) ((d & 0xff0000) >> 16)
#define Parse_VCFRONT_dischargePressureLimit(d) ((d & 0x7f0000) >> 16)
#define Parse_VCFRONT_estCompressorRpm(d) ((d & 0x3f0000) >> 16)
#define Parse_VCFRONT_fanControlRadCanCool(d) ((d & 0x10000) >> 16)
#define Parse_VCFRONT_minAllowedChillerPowerIn(d) ((d & 0xff0000) >> 16)
#define Parse_VCFRONT_minAllowedSuctionPressur(d) ((d & 0x7f0000) >> 16)
#define Parse_VCFRONT_targetBatActiveHeat(d) ((d & 0x7f0000) >> 16)
#define Parse_VCFRONT_wiperCycles(d) ((d & 0xffff0000) >> 16)
#define Parse_VCFRONT_evapFdFrwrdTargetMinimum(d) ((d & 0x3f8000) >> 15)
#define Parse_VCFRONT_fanControlFeedfwdActive(d) ((d & 0x8000) >> 15)
#define Parse_VCFRONT_lowSideLiftEnergyDrive(d) ((d & 0x3f8000) >> 15)
#define Parse_VCFRONT_radActiveRejectEstimate(d) ((d & 0x3fff8000) >> 15)
#define Parse_VCFRONT_ccRightExvRange(d) ((d & 0x7fc000) >> 14)
#define Parse_VCFRONT_coolantValveWindupEstR(d) ((d & 0xfc000) >> 14)
#define Parse_VCFRONT_fanControlRadiatorUa(d) ((d & 0x1fc000) >> 14)
#define Parse_VCFRONT_hpAtSteadyState(d) ((d & 0x4000) >> 14)
#define Parse_VCFRONT_pressureRefrigSuctionEst(d) ((d & 0xfe000) >> 13)
#define Parse_VCFRONT_radiatorFanFETTemp(d) ((d & 0xff000) >> 12)

enum VCFRONT_radiatorFanFETTemp_Values { 
  SNA = 255
 };

#define Parse_VCFRONT_estQLift(d) ((d & 0x1fc00) >> 10)
#define Parse_VCFRONT_coolantLevelVoltage(d) ((d & 0x3fe00) >> 9)
#define Parse_VCFRONT_chillerExvCalibFailed(d) ((d & 0x100) >> 8)
#define Parse_VCFRONT_coolantValveRadBypass(d) ((d & 0x7f00) >> 8)

enum VCFRONT_coolantValveRadBypass_Values { 
  SNA = 127
 };

#define Parse_VCFRONT_hpCompPowerIndexFiltered(d) ((d & 0x7f00) >> 8)
#define Parse_VCFRONT_lowSideWattsLift(d) ((d & 0x7f00) >> 8)
#define Parse_VCFRONT_maxCompressorRPMAllowed(d) ((d & 0x7f00) >> 8)
#define Parse_VCFRONT_passiveCoolingState(d) ((d & 0x300) >> 8)

enum VCFRONT_passiveCoolingState_Values { 
  ChillerCoolsSeriesLoop = 0,
  ChillerCoolsParallelBattLoop = 1,
  ChillerAndRadCoolSeriesLoop = 2,
  CannotCoolBattery = 3
 };

#define Parse_VCFRONT_evapExvCalibFailed(d) ((d & 0x80) >> 7)
#define Parse_VCFRONT_fanControlRadiatorInletT(d) ((d & 0x1f80) >> 7)
#define Parse_VCFRONT_recircExvCalibFailed(d) ((d & 0x40) >> 6)
#define Parse_VCFRONT_suctionSuperheatEstPsSNA(d) ((d & 0x3c0) >> 6)
#define Parse_VCFRONT_totalLoadCoolingDominant(d) ((d & 0x40) >> 6)
#define Parse_VCFRONT_feedfwdLoadCoolingDomina(d) ((d & 0x20) >> 5)
#define Parse_VCFRONT_lccExvCalibFailed(d) ((d & 0x20) >> 5)
#define Parse_VCFRONT_ccLeftExvCalibFailed(d) ((d & 0x10) >> 4)
#define Parse_VCFRONT_modelLoadCoolingDominant(d) ((d & 0x10) >> 4)
#define Parse_VCFRONT_radiatorFanRunReason(d) ((d & 0xf0) >> 4)

enum VCFRONT_radiatorFanRunReason_Values { 
  NONE = 0,
  ACTIVE_MANAGER = 1,
  AMBIENT_SNIFF = 2,
  NVH_MASKING = 3,
  HEAT_PUMP = 4,
  COAST_MODE = 5,
  MIN_ON_GLOBAL = 6,
  MIN_ON_NVH = 7,
  UDS = 8
 };

#define Parse_VCFRONT_ccRightExvCalibFailed(d) ((d & 0x8) >> 3)
#define Parse_VCFRONT_cycleModelConverged(d) ((d & 0x8) >> 3)
#define Parse_VCFRONT_hpPotentialLowRefrig(d) ((d & 0x8) >> 3)
#define Parse_VCFRONT_hpRefrigerantPurgeState(d) ((d & 0xc) >> 2)

enum VCFRONT_hpRefrigerantPurgeState_Values { 
  IDLE = 0,
  EVAP_PURGE = 1,
  COMPLETE = 2
 };

#define ID318SystemTimeUTC 0x318

#define Parse_UTCyear318(d) ((d & 0xff00) >> 8)
#define Parse_UTCmonth318(d) ((d & 0xff00000000000000) >> 56)
#define Parse_UTCseconds318(d) ((d & 0xff000000000000) >> 48)
#define Parse_UTChour318(d) ((d & 0xff0000000000) >> 40)
#define Parse_UTCday318(d) ((d & 0xff00000000) >> 32)
#define Parse_UTCminutes318(d) ((d & 0xff000000) >> 24)
#define ID528UnixTime 0x528

#define Parse_UnixTimeSeconds528(d) ((d & 0xffffffff) >> 0)
#define ID229GearLever 0x229

#define Parse_GearLeverPosition229(d) ((d & 0x7000) >> 12)

enum GearLeverPosition229_Values { 
  Center = 0,
  Half Down = 1,
  Full Down = 2,
  Half Up = 3,
  Full Up = 4
 };

#define Parse_GearLeverButton229(d) ((d & 0x300) >> 8)
#define ID249SCCMLeftStalk 0x249

#define Parse_SCCM_leftStalkCrc(d) ((d & 0xff00) >> 8)
#define Parse_SCCM_leftStalkCounter(d) ((d & 0xf000000) >> 24)
#define Parse_SCCM_highBeamStalkStatus(d) ((d & 0x300000) >> 20)

enum SCCM_highBeamStalkStatus_Values { 
  IDLE = 0,
  PULL = 1,
  PUSH = 2,
  SNA = 3
 };

#define Parse_SCCM_washWipeButtonStatus(d) ((d & 0xc0000) >> 18)

enum SCCM_washWipeButtonStatus_Values { 
  NOT_PRESSED = 0,
  1ST_DETENT = 1,
  2ND_DETENT = 2,
  SNA = 3
 };

#define Parse_SCCM_turnIndicatorStalkStatus(d) ((d & 0xf0000) >> 16)

enum SCCM_turnIndicatorStalkStatus_Values { 
  IDLE = 0,
  UP_0_5 = 1,
  UP_1 = 2,
  UP_1_5 = 3,
  UP_2 = 4,
  DOWN_0_5 = 5,
  DOWN_1 = 6,
  DOWN_1_5 = 7,
  DOWN_2 = 8,
  SNA = 9
 };

#define Parse_SCCM_leftStalkReserved1(d) ((d & 0x1f00) >> 8)
#define ID186DIF_torque 0x186

#define Parse_DIF_torqueChecksum(d) ((d & 0xff00) >> 8)
#define Parse_DIF_torqueCounter(d) ((d & 0xf00000000000000) >> 56)
#define Parse_DIF_torqueCommand(d) ((d & 0x3ffe) >> 1)

enum DIF_torqueCommand_Values { 
  SNA = 4096
 };

#define Parse_DIF_axleSpeedQF(d) ((d & 0x8000000000) >> 39)
#define Parse_DIF_torqueActual(d) ((d & 0x3ffe000000000) >> 37)

enum DIF_torqueActual_Values { 
  SNA = 4096
 };

#define Parse_DIF_axleSpeed(d) ((d & 0xffff000000) >> 24)

enum DIF_axleSpeed_Values { 
  SNA = 32768
 };

#define Parse_DIF_slavePedalPos(d) ((d & 0xff00) >> 8)

enum DIF_slavePedalPos_Values { 
  SNA = 255
 };

#define ID396FrontOilPump 0x396

#define Parse_FrontOilPumpState396(d) ((d & 0x38) >> 3)

enum FrontOilPumpState396_Values { 
  OIL_PUMP_STANDBY = 0,
  OIL_PUMP_ENABLE = 1,
  OIL_PUMP_COLD_STARTUP = 2,
  OIL_PUMP_FAULTED = 6,
  OIL_PUMP_SNA = 7
 };

#define Parse_FrontOilPumpOilTempEstConfident3(d) ((d & 0x2000000000000000) >> 61)
#define Parse_FrontOilPumpLeadAngle396(d) ((d & 0xf000000000000000) >> 60)
#define Parse_FrontOilPumpDutyCycle396(d) ((d & 0xff00000000000000) >> 56)
#define Parse_FrontOilFlowActual396(d) ((d & 0xff000000000000) >> 48)
#define Parse_FrontOilPumpFluidTemp396(d) ((d & 0xff0000000000) >> 40)
#define Parse_FrontOilPumpOilTempEst396(d) ((d & 0xff00000000) >> 32)
#define Parse_FrontOilPumpPressureEstimate396(d) ((d & 0xff000000) >> 24)
#define Parse_FrontOilPumpPressureExpected396(d) ((d & 0xff0000) >> 16)
#define Parse_FrontOilPumpPhaseCurrent396(d) ((d & 0xff00) >> 8)
#define ID395DIR_oilPump 0x395

#define Parse_DIR_oilPumpState(d) ((d & 0x38) >> 3)

enum DIR_oilPumpState_Values { 
  OIL_PUMP_STANDBY = 0,
  OIL_PUMP_ENABLE = 1,
  OIL_PUMP_COLD_STARTUP = 2,
  OIL_PUMP_FAULTED = 6,
  OIL_PUMP_SNA = 7
 };

#define Parse_DIR_oilPumpFluidTQF(d) ((d & 0x2000000000000000) >> 61)

enum DIR_oilPumpFluidTQF_Values { 
  OIL_PUMP_FLUIDT_LOW_CONFIDENCE = 0,
  OIL_PUMP_FLUIDT_HIGH_CONFIDENCE = 1
 };

#define Parse_DIR_oilPumpLeadAngle(d) ((d & 0xf000000000000000) >> 60)
#define Parse_DIR_oilPumpFlowTarget(d) ((d & 0xff00000000000000) >> 56)
#define Parse_DIR_oilPumpFlowActual(d) ((d & 0xff000000000000) >> 48)
#define Parse_DIR_oilPumpFluidT(d) ((d & 0xff0000000000) >> 40)
#define Parse_DIR_oilPumpPhaseCurrent(d) ((d & 0xff00000000) >> 32)
#define Parse_DIR_oilPumpPressureEstimate(d) ((d & 0xff000000) >> 24)
#define Parse_DIR_oilPumpPressureExpected(d) ((d & 0xff0000) >> 16)
#define Parse_DIR_oilPumpPressureResidual(d) ((d & 0xff00) >> 8)
#define ID1D8RearTorque 0x1d8

#define Parse_TorqueFlags1D8(d) ((d & 0xff00) >> 8)
#define Parse_RearTorqueRequest1D8(d) ((d & 0x3ffe0) >> 5)
#define Parse_RearTorque1D8(d) ((d & 0xfff80000000000) >> 43)
#define Parse_Counter1D8(d) ((d & 0x3800) >> 11)
#define Parse_Checksum1D8(d) ((d & 0xff00) >> 8)
#define ID155WheelAngles 0x155

#define Parse_WheelAngleTicsFL155(d) ((d & 0xff00) >> 8)
#define Parse_WheelAngleTicsFR155(d) ((d & 0xff00000000000000) >> 56)
#define Parse_WheelAngleTicsRL155(d) ((d & 0xff000000000000) >> 48)
#define Parse_WheelAngleTicsRR155(d) ((d & 0xff0000000000) >> 40)
#define Parse_ESP_WheelRotationReR(d) ((d & 0x300000000) >> 32)

enum ESP_WheelRotationReR_Values { 
  WR_FORWARD = 0,
  WR_BACKWARD = 1,
  WR_STANDSTILL = 2,
  WR_NOT_DEFINABLE = 3
 };

#define Parse_ESP_WheelRotationReL(d) ((d & 0xc0000000) >> 30)

enum ESP_WheelRotationReL_Values { 
  WR_FORWARD = 0,
  WR_BACKWARD = 1,
  WR_STANDSTILL = 2,
  WR_NOT_DEFINABLE = 3
 };

#define Parse_ESP_WheelRotationFrR(d) ((d & 0x30000000) >> 28)

enum ESP_WheelRotationFrR_Values { 
  WR_FORWARD = 0,
  WR_BACKWARD = 1,
  WR_STANDSTILL = 2,
  WR_NOT_DEFINABLE = 3
 };

#define Parse_ESP_WheelRotationFrL(d) ((d & 0xc000000) >> 26)

enum ESP_WheelRotationFrL_Values { 
  WR_FORWARD = 0,
  WR_BACKWARD = 1,
  WR_STANDSTILL = 2,
  WR_NOT_DEFINABLE = 3
 };

#define Parse_ESP_wheelSpeedsQF(d) ((d & 0x1000000) >> 24)

enum ESP_wheelSpeedsQF_Values { 
  ONE_OR_MORE_WSS_INVALID = 0,
  ALL_WSS_VALID = 1
 };

#define Parse_ESP_vehicleStandstillSts(d) ((d & 0x800000) >> 23)

enum ESP_vehicleStandstillSts_Values { 
  NOT_STANDSTILL = 0,
  STANDSTILL = 1
 };

#define Parse_ESP_vehicleSpeed(d) ((d & 0xffc00000) >> 22)

enum ESP_vehicleSpeed_Values { 
  ESP_VEHICLE_SPEED_SNA = 1023
 };

#define Parse_ESP_wheelRotationCounter(d) ((d & 0xf000) >> 12)
#define Parse_ESP_wheelRotationChecksum(d) ((d & 0xff00) >> 8)
#define ID175WheelSpeed 0x175

#define Parse_WheelSpeedFL175(d) ((d & 0x3ffe000) >> 13)

enum WheelSpeedFL175_Values { 
  SNA = 8191
 };

#define Parse_WheelSpeedFR175(d) ((d & 0xfff8000000000000) >> 51)

enum WheelSpeedFR175_Values { 
  SNA = 8191
 };

#define Parse_WheelSpeedRL175(d) ((d & 0x7ffc000000000) >> 38)

enum WheelSpeedRL175_Values { 
  SNA = 8191
 };

#define Parse_WheelSpeedRR175(d) ((d & 0x3ffe000000) >> 25)

enum WheelSpeedRR175_Values { 
  SNA = 8191
 };

#define Parse_ESP_wheelSpeedsCounter(d) ((d & 0xf000) >> 12)
#define Parse_ESP_wheelSpeedsChecksum(d) ((d & 0xff00) >> 8)
#define ID185ESP_brakeTorque 0x185

#define Parse_ESP_brakeTorqueFrL(d) ((d & 0xfff000) >> 12)

enum ESP_brakeTorqueFrL_Values { 
  SNA = 4095
 };

#define Parse_ESP_brakeTorqueFrR(d) ((d & 0xfff0000000000000) >> 52)

enum ESP_brakeTorqueFrR_Values { 
  SNA = 4095
 };

#define Parse_ESP_brakeTorqueReL(d) ((d & 0xfff0000000000) >> 40)

enum ESP_brakeTorqueReL_Values { 
  SNA = 4095
 };

#define Parse_ESP_brakeTorqueReR(d) ((d & 0xfff0000000) >> 28)

enum ESP_brakeTorqueReR_Values { 
  SNA = 4095
 };

#define Parse_ESP_brakeTorqueQF(d) ((d & 0x4000) >> 14)

enum ESP_brakeTorqueQF_Values { 
  UNDEFINABLE_ACCURACY = 0,
  IN_SPEC = 1
 };

#define Parse_ESP_brakeTorqueCounter(d) ((d & 0xf000) >> 12)
#define Parse_ESP_brakeTorqueChecksum(d) ((d & 0xff00) >> 8)
#define ID1D4FrontTorqueOld 0x1d4

#define Parse_RAWTorqueFront1D4(d) ((d & 0xfff000000) >> 24)
#define ID1D5FrontTorque 0x1d5

#define Parse_FrontTorqueRequest1D5(d) ((d & 0x3ffe0) >> 5)
#define Parse_FrontTorque1D5(d) ((d & 0xfff80000000000) >> 43)
#define ID281VCFRONT_CMPRequest 0x281

#define Parse_VCFRONT_CMPTargetDuty(d) ((d & 0xffff0000) >> 16)
#define Parse_VCFRONT_CMPPowerLimit(d) ((d & 0xffff000000000000) >> 48)
#define Parse_VCFRONT_CMPReset(d) ((d & 0xff00000000) >> 32)
#define Parse_VCFRONT_CMPEnable(d) ((d & 0x1000000) >> 24)
#define ID3C2VCLEFT_switchStatus 0x3c2

#define Parse_VCLEFT_switchStatusIndex(d) ((d & 0xc) >> 2)

enum VCLEFT_switchStatusIndex_Values { 
  VCLEFT_SWITCH_STATUS_INDEX_0 = 0,
  VCLEFT_SWITCH_STATUS_INDEX_1 = 1,
  VCLEFT_SWITCH_STATUS_INDEX_INVALID = 2
 };

#define Parse_VCLEFT_hornSwitchPressed(d) ((d & 0x4000000000000000) >> 62)
#define Parse_VCLEFT_hazardButtonPressed(d) ((d & 0x2000000000000000) >> 61)
#define Parse_VCLEFT_swcLeftTiltRight(d) ((d & 0x6000000000000000) >> 61)

enum VCLEFT_swcLeftTiltRight_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_brakeSwitchPressed(d) ((d & 0x1000000000000000) >> 60)
#define Parse_VCLEFT_rightMirrorTilt(d) ((d & 0x3800000000000000) >> 59)

enum VCLEFT_rightMirrorTilt_Values { 
  MIRROR_TILT_STOP = 0,
  MIRROR_TILT_DOWN = 1,
  MIRROR_TILT_UP = 2,
  MIRROR_TILT_RIGHT = 3,
  MIRROR_TILT_LEFT = 4
 };

#define Parse_VCLEFT_swcLeftPressed(d) ((d & 0x1800000000000000) >> 59)

enum VCLEFT_swcLeftPressed_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_frontSeatTrackBack(d) ((d & 0x300000000000000) >> 56)

enum VCLEFT_frontSeatTrackBack_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_swcRightTiltLeft(d) ((d & 0x300000000000000) >> 56)

enum VCLEFT_swcRightTiltLeft_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_frontSeatTrackForward(d) ((d & 0xc0000000000000) >> 54)

enum VCLEFT_frontSeatTrackForward_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_swcRightTiltRight(d) ((d & 0xc0000000000000) >> 54)

enum VCLEFT_swcRightTiltRight_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_frontSeatTiltDown(d) ((d & 0x30000000000000) >> 52)

enum VCLEFT_frontSeatTiltDown_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_swcRightPressed(d) ((d & 0x30000000000000) >> 52)

enum VCLEFT_swcRightPressed_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_frontSeatTiltUp(d) ((d & 0xc000000000000) >> 50)

enum VCLEFT_frontSeatTiltUp_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_swcLeftTiltLeft(d) ((d & 0xc000000000000) >> 50)

enum VCLEFT_swcLeftTiltLeft_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_frontSeatLiftDown(d) ((d & 0x3000000000000) >> 48)

enum VCLEFT_frontSeatLiftDown_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_swcLeftScrollTicks(d) ((d & 0x3f000000000000) >> 48)
#define Parse_VCLEFT_frontSeatLiftUp(d) ((d & 0xc00000000000) >> 46)

enum VCLEFT_frontSeatLiftUp_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_frontSeatBackrestBack(d) ((d & 0x300000000000) >> 44)

enum VCLEFT_frontSeatBackrestBack_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_frontSeatBackrestForward(d) ((d & 0xc0000000000) >> 42)

enum VCLEFT_frontSeatBackrestForward_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_frontSeatLumbarDown(d) ((d & 0x30000000000) >> 40)

enum VCLEFT_frontSeatLumbarDown_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_swcRightScrollTicks(d) ((d & 0x3f0000000000) >> 40)
#define Parse_VCLEFT_frontSeatLumbarUp(d) ((d & 0xc000000000) >> 38)

enum VCLEFT_frontSeatLumbarUp_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_frontSeatLumbarIn(d) ((d & 0x3000000000) >> 36)

enum VCLEFT_frontSeatLumbarIn_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_frontSeatLumbarOut(d) ((d & 0xc00000000) >> 34)

enum VCLEFT_frontSeatLumbarOut_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_btnWindowSwPackUpLF(d) ((d & 0x100000000) >> 32)
#define Parse_VCLEFT_btnWindowUpLR(d) ((d & 0x100000000) >> 32)
#define Parse_VCLEFT_btnWindowAutoUpLR(d) ((d & 0x80000000) >> 31)
#define Parse_VCLEFT_btnWindowSwPackAutoUpLF(d) ((d & 0x80000000) >> 31)
#define Parse_VCLEFT_btnWindowDownLR(d) ((d & 0x40000000) >> 30)
#define Parse_VCLEFT_btnWindowSwPackDownLF(d) ((d & 0x40000000) >> 30)
#define Parse_VCLEFT_btnWindowAutoDownLR(d) ((d & 0x20000000) >> 29)
#define Parse_VCLEFT_btnWindowSwPackAutoDownLF(d) ((d & 0x20000000) >> 29)
#define Parse_VCLEFT_2RowSeatReclineSwitch(d) ((d & 0x10000000) >> 28)
#define Parse_VCLEFT_btnWindowSwPackUpLR(d) ((d & 0x10000000) >> 28)
#define Parse_VCLEFT_2RowSeatCenterSwitch(d) ((d & 0x8000000) >> 27)
#define Parse_VCLEFT_btnWindowSwPackAutoUpLR(d) ((d & 0x8000000) >> 27)
#define Parse_VCLEFT_2RowSeatLeftFoldFlatSwitc(d) ((d & 0x4000000) >> 26)
#define Parse_VCLEFT_btnWindowSwPackDownLR(d) ((d & 0x4000000) >> 26)
#define Parse_VCLEFT_2RowSeatRightFoldFlatSwit(d) ((d & 0x2000000) >> 25)
#define Parse_VCLEFT_btnWindowSwPackAutoDownLR(d) ((d & 0x2000000) >> 25)
#define Parse_VCLEFT_2RowSeatBothFoldFlatSwitc(d) ((d & 0x1000000) >> 24)
#define Parse_VCLEFT_btnWindowSwPackUpRF(d) ((d & 0x1000000) >> 24)
#define Parse_VCLEFT_btnWindowSwPackAutoUpRF(d) ((d & 0x800000) >> 23)
#define Parse_VCLEFT_swcLeftDoublePress(d) ((d & 0x800000) >> 23)
#define Parse_VCLEFT_btnWindowSwPackDownRF(d) ((d & 0x400000) >> 22)
#define Parse_VCLEFT_swcRightDoublePress(d) ((d & 0x400000) >> 22)
#define Parse_VCLEFT_btnWindowSwPackAutoDownRF(d) ((d & 0x200000) >> 21)
#define Parse_VCLEFT_btnWindowSwPackUpRR(d) ((d & 0x100000) >> 20)
#define Parse_VCLEFT_btnWindowSwPackAutoUpRR(d) ((d & 0x80000) >> 19)
#define Parse_VCLEFT_btnWindowSwPackDownRR(d) ((d & 0x40000) >> 18)
#define Parse_VCLEFT_btnWindowSwPackAutoDownRR(d) ((d & 0x20000) >> 17)
#define Parse_VCLEFT_frontBuckleSwitch(d) ((d & 0x30000) >> 16)

enum VCLEFT_frontBuckleSwitch_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_frontOccupancySwitch(d) ((d & 0xc000) >> 14)

enum VCLEFT_frontOccupancySwitch_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_rearLeftBuckleSwitch(d) ((d & 0x3000) >> 12)

enum VCLEFT_rearLeftBuckleSwitch_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_rearCenterOccupancySwitch(d) ((d & 0xc00) >> 10)

enum VCLEFT_rearCenterOccupancySwitch_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_rearLeftOccupancySwitch(d) ((d & 0x300) >> 8)

enum VCLEFT_rearLeftOccupancySwitch_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_rearRightOccupancySwitch(d) ((d & 0xc0) >> 6)

enum VCLEFT_rearRightOccupancySwitch_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define Parse_VCLEFT_brakePressed(d) ((d & 0x10) >> 4)
#define Parse_VCLEFT_rearHVACButtonPressed(d) ((d & 0x8) >> 3)
#define Parse_VCLEFT_rearCenterBuckleSwitch(d) ((d & 0xc) >> 2)

enum VCLEFT_rearCenterBuckleSwitch_Values { 
  SWITCH_SNA = 0,
  SWITCH_OFF = 1,
  SWITCH_ON = 2,
  SWITCH_FAULT = 3
 };

#define ID336MaxPowerRating 0x336

#define Parse_DrivePowerRating336(d) ((d & 0xffc00) >> 10)
#define Parse_DriveRegenRating336(d) ((d & 0xff0000) >> 16)
#define Parse_DI_performancePackage(d) ((d & 0x700) >> 8)

enum DI_performancePackage_Values { 
  BASE = 0,
  PERFORMANCE = 1,
  BASE_PLUS = 3,
  SNA = 4
 };

#define ID293UI_chassisControl 0x293

#define Parse_UI_steeringTuneRequest(d) ((d & 0xc) >> 2)

enum UI_steeringTuneRequest_Values { 
  STEERING_TUNE_COMFORT = 0,
  STEERING_TUNE_STANDARD = 1,
  STEERING_TUNE_SPORT = 2
 };

#define Parse_UI_tractionControlMode(d) ((d & 0xe) >> 1)

enum UI_tractionControlMode_Values { 
  TC_NORMAL_SELECTED = 0,
  TC_SLIP_START_SELECTED = 1,
  TC_DEV_MODE_1_SELECTED = 2,
  TC_DEV_MODE_2_SELECTED = 3,
  TC_ROLLS_MODE_SELECTED = 4,
  TC_DYNO_MODE_SELECTED = 5,
  TC_OFFROAD_ASSIST_SELECTED = 6
 };

#define Parse_UI_parkBrakeRequest(d) ((d & 0x1800000000000000) >> 59)

enum UI_parkBrakeRequest_Values { 
  PARK_BRAKE_REQUEST_IDLE = 0,
  PARK_BRAKE_REQUEST_PRESSED = 1,
  PARK_BRAKE_REQUEST_SNA = 3
 };

#define Parse_UI_narrowGarages(d) ((d & 0x200000000000000) >> 57)
#define Parse_UI_winchModeRequest(d) ((d & 0x300000000000000) >> 56)

enum UI_winchModeRequest_Values { 
  WINCH_MODE_IDLE = 0,
  WINCH_MODE_ENTER = 1,
  WINCH_MODE_EXIT = 2
 };

#define Parse_UI_zeroSpeedConfirmed(d) ((d & 0xc0000000000000) >> 54)

enum UI_zeroSpeedConfirmed_Values { 
  ZERO_SPEED_CANCELED = 0,
  ZERO_SPEED_CONFIRMED = 1,
  ZERO_SPEED_UNUSED = 2,
  ZERO_SPEED_SNA = 3
 };

#define Parse_UI_trailerMode(d) ((d & 0x10000000000000) >> 52)

enum UI_trailerMode_Values { 
  TRAILER_MODE_OFF = 0,
  TRAILER_MODE_ON = 1
 };

#define Parse_UI_distanceUnits(d) ((d & 0x8000000000000) >> 51)

enum UI_distanceUnits_Values { 
  DISTANCEUNITS_KM = 0,
  DISTANCEUNITS_MILES = 1
 };

#define Parse_UI_dasDebugEnable(d) ((d & 0x4000000000000) >> 50)
#define Parse_UI_accOvertakeEnable(d) ((d & 0x3000000000000) >> 48)

enum UI_accOvertakeEnable_Values { 
  ACC_OVERTAKE_OFF = 0,
  ACC_OVERTAKE_ON = 1,
  SNA = 3
 };

#define Parse_UI_aebEnable(d) ((d & 0xc00000000000) >> 46)

enum UI_aebEnable_Values { 
  AEB_OFF = 0,
  AEB_ON = 1,
  SNA = 3
 };

#define Parse_UI_aesEnable(d) ((d & 0x300000000000) >> 44)

enum UI_aesEnable_Values { 
  AES_OFF = 0,
  AES_ON = 1,
  SNA = 3
 };

#define Parse_UI_ahlbEnable(d) ((d & 0xc0000000000) >> 42)

enum UI_ahlbEnable_Values { 
  AHLB_OFF = 0,
  AHLB_ON = 1,
  SNA = 3
 };

#define Parse_UI_autoLaneChangeEnable(d) ((d & 0x30000000000) >> 40)

enum UI_autoLaneChangeEnable_Values { 
  OFF = 0,
  ON = 1,
  SNA = 3
 };

#define Parse_UI_rebootAutopilot(d) ((d & 0x2000000000) >> 37)
#define Parse_UI_autoParkRequest(d) ((d & 0xf000000000) >> 36)

enum UI_autoParkRequest_Values { 
  NONE = 0,
  PARK_LEFT_PARALLEL = 1,
  PARK_LEFT_CROSS = 2,
  PARK_RIGHT_PARALLEL = 3,
  PARK_RIGHT_CROSS = 4,
  PARALLEL_PULL_OUT_TO_LEFT = 5,
  PARALLEL_PULL_OUT_TO_RIGHT = 6,
  ABORT = 7,
  COMPLETE = 8,
  SEARCH = 9,
  PAUSE = 10,
  RESUME = 11,
  SNA = 15
 };

#define Parse_UI_bsdEnable(d) ((d & 0x300000000) >> 32)

enum UI_bsdEnable_Values { 
  BSD_OFF = 0,
  BSD_ON = 1,
  SNA = 3
 };

#define Parse_UI_fcwEnable(d) ((d & 0xc0000000) >> 30)

enum UI_fcwEnable_Values { 
  FCW_OFF = 0,
  FCW_ON = 1,
  SNA = 3
 };

#define Parse_UI_fcwSensitivity(d) ((d & 0x30000000) >> 28)

enum UI_fcwSensitivity_Values { 
  AEB_SENSITIVITY_EARLY = 0,
  AEB_SENSITIVITY_AVERAGE = 1,
  AEB_SENSITIVITY_LATE = 2,
  SNA = 3
 };

#define Parse_UI_latControlEnable(d) ((d & 0xc000000) >> 26)

enum UI_latControlEnable_Values { 
  LATERAL_CONTROL_OFF = 0,
  LATERAL_CONTROL_ON = 1,
  LATERAL_CONTROL_UNAVAILABLE = 2,
  LATERAL_CONTROL_SNA = 3
 };

#define Parse_UI_ldwEnable(d) ((d & 0x3000000) >> 24)

enum UI_ldwEnable_Values { 
  NO_HAPTIC = 0,
  LDW_TRIGGERS_HAPTIC = 1,
  SNA = 3
 };

#define Parse_UI_pedalSafetyEnable(d) ((d & 0xc00000) >> 22)

enum UI_pedalSafetyEnable_Values { 
  PEDAL_SAFETY_OFF = 0,
  PEDAL_SAFETY_ON = 1,
  SNA = 3
 };

#define Parse_UI_redLightStopSignEnable(d) ((d & 0xc0000) >> 18)

enum UI_redLightStopSignEnable_Values { 
  RLSSW_OFF = 0,
  RLSSW_ON = 1,
  SNA = 3
 };

#define Parse_UI_selfParkTune(d) ((d & 0xf0000) >> 16)

enum UI_selfParkTune_Values { 
  SNA = 15
 };

#define Parse_UI_chassisControlCounter(d) ((d & 0xf000) >> 12)
#define Parse_UI_chassisControlChecksum(d) ((d & 0xff00) >> 8)
#define ID268SystemPower 0x268

#define Parse_SystemHeatPowerMax268(d) ((d & 0xff00) >> 8)
#define Parse_SystemHeatPower268(d) ((d & 0xff00000000) >> 32)
#define Parse_SystemDrivePowerMax268(d) ((d & 0x1ff000000) >> 24)
#define Parse_DI_primaryUnitSiliconType(d) ((d & 0x4000) >> 14)

enum DI_primaryUnitSiliconType_Values { 
  MOSFET = 0,
  IGBT = 1
 };

#define Parse_SystemRegenPowerMax268(d) ((d & 0xff00) >> 8)
#define ID04FGPSLatLong 0x4f

#define Parse_GPSLatitude04F(d) ((d & 0xfffffff0000000) >> 28)
#define Parse_GPSLongitude04F(d) ((d & 0xfffffff000000000) >> 36)
#define Parse_GPSAccuracy04F(d) ((d & 0x3f80) >> 7)
#define ID3D2TotalChargeDischarge 0x3d2

#define Parse_TotalDischargeKWh3D2(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_TotalChargeKWh3D2(d) ((d & 0xffffffff00000000) >> 32)
#define ID3F2BMSCounters 0x3f2

#define Parse_BMS_kwhCounter_Id(d) ((d & 0xf0) >> 4)
#define Parse_BMS_kwhAcChargeTotalModule3(d) ((d & 0xfffffff00000) >> 20)
#define Parse_BMS_kwhDischargeTotalModule2(d) ((d & 0xfffffff00000) >> 20)
#define Parse_BMS_kwhAcChargeTotalModule2(d) ((d & 0xfffffff00000) >> 20)
#define Parse_BMS_kwhDischargeTotalModule4(d) ((d & 0xfffffff00000) >> 20)
#define Parse_BMS_kwhAcChargeTotalModule4(d) ((d & 0xfffffff00000) >> 20)
#define Parse_BMS_kwhAcChargeTotalModule1(d) ((d & 0xfffffff00000) >> 20)
#define Parse_BMS_kwhDriveDischargeTotal(d) ((d & 0xffffffff000000) >> 24)
#define Parse_BMS_dcChargerKwhTotal(d) ((d & 0xffffffff000000) >> 24)
#define Parse_BMS_kwhDischargeTotalModule1(d) ((d & 0xfffffff00000) >> 20)
#define Parse_BMS_acChargerKwhTotal(d) ((d & 0xffffffff000000) >> 24)
#define Parse_BMS_kwhRegenChargeTotal(d) ((d & 0xffffffff000000) >> 24)
#define Parse_BMS_kwhDischargeTotalModule3(d) ((d & 0xfffffff00000) >> 20)
#define Parse_BMS_kwhDcChargeTotalModule3(d) ((d & 0xfffffff0000000) >> 28)
#define Parse_BMS_kwhChargeTotalModule2(d) ((d & 0xfffffff0000000) >> 28)
#define Parse_BMS_kwhDcChargeTotalModule2(d) ((d & 0xfffffff0000000) >> 28)
#define Parse_BMS_kwhChargeTotalModule4(d) ((d & 0xfffffff0000000) >> 28)
#define Parse_BMS_kwhDcChargeTotalModule4(d) ((d & 0xfffffff0000000) >> 28)
#define Parse_BMS_kwhDcChargeTotalModule1(d) ((d & 0xfffffff0000000) >> 28)
#define Parse_BMS_kwhChargeTotalModule1(d) ((d & 0xfffffff0000000) >> 28)
#define Parse_BMS_kwhChargeTotalModule3(d) ((d & 0xfffffff0000000) >> 28)
#define ID2D2BMSVAlimits 0x2d2

#define Parse_MinVoltage2D2(d) ((d & 0xffff0000) >> 16)
#define Parse_MaxVoltage2D2(d) ((d & 0xffff000000000000) >> 48)
#define Parse_MaxChargeCurrent2D2(d) ((d & 0x3fff00000000) >> 32)
#define Parse_MaxDischargeCurrent2D2(d) ((d & 0x3fff0000) >> 16)
#define ID541FastChargeMaxLimits 0x541

#define Parse_FCMaxPowerLimit541(d) ((d & 0x3ffe000) >> 13)
#define Parse_FCMaxCurrentLimit541(d) ((d & 0x1fff000000000000) >> 48)
#define ID244FastChargeLimits 0x244

#define Parse_FCPowerLimit244(d) ((d & 0x3ffe000) >> 13)
#define Parse_FCCurrentLimit244(d) ((d & 0x1fff000000000000) >> 48)
#define Parse_FCMaxVlimit244(d) ((d & 0x1fff00000000) >> 32)
#define Parse_FCMinVlimit244(d) ((d & 0x1fff0000) >> 16)
#define ID214FastChargeVA 0x214

#define Parse_FC_protocolVersion(d) ((d & 0xff00) >> 8)
#define Parse_FC_statusCode(d) ((d & 0xf00000000000000) >> 56)

enum FC_statusCode_Values { 
  FC_STATUS_NOTREADY_SNA = 0,
  FC_STATUS_READY = 1,
  FC_STATUS_UPDATE_IN_PROGRESS = 2,
  FC_STATUS_DEPRECATED_3 = 3,
  FC_STATUS_DEPRECATED_4 = 4,
  FC_STATUS_INT_ISOACTIVE = 5,
  FC_STATUS_EXT_ISOACTIVE = 6,
  FC_STATUS_POST_OUT_OF_SERVICE = 7,
  FC_STATUS_NOTCOMPATIBLE = 13,
  FC_STATUS_MALFUNCTION = 14,
  FC_STATUS_NODATA = 15
 };

#define Parse_FC_adapterLocked(d) ((d & 0x2000000000000) >> 49)
#define Parse_FC_minCurrentLimit(d) ((d & 0x1fff000000000000) >> 48)
#define Parse_FC_type(d) ((d & 0x3800000000) >> 35)

enum FC_type_Values { 
  FC_TYPE_SUPERCHARGER = 0,
  FC_TYPE_CHADEMO = 1,
  FC_TYPE_GB = 2,
  FC_TYPE_CC_EVSE = 3,
  FC_TYPE_COMBO = 4,
  FC_TYPE_MC_EVSE = 5,
  FC_TYPE_OTHER = 6,
  FC_TYPE_SNA = 7
 };

#define Parse_FC_dcCurrent(d) ((d & 0x3fff00000000) >> 32)
#define Parse_FC_postID(d) ((d & 0xc0000) >> 18)

enum FC_postID_Values { 
  FC_POST_MASTER = 0,
  FC_POST_SLAVE = 1,
  FC_POST_ID_2 = 2,
  FC_POST_ID_SNA = 3
 };

#define Parse_FC_dcVoltage(d) ((d & 0x1fff0000) >> 16)
#define Parse_FC_leakageTestNotSupported(d) ((d & 0x8) >> 3)
#define ID215FCisolation 0x215

#define Parse_FCIsolation215(d) ((d & 0xff00) >> 8)
#define ID217FC_status3 0x217

#define Parse_FC_status3DataSelect(d) ((d & 0x3f80) >> 7)

enum FC_status3DataSelect_Values { 
  Mux0 = 0,
  Mux1 = 1
 };

#define Parse_FC_status3DummySig(d) ((d & 0x200000000000000) >> 57)
#define Parse_FC_billingEnergy(d) ((d & 0xffff00) >> 8)
#define Parse_FC_class(d) ((d & 0xff00000000000000) >> 56)

enum FC_class_Values { 
  FC_CLASS_SNA = 0,
  FC_CLASS_SUPERCHARGER = 1,
  FC_CLASS_URBANCHARGER = 2
 };

#define Parse_FC_brand(d) ((d & 0xf000000000000) >> 48)

enum FC_brand_Values { 
  FC_BRAND_SNA = 0,
  FC_BRAND_TESLA = 1
 };

#define Parse_FC_coolingType(d) ((d & 0xf00000000000) >> 44)

enum FC_coolingType_Values { 
  FC_COOLING_TYPE_SNA = 0,
  FC_COOLING_TYPE_LIQUID = 1,
  FC_COOLING_TYPE_CONVECTION = 2
 };

#define Parse_FC_uiStopType(d) ((d & 0xf0000000000) >> 40)

enum FC_uiStopType_Values { 
  FC_UI_STOP_TYPE_SNA = 0,
  FC_UI_STOP_TYPE_TOGGLE = 1,
  FC_UI_STOP_TYPE_MOMENTARY = 2
 };

#define Parse_FC_generation(d) ((d & 0xff00000000) >> 32)

enum FC_generation_Values { 
  GENERATION_SNA = 0
 };

#define ID321VCFRONT_sensors 0x321

#define Parse_VCFRONT_tempCoolantBatInlet(d) ((d & 0xffc00) >> 10)

enum VCFRONT_tempCoolantBatInlet_Values { 
  SNA = 1023
 };

#define Parse_VCFRONT_tempCoolantPTInlet(d) ((d & 0xffe) >> 1)

enum VCFRONT_tempCoolantPTInlet_Values { 
  SNA = 2047
 };

#define Parse_VCFRONT_coolantLevel(d) ((d & 0x80000000000) >> 43)

enum VCFRONT_coolantLevel_Values { 
  NOT_OK = 0,
  FILLED = 1
 };

#define Parse_VCFRONT_brakeFluidLevel(d) ((d & 0xc0000000000) >> 42)

enum VCFRONT_brakeFluidLevel_Values { 
  SNA = 0,
  LOW = 1,
  NORMAL = 2
 };

#define Parse_VCFRONT_tempAmbient(d) ((d & 0xff0000000000) >> 40)

enum VCFRONT_tempAmbient_Values { 
  SNA = 0
 };

#define Parse_VCFRONT_washerFluidLevel(d) ((d & 0x300000000) >> 32)

enum VCFRONT_washerFluidLevel_Values { 
  SNA = 0,
  LOW = 1,
  NORMAL = 2
 };

#define Parse_VCFRONT_tempAmbientFiltered(d) ((d & 0xff000000) >> 24)

enum VCFRONT_tempAmbientFiltered_Values { 
  SNA = 0
 };

#define Parse_VCFRONT_battSensorIrrational(d) ((d & 0x10000) >> 16)
#define Parse_VCFRONT_ptSensorIrrational(d) ((d & 0x8000) >> 15)
#define ID301VCFRONT_info 0x301

#define Parse_VCFRONT_infoIndex(d) ((d & 0xff00) >> 8)

enum VCFRONT_infoIndex_Values { 
  BC_INFO_DEPRECATED_0 = 0,
  BC_INFO_DEPRECATED_1 = 1,
  BC_INFO_DEPRECATED_2 = 2,
  BC_INFO_DEPRECATED_3 = 3,
  BC_INFO_DEPRECATED_4 = 4,
  BC_INFO_DEPRECATED_5 = 5,
  BC_INFO_DEPRECATED_6 = 6,
  BC_INFO_DEPRECATED_7 = 7,
  BC_INFO_DEPRECATED_8 = 8,
  BC_INFO_DEPRECATED_9 = 9,
  BC_INFO_BUILD_HWID_COMPONENTID = 10,
  BC_INFO_PCBAID_ASSYID_USAGEID = 11,
  BC_INFO_APP_CRC = 13,
  BC_INFO_BOOTLOADER_SVN = 14,
  BC_INFO_BOOTLOADER_CRC = 15,
  BC_INFO_SUBCOMPONENT1 = 16,
  BC_INFO_APP_GITHASH = 17,
  BC_INFO_BOOTLOADER_GITHASH = 18,
  BC_INFO_VERSION_DEPRECATED = 19,
  BC_INFO_UDS_PROTOCOL_BOOTCRC = 20,
  BC_INFO_SUBCOMPONENT2 = 23,
  BC_INFO_SUBCOMPONENT3 = 24,
  BC_INFO_SUBCOMPONENT4 = 31,
  BC_INFO_SUBCOMPONENT5 = 32,
  BC_INFO_SUBCOMPONENT6 = 33,
  BC_INFO_END = 255
 };

#define Parse_VCFRONT_infoAppCrc(d) ((d & 0xffffffff000000) >> 24)
#define Parse_VCFRONT_infoAppGitHash(d) ((d & 0xffffffffffffff000000000000) >> 48)
#define Parse_VCFRONT_infoBootGitHash(d) ((d & 0xffffffffffffff000000000000) >> 48)
#define Parse_VCFRONT_infoBootUdsProtoVersion(d) ((d & 0xffff00) >> 8)
#define Parse_VCFRONT_infoBuildType(d) ((d & 0x700000000000000) >> 56)

enum VCFRONT_infoBuildType_Values { 
  INFO_UNKNOWN_BUILD = 0,
  INFO_PLATFORM_BUILD = 1,
  INFO_LOCAL_BUILD = 2,
  INFO_TRACEABLE_CI_BUILD = 3,
  INFO_MFG_BUILD = 4
 };

#define Parse_VCFRONT_infoPcbaId(d) ((d & 0xff00000000000000) >> 56)
#define Parse_VCFRONT_infoPlatformType(d) ((d & 0xff00000000000000) >> 56)
#define Parse_VCFRONT_infoAssemblyId(d) ((d & 0xff000000000000) >> 48)

enum VCFRONT_infoAssemblyId_Values { 
  ASSEMBLY1 = 1,
  ASSEMBLY_SNA = 255
 };

#define Parse_VCFRONT_infoBuildConfigId(d) ((d & 0xffff000000000000) >> 48)
#define Parse_VCFRONT_infoBootCrc(d) ((d & 0xffffffff00) >> 8)
#define Parse_VCFRONT_infoUsageId(d) ((d & 0xffff0000000000) >> 40)
#define Parse_VCFRONT_hcmlAppCRC(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_VCFRONT_hcmrAppCRC(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_VCFRONT_infoHardwareId(d) ((d & 0xffff00000000) >> 32)
#define Parse_VCFRONT_infoSubcomponent3(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_VCFRONT_infoSubcomponent4(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_VCFRONT_infoSubcomponent5(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_VCFRONT_infoSubcomponent6(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_VCFRONT_infoSubUsageId(d) ((d & 0xffff000000) >> 24)
#define Parse_VCFRONT_infoComponentId(d) ((d & 0xffff0000) >> 16)
#define ID201VCFRONT_loggingAndVitals10H 0x201

#define Parse_VCFRONT_loggingAndVitals10HzInde(d) ((d & 0x38) >> 3)

enum VCFRONT_loggingAndVitals10HzInde_Values { 
  TARGETS_AND_ACTUALS_0 = 0,
  TARGETS_SENSORS_AND_ACTUALS_1 = 1,
  STATES_AND_SENSORS = 2,
  EXV_FLOW = 3,
  HP_STATE = 4,
  EXV_FLOW_TARGET = 5,
  EXV_STATE = 6,
  END = 7
 };

#define Parse_VCFRONT_chillerExvFlow(d) ((d & 0x1ff80) >> 7)
#define Parse_VCFRONT_chillerExvFlowTarget(d) ((d & 0x1ff80) >> 7)
#define Parse_VCFRONT_chillerExvState(d) ((d & 0x1e) >> 1)

enum VCFRONT_chillerExvState_Values { 
  UNINIT = 0,
  INIT_OPEN = 1,
  INIT_CLOSE = 2,
  READY = 3,
  FAULTED = 4,
  WAIT = 5,
  OVERDRIVING_SHUT = 6,
  READY_SHUT = 7,
  CALIB_CLOSE = 8,
  CALIB_CLOSE_OVERDRIVE = 9
 };

#define Parse_VCFRONT_hpHighSideHX(d) ((d & 0x6000000000000000) >> 61)

enum VCFRONT_hpHighSideHX_Values { 
  NONE = 0,
  LCC = 1,
  CC = 2,
  BOTH = 3
 };

#define Parse_VCFRONT_pumpBatteryRPMActual(d) ((d & 0x1ff80) >> 7)

enum VCFRONT_pumpBatteryRPMActual_Values { 
  SNA = 1023
 };

#define Parse_VCFRONT_tempRefrigDischarge(d) ((d & 0x7ff00) >> 8)

enum VCFRONT_tempRefrigDischarge_Values { 
  SNA = 2047
 };

#define Parse_VCFRONT_hpLowSideHX(d) ((d & 0x1800000000000000) >> 59)

enum VCFRONT_hpLowSideHX_Values { 
  NONE = 0,
  CHILLER = 1,
  EVAP = 2,
  BOTH = 3
 };

#define Parse_VCFRONT_activeLouverOpenPosTarg(d) ((d & 0x7f00000000000000) >> 56)
#define Parse_VCFRONT_evapExvState(d) ((d & 0xf00000000000000) >> 56)

enum VCFRONT_evapExvState_Values { 
  UNINIT = 0,
  INIT_OPEN = 1,
  INIT_CLOSE = 2,
  READY = 3,
  FAULTED = 4,
  WAIT = 5,
  OVERDRIVING_SHUT = 6,
  READY_SHUT = 7,
  CALIB_CLOSE = 8,
  CALIB_CLOSE_OVERDRIVE = 9
 };

#define Parse_VCFRONT_hpDominantLoad(d) ((d & 0x700000000000000) >> 56)

enum VCFRONT_hpDominantLoad_Values { 
  NONE = 0,
  EVAP = 1,
  CHILLER = 2,
  LOW_BOTH = 3,
  LCC = 4,
  CC = 5,
  HIGH_BOTH = 6
 };

#define Parse_VCFRONT_hpBlendType(d) ((d & 0x60000000000000) >> 53)

enum VCFRONT_hpBlendType_Values { 
  HP_NONE = 0,
  HP_PARTIAL = 1,
  HP_FULL = 2
 };

#define Parse_VCFRONT_recircExvState(d) ((d & 0xf0000000000000) >> 52)

enum VCFRONT_recircExvState_Values { 
  UNINIT = 0,
  INIT_OPEN = 1,
  INIT_CLOSE = 2,
  READY = 3,
  FAULTED = 4,
  WAIT = 5,
  OVERDRIVING_SHUT = 6,
  READY_SHUT = 7,
  CALIB_CLOSE = 8,
  CALIB_CLOSE_OVERDRIVE = 9
 };

#define Parse_VCFRONT_evapExvFlow(d) ((d & 0x1ff8000000000000) >> 51)
#define Parse_VCFRONT_evapExvFlowTarget(d) ((d & 0x1ff8000000000000) >> 51)
#define Parse_VCFRONT_hpQuietModeEnabled(d) ((d & 0x8000000000000) >> 51)
#define Parse_VCFRONT_pumpPowertrainRPMActual(d) ((d & 0x1ff8000000000000) >> 51)

enum VCFRONT_pumpPowertrainRPMActual_Values { 
  SNA = 1023
 };

#define Parse_VCFRONT_hpCabinLoadType(d) ((d & 0xc000000000000) >> 50)

enum VCFRONT_hpCabinLoadType_Values { 
  NONE = 0,
  CC = 1,
  REHEAT = 2,
  EVAP = 3
 };

#define Parse_VCFRONT_activeLouverOpenPos(d) ((d & 0x7f000000000000) >> 48)
#define Parse_VCFRONT_fanDemand(d) ((d & 0x7f000000000000) >> 48)
#define Parse_VCFRONT_hpBatteryLoadType(d) ((d & 0x3000000000000) >> 48)

enum VCFRONT_hpBatteryLoadType_Values { 
  NONE = 0,
  BATT_HEAT = 1,
  BATT_COOL = 2
 };

#define Parse_VCFRONT_lccExvState(d) ((d & 0xf000000000000) >> 48)

enum VCFRONT_lccExvState_Values { 
  UNINIT = 0,
  INIT_OPEN = 1,
  INIT_CLOSE = 2,
  READY = 3,
  FAULTED = 4,
  WAIT = 5,
  OVERDRIVING_SHUT = 6,
  READY_SHUT = 7,
  CALIB_CLOSE = 8,
  CALIB_CLOSE_OVERDRIVE = 9
 };

#define Parse_VCFRONT_hpReqCoolantMode(d) ((d & 0x1c00000000000) >> 46)

enum VCFRONT_hpReqCoolantMode_Values { 
  ANY = 0,
  SERIES_NO_BYPASS = 1,
  SERIES_BYPASS = 2,
  PARALLEL = 3,
  AMBIENT_SOURCE = 4
 };

#define Parse_VCFRONT_cclExvState(d) ((d & 0xf00000000000) >> 44)

enum VCFRONT_cclExvState_Values { 
  UNINIT = 0,
  INIT_OPEN = 1,
  INIT_CLOSE = 2,
  READY = 3,
  FAULTED = 4,
  WAIT = 5,
  OVERDRIVING_SHUT = 6,
  READY_SHUT = 7,
  CALIB_CLOSE = 8,
  CALIB_CLOSE_OVERDRIVE = 9
 };

#define Parse_VCFRONT_ccrExvState(d) ((d & 0xf0000000000) >> 40)

enum VCFRONT_ccrExvState_Values { 
  UNINIT = 0,
  INIT_OPEN = 1,
  INIT_CLOSE = 2,
  READY = 3,
  FAULTED = 4,
  WAIT = 5,
  OVERDRIVING_SHUT = 6,
  READY_SHUT = 7,
  CALIB_CLOSE = 8,
  CALIB_CLOSE_OVERDRIVE = 9
 };

#define Parse_VCFRONT_compressorState(d) ((d & 0x30000000000) >> 40)

enum VCFRONT_compressorState_Values { 
  STANDBY = 0,
  READY = 1,
  RUNNING = 2,
  FAULT = 3
 };

#define Parse_VCFRONT_radiatorFanRPMActual(d) ((d & 0x3ff0000000000) >> 40)

enum VCFRONT_radiatorFanRPMActual_Values { 
  SNA = 1023
 };

#define Parse_VCFRONT_recircExvFlow(d) ((d & 0x3ff0000000000) >> 40)
#define Parse_VCFRONT_recircExvFlowTarget(d) ((d & 0x3ff0000000000) >> 40)
#define Parse_VCFRONT_tempRefrigLiquid(d) ((d & 0x7ff0000000000) >> 40)

enum VCFRONT_tempRefrigLiquid_Values { 
  SNA = 2047
 };

#define Parse_VCFRONT_compDemandEvap(d) ((d & 0x1fc000000000) >> 38)
#define Parse_VCFRONT_exvState(d) ((d & 0x780000000) >> 31)

enum VCFRONT_exvState_Values { 
  UNINIT = 0,
  INIT_OPEN = 1,
  INIT_CLOSE = 2,
  READY = 3,
  FAULTED = 4,
  WAIT = 5,
  OVERDRIVING_SHUT = 6,
  READY_SHUT = 7,
  CALIB_CLOSE = 8,
  CALIB_CLOSE_OVERDRIVE = 9
 };

#define Parse_VCFRONT_lccExvFlow(d) ((d & 0xffc0000000) >> 30)
#define Parse_VCFRONT_lccExvFlowTarget(d) ((d & 0xffc0000000) >> 30)
#define Parse_VCFRONT_tempSuperheatActual(d) ((d & 0xffc0000000) >> 30)

enum VCFRONT_tempSuperheatActual_Values { 
  SNA = 1023
 };

#define Parse_VCFRONT_solenoidEvapState(d) ((d & 0x18000000) >> 27)

enum VCFRONT_solenoidEvapState_Values { 
  UNDEFINED = 0,
  OPENED = 1,
  CLOSED = 2,
  FAULTED = 3
 };

#define Parse_VCFRONT_compDemandChiller(d) ((d & 0xfe000000) >> 25)
#define Parse_VCFRONT_pressureRefrigLiquid(d) ((d & 0xff000000) >> 24)

enum VCFRONT_pressureRefrigLiquid_Values { 
  SNA = 255
 };

#define Parse_VCFRONT_cclExvFlow(d) ((d & 0x3ff00000) >> 20)
#define Parse_VCFRONT_cclExvFlowTarget(d) ((d & 0x3ff00000) >> 20)
#define Parse_VCFRONT_tempSuperheatTarget(d) ((d & 0x3ff00000) >> 20)
#define Parse_VCFRONT_pressureRefrigSuctionVit(d) ((d & 0x1fc0000) >> 18)

enum VCFRONT_pressureRefrigSuctionVit_Values { 
  SNA = 127
 };

#define Parse_VCFRONT_tempSuperheatActFiltered(d) ((d & 0xff0000) >> 16)
#define Parse_VCFRONT_pressureRefrigDischargeV(d) ((d & 0x7f800) >> 11)

enum VCFRONT_pressureRefrigDischargeV_Values { 
  SNA = 255
 };

#define Parse_VCFRONT_ccrExvFlow(d) ((d & 0xffc00) >> 10)
#define Parse_VCFRONT_ccrExvFlowTarget(d) ((d & 0xffc00) >> 10)
#define Parse_VCFRONT_exvFlowTarget(d) ((d & 0xffc00) >> 10)
#define Parse_VCFRONT_coolantValveMode(d) ((d & 0x18) >> 3)

enum VCFRONT_coolantValveMode_Values { 
  SERIES = 0,
  PARALLEL = 1,
  BLEND = 2,
  AMBIENT_SOURCE = 3
 };

#define ID3D8Elevation 0x3d8

#define Parse_Elevation3D8(d) ((d & 0xfffc000) >> 14)
#define ID261_12vBattStatus 0x261

#define Parse_VCFRONT_12VBatteryStatusIndex(d) ((d & 0xc) >> 2)
#define Parse_v12vBattAH261(d) ((d & 0x3fff000) >> 12)
#define Parse_VCFRONT_voltageProfile(d) ((d & 0xc000000000000000) >> 62)

enum VCFRONT_voltageProfile_Values { 
  CHARGE = 0,
  FLOAT = 1,
  REDUCED_FLOAT = 2,
  ALWAYS_CLOSED_CONTACTORS = 3
 };

#define Parse_VCFRONT_IBSFault(d) ((d & 0x4000000000000000) >> 62)
#define Parse_VCFRONT_batterySupportRequest(d) ((d & 0x2000000000000000) >> 61)
#define Parse_VCFRONT_batterySMState(d) ((d & 0xf000000000000000) >> 60)

enum VCFRONT_batterySMState_Values { 
  INIT = 0,
  CHARGE = 1,
  DISCHARGE = 2,
  STANDBY = 3,
  RESISTANCE_ESTIMATION = 4,
  OTA_STANDBY = 5,
  DISCONNECTED_BATTERY_TEST = 6,
  SHORTED_CELL_TEST = 7,
  FAULT = 8,
  RECOVERY = 9
 };

#define Parse_VCFRONT_12VOverchargeCounter(d) ((d & 0xf000000000000000) >> 60)
#define Parse_VCFRONT_targetCurrent(d) ((d & 0xff00000000000000) >> 56)
#define Parse_VCFRONT_voltageDropCounter(d) ((d & 0xf00000000000000) >> 56)
#define Parse_VCFRONT_isVehicleSupported(d) ((d & 0x10000000000000) >> 52)
#define Parse_VCFRONT_shortedCellFaultCounter(d) ((d & 0x38000000000000) >> 51)
#define Parse_v12vBattCurrent261(d) ((d & 0xffff000000000000) >> 48)
#define Parse_VCFRONT_IBSCurrent(d) ((d & 0xffff000000000000) >> 48)
#define Parse_v12vBattTemp261(d) ((d & 0xffff000000000000) >> 48)
#define Parse_VCFRONT_silentWakeIBSCurrent(d) ((d & 0xfff00000000) >> 32)
#define Parse_v12vBattVoltage261(d) ((d & 0xfff00000000) >> 32)
#define Parse_VCFRONT_12VBatteryTargetVoltage(d) ((d & 0x3ff00000000) >> 32)
#define Parse_VCFRONT_firstChargeOTA(d) ((d & 0x100000) >> 20)
#define Parse_VCFRONT_PCSMia(d) ((d & 0x100000) >> 20)
#define Parse_VCFRONT_firstChargePOR(d) ((d & 0x80000) >> 19)
#define Parse_VCFRONT_chargeNeeded(d) ((d & 0x80000) >> 19)
#define Parse_VCFRONT_good12VforUpdate(d) ((d & 0x40000) >> 18)
#define Parse_VCFRONT_LVLoadRequest(d) ((d & 0x20000) >> 17)
#define Parse_VCFRONT_voltageFloorReachedCount(d) ((d & 0xf0000) >> 16)
#define Parse_VCFRONT_reverseBatteryFault(d) ((d & 0x10000) >> 16)
#define Parse_VCFRONT_LVBatterySupported(d) ((d & 0x8000) >> 15)
#define Parse_VCFRONT_LVBatteryDisconnected(d) ((d & 0x4000) >> 14)
#define Parse_VCFRONT_12VBatteryStatusCounter(d) ((d & 0xf000) >> 12)
#define Parse_VCFRONT_12VBatteryStatusChecksum(d) ((d & 0xff00) >> 8)
#define ID129SteeringAngle 0x129

#define Parse_SteeringAngle129(d) ((d & 0x3fff000000000000) >> 48)
#define Parse_SteeringSpeed129(d) ((d & 0x3fff00000000) >> 32)
#define Parse_SteeringSensorA129(d) ((d & 0xc0000) >> 18)
#define Parse_SteeringSensorB129(d) ((d & 0xff0000) >> 16)
#define Parse_SteeringSensorC129(d) ((d & 0xff00) >> 8)
#define ID264ChargeLineStatus 0x264

#define Parse_ChargeLineVoltage264(d) ((d & 0xfffc000) >> 14)
#define Parse_ChargeLineCurrent264(d) ((d & 0x7fc000000000000) >> 50)
#define Parse_ChargeLinePower264(d) ((d & 0xff0000000000) >> 40)
#define Parse_ChargeLineCurrentLimit264(d) ((d & 0x3ff00000000) >> 32)
#define ID224PCSDCDCstatus 0x224

#define Parse_DCDCPrechargeStatus224(d) ((d & 0xc) >> 2)
#define Parse_DCDC12VSupportStatus224(d) ((d & 0xc000000000000000) >> 62)
#define Parse_DCDCHvBusDischargeStatus224(d) ((d & 0x3000000000000000) >> 60)
#define Parse_DCDCstate224(d) ((d & 0x3c00000000000000) >> 58)

enum DCDCstate224_Values { 
  Idle = 0,
  12vChg = 1,
  PrechargeStart = 2,
  Precharge = 3,
  HVactive = 4,
  Shutdown = 5,
  Error = 6
 };

#define Parse_DCDCSubState224(d) ((d & 0x7c0000000000000) >> 54)

enum DCDCSubState224_Values { 
  PWR_UP_INIT = 0,
  STANDBY = 1,
  12V_SUPPORT_ACTIVE = 2,
  DIS_HVBUS = 3,
  PCHG_FAST_DIS_HVBUS = 4,
  PCHG_SLOW_DIS_HVBUS = 5,
  PCHG_DWELL_CHARGE = 6,
  PCHG_DWELL_WAIT = 7,
  PCHG_DI_RECOVERY_WAIT = 8,
  PCHG_ACTIVE = 9,
  PCHG_FLT_FAST_DIS_HVBUS = 10,
  SHUTDOWN = 11,
  12V_SUPPORT_FAULTED = 12,
  DIS_HVBUS_FAULTED = 13,
  PCHG_FAULTED = 14,
  CLEAR_FAULTS = 15,
  FAULTED = 16,
  NUM = 17
 };

#define Parse_DCDCFaulted224(d) ((d & 0x2000000000000) >> 49)
#define Parse_DCDCoutputCurrent224(d) ((d & 0xfff000000000000) >> 48)
#define Parse_DCDCOutputIsLimited224(d) ((d & 0x1000000000) >> 36)
#define Parse_DCDCoutputCurrentMax224(d) ((d & 0x7ff800000000) >> 35)
#define Parse_DCDCPrechargeRtyCnt224(d) ((d & 0x3800000) >> 23)
#define Parse_DCDC12VSupportRtyCnt224(d) ((d & 0xf00000) >> 20)
#define Parse_DCDCDischargeRtyCnt224(d) ((d & 0xf0000) >> 16)
#define Parse_DCDCPwmEnableLine224(d) ((d & 0x1000) >> 12)
#define Parse_DCDCSupportingFixedLvTarget224(d) ((d & 0x800) >> 11)
#define Parse_PCS_ecuLogUploadRequest224(d) ((d & 0xc00) >> 10)
#define Parse_DCDCPrechargeRestartCnt224(d) ((d & 0x700) >> 8)
#define Parse_DCDCInitialPrechargeSubState224(d) ((d & 0x3e0) >> 5)
#define ID227CMP_state 0x227

#define Parse_CMP_speedRPM(d) ((d & 0xffff0000) >> 16)
#define Parse_CMP_speedDuty(d) ((d & 0xffff000000000000) >> 48)
#define Parse_CMP_inverterTemperature(d) ((d & 0xff00000000) >> 32)
#define Parse_CMP_HVOverVoltage(d) ((d & 0x1000000) >> 24)
#define Parse_CMP_HVUnderVoltage(d) ((d & 0x800000) >> 23)
#define Parse_CMP_overTemperature(d) ((d & 0x400000) >> 22)
#define Parse_CMP_underTemperature(d) ((d & 0x200000) >> 21)
#define Parse_CMP_VCFRONTCANTimeout(d) ((d & 0x100000) >> 20)
#define Parse_CMP_overCurrent(d) ((d & 0x80000) >> 19)
#define Parse_CMP_currentSensorCal(d) ((d & 0x40000) >> 18)
#define Parse_CMP_failedStart(d) ((d & 0x20000) >> 17)
#define Parse_CMP_motorVoltageSat(d) ((d & 0x10000) >> 16)
#define Parse_CMP_shortCircuit(d) ((d & 0x8000) >> 15)
#define Parse_CMP_repeatOverCurrent(d) ((d & 0x4000) >> 14)
#define Parse_CMP_state(d) ((d & 0xf00) >> 8)

enum CMP_state_Values { 
  CMP_STATE_NONE = 0,
  CMP_STATE_NORMAL = 1,
  CMP_STATE_WAIT = 2,
  CMP_STATE_FAULT = 3,
  CMP_STATE_SOFT_START = 4,
  CMP_STATE_SOFT_SHUTDOWN = 5,
  CMP_STATE_SNA = 15
 };

#define Parse_CMP_ready(d) ((d & 0x2) >> 1)
#define ID118DriveSystemStatus 0x118

#define Parse_DI_systemStatusChecksum(d) ((d & 0xff00) >> 8)
#define Parse_DI_systemStatusCounter(d) ((d & 0xf00000000000000) >> 56)
#define Parse_DI_driveBlocked(d) ((d & 0x30000000000000) >> 52)

enum DI_driveBlocked_Values { 
  DRIVE_BLOCKED_NONE = 0,
  DRIVE_BLOCKED_FRUNK = 1,
  DRIVE_BLOCKED_PROX = 2
 };

#define Parse_DI_systemState(d) ((d & 0x7000000000000) >> 48)

enum DI_systemState_Values { 
  DI_SYS_UNAVAILABLE = 0,
  DI_SYS_IDLE = 1,
  DI_SYS_STANDBY = 2,
  DI_SYS_FAULT = 3,
  DI_SYS_ABORT = 4,
  DI_SYS_ENABLE = 5
 };

#define Parse_DI_brakePedalState(d) ((d & 0x600000000000) >> 45)

enum DI_brakePedalState_Values { 
  OFF = 0,
  ON = 1,
  INVALID = 2
 };

#define Parse_DI_gear(d) ((d & 0x380000000000) >> 43)

enum DI_gear_Values { 
  DI_GEAR_INVALID = 0,
  DI_GEAR_P = 1,
  DI_GEAR_R = 2,
  DI_GEAR_N = 3,
  DI_GEAR_D = 4,
  DI_GEAR_SNA = 7
 };

#define Parse_DI_regenLight(d) ((d & 0x4000000000) >> 38)
#define Parse_DI_immobilizerState(d) ((d & 0xe000000000) >> 37)

enum DI_immobilizerState_Values { 
  DI_IMM_STATE_INIT_SNA = 0,
  DI_IMM_STATE_REQUEST = 1,
  DI_IMM_STATE_AUTHENTICATING = 2,
  DI_IMM_STATE_DISARMED = 3,
  DI_IMM_STATE_IDLE = 4,
  DI_IMM_STATE_RESET = 5,
  DI_IMM_STATE_FAULT = 6
 };

#define Parse_DI_accelPedalPos(d) ((d & 0xff00000000) >> 32)

enum DI_accelPedalPos_Values { 
  SNA = 255
 };

#define Parse_DI_tractionControlMode(d) ((d & 0x7000000) >> 24)

enum DI_tractionControlMode_Values { 
  Standard = 0,
  Slip Start = 1,
  Dev1 = 2,
  Dev2 = 3,
  Rolls Mode = 4,
  Dyno Mode = 5,
  Offroad Assist = 6
 };

#define Parse_DI_epbRequest(d) ((d & 0x300000) >> 20)

enum DI_epbRequest_Values { 
  DI_EPBREQUEST_NO_REQUEST = 0,
  DI_EPBREQUEST_PARK = 1,
  DI_EPBREQUEST_UNPARK = 2
 };

#define Parse_DI_proximity(d) ((d & 0x40000) >> 18)
#define Parse_DI_keepDrivePowerStateRequest(d) ((d & 0x20000) >> 17)

enum DI_keepDrivePowerStateRequest_Values { 
  NO_REQUEST = 0,
  KEEP_ALIVE = 1
 };

#define Parse_DI_trackModeState(d) ((d & 0x30000) >> 16)

enum DI_trackModeState_Values { 
  TRACK_MODE_UNAVAILABLE = 0,
  TRACK_MODE_AVAILABLE = 1,
  TRACK_MODE_ON = 2
 };

#define ID352BMS_energyStatus 0x352

#define Parse_BMS_nominalFullPackEnergy(d) ((d & 0x3ff800) >> 11)

enum BMS_nominalFullPackEnergy_Values { 
  SNA = 2047
 };

#define Parse_BMS_nominalEnergyRemaining(d) ((d & 0xffe0000000000000) >> 53)

enum BMS_nominalEnergyRemaining_Values { 
  SNA = 2047
 };

#define Parse_BMS_expectedEnergyRemaining(d) ((d & 0x1ffc0000000000) >> 42)

enum BMS_expectedEnergyRemaining_Values { 
  SNA = 2047
 };

#define Parse_BMS_idealEnergyRemaining(d) ((d & 0x3ff80000000) >> 31)

enum BMS_idealEnergyRemaining_Values { 
  SNA = 2047
 };

#define Parse_BMS_energyToChargeComplete(d) ((d & 0x7ff00000) >> 20)

enum BMS_energyToChargeComplete_Values { 
  SNA = 2047
 };

#define Parse_BMS_energyBuffer(d) ((d & 0x1fe00) >> 9)

enum BMS_energyBuffer_Values { 
  SNA = 255
 };

#define Parse_BMS_fullChargeComplete(d) ((d & 0x2) >> 1)
#define ID37DCP_thermalStatus 0x37d

#define Parse_CP_dcPinTemperature(d) ((d & 0xff00) >> 8)
#define Parse_CP_acPinTemperature(d) ((d & 0xff000000000000) >> 48)
#define Parse_CP_dTdt_dcPinActual(d) ((d & 0xfff0000000000) >> 40)
#define Parse_CP_dcPinTemperatureModeled(d) ((d & 0xff000000) >> 24)
#define Parse_CP_dTdt_dcPinExpected(d) ((d & 0xfff0000) >> 16)
#define ID392BMS_packConfig 0x392

#define Parse_BMS_packConfigMultiplexer(d) ((d & 0xff00) >> 8)

enum BMS_packConfigMultiplexer_Values { 
  Mux0 = 0,
  Mux1 = 1
 };

#define Parse_BMS_moduleType(d) ((d & 0x700000000000000) >> 56)

enum BMS_moduleType_Values { 
  UNKNOWN = 0,
  E3_NCT = 1,
  E1_NCT = 2,
  E3_CT = 3,
  E1_CT = 4,
  E1_CP = 5
 };

#define Parse_BMS_reservedConfig_0(d) ((d & 0x1f00000000000000) >> 56)

enum BMS_reservedConfig_0_Values { 
  BMS_CONFIG_0 = 0,
  BMS_CONFIG_1 = 1,
  BMS_CONFIG_2 = 2,
  BMS_CONFIG_3 = 3,
  BMS_CONFIG_4 = 4,
  BMS_CONFIG_5 = 5,
  BMS_CONFIG_6 = 6,
  BMS_CONFIG_7 = 7,
  BMS_CONFIG_8 = 8,
  BMS_CONFIG_9 = 9,
  BMS_CONFIG_10 = 10,
  BMS_CONFIG_11 = 11,
  BMS_CONFIG_12 = 12,
  BMS_CONFIG_13 = 13,
  BMS_CONFIG_14 = 14,
  BMS_CONFIG_15 = 15,
  BMS_CONFIG_16 = 16,
  BMS_CONFIG_17 = 17,
  BMS_CONFIG_18 = 18,
  BMS_CONFIG_19 = 19,
  BMS_CONFIG_20 = 20,
  BMS_CONFIG_21 = 21,
  BMS_CONFIG_22 = 22,
  BMS_CONFIG_23 = 23,
  BMS_CONFIG_24 = 24,
  BMS_CONFIG_25 = 25,
  BMS_CONFIG_26 = 26,
  BMS_CONFIG_27 = 27,
  BMS_CONFIG_28 = 28,
  BMS_CONFIG_29 = 29,
  BMS_CONFIG_30 = 30,
  BMS_CONFIG_31 = 31
 };

#define Parse_BMS_packMass(d) ((d & 0xff000000000000) >> 48)
#define Parse_BMS_platformMaxBusVoltage(d) ((d & 0x3ff0000000000) >> 40)
#define ID252BMS_powerAvailable 0x252

#define Parse_BMS_maxRegenPower(d) ((d & 0xffff0000) >> 16)
#define Parse_BMS_maxDischargePower(d) ((d & 0xffff000000000000) >> 48)
#define Parse_BMS_maxStationaryHeatPower(d) ((d & 0x3ff00000000) >> 32)
#define Parse_BMS_notEnoughPowerForHeatPump(d) ((d & 0x400000) >> 22)
#define Parse_BMS_powerLimitsState(d) ((d & 0x10000) >> 16)

enum BMS_powerLimitsState_Values { 
  POWER_NOT_CALCULATED_FOR_DRIVE = 0,
  POWER_CALCULATED_FOR_DRIVE = 1
 };

#define Parse_BMS_hvacPowerBudget(d) ((d & 0xffc000) >> 14)
#define ID312BMSthermal 0x312

#define Parse_BMSdissipation312(d) ((d & 0xffc00) >> 10)
#define Parse_BMSflowRequest312(d) ((d & 0x1fc0000000000000) >> 54)
#define Parse_BMSinletActiveCoolTarget312(d) ((d & 0xff800000000000) >> 47)
#define Parse_BMSinletPassiveTarget312(d) ((d & 0x7fc000000000) >> 38)
#define Parse_BMSinletActiveHeatTarget312(d) ((d & 0x3fe0000000) >> 29)
#define Parse_BMSminPackTemperature(d) ((d & 0x1ff00000) >> 20)
#define Parse_BMSmaxPackTemperature(d) ((d & 0xff800) >> 11)
#define Parse_BMSpcsNoFlowRequest312(d) ((d & 0x4) >> 2)
#define Parse_BMSnoFlowRequest312(d) ((d & 0x2) >> 1)
#define ID292BMS_SOC 0x292

#define Parse_SOCmin292(d) ((d & 0xffc00) >> 10)
#define Parse_SOCUI292(d) ((d & 0xffc0000000000000) >> 54)
#define Parse_SOCmax292(d) ((d & 0x3ff00000000000) >> 44)
#define Parse_SOCave292(d) ((d & 0xffc00000000) >> 34)
#define Parse_BattBeginningOfLifeEnergy292(d) ((d & 0x3ff000000) >> 24)

enum BattBeginningOfLifeEnergy292_Values { 
  SNA = 1023
 };

#define Parse_BMS_battTempPct(d) ((d & 0x3fc000) >> 14)

enum BMS_battTempPct_Values { 
  SNA = 255
 };

#define ID257DIspeed 0x257

#define Parse_DI_speedChecksum(d) ((d & 0xff00) >> 8)
#define Parse_DI_speedCounter(d) ((d & 0xf00000000000000) >> 56)
#define Parse_DI_vehicleSpeed(d) ((d & 0xfff0000000000000) >> 52)

enum DI_vehicleSpeed_Values { 
  SNA = 4095
 };

#define Parse_DI_uiSpeed(d) ((d & 0x1ff0000000000) >> 40)

enum DI_uiSpeed_Values { 
  DI_UI_SPEED_SNA = 511
 };

#define Parse_DI_uiSpeedUnits(d) ((d & 0x80000000) >> 31)

enum DI_uiSpeedUnits_Values { 
  DI_SPEED_MPH = 0,
  DI_SPEED_KPH = 1
 };

#define Parse_DI_uiSpeedHighSpeed(d) ((d & 0x7fc0000000) >> 30)

enum DI_uiSpeedHighSpeed_Values { 
  DI_UI_HIGH_SPEED_SNA = 511
 };

#define ID2A8CMPD_state 0x2a8

#define Parse_CMPD_speedRPM(d) ((d & 0x3ff800) >> 11)
#define Parse_CMPD_speedDuty(d) ((d & 0x7fe0000000000000) >> 53)
#define Parse_CMPD_inputHVPower(d) ((d & 0x3ff80000000000) >> 43)
#define Parse_CMPD_inputHVCurrent(d) ((d & 0x1ff00000000) >> 32)
#define Parse_CMPD_inputHVVoltage(d) ((d & 0x3ff800000) >> 23)
#define Parse_CMPD_powerLimitActive(d) ((d & 0x200) >> 9)
#define Parse_CMPD_state(d) ((d & 0xf00) >> 8)

enum CMPD_state_Values { 
  CMPD_STATE_INIT = 0,
  CMPD_STATE_RUNNING = 1,
  CMPD_STATE_STANDBY = 2,
  CMPD_STATE_FAULT = 3,
  CMPD_STATE_IDLE = 4,
  CMPD_STATE_SNA = 15
 };

#define Parse_CMPD_wasteHeatState(d) ((d & 0x30) >> 4)

enum CMPD_wasteHeatState_Values { 
  CMPD_WASTE_HEAT_STATE_OFF = 0,
  CMPD_WASTE_HEAT_STATE_ACTIVE = 1,
  CMPD_WASTE_HEAT_STATE_NOT_AVAILABLE = 2,
  CMPD_WASTE_HEAT_STATE_UNUSED = 3
 };

#define Parse_CMPD_powerLimitTooLowToStart(d) ((d & 0x4) >> 2)
#define Parse_CMPD_ready(d) ((d & 0x2) >> 1)
#define ID405VIN 0x405

#define Parse_mux405(d) ((d & 0xff00) >> 8)
#define Parse_VINB405(d) ((d & 0xffffffffffffff000000000000) >> 48)
#define Parse_VINC405(d) ((d & 0xffffffffffffff000000000000) >> 48)
#define Parse_VINA405(d) ((d & 0xffffffffffffff000000000000) >> 48)
#define ID51EFC_info 0x51e

#define Parse_FC_infoIndex(d) ((d & 0xff00) >> 8)
#define Parse_FC_infoAppGitHashBytes(d) ((d & 0xffffffffffffff000000000000) >> 48)
#define Parse_FC_infoBootGitHashBytes(d) ((d & 0xffffffffffffff000000000000) >> 48)
#define Parse_FC_infoBootSvnRev(d) ((d & 0xffffff0000) >> 16)
#define Parse_FC_infoBootUdsProtoVersion(d) ((d & 0xff00000000000000) >> 56)
#define Parse_FC_infoBuildType(d) ((d & 0x700000000000000) >> 56)
#define Parse_FC_infoPlatformType(d) ((d & 0xff00000000000000) >> 56)
#define Parse_FC_partNumChar01(d) ((d & 0xff00000000000000) >> 56)
#define Parse_FC_partNumChar08(d) ((d & 0xff00000000000000) >> 56)
#define Parse_FC_partNumChar15(d) ((d & 0xff00000000000000) >> 56)
#define Parse_FC_infoBuildConfigID(d) ((d & 0xff000000000000) >> 48)
#define Parse_FC_infoCPLDVersionMajor(d) ((d & 0xff000000000000) >> 48)
#define Parse_FC_infoMajorVersion(d) ((d & 0xff000000000000) >> 48)
#define Parse_FC_infoPcbaID(d) ((d & 0xff000000000000) >> 48)
#define Parse_FC_partNumChar02(d) ((d & 0xff000000000000) >> 48)
#define Parse_FC_partNumChar09(d) ((d & 0xff000000000000) >> 48)
#define Parse_FC_partNumChar16(d) ((d & 0xff000000000000) >> 48)
#define Parse_FC_infoAssemblyID(d) ((d & 0xff0000000000) >> 40)
#define Parse_FC_infoBranchOrigin(d) ((d & 0xff0000000000) >> 40)
#define Parse_FC_infoCPLDVersionMinor(d) ((d & 0xff0000000000) >> 40)
#define Parse_FC_partNumChar03(d) ((d & 0xff0000000000) >> 40)
#define Parse_FC_partNumChar10(d) ((d & 0xff0000000000) >> 40)
#define Parse_FC_partNumChar17(d) ((d & 0xff0000000000) >> 40)
#define Parse_FC_infoApplicationCRC(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_FC_infoBootCRC(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_FC_infoHardwareID(d) ((d & 0xffff00000000) >> 32)
#define Parse_FC_infoMaturity(d) ((d & 0xff00000000) >> 32)
#define Parse_FC_infoUsageID(d) ((d & 0xffff00000000) >> 32)
#define Parse_FC_infoVariantCRC(d) ((d & 0xffffffff00000000) >> 32)
#define Parse_FC_partNumChar04(d) ((d & 0xff00000000) >> 32)
#define Parse_FC_partNumChar11(d) ((d & 0xff00000000) >> 32)
#define Parse_FC_partNumChar18(d) ((d & 0xff00000000) >> 32)
#define Parse_FC_infoHardwareRevision(d) ((d & 0xff000000) >> 24)
#define Parse_FC_partNumChar05(d) ((d & 0xff000000) >> 24)
#define Parse_FC_partNumChar12(d) ((d & 0xff000000) >> 24)
#define Parse_FC_partNumChar19(d) ((d & 0xff000000) >> 24)
#define Parse_FC_infoComponentID(d) ((d & 0xffff0000) >> 16)
#define Parse_FC_infoSubUsageID(d) ((d & 0xffff0000) >> 16)
#define Parse_FC_partNumChar06(d) ((d & 0xff0000) >> 16)
#define Parse_FC_partNumChar13(d) ((d & 0xff0000) >> 16)
#define Parse_FC_partNumChar20(d) ((d & 0xff0000) >> 16)
#define Parse_FC_partNumChar07(d) ((d & 0xff00) >> 8)
#define Parse_FC_partNumChar14(d) ((d & 0xff00) >> 8)
#define ID376FrontInverterTemps 0x376

#define Parse_TempInvPCB376(d) ((d & 0xff00) >> 8)
#define Parse_TempInverter376(d) ((d & 0xff00000000000000) >> 56)
#define Parse_TempStator376(d) ((d & 0xff000000000000) >> 48)
#define Parse_TempInvCapbank376(d) ((d & 0xff0000000000) >> 40)
#define Parse_TempInvHeatsink376(d) ((d & 0xff00000000) >> 32)
#define Parse_TempPctInverter376(d) ((d & 0xff000000) >> 24)
#define Parse_TempPctStator376(d) ((d & 0xff0000) >> 16)
#define Parse_DIF_inverterTQF(d) ((d & 0x300) >> 8)

enum DIF_inverterTQF_Values { 
  INVERTERT_INIT = 0,
  INVERTERT_IRRATIONAL = 1,
  INVERTERT_RATIONAL = 2,
  INVERTERT_UNKNOWN = 3
 };

#define ID315RearInverterTemps 0x315

#define Parse_RearTempInvPCB315(d) ((d & 0xff00) >> 8)
#define Parse_RearTempInverter315(d) ((d & 0xff00000000000000) >> 56)
#define Parse_RearTempStator315(d) ((d & 0xff000000000000) >> 48)
#define Parse_RearTempInvCapbank315(d) ((d & 0xff0000000000) >> 40)
#define Parse_RearTempInvHeatsink315(d) ((d & 0xff00000000) >> 32)
#define Parse_RearTempPctInverter315(d) ((d & 0xff000000) >> 24)
#define Parse_RearTempPctStator315(d) ((d & 0xff0000) >> 16)
#define Parse_DIR_inverterTQF(d) ((d & 0x300) >> 8)

enum DIR_inverterTQF_Values { 
  INVERTERT_INIT = 0,
  INVERTERT_IRRATIONAL = 1,
  INVERTERT_RATIONAL = 2,
  INVERTERT_UNKNOWN = 3
 };

#define ID154RearTorqueOld 0x154

#define Parse_RAWTorqueRear154(d) ((d & 0xfff000000) >> 24)
#define ID3B6odometer 0x3b6

#define Parse_Odometer3B6(d) ((d & 0xffffffff00000000) >> 32)

enum Odometer3B6_Values { 
  SNA = 4294967
 };

#define ID266RearInverterPower 0x266

#define Parse_RearPower266(d) ((d & 0x3ff800) >> 11)
#define Parse_RearHeatPowerOptimal266(d) ((d & 0xff000000000000) >> 48)
#define Parse_RearHeatPowerMax266(d) ((d & 0xff0000000000) >> 40)
#define Parse_RearHeatPower266(d) ((d & 0xff00000000) >> 32)
#define Parse_RearExcessHeatCmd(d) ((d & 0xff000000) >> 24)
#define Parse_RearPowerLimit266(d) ((d & 0x1ff0000) >> 16)
#define ID2E5FrontInverterPower 0x2e5

#define Parse_FrontPower2E5(d) ((d & 0x3ff800) >> 11)
#define Parse_FrontHeatPowerOptimal2E5(d) ((d & 0xff000000000000) >> 48)
#define Parse_FrontHeatPowerMax2E5(d) ((d & 0xff0000000000) >> 40)
#define Parse_FrontHeatPower2E5(d) ((d & 0xff00000000) >> 32)
#define Parse_FrontExcessHeatCmd(d) ((d & 0xff000000) >> 24)
#define Parse_FrontPowerLimit2E5(d) ((d & 0x1ff0000) >> 16)
#define ID2E6PlaidFrontPower 0x2e6

#define Parse_PFrontPower(d) ((d & 0x3ff800) >> 11)
#define Parse_PFrontHeatPowerOptimal(d) ((d & 0xff000000000000) >> 48)
#define Parse_PFrontHeatPowerMax(d) ((d & 0xff0000000000) >> 40)
#define Parse_PFrontHeatPower(d) ((d & 0xff00000000) >> 32)
#define Parse_PFrontExcessHeatCmd(d) ((d & 0xff000000) >> 24)
#define Parse_PFrontPowerLimit(d) ((d & 0x1ff0000) >> 16)
#define ID269LeftRearPower 0x269

#define Parse_LeftRearPower(d) ((d & 0x3ff800) >> 11)
#define Parse_LeftRearHeatPowerOptimal(d) ((d & 0xff000000000000) >> 48)
#define Parse_LeftRearPowerMax(d) ((d & 0xff0000000000) >> 40)
#define Parse_LeftRearHeatPower(d) ((d & 0xff00000000) >> 32)
#define Parse_LeftRearExcessHeatCmd(d) ((d & 0xff000000) >> 24)
#define Parse_LeftRearPowerLimit(d) ((d & 0x1ff0000) >> 16)
#define ID27CRightRearPower 0x27c

#define Parse_RightRearPower(d) ((d & 0x3ff800) >> 11)
#define Parse_RightRearHeatPowerOptimal(d) ((d & 0xff000000000000) >> 48)
#define Parse_RightRearHeatPowerMax(d) ((d & 0xff0000000000) >> 40)
#define Parse_RightRearHeatPower(d) ((d & 0xff00000000) >> 32)
#define Parse_RightRearExcessHeatCmd(d) ((d & 0xff000000) >> 24)
#define Parse_RightRearPowerLimit(d) ((d & 0x1ff0000) >> 16)
#define ID108DIR_torque 0x108

#define Parse_DIR_torqueChecksum(d) ((d & 0xff00) >> 8)
#define Parse_DIR_torqueCounter(d) ((d & 0xf00000000000000) >> 56)
#define Parse_DIR_torqueCommand(d) ((d & 0x3ffe) >> 1)

enum DIR_torqueCommand_Values { 
  SNA = 4096
 };

#define Parse_DIR_axleSpeedQF(d) ((d & 0x8000000000) >> 39)
#define Parse_DIR_torqueActual(d) ((d & 0x3ffe000000000) >> 37)

enum DIR_torqueActual_Values { 
  SNA = 4096
 };

#define Parse_DIR_axleSpeed(d) ((d & 0xffff000000) >> 24)

enum DIR_axleSpeed_Values { 
  SNA = 32768
 };

#define Parse_DIR_slavePedalPos(d) ((d & 0xff00) >> 8)

enum DIR_slavePedalPos_Values { 
  SNA = 255
 };

#define ID132HVBattAmpVolt 0x132

#define Parse_BattVoltage132(d) ((d & 0xffff0000) >> 16)
#define Parse_SmoothBattCurrent132(d) ((d & 0xffff000000000000) >> 48)
#define Parse_RawBattCurrent132(d) ((d & 0xffff00000000) >> 32)
#define Parse_ChargeHoursRemaining132(d) ((d & 0xfff0000) >> 16)
#define ID126RearHVStatus 0x126

#define Parse_RearHighVoltage126(d) ((d & 0xffc00) >> 10)
#define Parse_DIR_vBatQF(d) ((d & 0x400000000000) >> 46)

enum DIR_vBatQF_Values { 
  NOT_QUALIFIED = 0,
  QUALIFIED = 1
 };

#define Parse_RearMotorCurrent126(d) ((d & 0xffe00000000000) >> 45)
#define Parse_DIR_dcCableCurrentEst(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIR_switchingFrequency(d) ((d & 0x7ff0000) >> 16)
#define Parse_DIR_targetFluxMode(d) ((d & 0x60) >> 5)
#define ID1A5FrontHVStatus 0x1a5

#define Parse_FrontHighVoltage1A5(d) ((d & 0xffc00) >> 10)
#define Parse_DIF_vBatQF(d) ((d & 0x400000000000) >> 46)

enum DIF_vBatQF_Values { 
  NOT_QUALIFIED = 0,
  QUALIFIED = 1
 };

#define Parse_FrontMotorCurrent1A5(d) ((d & 0xffe00000000000) >> 45)
#define Parse_DIF_dcCableCurrentEst(d) ((d & 0xffff00000000) >> 32)
#define Parse_DIF_switchingFrequency(d) ((d & 0x7ff0000) >> 16)
#define Parse_DIF_targetFluxMode(d) ((d & 0x60) >> 5)
#define ID127LeftRearHVStatus 0x127

#define Parse_LeftRear_vBat(d) ((d & 0xffc00) >> 10)
#define Parse_LeftRear_vBatQF(d) ((d & 0x400000000000) >> 46)
#define Parse_LeftRear_motorCurrent(d) ((d & 0xffe00000000000) >> 45)
#define Parse_LeftRear_dcCableCurrentEst(d) ((d & 0xffff00000000) >> 32)
#define Parse_LeftRear_switchingFrequency(d) ((d & 0x7ff0000) >> 16)
#define Parse_LeftRear_targetFluxMode(d) ((d & 0x60) >> 5)
#define ID12ARightRearHVStatus 0x12a

#define Parse_RightRear_vBat(d) ((d & 0xffc00) >> 10)
#define Parse_RightRear_vBatQF(d) ((d & 0x400000000000) >> 46)
#define Parse_RightRear_motorCurrent(d) ((d & 0xffe00000000000) >> 45)
#define Parse_RightRear_dcCableCurrentEst(d) ((d & 0xffff00000000) >> 32)
#define Parse_RightRear_switchingFrequency(d) ((d & 0x7ff0000) >> 16)
#define Parse_RightRear_targetFluxMode(d) ((d & 0x60) >> 5)
#define ID31FTPMSsensors 0x31f

#define Parse_TPMSFLpressure31F(d) ((d & 0xff00) >> 8)
#define Parse_TPMSFLtemp31F(d) ((d & 0xff00000000000000) >> 56)
#define Parse_TPMSFRpressure31F(d) ((d & 0xff000000000000) >> 48)
#define Parse_TPMSFRtemp31F(d) ((d & 0xff0000000000) >> 40)
#define Parse_TPMSRLpressure31F(d) ((d & 0xff00000000) >> 32)
#define Parse_TPMSRLtemp31F(d) ((d & 0xff000000) >> 24)
#define Parse_TPMSRRpressure31F(d) ((d & 0xff0000) >> 16)
#define Parse_TPMSRRtemp31F(d) ((d & 0xff00) >> 8)
#define ID3FEbrakeTemps 0x3fe

#define Parse_BrakeTempFL3FE(d) ((d & 0xffc00) >> 10)
#define Parse_BrakeTempFR3FE(d) ((d & 0xffc0000000) >> 30)
#define Parse_BrakeTempRL3FE(d) ((d & 0x3ff00000) >> 20)
#define Parse_BrakeTempRR3FE(d) ((d & 0xffc00) >> 10)
#define ID228EPBrightStatus 0x228

#define Parse_EPBRunitStatus228(d) ((d & 0x3e0) >> 5)

enum EPBRunitStatus228_Values { 
  DriveReleased = 1,
  ParkEngaged = 3,
  Engaging = 8,
  Disengaging = 10
 };

#define Parse_EPBRunitFaultStatus228(d) ((d & 0x1800000000000000) >> 59)
#define Parse_EPBRsummonState228(d) ((d & 0xe00000000000000) >> 57)
#define Parse_EPBRdisconnected228(d) ((d & 0x40000000000000) >> 54)
#define Parse_EPBRlockoutUnlockCount228(d) ((d & 0xfe0000000000000) >> 53)
#define Parse_EPBResmCaliperRequest228(d) ((d & 0x1c00000000000) >> 46)

enum EPBResmCaliperRequest228_Values { 
  idle = 1,
  engaging = 2,
  disengaging = 3
 };

#define Parse_EPBRcsmFaultReason228(d) ((d & 0xf80000000000) >> 43)
#define Parse_EPBResmOperationTrigger228(d) ((d & 0x7c000000000) >> 38)

enum EPBResmOperationTrigger228_Values { 
  ParkEngaged = 1,
  Released = 6,
  SuperPark = 22
 };

#define Parse_EPBRsummonFaultReason228(d) ((d & 0x3e00000000) >> 33)
#define Parse_EPBRlocalServiceModeActive228(d) ((d & 0x10000000) >> 28)
#define Parse_EPBR12VFilt228(d) ((d & 0x7ff8000000) >> 27)
#define Parse_EPBRCDPQualified228(d) ((d & 0x18000) >> 15)
#define Parse_EPBRinternalCDPRequest228(d) ((d & 0x2000) >> 13)
#define Parse_EPBRinternalStatusCounter228(d) ((d & 0xf000) >> 12)
#define Parse_EPBRinternalStatusChecksum228(d) ((d & 0xff00) >> 8)
#define ID288EPBleftStatus 0x288

#define Parse_EPBLunitStatus288(d) ((d & 0x3e0) >> 5)

enum EPBLunitStatus288_Values { 
  DriveReleased = 1,
  ParkEngaged = 3,
  Engaging = 8,
  Disengaging = 10
 };

#define Parse_EPBLunitFaultStatus288(d) ((d & 0x1800000000000000) >> 59)
#define Parse_EPBLsummonState288(d) ((d & 0xe00000000000000) >> 57)
#define Parse_EPBLdisconnected288(d) ((d & 0x40000000000000) >> 54)
#define Parse_EPBLlockoutUnlockCount288(d) ((d & 0xfe0000000000000) >> 53)
#define Parse_EPBLesmCaliperRequest288(d) ((d & 0x1c00000000000) >> 46)

enum EPBLesmCaliperRequest288_Values { 
  idle = 1,
  engaging = 2,
  disengaging = 3
 };

#define Parse_EPBLcsmFaultReason288(d) ((d & 0xf80000000000) >> 43)
#define Parse_EPBLesmOperationTrigger288(d) ((d & 0x7c000000000) >> 38)

enum EPBLesmOperationTrigger288_Values { 
  ParkEngaged = 1,
  Released = 6,
  SuperPark = 22
 };

#define Parse_EPBLsummonFaultReason288(d) ((d & 0x3e00000000) >> 33)
#define Parse_EPBLlocalServiceModeActive288(d) ((d & 0x10000000) >> 28)
#define Parse_EPBL12VFilt288(d) ((d & 0x7ff8000000) >> 27)
#define Parse_EPBLCDPQualified288(d) ((d & 0x18000) >> 15)
#define Parse_EPBLinternalCDPRequest288(d) ((d & 0x2000) >> 13)
#define Parse_EPBLinternalStatusCounter288(d) ((d & 0xf000) >> 12)
#define Parse_EPBLinternalStatusChecksum288(d) ((d & 0xff00) >> 8)
#define ID72ABMS_serialNumber 0x72a

#define Parse_BMS_serialNumberMultiplexer(d) ((d & 0x2) >> 1)
#define Parse_BMS_packSerialNumberByte01(d) ((d & 0xff00000000000000) >> 56)
#define Parse_BMS_packSerialNumberByte08(d) ((d & 0xff00000000000000) >> 56)
#define Parse_BMS_packSerialNumberByte02(d) ((d & 0xff000000000000) >> 48)
#define Parse_BMS_packSerialNumberByte09(d) ((d & 0xff000000000000) >> 48)
#define Parse_BMS_packSerialNumberByte03(d) ((d & 0xff0000000000) >> 40)
#define Parse_BMS_packSerialNumberByte10(d) ((d & 0xff0000000000) >> 40)
#define Parse_BMS_packSerialNumberByte04(d) ((d & 0xff00000000) >> 32)
#define Parse_BMS_packSerialNumberByte11(d) ((d & 0xff00000000) >> 32)
#define Parse_BMS_packSerialNumberByte05(d) ((d & 0xff000000) >> 24)
#define Parse_BMS_packSerialNumberByte12(d) ((d & 0xff000000) >> 24)
#define Parse_BMS_packSerialNumberByte06(d) ((d & 0xff0000) >> 16)
#define Parse_BMS_packSerialNumberByte13(d) ((d & 0xff0000) >> 16)
#define Parse_BMS_packSerialNumberByte07(d) ((d & 0xff00) >> 8)
#define Parse_BMS_packSerialNumberByte14(d) ((d & 0xff00) >> 8)
#define ID7FFcarConfig 0x7ff

#define Parse_GTW_carConfigMultiplexer(d) ((d & 0xff00) >> 8)
#define Parse_GTW_birthday(d) ((d & 0xffffffff000000) >> 24)
#define Parse_GTW_deliveryStatus(d) ((d & 0x100000000000000) >> 56)

enum GTW_deliveryStatus_Values { 
  NOT_DELIVERED = 0,
  DELIVERED = 1
 };

#define Parse_GTW_mapRegion(d) ((d & 0xf00000000000000) >> 56)

enum GTW_mapRegion_Values { 
  US = 0,
  EU = 1,
  NONE = 2,
  CN = 3,
  AU = 4,
  JP = 5,
  TW = 6,
  KR = 7,
  ME = 8,
  HK = 9,
  MO = 10
 };

#define Parse_GTW_epasType(d) ((d & 0x80000000000000) >> 55)

enum GTW_epasType_Values { 
  MANDO_VGR69_GEN3 = 0
 };

#define Parse_GTW_frontSeatHeaters(d) ((d & 0x80000000000000) >> 55)

enum GTW_frontSeatHeaters_Values { 
  NONE = 0,
  KONGSBERG_LOW_POWER = 1
 };

#define Parse_GTW_drivetrainType(d) ((d & 0x40000000000000) >> 54)

enum GTW_drivetrainType_Values { 
  RWD = 0,
  AWD = 1
 };

#define Parse_GTW_rearSeatHeaters(d) ((d & 0x40000000000000) >> 54)

enum GTW_rearSeatHeaters_Values { 
  NONE = 0,
  KONGSBERG_LOW_POWER = 1
 };

#define Parse_GTW_rightHandDrive(d) ((d & 0x20000000000000) >> 53)

enum GTW_rightHandDrive_Values { 
  LEFT = 0,
  RIGHT = 1
 };

#define Parse_GTW_tpmsType(d) ((d & 0x20000000000000) >> 53)

enum GTW_tpmsType_Values { 
  CONTI_2 = 0,
  TESLA_BLE = 1
 };

#define Parse_GTW_performancePackage(d) ((d & 0x70000000000000) >> 52)

enum GTW_performancePackage_Values { 
  BASE = 0,
  PERFORMANCE = 1,
  LUDICROUS = 2,
  BASE_PLUS = 3,
  BASE_PLUS_AWD = 4
 };

#define Parse_GTW_rearLightType(d) ((d & 0x30000000000000) >> 52)

enum GTW_rearLightType_Values { 
  NA = 0,
  EU_CN = 1,
  GLOBAL = 2
 };

#define Parse_GTW_homelinkType(d) ((d & 0x8000000000000) >> 51)

enum GTW_homelinkType_Values { 
  NONE = 0,
  HOMELINK_V_OPT_2 = 1
 };

#define Parse_GTW_headlamps(d) ((d & 0xc000000000000) >> 50)

enum GTW_headlamps_Values { 
  BASE = 0,
  PREMIUM = 1
 };

#define Parse_GTW_vdcType(d) ((d & 0x4000000000000) >> 50)

enum GTW_vdcType_Values { 
  BOSCH_VDC = 0,
  TESLA_VDC = 1
 };

#define Parse_GTW_towPackage(d) ((d & 0x2000000000000) >> 49)

enum GTW_towPackage_Values { 
  NONE = 0,
  TESLA_REV1 = 1
 };

#define Parse_GTW_xcpIbst(d) ((d & 0x2000000000000) >> 49)

enum GTW_xcpIbst_Values { 
  FALSE = 0,
  TRUE = 1
 };

#define Parse_GTW_country(d) ((d & 0xffff000000000000) >> 48)
#define Parse_GTW_xcpESP(d) ((d & 0x1000000000000) >> 48)

enum GTW_xcpESP_Values { 
  FALSE = 0,
  TRUE = 1
 };

#define Parse_GTW_coolantPumpType(d) ((d & 0x800000000000) >> 47)

enum GTW_coolantPumpType_Values { 
  DUAL = 0,
  SINGLE_PUMP_BATT = 1
 };

#define Parse_GTW_memoryMirrors(d) ((d & 0x800000000000) >> 47)

enum GTW_memoryMirrors_Values { 
  NOT_INSTALLED = 0,
  SMR = 1
 };

#define Parse_GTW_chassisType(d) ((d & 0x1c00000000000) >> 46)

enum GTW_chassisType_Values { 
  MODEL_S_CHASSIS = 0,
  MODEL_X_CHASSIS = 1,
  MODEL_3_CHASSIS = 2,
  MODEL_Y_CHASSIS = 3
 };

#define Parse_GTW_powerSteeringColumn(d) ((d & 0x400000000000) >> 46)

enum GTW_powerSteeringColumn_Values { 
  NOT_INSTALLED = 0,
  TK = 1
 };

#define Parse_GTW_frontFogLamps(d) ((d & 0x100000000000) >> 44)

enum GTW_frontFogLamps_Values { 
  NOT_INSTALLED = 0,
  INSTALLED = 1
 };

#define Parse_GTW_airSuspension(d) ((d & 0xc0000000000) >> 42)

enum GTW_airSuspension_Values { 
  NONE = 0,
  TESLA_STANDARD = 1,
  TESLA_ADAPTIVE = 2
 };

#define Parse_GTW_lumbarECUType(d) ((d & 0x20000000000) >> 41)

enum GTW_lumbarECUType_Values { 
  NONE = 0,
  ALFMEIER = 1
 };

#define Parse_GTW_passengerOccupancySensorType(d) ((d & 0x10000000000) >> 40)

enum GTW_passengerOccupancySensorType_Values { 
  OCS = 0,
  RESISTIVE_PAD = 1
 };

#define Parse_GTW_autopilotCameraType(d) ((d & 0x4000000000) >> 38)

enum GTW_autopilotCameraType_Values { 
  RCCB_CAMERAS = 0
 };

#define Parse_GTW_auxParkLamps(d) ((d & 0xc000000000) >> 38)

enum GTW_auxParkLamps_Values { 
  NA_BASE = 0,
  NA_PREMIUM = 1,
  EU = 2,
  NONE = 3
 };

#define Parse_GTW_connectivityPackage(d) ((d & 0x2000000000) >> 37)

enum GTW_connectivityPackage_Values { 
  BASE = 0,
  PREMIUM = 1
 };

#define Parse_GTW_plcSupportType(d) ((d & 0x3000000000) >> 36)

enum GTW_plcSupportType_Values { 
  NONE = 0,
  ONBOARD_ADAPTER = 1,
  NATIVE_CHARGE_PORT = 2
 };

#define Parse_GTW_hvacPanelVaneType(d) ((d & 0x800000000) >> 35)

enum GTW_hvacPanelVaneType_Values { 
  PARALLEL_V1 = 0,
  CONVERGENT_V1 = 1
 };

#define Parse_GTW_audioType(d) ((d & 0xc00000000) >> 34)

enum GTW_audioType_Values { 
  BASE = 0,
  PREMIUM = 1,
  BASE_WITH_PREMIUM200 = 2
 };

#define Parse_GTW_cabinPTCHeaterType(d) ((d & 0x200000000) >> 33)

enum GTW_cabinPTCHeaterType_Values { 
  BORGWARNER = 0,
  NONE = 1
 };

#define Parse_GTW_eBuckConfig(d) ((d & 0x100000000) >> 32)

enum GTW_eBuckConfig_Values { 
  NONE = 0,
  DEV_BUCK = 1
 };

#define Parse_GTW_packEnergy(d) ((d & 0x1f00000000) >> 32)

enum GTW_packEnergy_Values { 
  PACK_50_KWH = 0,
  PACK_74_KWH = 1,
  PACK_62_KWH = 2,
  PACK_100_KWH = 3,
  PACK_75_KWH = 4
 };

#define Parse_GTW_tireType(d) ((d & 0x1f00000000) >> 32)

enum GTW_tireType_Values { 
  UNKNOWN = 0,
  MICHELIN_ALL_SEASON_18 = 1,
  MICHELIN_SUMMER_18 = 2,
  HANKOOK_SUMMER_19 = 3,
  CONTI_ALL_SEASON_19 = 4,
  MICHELIN_SUMMER_20 = 5,
  GOODYEAR_ALL_SEASON_20 = 17,
  PIRELLI_SUMMER_21 = 18,
  MICHELIN_ALL_SEASON_21 = 19
 };

#define Parse_GTW_windshieldType(d) ((d & 0x80000000) >> 31)

enum GTW_windshieldType_Values { 
  SEKISUI_ACOUSTIC = 0,
  EASTMAN_ACOUSTIC = 1
 };

#define Parse_GTW_activeHighBeam(d) ((d & 0x40000000) >> 30)

enum GTW_activeHighBeam_Values { 
  NOT_ACTIVE = 0,
  ACTIVE = 1
 };

#define Parse_GTW_airbagCutoffSwitch(d) ((d & 0x20000000) >> 29)

enum GTW_airbagCutoffSwitch_Values { 
  CUTOFF_SWITCH_DISABLED = 0,
  CUTOFF_SWITCH_ENABLED = 1
 };

#define Parse_GTW_intrusionSensorType(d) ((d & 0x10000000) >> 28)

enum GTW_intrusionSensorType_Values { 
  NOT_INSTALLED = 0,
  VODAFONE = 1
 };

#define Parse_GTW_frontSeatReclinerHardware(d) ((d & 0x18000000) >> 27)

enum GTW_frontSeatReclinerHardware_Values { 
  STANDARD_RANGE = 0,
  RIGHT_SEAT_REDUCED_RANGE = 1,
  LEFT_SEAT_REDUCED_RANGE = 2,
  LEFT_RIGHT_SEAT_REDUCED_RANGE = 3
 };

#define Parse_GTW_spoilerType(d) ((d & 0x8000000) >> 27)

enum GTW_spoilerType_Values { 
  NOT_INSTALLED = 0,
  PASSIVE = 1
 };

#define Parse_GTW_rearGlassType(d) ((d & 0x4000000) >> 26)

enum GTW_rearGlassType_Values { 
  NX = 0,
  TSA5_NOPET = 1
 };

#define Parse_GTW_brakeLineSwitchType(d) ((d & 0x2000000) >> 25)

enum GTW_brakeLineSwitchType_Values { 
  DI_VC_SHARED = 0,
  VC_ONLY = 1
 };

#define Parse_GTW_rearFogLamps(d) ((d & 0x2000000) >> 25)

enum GTW_rearFogLamps_Values { 
  NOT_INSTALLED = 0,
  INSTALLED = 1
 };

#define Parse_GTW_dasHw(d) ((d & 0x7000000) >> 24)

enum GTW_dasHw_Values { 
  PARKER_PASCAL_2_5 = 3,
  TESLA_AP3 = 4
 };

#define Parse_GTW_eCallEnabled(d) ((d & 0x3000000) >> 24)

enum GTW_eCallEnabled_Values { 
  DISABLED = 0,
  ENABLED_OHC_SOS = 1,
  ENABLED_UI_SOS = 2
 };

#define Parse_GTW_espValveType(d) ((d & 0x3000000) >> 24)

enum GTW_espValveType_Values { 
  UNKNOWN = 0,
  VALVE_TYPE_1 = 1,
  VALVE_TYPE_2 = 2
 };

#define Parse_GTW_roofType(d) ((d & 0x1000000) >> 24)

enum GTW_roofType_Values { 
  METAL = 0,
  FIXED_GLASS = 1,
  PANORAMIC = 2
 };

#define Parse_GTW_autopilot(d) ((d & 0x1c00000) >> 22)

enum GTW_autopilot_Values { 
  NONE = 0,
  HIGHWAY = 1,
  ENHANCED = 2,
  SELF_DRIVING = 3,
  BASIC = 4
 };

#define Parse_GTW_softRange(d) ((d & 0x1c00000) >> 22)

enum GTW_softRange_Values { 
  STANDARD = 0,
  RANGE_220_MILES = 1,
  RANGE_93_MILES = 2
 };

#define Parse_GTW_passengerAirbagType(d) ((d & 0x600000) >> 21)

enum GTW_passengerAirbagType_Values { 
  FULL_SUPPRESSION = 0,
  SAFETY_VENT = 1,
  EUROW = 2
 };

#define Parse_GTW_refrigerantType(d) ((d & 0x180000) >> 19)

enum GTW_refrigerantType_Values { 
  Default = 0,
  R134A = 1,
  R1234YF = 2
 };

#define Parse_GTW_superchargingAccess(d) ((d & 0x180000) >> 19)

enum GTW_superchargingAccess_Values { 
  NOT_ALLOWED = 0,
  ALLOWED = 1,
  PAY_AS_YOU_GO = 2
 };

#define Parse_GTW_compressorType(d) ((d & 0xc0000) >> 18)

enum GTW_compressorType_Values { 
  HANON_33CC = 0,
  DENSO_41CC_8K = 1,
  DENSO_41CC_11K = 2
 };

#define Parse_GTW_headlightLevelerType(d) ((d & 0x20000) >> 17)

enum GTW_headlightLevelerType_Values { 
  NONE = 0,
  GEN1 = 1
 };

#define Parse_GTW_efficiencyPackage(d) ((d & 0x70000) >> 16)

enum GTW_efficiencyPackage_Values { 
  DEFAULT = 0,
  M3_SR_PLUS_2020 = 1,
  M3_LR_2020 = 2,
  M3_LR_PERFORMANCE_2020 = 3
 };

#define Parse_GTW_exteriorColor(d) ((d & 0x70000) >> 16)

enum GTW_exteriorColor_Values { 
  RED_MULTICOAT = 0,
  SOLID_BLACK = 1,
  SILVER_METALLIC = 2,
  MIDNIGHT_SILVER = 3,
  DEEP_BLUE = 5,
  PEARL_WHITE = 6
 };

#define Parse_GTW_restraintsHardwareType(d) ((d & 0xff0000) >> 16)

enum GTW_restraintsHardwareType_Values { 
  NA_M3 = 21,
  EUROW_ECALL_M3 = 22,
  EUROW_NO_ECALL_M3 = 23,
  NA_MY_OLD = 31,
  NA_MY = 32,
  EUROW_ECALL_MY = 33,
  EUROW_NO_ECALL_MY = 34
 };

#define Parse_GTW_wheelType(d) ((d & 0x7f0000) >> 16)

enum GTW_wheelType_Values { 
  PINWHEEL_18 = 0,
  STILETTO_19 = 1,
  STILETTO_20 = 2,
  STILETTO_20_DARK_STAGGERED = 3,
  GEMINI_19_SQUARE = 4,
  GEMINI_19_STAGGERED = 5,
  STILETTO_20_DARK_SQUARE = 14,
  INDUCTION_20_BLACK = 15,
  UBERTURBINE_21_BLACK = 16,
  APOLLO_19_SILVER = 17,
  PINWHEEL_18_CAP_KIT = 18,
  ZEROG_20_GUNPOWDER = 19,
  APOLLO_19_SILVER_CAP_KIT = 20
 };

#define Parse_GTW_numberHVILNodes(d) ((d & 0x6000) >> 13)

enum GTW_numberHVILNodes_Values { 
  HVIL_NODES_0 = 0,
  HVIL_NODES_1 = 1,
  HVIL_NODES_2 = 2,
  HVIL_NODES_3 = 3,
  HVIL_NODES_4 = 4,
  HVIL_NODES_5 = 5
 };

#define Parse_GTW_steeringColumnMotorType(d) ((d & 0x1000) >> 12)

enum GTW_steeringColumnMotorType_Values { 
  BOSCH = 0,
  JE = 1
 };

#define Parse_GTW_pedestrianWarningSound(d) ((d & 0x400) >> 10)

enum GTW_pedestrianWarningSound_Values { 
  NONE = 0,
  SPEAKER = 1
 };

#define Parse_GTW_radarHeaterType(d) ((d & 0x200) >> 9)

enum GTW_radarHeaterType_Values { 
  NONE = 0,
  BECKER_THIN_3M = 1
 };

#define Parse_GTW_steeringColumnUJointType(d) ((d & 0x200) >> 9)

enum GTW_steeringColumnUJointType_Values { 
  B_SAMPLE_PHASING = 0,
  C_SAMPLE_PHASING = 1
 };

#define Parse_GTW_bPillarNFCParam(d) ((d & 0x100) >> 8)

enum GTW_bPillarNFCParam_Values { 
  MODEL_3 = 0,
  MODEL_Y = 1
 };

#define Parse_GTW_immersiveAudio(d) ((d & 0x300) >> 8)

enum GTW_immersiveAudio_Values { 
  DISABLED = 0,
  BASE = 1,
  PREMIUM = 2
 };

#define Parse_GTW_interiorLighting(d) ((d & 0x180) >> 7)

enum GTW_interiorLighting_Values { 
  BASE = 0,
  PREMIUM = 1,
  PREMIUM_NO_POCKET_LIGHT = 2
 };

#define Parse_GTW_brakeHWType(d) ((d & 0x60) >> 5)

enum GTW_brakeHWType_Values { 
  BREMBO_P42_MANDO_43MOC = 0,
  BREMBO_LARGE_P42_BREMBO_44MOC = 1,
  BREMBO_LARGE_P42_MANDO_43MOC = 2,
  BREMBO_LARGE_P42_BREMBO_LARGE_44MOC = 3
 };

#define Parse_GTW_frontSeatType(d) ((d & 0x70) >> 4)

enum GTW_frontSeatType_Values { 
  BASE_TESLA = 0,
  PREMIUM_TESLA = 1,
  PREMIUM_L_YANFENG_R_TESLA = 2,
  PREMIUM_L_TESLA_R_YANFENG = 3,
  PREMIUM_YANFENG = 4
 };

#define Parse_GTW_roofGlassType(d) ((d & 0x8) >> 3)

enum GTW_roofGlassType_Values { 
  TSA3_PET = 0,
  TSA5_NOPET = 1
 };

#define Parse_GTW_twelveVBatteryType(d) ((d & 0x2) >> 1)

enum GTW_twelveVBatteryType_Values { 
  ATLASBX_B24_FLOODED = 0,
  CLARIOS_B24_FLOODED = 1
 };

#define ID332BattBrickMinMax 0x332

#define Parse_BattBrickMultiplexer332(d) ((d & 0xc) >> 2)
#define Parse_BattBrickempMaxNum332(d) ((d & 0x3c) >> 2)
#define Parse_BattBrickVoltageMax332(d) ((d & 0x3ffc00) >> 10)
#define Parse_BattBrickTempMinNum332(d) ((d & 0xf0000000000) >> 40)
#define Parse_BattBrickTempMax332(d) ((d & 0xff00000000) >> 32)
#define Parse_BattBrickVoltageMin332(d) ((d & 0xfff00000000) >> 32)
#define Parse_BattBrickTempMin332(d) ((d & 0xff000000) >> 24)
#define Parse_BattBrickModelTMax332(d) ((d & 0xff0000) >> 16)
#define Parse_BattBrickVoltageMaxNum332(d) ((d & 0x7f0000) >> 16)
#define Parse_BattBrickModelTMin332(d) ((d & 0xff00) >> 8)
#define Parse_BattBrickVoltageMinNum332(d) ((d & 0x7f00) >> 8)
#define ID401BrickVoltages 0x401

#define Parse_MultiplexSelector(d) ((d & 0xff00) >> 8)
#define Parse_StatusFlags(d) ((d & 0xff00000000000000) >> 56)
#define Parse_Brick0(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick3(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick6(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick9(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick12(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick15(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick18(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick21(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick24(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick27(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick30(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick34(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick36(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick39(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick42(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick45(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick48(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick51(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick54(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick57(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick60(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick63(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick66(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick69(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick72(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick75(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick78(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick81(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick84(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick87(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick90(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick93(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick96(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick99(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick102(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick105(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick108(d) ((d & 0xffff000000000000) >> 48)
#define Parse_Brick1(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick4(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick7(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick10(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick13(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick16(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick19(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick22(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick25(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick28(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick31(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick33(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick37(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick40(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick43(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick46(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick49(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick52(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick55(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick58(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick61(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick64(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick67(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick70(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick73(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick76(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick79(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick82(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick85(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick88(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick91(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick94(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick97(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick100(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick103(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick106(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick109(d) ((d & 0xffff00000000) >> 32)
#define Parse_Brick2(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick5(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick8(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick11(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick14(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick17(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick20(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick23(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick26(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick29(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick32(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick35(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick38(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick41(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick44(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick47(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick50(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick53(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick56(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick59(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick62(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick65(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick68(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick71(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick74(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick77(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick80(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick83(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick86(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick89(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick92(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick95(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick98(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick101(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick104(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick107(d) ((d & 0xffff0000) >> 16)
#define Parse_Brick110(d) ((d & 0xffff0000) >> 16)
