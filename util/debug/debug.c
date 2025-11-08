#include "debug.h"

#include <stdarg.h>
#include <stdio.h>

#ifndef NDEBUG

static FILE *log_file = NULL;

void dbg_set_log_file(FILE *fp)
{
    log_file = fp;
}

void dbg_log(const char *fmt, ...)
{
    if (!log_file) log_file = stderr;
    va_list args;
    va_start(args, fmt);
    vfprintf(log_file, fmt, args);
    va_end(args);
}

#else

void dbg_set_log_file(FILE *fp) {}
void dbg_printf(const char *fmt, ...) {}

#endif
