#ifndef BW_ASIO_H
#define BW_ASIO_H

#include "stdint.h"
#include <bw-types.h>

typedef struct {
    char name[32];
    uint8_t device_usable; //If there are not enough input / output channels available this will be 0, else 1

    uint32_t input_latency;
    uint32_t output_latency;
} _asio_device;

/*
*   Activate a connection with a device
*/
BWError BWAsio_Activate(BWHostApi_AudioDevice* audio_device);

/*
*   Close the connection to the device
*/
BWError BWAsio_Deactivate();

/*
*   Query all of the devices available to ASIO
*/
BWError BWAsio_QueryDevices(_asio_device** devices, uint32_t* num_devices);

#endif //BW_ASIO_H
