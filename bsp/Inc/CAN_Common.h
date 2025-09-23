#pragma once

// 8 for now unless extended payload is supported
#define CAN_DATA_SIZE (8)

// return code for can driver
typedef enum {
    CAN_ERR,   // unsuccessful operation
    CAN_OK,
    CAN_SENT,  // successful send
    CAN_RECV,  // successful recieve
    CAN_EMPTY, // recieved nothing with no errors
} can_status_t;