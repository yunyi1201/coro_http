#pragma once
#include <assert.h>

/* debug */
#define Assert(cond, format, ...)                                              \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fprintf(stderr, "[%s:%d %s]" format "\n", __FILE__, __LINE__, __func__,  \
              ##__VA_ARGS__);                                                  \
      assert(cond);                                                            \
    }                                                                          \
  } while (0)

#define panic(format, ...) Assert(0, format, ##__VA_ARGS__)
#define TODO() panic("please implement me")
#define Unreachable() Assert(0, "there are an bug in code, please debug it")

/* Branch predictor */
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

/* Alignment */
#define MEM_ALIGN sizeof(unsigned long)

#define ALIGN_MASK(x, mask) (((x) + (mask)) & ~(mask))
#define ALIGN(x, a) ALIGN_MASK(x, (typeof(x))(a)-1)
#define ROUND_UP(x, y) ((((x) + ((y)-1)) / (y)) * (y))

/* Align the variable or type to at least x bytes.
 * x must be a power of two.
 */
#define __ALIGNED(x) __attribute__((aligned(x)))

/* GNU extensions */
#define __INIT __attribute__((constructor))
#define __UNUSED __attribute__((unused))
#define __CONST __attribute__((const))

#if defined(__i386__) || defined(__x86_64__)
#define ATTRIBUTE_REGPARM(n) __attribute__((regparm((n))))
#else
#define ATTRIBUTE_REGPARM(n)
#endif

#if __has_attribute(__fallthrough__)
#define fallthrough __attribute__((__fallthrough__))
#else
#define fallthrough                                                            \
  do {                                                                         \
  } while (0) /* fallthrough */
#endif

/* Misc */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
