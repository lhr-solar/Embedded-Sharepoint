#include "bl_status.h"
#include "bl_can.h"

void bl_status_fault(uint8_t code)
{
    uint8_t frame[8] = {code, 0, 0, 0, 0, 0, 0, 0};
    (void)bl_can_send_status(frame, 1U);
}
