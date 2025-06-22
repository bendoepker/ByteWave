/*
*	Bytewave: A Lightweight Digital Audio Workstation
*	Copyright (C) 2025  Ben Doepker
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "bw-asio.h"
/* ASIO Function Interface Layer and ASIO Types */
#include "bw-asio-il.h"

/* For main processing function */
#include "../../process/bw-process.h"

/* For Logging */
#include <bw-log.h>
#include <bw-conversions.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <assert.h>
#include <math.h>

#define ASIO_MAX_INPUT_CHANNELS 32
#define ASIO_MAX_OUTPUT_CHANNELS 2

typedef struct {
    long num_input_channels;
    long num_output_channels;
    long num_channels;

    ASIOBufferInfo buffer_infos[ASIO_MAX_INPUT_CHANNELS + ASIO_MAX_OUTPUT_CHANNELS];
    long min_buffer_size;
    long max_buffer_size;
    long preferred_buffer_size;
    long buffer_granularity;

    double sample_rate;
    long input_latency;
    long output_latency;

    /* Optimize the processing by reporting when it finishes to the interface */
    long post_output;

    ASIOChannelInfo channel_infos[ASIO_MAX_INPUT_CHANNELS + ASIO_MAX_OUTPUT_CHANNELS];
    BWSampleTypes sample_format;
} _asio_device_attributes;

//Globals
ASIODriverInfo* _bw_asio_driver_info = 0;
uint32_t _bw_asio_num_devices = 0;
unsigned char _bw_asio_is_initialized = 0;
ASIOBufferInfo* _bw_asio_buffer_infos = 0;
_asio_device _active_asio_device = {0};

//WHY DOES THIS NOT THROW A LINKER ERROR???
extern BWHostApi_AudioDevice* _active_audio_device;

//This is defined separate from _active_asio_device because it accesses structures defined
//in asio.h which I do not wish to include in the header files pertaining to any hostapis
_asio_device_attributes _device_attr = {0}; //Attributes pertaining to the active asio device

//Inner ASIO Functions
void bufferSwitch(long doubleBufferIndex, ASIOBool directProcess);
void sampleRateDidChange(ASIOSampleRate sRate);
long asioMessage(long selector, long value, void* message, double* opt);
ASIOTime* bufferSwitchTimeInfo(ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess);
ASIOCallbacks _asio_callbacks = {bufferSwitch, sampleRateDidChange, asioMessage, bufferSwitchTimeInfo};

