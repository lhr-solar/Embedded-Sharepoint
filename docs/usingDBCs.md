# Integrating CAN message into your code

## Generating CAN message headers

We have a python script located in the `can/` folder called `generate_can_headers.py`. This script takes a DBC file, and turns it into a set of macros, structs, and enums.

Usage guide:
```sh
python [path to python script]/generate_can_headers.py [path to dbc file]
```

Usage example (called from the root directory of Embedded-Sharepoint):
```sh
python can/generate_can_headers.py can/dbc/Mcqueen/CarCAN.dbc
```

## What's included in the header
The generated header file includes macros for every CAN_ID
```c
/* ================= CAN ID Macros ================= */

#define CAN_ID_VCU_STATUS 0x10
#define CAN_ID_CONTROLS_STATUS 0x15
```

The generated header file includes macros for every CAN message size. This is a useful field to use when creating CAN transmit or CAN recieve header files. 
```c
/* ================= CAN Length Macros ================= */

#define CAN_DLC_VCU_STATUS 3
#define CAN_DLC_CONTROLS_STATUS 4
```

The generated header file includes enums for every CAN value tables, this is useful for encoding and decoding CAN messages without using magic numbers. 

```c
/* ================= Value Table Enums ================= */

typedef enum {
    VCU_STATUS_VCU_FAULT_MOTOR_HV_UNDERVOLTAGE = 6,
    VCU_STATUS_VCU_FAULT_MOTOR_HV_OVERVOLTAGE = 5,
    VCU_STATUS_VCU_FAULT_MOTOR_CONTROLLER_FAULT = 4,
    VCU_STATUS_VCU_FAULT_MOTOR_PRECHARGE_TIMEOUT = 3,
    VCU_STATUS_VCU_FAULT_MOTOR_PCHG_CONTACTOR_SENSE = 2,
    VCU_STATUS_VCU_FAULT_MOTOR_CONTACTOR_SENSE = 1,
    VCU_STATUS_VCU_FAULT_NO_FAULT = 0,
} vcu_status_vcu_fault_e;
```

The generated header file includes structs for every CAN message, this is useful for storing CAN data in a more readable format than an array of bytes.
```c
/* ================= Message Structs ================= */

typedef struct {
    uint8_t VCU_Fault;
    uint8_t Motor_Contactor_State;
    uint8_t Motor_Precharge_Contactor_State;
    uint8_t Motor_Ready_To_Drive;
    uint8_t VCU_Driver_Input_OK;
    uint8_t VCU_Pedals_OK;
    uint8_t VCU_Regen_OK;
    uint8_t VCU_Regen_Active;
    uint8_t VCU_FSM_State;
} vcu_status_t;
```

## Writting packing functions
To maintain more modular code, it's good practice to write a function to "pack" a can message struct into an array of bytes. For example, 
```c

// encodes a drive command struct into an array of bytes for can_send
static void packDriveCommand(mc_drivecommand_t motorDriveCommand, uint8_t tx_data[8]){
    memcpy(&tx_data[4], &(motorDriveCommand.MC_MotorCurrentSetpoint), sizeof(float));
    memcpy(&tx_data[0], &(motorDriveCommand.MC_MotorVelocitySetpoint), sizeof(float));
}
```
Data from the `motorDriveCommand` can message struct is packed into the `tx_data` array of bytes to send over CAN. 

This reverse can also be done on a recieved can message to "unpack" an array of bytes into a can message struct.