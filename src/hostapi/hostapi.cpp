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

#include <vector>

std::vector<BWAudioDevice> available_devices;

BWError BWHostApi_Initialize(BWConfigData* conf_data) {
    //SECTION: Enumerate devices
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
    std::vector<BWAudioDevice> jack_devs = BWJack_QueryDevices();
    available_devices.insert(available_devices.end(), jack_devs.begin(), jack_devs.end());
#endif

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

    //TODO: Other host api enumerations (WASAPI, JACK, ALSA...)

    //SECTION: LOG All Devices
#ifdef BW_LOG
    for(auto dev : available_devices) {
        BW_LOG_GEN("%s:\tInput:%d\tOutput:%d", dev.device_name, dev.is_input, dev.is_output);
    }
#endif //Log Device Names

    return BW_OK;
}

BWError BWHostApi_Terminate() {
    return BW_OK;
}

BWError BWHostApi_Activate(BWHostApi hostapi) {
    //Device selection
    switch(hostapi) {
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
            return BWJack_Activate();
            #endif //BW_JACK
        default:
            break;
    }
    return BW_OK;
}

BWError BWHostApi_Deactivate() {
    switch(0) {
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
            break;
    }
    return BW_OK;
}

bool BWHostApi_IsActivated() {
    //TODO:
    return true;
}
