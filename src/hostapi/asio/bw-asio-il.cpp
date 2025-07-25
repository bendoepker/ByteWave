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

#include "bw-asio-il.h"
#include <bw-log.h>
#include <asiodrivers.h>

//Asio driver functions (asiodrivers.h)

//Globals
//PERF: !!THE LINE BELOW MUST REMAIN THE SAME TO CORRECTLY LINK WITH ASIO!!
extern AsioDrivers* asioDrivers;

//Wrap constructor / destructor
void _bw_asio_construct_drivers() {
    if(asioDrivers) {
        BW_LOG_GEN("asioDrivers already initialized");
        return;
    }
    asioDrivers = new AsioDrivers();
}

void _bw_asio_destroy_drivers() { 
    if(asioDrivers != 0) {
        delete asioDrivers; 
        asioDrivers = 0;
    }
}

//Get Driver Names
//Returns the number of drivers (up to the total number of drivers)
//or -1010 if the class is not instantiated
long _bw_asio_get_drv_names(char** names, long max_drivers) {
    if(!asioDrivers) return -1010;
    return asioDrivers->getDriverNames(names, max_drivers);
}

//Retrieves the name of the current loaded driver
BWError _bw_asio_get_current_drv_name(char* name) {
    if(!asioDrivers) return BW_UNINITIALIZED;
    if(asioDrivers->getCurrentDriverName(name))
        return BW_OK;
    else return BW_FAILED;
}

//Retrieves the index of the current loaded driver
long _bw_asio_get_current_driver_index() {
    if(!asioDrivers) return BW_UNINITIALIZED;
    return asioDrivers->getCurrentDriverIndex();
}

BWError _bw_asio_load_driver(char* name) {
    if(!asioDrivers) return BW_UNINITIALIZED;
    if(asioDrivers->loadDriver(name))
        return BW_OK;
    else return BW_FAILED;
}

void _bw_asio_remove_current_driver() {
    if(!asioDrivers) return;
    asioDrivers->removeCurrentDriver();
    return;
}

//Asio stream functions (asio.h)

//Mapping function from ASIOError to BWError
static inline BWError _bw_asio_map_error(ASIOError err);

BWError _bw_asio_init(ASIODriverInfo* info) {
    return _bw_asio_map_error(
        ASIOInit(info)
    );
}

BWError _bw_asio_exit(void) {
    return _bw_asio_map_error(
        ASIOExit()
    );
}

//PERF: DONE
BWError _bw_asio_start(void) {
    return _bw_asio_map_error(
        ASIOStart()
    );
}

BWError _bw_asio_stop(void) {
    return _bw_asio_map_error(
        ASIOStop()
    );
}

BWError _bw_asio_get_channels(long* num_input_channels, long* num_output_channels) {
    return _bw_asio_map_error(
        ASIOGetChannels(num_input_channels, num_output_channels)
    );
}

BWError _bw_asio_get_latencies(long* input_latency, long* output_latency) {
    return _bw_asio_map_error(
        ASIOGetLatencies(input_latency, output_latency)
    );
}

BWError _bw_asio_get_buffer_size(long* min_size, long* max_size, long* preferred_size, long* granularity) {
    return _bw_asio_map_error(
        ASIOGetBufferSize(min_size, max_size, preferred_size, granularity)
    );
}

BWError _bw_asio_can_sample_rate(double sample_rate, uint8_t* output) {
    switch(ASIOCanSampleRate(sample_rate)) {
        case ASE_OK:
            *output = 1;
            return BW_OK;
        case ASE_NoClock:
            *output = 0;
            return BW_OK;
        case ASE_NotPresent:
            *output = 0;
            return BW_UNINITIALIZED;
        default:
            *output = 0;
            return BW_FAILED;
    }
}

BWError _bw_asio_get_sample_rate(double* current_sample_rate) {
    return _bw_asio_map_error(
        ASIOGetSampleRate(current_sample_rate)
    );
}

BWError _bw_asio_set_sample_rate(double sample_rate) {
    return _bw_asio_map_error(
        ASIOSetSampleRate(sample_rate)
    );
}

BWError _bw_asio_get_clock_sources(ASIOClockSource* clocks, long* num_sources) {
    return _bw_asio_map_error(
        ASIOGetClockSources(clocks, num_sources)
    );
}

BWError _bw_asio_set_clock_source(long reference) {
    return _bw_asio_map_error(
        ASIOSetClockSource(reference)
    );
}

BWError _bw_asio_get_sample_position(ASIOSamples* s_pos, ASIOTimeStamp* t_stamp) {
    return _bw_asio_map_error(
        ASIOGetSamplePosition(s_pos, t_stamp)
    );
}

BWError _bw_asio_get_channel_info(ASIOChannelInfo* info) {
    return _bw_asio_map_error(
        ASIOGetChannelInfo(info)
    );
}

BWError _bw_asio_create_buffers(ASIOBufferInfo* buffer_infos, long num_channels, long buffer_size, ASIOCallbacks* callbacks) {
    return _bw_asio_map_error(
        ASIOCreateBuffers(buffer_infos, num_channels, buffer_size, callbacks)
    );
}

BWError _bw_asio_dispose_buffers(void) {
    return _bw_asio_map_error(
        ASIODisposeBuffers()
    );
}

//This panel may or may not depend on the lifetime of the application
//i.e. Focusrite USB Driver creates a separate process, ASIO4ALLv2 creates a sub-process
BWError _bw_asio_control_panel(void) {
    return _bw_asio_map_error(
        ASIOControlPanel()
    );
}

BWError _bw_asio_future(long selector, void* params) {
    return _bw_asio_map_error(
        ASIOFuture(selector, params)
    );
}

BWError _bw_asio_output_ready(void) {
    return _bw_asio_map_error(
        ASIOOutputReady()
    );
}


static inline BWError _bw_asio_map_error(ASIOError err) {
    switch(err) {
        case ASE_OK:
            return BW_OK;
        case ASE_SUCCESS:
            return BW_OK; //May need new value, used in ASIOFuture calls
        case ASE_NotPresent:
            return BW_UNINITIALIZED;
        case ASE_HWMalfunction:
            return BW_HARDWARE_MALFUNCTION; //Failed beyond the scope of software
        case ASE_InvalidParameter:
            return BW_BAD_PARAMS; //Bad input params
        case ASE_InvalidMode:
            return BW_HARDWARE_MALFUNCTION;
        case ASE_SPNotAdvancing:
            return BW_HARDWARE_MALFUNCTION;
        case ASE_NoClock:
            return BW_FAILED; //Should be a rare error, for now it is under the default error code
        case ASE_NoMemory:
            return BW_FAILED_MALLOC; //Ran out of memory, malloc failed
        default:
            return BW_FAILED; //Catch all in case of future ASIO defined errors
    }
}
