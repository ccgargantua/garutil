#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>
#include <stdlib.h>


#ifdef ASSERT
    #undef ASSERT
#endif


#ifdef DEBUG
    #define FAIL_PRINT(msg) (void)(0)
    #define ASSERT(condition, msg) (void)(0)
#else
    #define FAIL_PRINT(msg) fprintf(stderr, "FAILED ASSERTION: \"%s\" %s:%d\n", msg, __FILE__, __LINE__)
    #define ASSERT(condition, msg) \
    do                             \
    {                              \
        if(!(condition))           \
        {                          \
            FAIL_PRINT(msg);       \
            abort();               \
        }                          \
    }while(0)
#endif


#define ASSERT_EQUAL(a,b)     ASSERT(a == b,    #a " != " #b)
#define ASSERT_NOT_EQUAL(a,b) ASSERT(a != b,    #a " == " #b)
#define ASSERT_NULL(a)        ASSERT(a == NULL, #a " is not NULL")
#define ASSERT_NOT_NULL(a)    ASSERT(a != NULL, #a " is NULL")
#define ASSERT_ZERO(a)        ASSERT(a == 0,    #a " is not 0")
#define ASSERT_NOT_ZERO(a)    ASSERT(a != 0,    #a " is 0")
#define ASSERT_TRUE(a)        ASSERT(a,         #a " is false (0)")
#define ASSERT_FALSE(a)       ASSERT(!a,        #a " is true (!0)")
#define ASSERT_UNREACHABLE    ASSERT(0, "Unreachable code reached.")


#endif