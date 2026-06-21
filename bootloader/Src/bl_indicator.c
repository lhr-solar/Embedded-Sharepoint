#include "bl_indicator.h"

#include "stm32g4xx.h"

/* CMSIS exposes __WEAK; the lowercase __weak HAL spelling is not on the BL path. */
#ifndef BL_WEAK
#define BL_WEAK __attribute__((weak))
#endif

/* Pattern timing, in microseconds. */
#define BL_IND_BREATHE_US 3000000UL /* full dim -> bright -> dim cycle */
#define BL_IND_BLINK_US   200000UL  /* on+off period (~5 Hz hard blink) */
#define BL_IND_PWM_STEP_US 20UL     /* software-PWM tick (~195 Hz over 256 steps) */

static bl_indicator_pattern_t s_pattern = BL_INDICATOR_OFF;

BL_WEAK void bl_indicator_led_init(void) {}

BL_WEAK void bl_indicator_led_set(uint8_t on) {
    (void)on;
}

/*
 * Free-running microsecond clock from the DWT cycle counter. The BL runs on the
 * 16 MHz HSI; SystemCoreClock guards against a different reset clock. The 32-bit
 * counter wraps about every 268 s at 16 MHz, which only nudges the pattern phase.
 */
static uint32_t bl_indicator_now_us(void) {
    uint32_t hz = SystemCoreClock ? SystemCoreClock : 16000000UL;
    return DWT->CYCCNT / (hz / 1000000UL);
}

/* Default renderer: stateless software PWM of a binary LED, sampled from time. */
BL_WEAK void bl_indicator_render(uint8_t brightness) {
    uint32_t saw = (bl_indicator_now_us() / BL_IND_PWM_STEP_US) & 0xFFU;
    bl_indicator_led_set(brightness > (uint8_t)saw ? 1U : 0U);
}

static void bl_indicator_time_init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0U;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void bl_indicator_set(bl_indicator_pattern_t pattern) {
    s_pattern = pattern;
    bl_indicator_time_init();
    bl_indicator_led_init();
}

/* Gamma-corrected triangle so the ramp reads as a linear "breath" to the eye. */
static uint8_t bl_indicator_breathe(uint32_t t_us) {
    uint32_t phase = t_us % BL_IND_BREATHE_US;
    uint32_t half = BL_IND_BREATHE_US / 2U;
    uint32_t ramp = (phase < half) ? phase : (BL_IND_BREATHE_US - phase);
    uint32_t level = (ramp * 255U) / half;
    return (uint8_t)((level * level) / 255U);
}

void bl_indicator_poll(void) {
    uint32_t t;
    uint8_t brightness;

    if (s_pattern == BL_INDICATOR_OFF) {
        return;
    }

    t = bl_indicator_now_us();
    if (s_pattern == BL_INDICATOR_BREATHE) {
        brightness = bl_indicator_breathe(t);
    } else {
        brightness = ((t % BL_IND_BLINK_US) < (BL_IND_BLINK_US / 2U)) ? 255U : 0U;
    }
    bl_indicator_render(brightness);
}
