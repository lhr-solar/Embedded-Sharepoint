#pragma once

#include <stdint.h>
#include <stddef.h>

/*
 * Convert CAN frame to SLCAN string.
 *
 * id        : 11-bit CAN ID
 * data      : pointer to payload
 * dlc       : payload length (0-8)
 * out       : output buffer
 * out_size  : size of output buffer
 *
 * returns number of bytes written, or -1 on error
 */

/**
 * @brief Convert CAN frame to SLCAN string.
 *
 * Activates the FDCAN peripheral so it can transmit and receive messages.
 *
 * @param id 11-bit CAN ID
 * @param data pointer to payload
 * @param dlc payload length (0-8)
 * @param out output buffer
 * @param out_size size of output buffer

 *
 * @return int Returns number of bytes written, or -1 on error
 */
int can_to_slcan(uint16_t id, const uint8_t *data, uint8_t dlc, char *out, uint8_t out_size);