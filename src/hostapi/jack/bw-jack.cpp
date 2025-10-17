#include "bw-jack.h"
#include <jack/jack.h>
#include <log.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static jack_client_t* client = NULL;
static const char* client_name = "ByteWave";
static const char* server_name = NULL;

static bool IsPortAudio(const char* port_name) {
    jack_port_t* port = jack_port_by_name(client, port_name);
    if(strcmp(JACK_DEFAULT_AUDIO_TYPE, jack_port_type(port)) == 0)
        return true;
    return false;
}

BWError BWJack_Activate() {
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

std::vector<BWAudioDevice> BWJack_QueryDevices() {
    if(!client) {
        jack_status_t status;
        client = jack_client_open(client_name, JackNullOption, &status);
        if(!client) {
            return {};
        }
    }

    std::vector<BWAudioDevice> out;

    //TODO: Specify input, output, audio, and midi port differences
    const char** in_ports = jack_get_ports(client, NULL, NULL, JackPortIsPhysical | JackPortIsInput);
    const char** out_ports = jack_get_ports(client, NULL, NULL, JackPortIsPhysical | JackPortIsOutput);

    //Count the number of inputs and outputs
    int num_inputs = 0, num_outputs = 0;
    if(in_ports)
        for(;;num_inputs++) if(!in_ports[num_inputs]) break;
    if(out_ports)
        for(;;num_outputs++) if(!out_ports[num_outputs]) break;

    int max_read_length = jack_port_name_size() < 128 ? jack_port_name_size() : 128;
    if(in_ports) {
        for(int i = 0; i < num_inputs; i++) {
            if(!IsPortAudio(in_ports[i]))
                continue;
            out.push_back({});
            out[i].is_input = true;
            strncpy(out[i].device_name, in_ports[i], max_read_length);
        }
        jack_free(in_ports);
    }

    if(out_ports) {
        for(int i = 0; i < num_outputs; i++) {
            if(!IsPortAudio(out_ports[i]))
                continue;
            out.push_back({});
            out[out.size() - 1].is_output = true;
            strncpy(out[out.size() - 1].device_name, out_ports[i], max_read_length);
        }
        jack_free(out_ports);
    }

    return out;
}
