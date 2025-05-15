#ifndef BW_HOSTAPI_H
#define BW_HOSTAPI_H

#ifdef BW_ASIO
    #include "asio/bw-asio.h"
#endif
#ifdef BW_WASAPI
    #include "wasapi/bw-wasapi.h"
#endif
#ifdef BW_DSOUND
    #include "dsound/bw-dsound.h"
#endif
#include <bw-types.h>

BWError BWHostApi_Initialize(BWConfigData* conf_data);
BWError BWHostApi_Terminate();

BWError BWHostApi_Activate();
BWError BWHostApi_Deactivate();

#endif // BW_HOSTAPI_H
