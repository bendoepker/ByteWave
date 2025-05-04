#include "bw-hostapi.h"

BWHostApi_AudioDevice* _active_audio_device = 0;
int num_devices = 0;
BWAudioDeviceEnumeration* devices_enumeration = 0;

BWError BWHostApi_Initialize(BWConfigData conf_data) {
    //SECTION: Enumerate devices
    //ASIO
    uint32_t num_asio_devs = 0;
    _asio_device* asio_devs = 0;
    BWError ret = BWAsio_QueryDevices(&asio_devs, &num_asio_devs);
    if(ret != BW_OK) return ret;
    num_devices += num_asio_devs;

    //TODO: Other host api enumerations (WASAPI, JACK, ALSA...)

    if(num_devices == 0) return BW_NO_DEVICES;

    devices_enumeration = malloc(sizeof *devices_enumeration * num_devices);
    if(devices_enumeration == 0) return BW_FAILED_MALLOC;

    //SECTION: Copy names and assign host api identifier
    //ASIO
    for(int i = 0; i < num_asio_devs; i++) {
        memset(devices_enumeration[i].device_name, 0, 128);
        memcpy(devices_enumeration[i].device_name, asio_devs[i].name, 32);
        devices_enumeration[i].host_api = ASIO;
    }
    free(asio_devs);
    asio_devs = 0;

    //TODO: Other host api enumerations (WASAPI, JACK, ALSA...)

    //SECTION: Set the active audio device based on defaults
    _active_audio_device = malloc(sizeof* _active_audio_device);
    if(_active_audio_device == 0) return BW_FAILED_MALLOC;
    memset(_active_audio_device, 0, sizeof* _active_audio_device);

    //Search for audio device matching the one in the config data
    for(int i = 0; i < num_devices; i++) {
        if(devices_enumeration[i].host_api == conf_data.host_api) {
            if(devices_enumeration[i].host_api == ASIO) {
                if(strncmp(devices_enumeration[i].device_name, conf_data.device_name, 32) == 0){
                    strncpy(_active_audio_device->device_name, devices_enumeration[i].device_name, 32);
                    _active_audio_device->host_api = devices_enumeration[i].host_api;
                    break;
                }
            } else {
                if(strncmp(devices_enumeration[i].device_name, conf_data.device_name, 128) == 0) {
                    strncpy(_active_audio_device->device_name, devices_enumeration[i].device_name, 128);
                    _active_audio_device->host_api = devices_enumeration[i].host_api;
                    break;
                }
            }
        }
    }
    if(_active_audio_device->host_api == UNKNOWN || _active_audio_device->device_name[0] == 0)
        return BW_NO_DEVICES;

    return BW_OK;
}

BWError BWHostApi_Terminate() {
    free(devices_enumeration);
    free(_active_audio_device);
    return BW_OK;
}

BWError BWHostApi_Activate(BWAudioDeviceEnumeration device) {
    //Device selection
    switch(device.host_api) {
        case ASIO:
            BWAsio_Activate();
            break;
        case WASAPI:
            //TODO:
            break;
    }
    return BW_OK;
}

BWError BWHostApi_Deactivate() {
    switch(_active_audio_device->host_api) {
        case ASIO:
            BWAsio_Deactivate();
            break;
        case WASAPI:
            //TODO:
            break;
    }
    return BW_OK;
}

