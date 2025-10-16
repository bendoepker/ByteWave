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

#include "hostapi.h"

/* For memcpy() */
#include <string.h>

/* For malloc() and free() */
#include <stdlib.h>

/* Logging */
#include "log.h"

/* For assert() */
#include <assert.h>

BWHostApi_AudioDevice* _active_audio_device = 0;
int num_devices = 0;
BWAudioDeviceEnumeration* devices_enumeration = 0;

BWError BWHostApi_Initialize(BWConfigData* conf_data) {
    //SECTION: Enumerate devices
    BWError ret = BW_OK;
#ifdef BW_ASIO
    //ASIO
    uint32_t num_asio_devs = 0;
    _asio_device* asio_devs = 0;
    ret = BWAsio_QueryDevices(&asio_devs, &num_asio_devs);
    if(ret != BW_OK) return ret;
    num_devices += num_asio_devs;
#endif

#ifdef BW_WASAPI
    //WASAPI
    uint32_t num_wasapi_devs = 0;
    _wasapi_device* wasapi_devs = 0;
    ret = BWWASAPI_QueryDevices(&wasapi_devs, &num_wasapi_devs);
    if(ret != BW_OK) return ret;
    num_devices += num_wasapi_devs;
#endif

#ifdef BW_JACK
    //JACK
    uint32_t num_jack_devs = 0;
    _jack_device* jack_devs = 0;
    ret = BWJack_QueryDevices(&jack_devs, &num_jack_devs);
    if(ret != BW_OK) return ret;
    num_devices += num_jack_devs;
#endif

    //TODO: Other host api enumerations (WASAPI, JACK, ALSA...)

    if(num_devices == 0) return BW_NO_DEVICES;

    devices_enumeration = (BWAudioDeviceEnumeration*)malloc(sizeof *devices_enumeration * num_devices);
    if(devices_enumeration == 0) return BW_FAILED_MALLOC;

    //SECTION: Copy names and assign host api identifier
    int offset = 0;
#ifdef BW_ASIO
    //ASIO
    for(int i = 0; i < num_asio_devs; i++) {
        memcpy(devices_enumeration[offset + i].device_name, asio_devs[i].name, 32);
        devices_enumeration[offset + i].host_api = ASIO;
    }
    offset += num_asio_devs;
    free(asio_devs);
    asio_devs = 0;
#endif //ASIO Enumeration

#ifdef BW_WASAPI
    //WASAPI
    for(int i = 0; i < num_wasapi_devs; i++) {
        memcpy(devices_enumeration[offset + i].device_name, wasapi_devs[i].name, 128);
        devices_enumeration[offset + i].host_api = WASAPI;
    }
    offset += num_wasapi_devs;
    free(wasapi_devs);
    wasapi_devs = 0;
#endif //WASAPI Enumeration

#ifdef BW_JACK
    for(int i = 0; i < num_jack_devs; i++) {
        memcpy(devices_enumeration[offset + i].device_name, jack_devs[i].name, 128);
        devices_enumeration[offset + i].host_api = JACK;
    }
    offset += num_jack_devs;
    free(jack_devs);
    jack_devs = 0;
#endif //Jack Enumeration

    //TODO: Other host api enumerations (WASAPI, JACK, ALSA...)

    //SECTION: LOG All Devices
#ifdef BW_LOG
    for(int i = 0; i < num_devices; i++) {
        BW_LOG_GEN("%s", devices_enumeration[i].device_name);
    }
#endif //Log Device Names

    //SECTION: Set the active audio device based on defaults
    _active_audio_device = (BWHostApi_AudioDevice*)malloc(sizeof* _active_audio_device);
    if(_active_audio_device == 0) return BW_FAILED_MALLOC;
    memset(_active_audio_device, 0, sizeof* _active_audio_device);

    /*
     * TODO: Fix this (aka add it back, but better)
    //Search for audio device matching the one in the config data
    for(int i = 0; i < num_devices; i++) {
        if(devices_enumeration[i].host_api == conf_data->host_api) {
            if(devices_enumeration[i].host_api == ASIO) {
                if(strncmp(devices_enumeration[i].device_name, conf_data->device_name, 32) == 0){
                    strncpy(_active_audio_device->device_name, devices_enumeration[i].device_name, 32);
                    _active_audio_device->host_api = devices_enumeration[i].host_api;
                    break;
                }
            } else {
                if(strncmp(devices_enumeration[i].device_name, conf_data->device_name, 128) == 0) {
                    strncpy(_active_audio_device->device_name, devices_enumeration[i].device_name, 128);
                    _active_audio_device->host_api = devices_enumeration[i].host_api;
                    break;
                }
            }
        }
    }
    if(_active_audio_device->host_api == UNKNOWN || _active_audio_device->device_name[0] == 0) {
        //Default to the first device
        _active_audio_device->host_api = devices_enumeration[0].host_api;
        strncpy(_active_audio_device->device_name, devices_enumeration[0].device_name, 128);
        strncpy(conf_data->device_name, _active_audio_device->device_name, 128);
        conf_data->host_api = _active_audio_device->host_api;
    }
    */

    return BW_OK;
}

BWError BWHostApi_Terminate() {
    free(devices_enumeration);
    free(_active_audio_device);
    return BW_OK;
}

BWError BWHostApi_Activate() {
    //Device selection
    switch(_active_audio_device->host_api) {
        case ASIO:
            #ifdef BW_ASIO
            return BWAsio_Activate(_active_audio_device);
            #endif //BW_ASIO
        case WASAPI:
            #ifdef BW_WASAPI
            return BWWASAPI_Activate(_active_audio_device);
            #endif //BW_WASAPI
        case JACK:
            #ifdef BW_JACK
            return BWJack_Activate(_active_audio_device);
            #endif //BW_JACK
        default:
            assert(1 && "Host Api Not Implemented");
            break;
    }
    return BW_OK;
}

BWError BWHostApi_Deactivate() {
    switch(_active_audio_device->host_api) {
        case ASIO:
            #ifdef BW_ASIO
            return BWAsio_Deactivate();
            #endif //BW_ASIO
        case WASAPI:
            #ifdef BW_WASAPI
            return BWWASAPI_Deactivate();
            #endif //BW_WASAPI
        case JACK:
            #ifdef BW_JACK
            return BWJack_Deactivate();
            #endif //BW_JACK
        default:
            assert(1 && "Host Api Not Implemented");
            break;
    }
    return BW_OK;
}