BWError BWAsio_Activate(BWHostApi_AudioDevice* audio_device) {
    BWError result = BW_OK;

    //Translate audio_device to _active_asio_device
    strncpy(_active_asio_device.name, audio_device->device_name, 32);

    //Cpp class constructor
    _bw_asio_construct_drivers();

    //SECTION: Load driver
    BW_LOG_GEN("Current Device: %s", _active_asio_device.name);
    result = _bw_asio_load_driver(_active_asio_device.name);
    if(result != BW_OK) return result;

    //SECTION: Driver info struct initialization
    _bw_asio_driver_info = malloc(sizeof *_bw_asio_driver_info);
    if(_bw_asio_driver_info == 0) return BW_FAILED_MALLOC;
    memset(_bw_asio_driver_info, 0, sizeof(ASIODriverInfo));
    _bw_asio_driver_info->sysRef = GetCurrentProcess();
    _bw_asio_driver_info->asioVersion = 2;

    result = _bw_asio_init(_bw_asio_driver_info);
    if(result != BW_OK) return result;

    _bw_asio_is_initialized = 1;

    //SECTION: Buffer creation
    result = _bw_asio_get_channels(&_device_attr.num_input_channels,
                                   &_device_attr.num_output_channels);
    if(result != BW_OK) return result;
    _device_attr.num_channels = _device_attr.num_input_channels + _device_attr.num_output_channels;

    result = _bw_asio_get_buffer_size(&_device_attr.min_buffer_size, &_device_attr.max_buffer_size,
                                      &_device_attr.preferred_buffer_size, &_device_attr.buffer_granularity);
    if(result != BW_OK) return result;

    //Limit the number of channels to the pre-defined limits
    if(_device_attr.num_input_channels > ASIO_MAX_INPUT_CHANNELS)
        _device_attr.num_input_channels = ASIO_MAX_INPUT_CHANNELS;
    if(_device_attr.num_output_channels > ASIO_MAX_OUTPUT_CHANNELS)
        _device_attr.num_output_channels = ASIO_MAX_OUTPUT_CHANNELS;

    //Set the input buffer infos to reflect that they are input buffers
    for(int i = 0; i < _device_attr.num_input_channels; i++) {
        _device_attr.buffer_infos[i].isInput = ASIOTrue;
        _device_attr.buffer_infos[i].channelNum = i;
        _device_attr.buffer_infos[i].buffers[0] = 0;
        _device_attr.buffer_infos[i].buffers[1] = 0;
    }

    //Set the output buffer infos to reflect that they are output buffers
    for(int i = 0; i < _device_attr.num_output_channels; i++) {
        _device_attr.buffer_infos[i + _device_attr.num_input_channels].isInput = ASIOFalse;
        _device_attr.buffer_infos[i + _device_attr.num_input_channels].channelNum = i;
        _device_attr.buffer_infos[i + _device_attr.num_input_channels].buffers[0] = 0;
        _device_attr.buffer_infos[i + _device_attr.num_input_channels].buffers[1] = 0;
    }

    result = _bw_asio_get_sample_rate(&_device_attr.sample_rate);
    if(result != BW_OK) return result;

    BW_PRINT("Sample Rate: %f", _device_attr.sample_rate);
    BW_PRINT("Input Channels: %d", _device_attr.num_input_channels);
    BW_PRINT("Output Channels: %d", _device_attr.num_output_channels);
    BW_PRINT("Buffer Size: %d", _device_attr.preferred_buffer_size);

    result = _bw_asio_create_buffers(_device_attr.buffer_infos,
                                     _device_attr.num_input_channels + _device_attr.num_output_channels,
                                     _device_attr.preferred_buffer_size,
                                     &_asio_callbacks);
    if(result != BW_OK) return result;

    //Match the Channel Info Structures to the Buffer Info Structures
    for(int i = 0; i < _device_attr.num_input_channels + _device_attr.num_output_channels; i++) {
        _device_attr.channel_infos[i].channel = _device_attr.buffer_infos[i].channelNum;
        _device_attr.channel_infos[i].isInput = _device_attr.buffer_infos[i].isInput;
        result = _bw_asio_get_channel_info(&_device_attr.channel_infos[i]);
        if(result != BW_OK) return result;
        BW_PRINT("Channel %d Info", i);
        BW_PRINT("    Channel Number: %ld", _device_attr.channel_infos[i].channel);
        BW_PRINT("    Is Input: %ld", _device_attr.channel_infos[i].isInput);
        BW_PRINT("    Is Input of Assoc. Buf: %ld", _device_attr.buffer_infos[i].isInput);
        BW_PRINT("    Is Active: %ld", _device_attr.channel_infos[i].isActive);
        BW_PRINT("    Channel Group: %ld", _device_attr.channel_infos[i].channelGroup);
        BW_PRINT("    Sample Type: %ld", _device_attr.channel_infos[i].type);
        BW_PRINT("    Name: %s\n", _device_attr.channel_infos[i].name);
    }

    /*
        *WARN:
        * It isn't specified in the ASIOSDK that all of the devices' channels must have the
        * same input / output sample type format, for the time being all of the channels will
        * be given the same conversion function, if there is an interface which requires different
        * functions per channel this will have to be assigned on a channel by channel basis
    */

    switch(_device_attr.channel_infos[0].type) {
        case ASIOSTInt16LSB:
            _device_attr.sample_format = INT_16_BIT;
            break;
        case ASIOSTInt24LSB:
            _device_attr.sample_format = INT_24_BIT;
            break;
        case ASIOSTInt32LSB:
            _device_attr.sample_format = INT_32_BIT;
            break;
        case ASIOSTFloat32LSB:
            _device_attr.sample_format = FLOAT_32_BIT;
            break;
        default:
            //TODO: Add support for more sample types (especially Big Endian Types for mobile port)
            assert(1 && "Device sample type not supported\n");
            break;
    }

    //As per the ASIOSDK: Latencies may be invalid if requested before ASIOCreateBuffers()
    result = _bw_asio_get_latencies(&_device_attr.input_latency, &_device_attr.output_latency);
    if(result != BW_OK) return result;
    BW_LOG_GEN("Input Latency | Output Latency: %d | %d", _device_attr.input_latency, _device_attr.output_latency);

    //Test if the device supports the post output optimization
    if(_bw_asio_output_ready() == BW_OK)
        _device_attr.post_output = 1;
    else _device_attr.post_output = 0;
    BW_PRINT("Supports Post Output: %d", _device_attr.post_output);

    result = _bw_asio_start();
    if(result != BW_OK) return result;

    return result;
}

BWError BWAsio_Deactivate() {
    BWError result = _bw_asio_stop();
    result = _bw_asio_dispose_buffers();
    result = _bw_asio_exit();
    if(result != BW_OK) return result;
    _bw_asio_remove_current_driver();
    free(_bw_asio_driver_info);
    _bw_asio_destroy_drivers();
    _bw_asio_is_initialized = 0;
    return BW_OK;

}

