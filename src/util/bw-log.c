#include "bw-log.h"
#include <stdarg.h>
#include <stdio.h>

void _bw_log(const char* s, ...) {
    va_list ap;
    va_start(ap, s);
    vfprintf(stderr, s, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    fflush(stderr);
}
