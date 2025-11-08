#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

void dbg_set_log_file(FILE *fp);
void dbg_log(const char *fmt, ...);

#endif //DEBUG_H
