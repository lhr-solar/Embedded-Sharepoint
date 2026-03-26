#include "can_msgs.h"
// CAN_RECV_ENTRY(ID, SIZE, CIRCULAR)

CAN_RECV_ENTRY(0x001, 5, false)
CAN_RECV_ENTRY(0x002, 8, false)
CAN_RECV_ENTRY(0x003, 3, false)
<<<<<<< HEAD
CAN_RECV_ENTRY(0x321, 4, true)
=======
CAN_RECV_ENTRY(0x004, 4, true)
CAN_RECV_ENTRY(BPS_FAULT_ID, 4, true)
>>>>>>> main
