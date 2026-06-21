#include "bootloader.h"
#include "bl_can.h"
#include "bl_config.h"
#include "bl_indicator.h"
#include "bl_jump.h"
#include "bl_target.h"

int bl_should_enter_update(void)
{
    if (bl_config_update_requested()) {
        return 1;
    }
    if (!bl_config_app_valid()) {
        return 1;
    }
    if (bl_config_dfu_requested() || bl_config_dfu_gpio_asserted()) {
        return 1;
    }
    return 0;
}

int main(void)
{
    bl_config_init();

    if (bl_config_dfu_requested() || bl_config_dfu_gpio_asserted()) {
        bl_config_set_dfu_requested(0);
        bl_enter_rom_dfu();
    }

    if (!bl_should_enter_update()) {
        bl_jump_to_app(BL_APP_VECTOR_BASE);
    }

    bl_can_init();

    /*
     * Breathe when we are resident by request or after a fault; hard-blink when
     * we are resident only because there is no valid app (fresh/invalid image).
     * A fault sets UPDATE too, so update_requested() also covers the fault case.
     */
    bl_indicator_set((bl_config_update_requested() || bl_config_fault_entry())
                         ? BL_INDICATOR_BREATHE
                         : BL_INDICATOR_BLINK);

    for (;;) {
        bl_can_run();
        bl_indicator_poll();
    }
}
