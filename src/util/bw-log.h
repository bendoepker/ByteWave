#ifndef BW_LOG_H
#define BW_LOG_H

/* NOTE: Unreadable log messages will be written when multiple threads are writing to
*        stdout / stderr at the same time, use with caution on multithreaded functions
*/

#include <stdarg.h>

void _bw_log(const char* s, ...);

//General Log Statements
#if defined BW_LOG || defined BW_LOG_GEN
#define BW_LOG_GEN(s) \
    _bw_log("[LOG] %s\n", s)
#else 
#define BW_LOG_GEN
#endif

//Error Log Statements
#if defined BW_LOG || defined BW_LOG_ERRORS
#define BW_LOG_ERROR(s) \
    _bw_log("[ERROR] %s\n", s)
#else
#define BW_LOG_ERROR(s)
#endif

//Function Log Statements
#if defined BW_LOG || defined BW_LOG_FUNCTIONS
#define BW_LOG_FUNCTION(s) \
    _bw_log("[LOG FUNC] Called Function: %s\n", s)
#else
#define BW_LOG_FUNCTION(s)
#endif

#endif //BW_LOG_H
