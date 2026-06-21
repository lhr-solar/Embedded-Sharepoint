#include "log.h"
#include "stm32xx_hal.h"  // FreeRTOS pcTaskGetName()

#define LOG_COLOR_RESET "\x1b[0m"

static const char *log_color(int level) {
    switch (level) {
    case L_FATAL: return "\x1b[1;37;41m";  // bold white on red
    case L_ERROR: return "\x1b[31m";       // red
    case L_WARN:  return "\x1b[33m";       // yellow
    case L_INFO:  return "\x1b[32m";       // green
    case L_DEBUG: return "\x1b[36m";       // cyan
    case L_TRACE: return "\x1b[90m";       // bright black (gray)
    default:      return LOG_COLOR_RESET;
    }
}

static const char *log_name(int level) {
    switch (level) {
    case L_FATAL: return "FATAL";
    case L_ERROR: return "ERROR";
    case L_WARN:  return "WARN";
    case L_INFO:  return "INFO";
    case L_DEBUG: return "DEBUG";
    case L_TRACE: return "TRACE";
    default:      return "NONE";
    }
}

// msg is already-formatted user text; it is passed as a %s argument (not as a
// format string) so embedded '%' characters are safe.
void log_emit(int level, const char *msg) {
    printf("%s[%s][%s] %s" LOG_COLOR_RESET "\n",
           log_color(level), log_name(level), pcTaskGetName(NULL), msg);
}
