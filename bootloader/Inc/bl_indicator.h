#ifndef BL_INDICATOR_H
#define BL_INDICATOR_H

#include <stdint.h>

/*
 * Resident-bootloader status LED. The pattern says *why* the BL is resident:
 *
 *   BL_INDICATOR_BREATHE  smooth pulse — entered on request (host ENTER, or the
 *                                        app called BootloaderRequestUpdate) or
 *                                        recovered from a fault
 *                                        (BootloaderRebootFromFault).
 *   BL_INDICATOR_BLINK    hard blink   — no valid app to run: invalid-image
 *                                        fallback, or a fresh/blank MCU.
 *
 * The pattern engine is board-agnostic; the physical LED is user-defined. A board
 * provides the LED by giving bl_indicator_led_set() (and usually
 * bl_indicator_led_init()) a strong definition in its resident-BL build; the weak
 * defaults here are no-ops, so a board without a status LED builds and runs with
 * the feature inert. See docs/Bootloader.md.
 */
typedef enum {
    BL_INDICATOR_OFF = 0,
    BL_INDICATOR_BREATHE,
    BL_INDICATOR_BLINK,
} bl_indicator_pattern_t;

/* Select the pattern and initialise the LED + time base. Call once. */
void bl_indicator_set(bl_indicator_pattern_t pattern);

/* Advance the pattern; call as often as possible from the BL main loop. */
void bl_indicator_poll(void);

/*
 * User-defined hooks (weak no-op defaults). Common case: override these two.
 *   bl_indicator_led_init — configure the LED GPIO (called by bl_indicator_set).
 *   bl_indicator_led_set  — drive the LED: on != 0 lit, 0 dark. The engine
 *                           software-PWMs this to render the breathe ramp, so a
 *                           plain GPIO LED breathes with no timer needed.
 *
 * Advanced: a board with a PWM-capable LED can override bl_indicator_render() to
 * receive 0..255 brightness and drive a hardware PWM compare directly (then
 * bl_indicator_led_set is unused).
 */
void bl_indicator_led_init(void);
void bl_indicator_led_set(uint8_t on);
void bl_indicator_render(uint8_t brightness);

#endif /* BL_INDICATOR_H */
