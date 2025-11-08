#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>
#include <stdlib.h>

#ifdef NDEBUG
    #define assert_true(value)
    #define assert_false(value)
    #define assert_zero(value)
    #define assert_nonzero(value)
    #define assert_null(value)
    #define assert_nonnull(value)
    #define assert_positive(value)
    #define assert_nonnegative(value)
    #define assert_negative(value)
    #define assert_nonpositive(value)

    #define assert_unreachable()      __builtin_unreachable()

    #define assert_report_failure(message, file, line)

#else

    #define assert_true(value)        assert_impl((value),                 #value " is not true.",                    __FILE__, __LINE__)
    #define assert_false(value)       assert_impl(!(value),                #value " is not false.",                   __FILE__, __LINE__)
    #define assert_zero(value)        assert_impl(!(value),                #value " is not zero.",                    __FILE__, __LINE__)
    #define assert_nonzero(value)     assert_impl((value),                 #value " is not non-zero.",                __FILE__, __LINE__)
    #define assert_null(value)        assert_impl(!(value),                #value " is not NULL.",                    __FILE__, __LINE__)
    #define assert_nonnull(value)     assert_impl((value),                 #value " is not non-NULL.",                __FILE__, __LINE__)
    #define assert_positive(value)    assert_impl((value) > 0,             #value " is not positive.",                __FILE__, __LINE__)
    #define assert_nonpositive(value) assert_impl((value) <= 0,            #value " is not non-positive.",            __FILE__, __LINE__)
    #define assert_negative(value)    assert_impl((value) < 0,             #value " is not negative.",                __FILE__, __LINE__)
    #define assert_nonnegative(value) assert_impl((value) >= 0,            #value " is not non-negative.",            __FILE__, __LINE__)
    #define assert_unreachable()      assert_impl(0,                       "Unreachable execution path was reached.", __FILE__, __LINE__)
    #define not_implemented()         assert_impl(0,                       "This function has not been implemented.", __FILE__, __LINE__)

    #define assert_impl(assertion, message, file, line) \
        do { \
            if (!(assertion)) \
            assert_report_failure(message, file, line); \
        } while(0)

    #define assert_report_failure(message, file, line) \
      do { \
        fprintf(stderr, "Assertion failure: %s:%u: %s\n", (file), (line), (message)); \
        abort(); \
      } while (0)

#endif

#if !defined(assert)
    #define assert(assertion) assert_true(assertion)
#endif

#endif