#ifndef COMPILER_H
#define COMPILER_H

#if defined(__GNUC__) || defined(__clang__)

#define UNUSED              __attribute__((unused))
#define USED                __attribute__((used))
#define NOINLINE            __attribute__((noinline))
#define ALWAYS_INLINE       __attribute__((always_inline)) inline
#define PURE                __attribute__((pure))
#define CONST_FN            __attribute__((const))
#define MALLOC_FN           __attribute__((malloc))
#define HOT                 __attribute__((hot))
#define COLD                __attribute__((cold))
#define LIKELY(exp)         __builtin_expect(!!(exp), 1)
#define UNLIKELY(exp)       __builtin_expect(!!(exp), 0)
#define PACKED              __attribute__((packed))
#define ALIGNED(n)          __attribute__((aligned(n)))
#define HIDDEN              __attribute__((visibility("hidden")))
#define EXPORT              __attribute__((visibility("default")))
#define DEPRECATED(msg)     __attribute__((deprecated(msg)))
#define FALLTHROUGH         __attribute__((fallthrough))
#define FORMAT(archetype, fmt_idx, first_arg) \
                            __attribute__((format(archetype, fmt_idx, first_arg)))
#define SECTION(name)       __attribute__((section(name)))
#define INTERRUPT           __attribute__((interrupt))

#elif defined(_MSC_VER)

#define UNUSED              __pragma(warning(suppress:4100 4101))
#define USED
#define NOINLINE            __declspec(noinline)
#define ALWAYS_INLINE       __forceinline
#define PURE
#define CONST_FN
#define MALLOC_FN
#define HOT
#define COLD
#define LIKELY(exp)         (exp)
#define UNLIKELY(exp)       (exp)
#define PACKED
#define ALIGNED(n)          __declspec(align(n))
#define HIDDEN
#define EXPORT              __declspec(dllexport)
#define DEPRECATED(msg)     __declspec(deprecated(msg))
#define FALLTHROUGH
#define FORMAT(archetype, fmt_idx, first_arg)
#define SECTION(name)       __pragma(data_seg(name))
#define INTERRUPT           __declspec(naked)

#else

#define UNUSED
#define USED
#define NOINLINE
#define ALWAYS_INLINE       inline
#define PURE
#define CONST_FN
#define MALLOC_FN
#define HOT
#define COLD
#define LIKELY(exp)         (exp)
#define UNLIKELY(exp)       (exp)
#define PACKED
#define ALIGNED(n)
#define HIDDEN
#define EXPORT
#define DEPRECATED(msg)
#define FALLTHROUGH
#define FORMAT(archetype, fmt_idx, first_arg)
#define SECTION(name)
#define INTERRUPT

#endif


#endif //COMPILER_H
