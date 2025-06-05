#include "bw-hostapi.h"

/* For memcpy() */
#include <string.h>

/* For malloc() and free() */
#include <stdlib.h>

/* For device enumeration dynamic array */
#include <bw-dynamic-array.h>

/* Logging */
#include "bw-log.h"

/* For assert() */
#include <assert.h>

BWHostApi_AudioDevice* _active_audio_device = 0;
int num_devices = 0;
BWAudioDeviceEnumeration* devices_enumeration = 0;

BWError BWHostApi_Initialize(BWConfigData* conf_data) {
    //SECTION: Enumerate devices
    BWError ret = 0;
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

    //TODO: Other host api enumerations (WASAPI, JACK, ALSA...)

    if(num_devices == 0) return BW_NO_DEVICES;

    devices_enumeration = malloc(sizeof *devices_enumeration * num_devices);
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

    //TODO: Other host api enumerations (WASAPI, JACK, ALSA...)

    //SECTION: LOG All Devices
#ifdef BW_LOG
    for(int i = 0; i < num_devices; i++) {
        BW_LOG_GEN("%s", devices_enumeration[i].device_name);
    }
#endif //Log Device Names

    //SECTION: Set the active audio device based on defaults
    _active_audio_device = malloc(sizeof* _active_audio_device);
    if(_active_audio_device == 0) return BW_FAILED_MALLOC;
    memset(_active_audio_device, 0, sizeof* _active_audio_device);

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
        default:
            assert(1 && "Host Api Not Implemented");
            break;
    }
    return BW_OK;
}

