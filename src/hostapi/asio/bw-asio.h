#ifndef BW_ASIO_H
#define BW_ASIO_H

#include "stdint.h"
#include <bw-types.h>

typedef struct {

} _asio_stream_params;

typedef struct {
    char name[32];
    //long device_index;
    uint16_t num_input_channels;
    uint16_t num_output_channels;

    uint8_t device_usable; //If there are not enough input / output channels available this will be 0, else 1

    //All available buffer sizes are stored in an array of uint32_t
    //Index 0 represents the minimum buffer size
    //Index num_buffer_sizes - 1 = the maximum buffer size
    uint32_t buffer_size;

    uint32_t input_latency;
    uint32_t output_latency;
} _asio_device;

BWError BWAsio_QueryDevices(_asio_device** devices, uint32_t* num_devices);
BWError BWAsio_Activate();
BWError BWAsio_Deactivate();

BWError BWAsio_ChangeDevice(char device[32]);

//Helper functions

//Retrieve the attributes associated with the loaded device and store them in @param device
BWError _bw_asio_get_devices_attributes(_asio_device* device);

#endif //BW_ASIO_H
