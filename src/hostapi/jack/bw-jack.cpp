#include "bw-jack.h"
#include <jack/jack.h>
#include <log.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include "../process/process.h"
#include <hostapi.h>

#define MAX_JACK_INPUT_CHANNELS 32
#define MAX_JACK_OUTPUT_CHANNELS 2

typedef struct {
    jack_port_t* sw_port;
    jack_port_t* hw_port;
    std::string hw_port_name;
    std::string sw_port_name;
} JackDeviceContainer;

static jack_client_t* client = NULL;
static const char* client_name = "ByteWave";
static const char* server_name = NULL;
static std::vector<JackDeviceContainer> inputs;
static std::vector<JackDeviceContainer> outputs;

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
        in_bufs[i] = (float*)jack_port_get_buffer(inputs[i].sw_port, nframes);
    for(int i = 0; i < num_outputs; i++)
        out_bufs[i] = (float*)jack_port_get_buffer(outputs[i].sw_port, nframes);

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
        inputs.push_back({.hw_port = jack_port_by_name(client, dev.device_name)});
    for(auto dev : output_devs)
        outputs.push_back({.hw_port = jack_port_by_name(client, dev.device_name)});

    // Set up the software ports that we will read and write audio data to and from
    for(int i = 0; i < inputs.size(); i++) {
        inputs[i].hw_port_name = std::string(input_devs[i].device_name);
        char buf[128] = {0};
        sprintf(buf, "input:%d", i);
        inputs[i].sw_port_name = std::string(buf);
        inputs[i].sw_port = jack_port_register(client, inputs[i].sw_port_name.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
        BW_PRINT("%s\n%s\n%p\n%p\n", inputs[i].sw_port_name.c_str(), inputs[i].hw_port_name.c_str(), inputs[i].sw_port, inputs[i].hw_port);
    }
    for(int i = 0; i < outputs.size(); i++) {
        outputs[i].hw_port_name = std::string(output_devs[i].device_name);
        char buf[128] = {0};
        sprintf(buf, "output:%d", i);
        outputs[i].sw_port_name = std::string(buf);
        outputs[i].sw_port = jack_port_register(client, outputs[i].sw_port_name.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
        BW_PRINT("%s\n%s\n%p\n%p\n", outputs[i].sw_port_name.c_str(), outputs[i].hw_port_name.c_str(), outputs[i].sw_port, outputs[i].hw_port);
    }

    if(jack_set_process_callback(client, JackProcessFunction, 0)) {
        BW_LOG_ERR("Failed to set Jack process callback");
        return BW_FAILED;
    }

    jack_on_shutdown(client, JackShutdownFallback, 0);

    if(jack_activate(client)) {
        BW_LOG_ERR("Failed to activate Jack client");
        return BW_FAILED;
    }

    for(auto in : inputs) {
        int ret = jack_connect(client, jack_port_name(in.hw_port), jack_port_name(in.sw_port));
        if(ret)
            BW_LOG_ERR("Could not connect port %s to port %s", in.hw_port_name.c_str(), in.sw_port_name.c_str());
        if(ret == EEXIST)
            BW_LOG_ERR("Port %s is already connected to port %s", in.hw_port_name.c_str(), in.sw_port_name.c_str());
    }

    for(auto out : outputs) {
        int ret = jack_connect(client, jack_port_name(out.sw_port), jack_port_name(out.hw_port));
        if(ret)
            BW_LOG_ERR("Could not connect port %s to port %s", out.sw_port_name.c_str(), out.hw_port_name.c_str());
        if(ret == EEXIST)
            BW_LOG_ERR("Port %s is already connected to port %s", out.sw_port_name.c_str(), out.hw_port_name.c_str());
    }

    return BW_OK;
}

void BWAudioBackend::Jack::Deactivate() {
    if(client) {
        jack_deactivate(client);
        jack_client_close(client);
        client = 0;
    }
}

std::vector<BWAudioDevice> BWAudioBackend::Jack::QueryDevices() {
    if(JackInitEnv() != BW_OK)
        return {};

    std::vector<BWAudioDevice> out;

    const char** in_ports = jack_get_ports(client, NULL, NULL, JackPortIsPhysical | JackPortIsOutput);
    const char** out_ports = jack_get_ports(client, NULL, NULL, JackPortIsPhysical | JackPortIsInput);

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
