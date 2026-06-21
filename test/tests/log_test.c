// Build test for the leveled, ANSI-colored log() macro (driver/Inc/log.h).
// Firmware is cross-compiled, not executed, so this only needs to compile and
// link cleanly on every target. It exercises the variadic forwarding path
// (with and without trailing args) across several levels.

#define LOGGING_ENABLE
#define LOGGING_LEVEL L_TRACE
#include "log.h"

#include "stm32xx_hal.h"

int main(void) {
    HAL_Init();

    log(L_FATAL, "fatal: code=%d", 1);
    log(L_ERROR, "error: %s=%d", "rc", -7);
    log(L_WARN, "warn: %u%%", 80u);
    log(L_INFO, "x=%d", 5);
    log(L_DEBUG, "debug: %s @ %d", "here", 42);
    log(L_TRACE, "trace with no variadic args");  // exercises ##__VA_ARGS__ comma elision

    while (1) {
    }
}
