#ifndef BW_LOG_H
#define BW_LOG_H

/* NOTE: Unreadable log messages will be written when multiple threads are writing to
*        stdout / stderr at the same time, use with caution on multithreaded functions
*/

#include <stdarg.h>

void _bw_log(const char* s, ...);

//General Log Statements
#if defined BW_LOG || defined BW_LOG_GEN
#define BW_LOG_GEN(fmt, ...) \
    _bw_log("[LOG] " fmt, __VA_ARGS__)
#else 
#define BW_LOG_GEN
#endif

//Error Log Statements
#if defined BW_LOG || defined BW_LOG_ERRORS
#define BW_LOG_ERR(fmt, ...) \
    _bw_log("[ERROR] " fmt, __VA_ARGS__)
#else
#define BW_LOG_ERROR(s)
#endif

//Function Log Statements
#if defined BW_LOG || defined BW_LOG_FUNCTIONS
#define BW_LOG_FUNC(fmt, ...) \
    _bw_log("[FUNC] " fmt, __VA_ARGS__)
#else
#define BW_LOG_FUNCTION(s)
#endif

//#define LOG_TEST(fmt, ...) \
//  _bw_log("[TEST] " fmt, __VA_ARGS__)

#endif //BW_LOG_H
