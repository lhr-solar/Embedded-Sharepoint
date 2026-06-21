#include <stdint.h>

#include "bootloader_hooks.h"

/* Build/link test only: firmware is cross-compiled, never executed on host. */
int main(void)
{
    static const uint8_t dummy[2] = {0U, 0U};

    volatile int sink = 0;

    /* Non-matching id -> returns 0; no reset path is taken semantically. */
    sink += BootloaderHandleCanFrame(0x000U, dummy, 2U, 5U);
    sink += BootloaderShouldEnter();

    /* Force linkage of the reset-path entry points without invoking them. */
    void (*volatile reset_fn)(void) = BootloaderRequestUpdate;
    reset_fn = EnterRomDfu;

    (void)reset_fn;
    (void)sink;

    return 0;
}
