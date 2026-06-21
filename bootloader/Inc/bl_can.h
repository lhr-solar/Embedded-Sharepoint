#ifndef BL_CAN_H
#define BL_CAN_H

#include <stdint.h>

void bl_can_init(void);
void bl_can_run(void);
int bl_can_send_resp(uint8_t code, const uint8_t *extra, uint8_t extra_len);
int bl_can_send_status(const uint8_t *payload, uint8_t len);

#endif /* BL_CAN_H */
