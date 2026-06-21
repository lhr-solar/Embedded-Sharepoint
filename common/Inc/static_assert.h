#pragma once

/*
 * STATIC_ASSERT(expr, msg) -- fail the build (not the runtime) when expr is
 * false. Use it to lock invariants the compiler can check: struct/wire sizes,
 * buffer-vs-count relationships, enum ranges, alignment, config bounds.
 *
 * Prefers C11 _Static_assert; falls back to a negative-sized-array typedef on
 * older toolchains so the macro is always usable. Works at file or block scope.
 */

#if defined(__cplusplus)
#define STATIC_ASSERT(expr, msg) static_assert((expr), msg)
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#define STATIC_ASSERT(expr, msg) _Static_assert((expr), msg)
#else
#define STATIC_ASSERT_CONCAT_(a, b) a##b
#define STATIC_ASSERT_CONCAT(a, b) STATIC_ASSERT_CONCAT_(a, b)
#define STATIC_ASSERT(expr, msg)                  \
  typedef char STATIC_ASSERT_CONCAT(static_assertion_failed_at_line_, \
                                    __LINE__)[(expr) ? 1 : -1]
#endif
