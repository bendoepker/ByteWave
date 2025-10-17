#include "bw-jack.h"
#include <jack/jack.h>
#include <log.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../process/process.h"
#include <hostapi.h>

#define MAX_JACK_INPUT_CHANNELS 32
#define MAX_JACK_OUTPUT_CHANNELS 2

static jack_client_t* client = NULL;
static const char* client_name = "ByteWave";
static const char* server_name = NULL;
static std::vector<jack_port_t*> inputs;
static std::vector<jack_port_t*> outputs;

static bool IsAudioPort(const char* port_name) {
    jack_port_t* port = jack_port_by_name(client, port_name);
    if(strcmp(JACK_DEFAULT_AUDIO_TYPE, jack_port_type(port)) == 0)
        return true;
    return false;
}

int JackProcessFunction(jack_nframes_t nframes, void* arg) {
    int num_inputs = inputs.size();
    int num_outputs = outputs.size();
    jack_default_audio_sample_t* in_bufs[MAX_JACK_INPUT_CHANNELS] = {0};
    jack_default_audio_sample_t* out_bufs[MAX_JACK_OUTPUT_CHANNELS] = {0};

    for(int i = 0; i < num_inputs; i++)
        in_bufs[i] = (float*)jack_port_get_buffer(inputs[i], nframes);
    for(int i = 0; i < num_outputs; i++)
        out_bufs[i] = (float*)jack_port_get_buffer(outputs[i], nframes);

    process_buffers(in_bufs, out_bufs, num_inputs, num_outputs, nframes);

    return 0;
}

void JackShutdownFallback(void* arg) {
    BWAudioBackend::Deactivate();
    client = 0;
}

static BWError JackInitEnv() {
    if(!client) {
        jack_status_t status;
        client = jack_client_open(client_name, JackNullOption, &status, server_name);
        if(!client)
            return BW_FAILED;
        if(status & JackNameNotUnique) {
            client_name = jack_get_client_name(client);
            BW_LOG_GEN("Multiple instances of Bytewave detected, using Jack client name %s", client_name);
        }
        //if(status & JackServerStarted) (void)0;
    }

    return BW_OK;
}

BWError BWAudioBackend::Jack::Activate() {
    BWError ret = JackInitEnv();
    if(ret != BW_OK) return ret;

    auto input_devs = BWAudioBackend::GetAudioDevices(true);
    auto output_devs = BWAudioBackend::GetAudioDevices(false);
    for(auto dev : input_devs)
        inputs.push_back(jack_port_by_name(client, dev.device_name));
    for(auto dev : output_devs)
        inputs.push_back(jack_port_by_name(client, dev.device_name));

    jack_set_process_callback(client, JackProcessFunction, 0);

    jack_on_shutdown(client, JackShutdownFallback, 0);

    return BW_OK;
}

void BWAudioBackend::Jack::Deactivate() {
    if(client) {
        jack_client_close(client);
        client = 0;
    }
}

std::vector<BWAudioDevice> BWAudioBackend::Jack::QueryDevices() {
    if(JackInitEnv() != BW_OK)
        return {};

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
            if(!IsAudioPort(in_ports[i]))
                continue;
            out.push_back({});
            out[i].is_input = true;
            strncpy(out[i].device_name, in_ports[i], max_read_length);
        }
        jack_free(in_ports);
    }

    if(out_ports) {
        for(int i = 0; i < num_outputs; i++) {
            if(!IsAudioPort(out_ports[i]))
                continue;
            out.push_back({});
            out[out.size() - 1].is_output = true;
            strncpy(out[out.size() - 1].device_name, out_ports[i], max_read_length);
        }
        jack_free(out_ports);
    }

    return out;
}
