#ifndef BW_LOG_H
#define BW_LOG_H

#include <stdarg.h>
#ifdef __cplusplus
extern "C" {
#endif

[[gnu::format(printf, 1, 2)]]
void _bw_log(const char* s, ...);
[[gnu::format(printf, 1, 2)]]
void _bw_print(const char* s, ...);

#ifdef __cplusplus
}
#endif

//Log
#if defined BW_LOG
# define BW_LOG_GEN(fmt, ...) \
    _bw_log("[LOG] " fmt, ##__VA_ARGS__)
# define BW_LOG_ERR(fmt, ...) \
    _bw_log("[ERROR] " fmt, ##__VA_ARGS__)
# define BW_LOG_FUNC(fmt, ...) \
    _bw_log("[FUNC] " fmt, ##__VA_ARGS__)
# define BW_PRINT(fmt, ...) \
    _bw_print("" fmt, ##__VA_ARGS__)
# define BW_LOG_CUSTOM(specifier, fmt, ...) \
    _bw_print("[" specifier "] " fmt, ##__VA_ARGS__)
#else 
# define BW_LOG_GEN(fmt, ...)
# define BW_LOG_ERR(fmt, ...)
# define BW_LOG_FUNCTION(fmt, ...)
# define BW_PRINT(fmt, ...)
# define BW_LOG_CUSTOM(specifier, fmt, ...)
#endif

#endif //BW_LOG_H
