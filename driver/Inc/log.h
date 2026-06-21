#pragma once

// Leveled, ANSI-colored logging layered on the existing nanoprintf backend
// (driver/Src/printf.c). APP-only: log_emit() uses FreeRTOS pcTaskGetName().
//
// Usage:
//   #define LOGGING_ENABLE          // turn logging on for this translation unit
//   #define LOGGING_LEVEL L_DEBUG   // optional; defaults to L_INFO
//   #include "log.h"
//   log(L_INFO, "x=%d", x);
//
// When LOGGING_ENABLE is undefined, log() is a no-op that does not evaluate its
// arguments. When defined, a message is emitted iff LEVEL <= LOGGING_LEVEL.

#include "printf.h"  // snprintf()/printf() backend

// Size of the per-call buffer used to format the user message before it is
// decorated and handed to printf(). Override before including if needed.
// ponytail: macro-local buffer lives on the caller's stack; size it (or the
// task stack) up if log lines are longer than this.
#ifndef LOG_MSG_SIZE
#define LOG_MSG_SIZE 128
#endif

typedef enum {
    L_NONE  = 0,  // OFF sentinel: set LOGGING_LEVEL to L_NONE to silence all levels
    L_FATAL = 1,
    L_ERROR = 2,
    L_WARN  = 3,
    L_INFO  = 4,
    L_DEBUG = 5,
    L_TRACE = 6,
} log_level_t;

#ifndef LOGGING_LEVEL
#define LOGGING_LEVEL L_INFO
#endif

// Decorates msg with color + [LEVEL][taskname] + reset and prints one line.
void log_emit(int level, const char *msg);

#ifdef LOGGING_ENABLE
#define log(LEVEL, fmt, ...)                                            \
    do {                                                                \
        if ((LEVEL) <= (LOGGING_LEVEL)) {                               \
            char _log_msg[LOG_MSG_SIZE];                                \
            snprintf(_log_msg, sizeof(_log_msg), (fmt), ##__VA_ARGS__); \
            log_emit((LEVEL), _log_msg);                                \
        }                                                               \
    } while (0)
#else
#define log(LEVEL, fmt, ...) \
    do {                     \
    } while (0)
#endif
