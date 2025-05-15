#ifndef BW_ASIO_H
#define BW_ASIO_H

#include "stdint.h"
#include <bw-types.h>

typedef struct {

} _asio_stream_params;

typedef struct {
    char name[32];
    uint16_t num_input_channels;
    uint16_t num_output_channels;

    uint8_t device_usable; //If there are not enough input / output channels available this will be 0, else 1

    uint32_t input_latency;
    uint32_t output_latency;
} _asio_device;

BWError BWAsio_QueryDevices(_asio_device** devices, uint32_t* num_devices);
BWError BWAsio_Activate(BWHostApi_AudioDevice* audio_device);
BWError BWAsio_Deactivate();

#endif //BW_ASIO_H