BWError BWAsio_QueryDevices(_asio_device** devices, uint32_t* num_devices){

    _bw_asio_construct_drivers();

    long max_drivers = 32; //Arbitrary number, unlikely to be more than 32 drivers on a pc
    char* driver_names[max_drivers];
    for(int i = 0; i < max_drivers; i++) {
        driver_names[i] = malloc(32); //Max 32 Bytes per name as per asio standard
        if(driver_names[i] == 0) return BW_FAILED_MALLOC;
    }
    long num_drivers = _bw_asio_get_drv_names(driver_names, max_drivers);
    if(num_drivers < 0){
        if(num_drivers == -1010) return BW_UNINITIALIZED;
        return BW_FAILED;
    }

    //Translate the driver_names array into an array of asio_device
    *num_devices = num_drivers;
    (*devices) = malloc(sizeof *(*devices) * num_drivers);
    if((*devices) == 0) return BW_FAILED_MALLOC;
    for(int i = 0; i < num_drivers; i++) {
        memcpy((*devices)[i].name, driver_names[i], 32);
    }

    //Cleanup temp memory
    for(int i = 0; i < max_drivers; i++) {
        free(driver_names[i]);
        driver_names[i] = 0;
    }

    _bw_asio_destroy_drivers();

    return BW_OK;
}

void bufferSwitch(long doubleBufferIndex, ASIOBool directProcess) {
    //Backdoor into the actual audio callback (bufferSwitchTimeInfo)

    //Taken from the ASIO2 SDK, explanation in ASIOSDK/host/sample/hostsample.cpp
    ASIOTime timeInfo = {0};

	if(_bw_asio_get_sample_position(&timeInfo.timeInfo.samplePosition, &timeInfo.timeInfo.systemTime) == BW_OK)
		timeInfo.timeInfo.flags = kSystemTimeValid | kSamplePositionValid;

    bufferSwitchTimeInfo(&timeInfo, doubleBufferIndex, directProcess);
}

void sampleRateDidChange(ASIOSampleRate sRate) {
    //TODO: This, likely just update a conversion algorithm and always use a single internal sample rate
    //      (likely 48000)
}

long asioMessage(long selector, long value, void* message, double* opt) {
    //Unused as of ASIO 2.3
    (void)message;
    (void)opt;

    switch(selector) {
        case kAsioSelectorSupported:
            if(value == kAsioResetRequest
                || value == kAsioEngineVersion
                || value == kAsioResyncRequest
                || value == kAsioLatenciesChanged
                // the following three were added for ASIO 2.0, you don't necessarily have to support them
                || value == kAsioSupportsTimeInfo
                || value == kAsioSupportsTimeCode
                || value == kAsioSupportsInputMonitor)
                return 1L;
            else return 0;
        case kAsioBufferSizeChange:
            BWAsio_Deactivate();
            BWAsio_Activate(_active_audio_device);
            return 1L;
        case kAsioResetRequest:
            //TODO: Handle reset request, currently we will just crash
            assert(1 && "ASIO Reset Request was sent");
            break;
        case kAsioResyncRequest:
            return 1L;
        case kAsioLatenciesChanged:
            //NOTE: Ignored for the time being, should trigger a latency tracking update if
            //      latency is being tracked
            return 1L;
        case kAsioEngineVersion:
            return 2L;
        case kAsioSupportsTimeInfo:
            return 1L;
        case kAsioSupportsTimeCode:
            return 0; //NOTE: May need to keep track of this info, needs more research
        case kAsioSupportsInputMonitor:
            return 0;
    }
    return 0;
}

ASIOTime* bufferSwitchTimeInfo(ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess) {
    (void) directProcess; //Processing should never be deferred

    long buf_size = _device_attr.preferred_buffer_size;
    long num_inputs = _device_attr.num_input_channels;
    long num_outputs = _device_attr.num_output_channels;
    BWSampleTypes native_format = _device_attr.sample_format;
    ASIOBufferInfo* infos = _device_attr.buffer_infos;

    //Internal buffer declaration
    float input_bufs[num_inputs][buf_size];
    float output_bufs[num_outputs][buf_size];

    //Convert the input buffers into floats
    for(int i = 0; i < num_inputs; i++, infos++) {
        BWUtil_ConvertToFloat(infos->buffers[doubleBufferIndex], input_bufs[i], native_format, buf_size);
    }

    //Processing

    /*
    *   At this point all of the derived parameters should be passed to the processing function,
    *   namely input_bufs, output_bufs, num_inputs, num_outputs; There should also be a timing
    *   function to roughly measure input overflow and output underflows
    *
    *   process_buffers(input_bufs, output_bufs, num_inputs, num_outputs, buf_size);
    */
    //Zero Initialize all of the output buffers
    for(int i = 0; i < num_outputs; i++) {
        memset(output_bufs[i], 0, buf_size * sizeof(float));

        //Add the inputs to the outputs
        for(int j = 0; j < num_inputs; j++) {
            for(int k = 0; k < buf_size; k++) {
                output_bufs[i][k] += input_bufs[j][k];
                if(output_bufs[i][k] > 1.0) output_bufs[i][k] = 1.0;
                if(output_bufs[i][k] < -1.0) output_bufs[i][k] = -1.0;
            }
        }
    }

    //Convert the output buffers to the native format
    for(int i = 0; i < num_outputs; i++, infos++) {
        BWUtil_ConvertFromFloat(output_bufs[i], infos->buffers[doubleBufferIndex], native_format, buf_size);
    }

    if(_device_attr.post_output)
        _bw_asio_output_ready();

    return 0;
}
