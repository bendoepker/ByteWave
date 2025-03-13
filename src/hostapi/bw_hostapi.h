#ifndef BW_HOSTAPI_H
#define BW_HOSTAPI_H

#ifdef BW_ASIO
    #include "asio/bw_asio.h"
#endif
#ifdef BW_WASAPI
    #include "wasapi/bw_wasapi.h"
#endif
#ifdef BW_DSOUND
    #include "dsound/bw_dsound.h"
#endif
#include <bw_types.h>

BWError BWInitialize();

#endif // BW_HOSTAPI_H
