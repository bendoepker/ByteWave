#ifndef BW_DSOUND_H
#define BW_DSOUND_H
#include <dsound.h>
#include <bw-types.h>

BWError BWDSound_Initialize();
BWError BWDSound_Terminate();
BWError BWDSound_QueryDevices();
BWError BWDSound_OpenStream();
BWError BWDSound_CloseStream();

#endif //BW_DSOUND_H
