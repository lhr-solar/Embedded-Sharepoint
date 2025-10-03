#include <stdbool.h>
#include "printf.h"

#ifndef LOGGING_ENABLE
#define LOGGING_ENABLE 0
#endif

#ifndef LOGGING_LEVEL
#define LOGGING_LEVEL 6
#endif

#define LOG_MAX_BEFORE_SIZE 64
#define LOG_MAX_LOG_SIZE    256

#define CONVERT_TO_STRING(arg) #arg
#define AS_STRING(arg)         CONVERT_TO_STRING(arg)

static char log_buf[LOG_MAX_LOG_SIZE];
static char log_before_buf[LOG_MAX_BEFORE_SIZE];

typedef enum {
    L_NONE  = 0,
    L_FATAL = 1,
    L_ERROR = 2,
    L_WARN  = 3,
    L_INFO  = 4,
    L_DEBUG = 5,
    L_TRACE = 6,
} LoggingLevel;

static inline const char *
log_level_name(LoggingLevel level)
{
    switch (level) {
    case L_TRACE: return "TRACE";
    case L_DEBUG: return "DEBUG";
    case L_INFO:  return "INFO";
    case L_WARN:  return "WARN";
    case L_ERROR: return "ERROR";
    case L_FATAL: return "FATAL";
    case L_NONE:  return "NONE";
    }
    /* Should be unreachable */
    return NULL;
}

static inline const char *
log_level_color(LoggingLevel level)
{
    switch (level) {
    case L_TRACE: return "\x1b[00;34m";
    case L_DEBUG: return "\x1b[00;35m";
    case L_INFO:  return "\x1b[00;36m";
    case L_WARN:  return "\x1b[00;33m";
    case L_ERROR: return "\x1b[00;31m";
    case L_FATAL: return "\x1b[37;41m";
    case L_NONE:  return "\x1b[0m";
    }
    /* Should be unreachable */
    return NULL;
}

static inline void
log_printf(void)
{
    printf("%-" AS_STRING(LOG_MAX_BEFORE_SIZE) "s | %s%s\n\r\r",
           (char *)&log_before_buf, (char *)&log_buf, log_level_color(L_NONE));
}

#if LOGGING_ENABLE == 0 || LOGGING_LEVEL == 0
#define log(LEVEL, fmt ...)
#else
#define log(LEVEL, fmt ...)                                         \
    do {                                                            \
        LoggingLevel level = LEVEL;                                 \
        if (LOGGING_LEVEL >= level) {                               \
            snprintf(log_before_buf, LOG_MAX_BEFORE_SIZE,           \
                     "%s%-5s | %s:%d:%s() ",                        \
                     log_level_color(level), log_level_name(level), \
                     __FILE__, __LINE__, __func__);                 \
            snprintf((char *)&log_buf, LOG_MAX_LOG_SIZE, fmt);      \
            log_printf();                                           \
        }                                                           \
    } while (0)
#endif
