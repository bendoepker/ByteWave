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

/* Logging */
#include "log.h"

#include <vector>

BWHostApi active_backend = UNKNOWN;
std::vector<BWAudioDevice> available_devices;

//TODO: Asio / WASPI refactor to use CPP
BWError BWAudioBackend::Initialize(BWConfigData* conf_data) {
    active_backend = conf_data->host_api;
#ifdef BW_ASIO
    std::vector<BWAudioDevice> asio_devs = Asio::QueryDevices();
    available_devices.insert(available_devices.end(), asio_devs.begin(), asio_devs.end());
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
    std::vector<BWAudioDevice> jack_devs = Jack::QueryDevices();
    available_devices.insert(available_devices.end(), jack_devs.begin(), jack_devs.end());
#endif

#ifdef BW_LOG
    for(auto dev : available_devices) {
        BW_LOG_GEN("%s:\tInput:%d\tOutput:%d", dev.device_name, dev.is_input, dev.is_output);
    }
#endif //Log Device Names

    return BW_OK;
}

BWError BWAudioBackend::Terminate() {
    return BW_OK;
}

BWError BWAudioBackend::Activate() {
    //Device selection
    switch(active_backend) {
        case ASIO:
            #ifdef BW_ASIO
        {
            auto ret = Asio::Activate(0);
            return ret;
        }
            #endif //BW_ASIO
        case WASAPI:
            #ifdef BW_WASAPI
            return BWWASAPI_Activate(_active_audio_device);
            #endif //BW_WASAPI
        case JACK:
            #ifdef BW_JACK
            return Jack::Activate();
            #endif //BW_JACK
        default:
            break;
    }
    return BW_OK;
}

void BWAudioBackend::Deactivate() {
    switch(active_backend) {
        case ASIO:
            #ifdef BW_ASIO
            Asio::Deactivate();
            return;
            #endif //BW_ASIO
        case WASAPI:
            #ifdef BW_WASAPI
            WASAPI::Deactivate();
            return;
            #endif //BW_WASAPI
        case JACK:
            #ifdef BW_JACK
            Jack::Deactivate();
            return;
            #endif //BW_JACK
        default:
            break;
    }
    active_backend = UNKNOWN;
}

bool BWAudioBackend::IsActivated() {
    if(active_backend == UNKNOWN) return false;
    return true;
}

void BWAudioBackend::ChangeHostApi(BWHostApi new_api) {
    if(active_backend != UNKNOWN) {
        BWAudioBackend::Deactivate();
    }
    active_backend = new_api;
    BWAudioBackend::Activate();
}

BWHostApi BWAudioBackend::GetCurrentApi() {
    return active_backend;
}

const std::vector<BWAudioDevice> BWAudioBackend::GetAudioDevices(bool is_input) {
    std::vector<BWAudioDevice> out = {};
    for(auto dev : available_devices) {
        if(is_input) {
            if(dev.is_input) {
                out.push_back(dev);
            }
        } else {
            if(dev.is_output)
                out.push_back(dev);
        }
    }
    return out;
}
