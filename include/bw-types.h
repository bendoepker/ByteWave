#ifndef BW_TYPES_H
#define BW_TYPES_H
#include <stdint.h>

typedef enum BWError {
    BW_OK = 0,
    BW_FAILED,
    BW_UNINITIALIZED,
    BW_WINDOWS_ERROR,
    BW_FAILED_MALLOC,
    BW_BAD_PARAMS,
    BW_HARDWARE_MALFUNCTION,
} BWError;

typedef enum {
    BW_FALSE = 0,
    BW_TRUE = 1,
} BWBool;

//At the moment I will only be supporting these sample rates, this may
//change in the future, especially if the implementation is trivial
const static uint32_t BWSampleRates[] = {
    //8000,
    //11025,
    //16000,
    //22050,
    44100,
    48000,
    88200,
    96000,
    176400,
    192000,
    //352800,
    //384000,
};

#endif /* BW_TYPES_H */
