#include "bw-jack.h"
#include <jack/jack.h>
#include <log.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static jack_client_t* client = NULL;
static const char* client_name = "ByteWave";
static const char* server_name = NULL;

BWError BWJack_Activate(BWHostApi_AudioDevice* audio_device) {
    if(!client) {
        jack_status_t status;
        client = jack_client_open(client_name, JackNullOption, &status);
        if(!client)
            return BW_FAILED;
    }
    return BW_OK;
}

BWError BWJack_Deactivate() {
    if(client)
        jack_client_close(client);
    return BW_FAILED;
}

BWError BWJack_QueryDevices(_jack_device** devices, uint32_t* num_devices) {
    if(!client) {
        jack_status_t status;
        client = jack_client_open(client_name, JackNullOption, &status);
        if(!client) {
            devices = NULL;
            *num_devices = 0;
            return BW_FAILED;
        }
    }

    //TODO: Specify input, output, audio, and midi port differences
    const char** ports = jack_get_ports(client, NULL, NULL, 0);
    if(ports) {
        int num_devs = 0;
        for(;;num_devs++) if(!ports[num_devs]) break;
        *devices = (_jack_device*)malloc(sizeof(*(*devices)) * num_devs);
        if(!*devices) {
            *num_devices = 0;
            return BW_FAILED_MALLOC;
        }
        *num_devices = num_devs;
        int max_read_length = jack_port_name_size() < 128 ? jack_port_name_size() : 128;
        for(int i = 0; i < num_devs; i++)
            strncpy((*devices)[i].name, ports[i], max_read_length);
        jack_free(ports);
    } else {
        devices = NULL;
        *num_devices = 0;
        return BW_NO_DEVICES;
    }

    return BW_OK;
}
