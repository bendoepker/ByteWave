#ifndef BW_TYPES_H
#define BW_TYPES_H

typedef enum BWError {
    BW_OK = 0,
    BW_FAILED = 1,
    BW_UNINITIALIZED = 2,
    BW_WINDOWS_ERROR = 3,
    BW_FAILED_MALLOC = 4,
} BWError;

typedef enum {
    BW_FALSE = 0,
    BW_TRUE = 1,
} BWBool;

#endif /* BW_TYPES_H */
