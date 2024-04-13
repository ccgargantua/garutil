#ifndef TEST_H
#define TEST_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int passed_tests = 0;
int total_tests = 0;
int temp_passed;
int temp_total;


#define BUFFER_SIZE 4096
char buffer[BUFFER_SIZE];
int buffer_index = 0;


#define REPORT(msg)                                        \
    buffer_index += snprintf(buffer + buffer_index,        \
                             BUFFER_SIZE - buffer_index,   \
                             "  FAILURE: '%s' at %s:%d\n", \
                             msg, __FILE__, __LINE__)


#define REPORT_FATAL(msg)                                        \
    buffer_index += snprintf(buffer + buffer_index,              \
                             BUFFER_SIZE - buffer_index,         \
                             "  FATAL FAILURE: '%s' at %s:%d\n", \
                             msg, __FILE__, __LINE__)


#define TEST(exp, msg)      \
    do                      \
    {                       \
        if(!(exp))          \
        {                   \
            REPORT(msg);    \
        }                   \
        else                \
        {                   \
            passed_tests++; \
        }                   \
        total_tests++;      \
    }while(0)


#define TEST_FATAL(exp, msg) do{if(!(exp)){REPORT_FATAL(msg);return;}}while(0)


#define TEST_NULL(a)                TEST(a == NULL,         #a " is not NULL")
#define TEST_NOT_NULL(a)            TEST(a != NULL,         #a " is NULL")
#define TEST_EQUAL(a, b)            TEST(a == b,            #a " does not equal " #b)


#define TEST_ARRAY_EQUAL(a, b, s)                 \
    do                                            \
    {                                             \
        int i;                                    \
        total_tests++;                            \
        for(i = 0; i < s; i++)                    \
        {                                         \
            if(a[i] != b[i])                      \
            {                                     \
                REPORT(#a " does not equal " #b); \
                break;                            \
            }                                     \
        }                                         \
        passed_tests++;                           \
    }while(0)


#define SUITE(suite, name)                              \
    do                                                  \
    {                                                   \
        temp_passed = passed_tests;                     \
        temp_total = total_tests;                       \
        suite();                                        \
        fprintf(stderr, "Passed %d/%d tests in '%s'\n", \
                passed_tests-temp_passed,               \
                total_tests-temp_total, name);          \
        fprintf(stderr, "%s", buffer);                  \
        buffer_index = 0;                               \
        buffer[0] = '\0';                               \
    } while(0)


#endif