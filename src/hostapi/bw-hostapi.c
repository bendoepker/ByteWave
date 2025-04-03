#include "bw-hostapi.h"

BWError BWInitialize() {
    BWError result = BW_OK;

    #ifdef BW_ASIO
    result = BWAsioInitialize();
    if(result != BW_OK) return result;
    #endif
    #ifdef BW_WASAPI
    result = BWWASAPIInitialize();
    if(result != BW_OK) return result;
    #endif
    #ifdef BW_DSOUND
    result = BWDSoundInitialize();
    if(result != BW_OK) return result;
    #endif

    return result;
}
