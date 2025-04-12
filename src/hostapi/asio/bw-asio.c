#include "bw-asio.h"
#include "bw-asio-il.h"
#include <bw-log.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>

//Globals
ASIODriverInfo* _bw_asio_driver_info = 0;
_asio_device* _bw_asio_devices = 0;
uint32_t _bw_asio_num_devices = 0;
unsigned char _bw_asio_is_initialized = 0;

//TODO: Use saved default device (saved_device)
BWError BWAsio_Initialize() {
    BWError result = BW_OK;

    //Cpp class constructor
    _bw_asio_construct_drivers();

    result = BWAsio_QueryDevices(&_bw_asio_devices, & _bw_asio_num_devices);
    if(_bw_asio_num_devices == 0) return BW_FAILED; //No asio devices available

    long saved_device = 3;
    result = _bw_asio_load_driver(_bw_asio_devices[saved_device].name);
    if(result != BW_OK) return result;

    //SECTION: Driver info struct initialization
    _bw_asio_driver_info = (ASIODriverInfo*)malloc(sizeof(ASIODriverInfo));
    if(_bw_asio_driver_info == 0) return BW_FAILED_MALLOC;
    memset(_bw_asio_driver_info, 0, sizeof(ASIODriverInfo));
    _bw_asio_driver_info->sysRef = GetCurrentProcess();
    _bw_asio_driver_info->asioVersion = 2;

    result = _bw_asio_init(_bw_asio_driver_info);
    if(result != BW_OK) return result;

    //SECTION: Get device attributes
    result = _bw_asio_get_devices_attributes(&_bw_asio_devices[saved_device]);
    if(result != BW_OK) return result;

    _bw_asio_is_initialized = 1;

    return result;
}

BWError BWAsio_Terminate() {
    BWError result = _bw_asio_exit();
    if(result != BW_OK) return result;
    _bw_asio_remove_current_driver();
    _bw_asio_destroy_drivers();
    _bw_asio_is_initialized = 0;
    return BW_OK;
}

BWError _bw_asio_get_devices_attributes(_asio_device* device) {
    //SECTION: Channels
    long num_inputs = 0;
    long num_outputs = 0;
    BWError result = _bw_asio_get_channels(&num_inputs, &num_outputs);
    if(result == BW_UNINITIALIZED) {
        //No input or output channels are present, hence ASE_NotPresent was returned
        device->num_input_channels = 0;
        device->num_output_channels = 0;
        device->device_usable = 0;
        return BW_HARDWARE_MALFUNCTION; //HACK: Not sure about this error code
    } else {
        device->num_input_channels = num_inputs;
        device->num_output_channels = num_outputs;
        device->device_usable = 1;
    }

    //SECTION: Buffer Sizes
    long min_buffer_size = 0;
    long max_buffer_size = 0;
    long preferred_size = 0;
    long granularity = 0;
    result = _bw_asio_get_buffer_size(&min_buffer_size, &max_buffer_size,
                                      &preferred_size, &granularity);
    if(result != BW_OK) return result;
    if(min_buffer_size <= 0 || max_buffer_size <= 0 || preferred_size <= 0) return BW_HARDWARE_MALFUNCTION;
    switch(granularity) {
        long current_buffer_size;
        long num_buffers;
        int buffer_size_index;
        case -1:
            //A granularity or -1 indicates that the buffer sizes are in powers of 2
            current_buffer_size = min_buffer_size;
            num_buffers = (long)(log2((double)max_buffer_size / (double)min_buffer_size)) + 1;
            device->num_buffer_sizes = num_buffers;

            device->buffer_sizes = (uint32_t*)malloc(sizeof(uint32_t) * num_buffers);
            if(device->buffer_sizes == 0) return BW_FAILED_MALLOC;

            buffer_size_index = 0;
            while(current_buffer_size <= max_buffer_size) {
                device->buffer_sizes[buffer_size_index] = current_buffer_size;
                current_buffer_size *= 2; //Increase in powers of 2
                if(current_buffer_size == preferred_size)
                    device->preferred_buffer_size_index = buffer_size_index;
                buffer_size_index++;
            }
            break;
        case 0:
            if(min_buffer_size != max_buffer_size && granularity != 0)
                return BW_HARDWARE_MALFUNCTION;
            else {
                device->num_buffer_sizes = 1;
                device->buffer_sizes = (uint32_t*)malloc(sizeof(uint32_t));
                *(device->buffer_sizes) = min_buffer_size;
                device->preferred_buffer_size_index = 0;
            }
            break;
        default:
            //Any other granularity is the difference between the buffer sizes
            current_buffer_size = min_buffer_size;
            num_buffers = ((max_buffer_size - min_buffer_size) / granularity) + 1;
            device->buffer_sizes = (uint32_t*)malloc(sizeof(uint32_t) * num_buffers);
            device->num_buffer_sizes = num_buffers;

            buffer_size_index = 0;
            while(current_buffer_size <= max_buffer_size) {
                device->buffer_sizes[buffer_size_index] = current_buffer_size;
                current_buffer_size += granularity; //Increase by difference of granularity
                if(current_buffer_size == preferred_size)
                    device->preferred_buffer_size_index = buffer_size_index;
                buffer_size_index++;
            }
            break;
    }

    //SECTION: Latencies
    long input_latency = 0;
    long output_latency = 0;
    result = _bw_asio_get_latencies(&input_latency, &output_latency);
    if(result != BW_OK) return result;
    if(input_latency < 0 || output_latency < 0)
        return BW_HARDWARE_MALFUNCTION;
    device->input_latency = input_latency;
    device->output_latency = output_latency;

    return BW_OK;
}

//WARN: Not thread safe, uses malloc
BWError BWAsio_QueryDevices(_asio_device** devices, uint32_t* num_devices){
    BWError result = BW_OK;

    long max_drivers = 32; //Arbitrary number, unlikely to be more than 32 drivers on a pc
    char* driver_names[max_drivers];
    for(int i = 0; i < max_drivers; i++) {
        driver_names[i] = (char*)malloc(32); //Max 32 Bytes per name as per asio standard
        if(driver_names[i] == 0) return BW_FAILED_MALLOC;
    }
    long num_drivers = _bw_asio_get_drv_names(driver_names, max_drivers);
    if(num_drivers < 0){
        if(num_drivers == -1010) return BW_UNINITIALIZED;
        return BW_FAILED;
    }

    //Translate the driver_names array into an array of asio_device
    *num_devices = num_drivers;
    (*devices) = (_asio_device*)malloc(sizeof(_asio_device) * num_drivers);
    if((*devices) == 0) return BW_FAILED_MALLOC;
    for(int i = 0; i < num_drivers; i++) {
        (*devices)[i].device_index = i;
        memcpy((*devices)[i].name, driver_names[i], 32);
    }

    //Cleanup temp memory
    for(int i = 0; i < max_drivers; i++) {
        free(driver_names[i]);
        driver_names[i] = 0;
    }

    /*
    if(num_drivers == 0) {
        return BW_OK;
    }
    */

    return result;
}
