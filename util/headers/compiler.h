#ifndef COMPILER_H
#define COMPILER_H

#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#define LIKELY(exp) __builtin_expect(!!(exp), 1)
#define UNLIKELY(exp) __builtin_expect(!!(exp), 0)
#define ALWAYS_INLINE __attribute__((always_inline))
#else
#define UNUSED
#define LIKELY(exp) (exp)
#define UNLIKELY(exp) (exp)
#define ALWAYS_INLINE
#endif

#endif //COMPILER_H
