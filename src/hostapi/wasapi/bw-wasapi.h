#ifndef BW_WASAPI_H
#define BW_WASAPI_H

#include <bw-types.h>

typedef struct {
    char name[128];
    uint8_t device_usable;
} _wasapi_device;

/*
*   Activate a connection and start I/O with a device
*/
BWError BWWASAPI_Activate(BWHostApi_AudioDevice* audio_device);

/*
*   Close the connection to the active device
*/
BWError BWWASAPI_Deactivate();

/*
*   Query all of the devices available to WASAPI
*/
BWError BWWASAPI_QueryDevices(_wasapi_device** devices, uint32_t* num_devices);

#endif //BW_WASAPI_H
