#ifndef BW_ASIO_H
#define BW_ASIO_H

#include "stdint.h"
#include <bw-types.h>

typedef struct {

} _asio_stream_params;

typedef struct {
    char name[32];
    long device_index;
} _asio_device;

BWError BWAsio_Initialize();
BWError BWAsio_Terminate();
BWError BWAsio_QueryDevices(_asio_device** devices, uint32_t* num_devices);
BWError BWAsio_OpenStream();
BWError BWAsio_CloseStream();

BWError BWAsio_ChangeDevice(char device[32]);

#endif //BW_ASIO_H
