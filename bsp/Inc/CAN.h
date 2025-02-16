#ifndef CAN_H
#define CAN_H
#ifndef CAN_UNDEFINED

#include "stm32xx_hal.h"

/* Function Descriptions:
 * Init initializes the CAN peripheral given the configuration
 (does not start the CAN peripheral)
 * DeInit deinitlaizes the CAN peripheral meaning the configuration is
 reset (default values)
 * Start starts the CAN peripheral meaning it is able to interrupt
 and act on messages
 * Stop stops the CAN peripheral meaning the configuration is unchanged
 but the peripheral cannot interrupt or act on messages
 * Send readies the message to be sent when the CAN peripheral has
 a chance to send it (puts it into a queue)
 * Recv reads a message (reads from a queue depending on ID)
 *
 * How to Use This Driver:
 * Init to initialize the CAN peripheral
 * Start to activate the CAN peripheral
 * Send, Recv, Stop, Start
 * DeInit to deinitialize
 * Init ...
 *
 * Notes for usage:
 * CAN2 is usually a slave CAN, meaning that it shares
 transistors with CAN1, meaning for CAN2 to work, CAN1
 must be initialized. Thus, make sure CAN1 is initialized
 before CAN2 is initialized.
 * CAN2 usually shares the filter banks with CAN1, since it
 is a slave CAN. Thus, consider the filter configs you pass
 to the can_init function, when initializing CAN1 and CAN2.
 * CAN3 is usually a master CAN, meaning you don't have to 
 worry about sharing.
 * This driver uses a macro to define which entries a CAN
 interface should recieve on. Look at can1_recv_entries.h
 for an example. Make sure these entries line up with the
 filter config.
 */

// return code for can driver
typedef enum {
    CAN_ERR,   // unsuccessful operation
    CAN_OK,
    CAN_SENT,  // successful send
    CAN_RECV,  // successful recieve
    CAN_EMPTY, // recieved nothing with no errors
} can_status_t;

// can handlers
#ifdef CAN1
extern CAN_HandleTypeDef* hcan1;
#endif /* CAN1 */

#ifdef CAN2
extern CAN_HandleTypeDef* hcan2;
#endif /* CAN2 */

#ifdef CAN3
extern CAN_HandleTypeDef* hcan3;
#endif /* CAN3 */

can_status_t can_init(CAN_HandleTypeDef* handle, CAN_FilterTypeDef* filter);
can_status_t can_deinit(CAN_HandleTypeDef* handle);
can_status_t can_start(CAN_HandleTypeDef* handle);
can_status_t can_stop(CAN_HandleTypeDef* handle);
can_status_t can_send(CAN_HandleTypeDef* handle, const CAN_TxHeaderTypeDef* header, const uint8_t data[], TickType_t delay_ticks);
can_status_t can_recv(CAN_HandleTypeDef* handle, uint16_t id, CAN_RxHeaderTypeDef* header, uint8_t data[], TickType_t delay_ticks);

#endif /* CAN_UNDEFINED */
#endif /* CAN_H */
