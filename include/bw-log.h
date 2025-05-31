#ifndef BW_LOG_H
#define BW_LOG_H

/* NOTE: Unreadable log messages will be written when multiple threads are writing to
*        stdout / stderr at the same time, use with caution on multithreaded functions
*/

#include <stdarg.h>
#ifdef __cplusplus
extern "C" {
#endif

void _bw_log(const char* s, ...);
void _bw_print(const char* s, ...);

#ifdef __cplusplus
}
#endif

//General Log Statements
#if defined BW_LOG || defined BW_LOG_GEN
#define BW_LOG_GEN(fmt, ...) \
    _bw_log("[LOG] " fmt, ##__VA_ARGS__)
#else 
#define BW_LOG_GEN(fmt, ...)
#endif

//Error Log Statements
#if defined BW_LOG || defined BW_LOG_ERRORS
#define BW_LOG_ERR(fmt, ...) \
    _bw_log("[ERROR] " fmt, ##__VA_ARGS__)
#else
#define BW_LOG_ERR(fmt, ...)
#endif

//Function Log Statements
#if defined BW_LOG || defined BW_LOG_FUNCTIONS
#define BW_LOG_FUNC(fmt, ...) \
    _bw_log("[FUNC] " fmt, ##__VA_ARGS__)
#else
#define BW_LOG_FUNCTION(fmt, ...)
#endif

//BW_PRINT does not print any [identifier]s and prints to stdout rather than stderr
#if defined BW_LOG
#define BW_PRINT(fmt, ...) \
    _bw_print("" fmt, ##__VA_ARGS__)
#else
#define BW_PRINT(fmt, ...)
#endif

#if defined BW_LOG
#define BW_LOG_CUSTOM(specifier, fmt, ...) \
    _bw_print("[" specifier "] " fmt, ##__VA_ARGS__)
#else
#define BW_LOG_CUSTOM(specifier, fmt, ...)
#endif

#endif //BW_LOG_H
