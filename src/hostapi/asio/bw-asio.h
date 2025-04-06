#ifndef BW_ASIO_H
#define BW_ASIO_H

#include "stdint.h"
#include <bw-types.h>

typedef struct {

} asio_stream_params;

typedef struct {
    char name[32];
    long device_index;
} asio_device;

BWError BWAsioInitialize();
BWError BWAsioTerminate();
BWError BWAsioQueryDevices(asio_device** devices, uint32_t* num_devices);
BWError BWAsioOpenStream();
BWError BWAsioCloseStream();

#endif //BW_ASIO_H
