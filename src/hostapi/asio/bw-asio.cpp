#include "bw-asio.h"
#include <asiosys.h>
#include <asio.h>
#include <asiodrivers.h>
#include <log.h>
#include <assert.h>
#include <conversions.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern AsioDrivers* asioDrivers;

#define ASIO_MAX_INPUT_CHANNELS 32
#define ASIO_MAX_OUTPUT_CHANNELS 2
#define ASIO_MAX_DRIVERS 32

typedef struct {
    char name[128];

    long num_input_channels;
    long num_output_channels;
    long num_channels;

    ASIOBufferInfo buffer_infos[ASIO_MAX_INPUT_CHANNELS + ASIO_MAX_OUTPUT_CHANNELS];
    long min_buffer_size;
    long max_buffer_size;
    long preferred_buffer_size;
    long buffer_granularity;

    double sample_rate;
    long input_latency;
    long output_latency;

    /* Optimize the processing by reporting when it finishes to the interface */
    long post_output;

    ASIOChannelInfo channel_infos[ASIO_MAX_INPUT_CHANNELS + ASIO_MAX_OUTPUT_CHANNELS];
    BWSampleTypes sample_format;

    ASIODriverInfo driver_info;
} BWAsioDevice;

BWAsioDevice active_device = {};

void bufferSwitch(long doubleBufferIndex, ASIOBool directProcess);
void sampleRateDidChange(ASIOSampleRate sRate);
long asioMessage(long selector, long value, void* message, double* opt);
ASIOTime* bufferSwitchTimeInfo(ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess);
ASIOCallbacks asioCallbacks = {bufferSwitch, sampleRateDidChange, asioMessage, bufferSwitchTimeInfo};

static void InitAsioEnv() {
    if(!asioDrivers)
        asioDrivers = new AsioDrivers();
}

static void TerminateAsioEnv() {
    if(asioDrivers) {
        delete asioDrivers;
        asioDrivers = 0;
    }
}

BWError BWAudioBackend::Asio::Activate(const char* driver_name) {
    InitAsioEnv();
    if(!asioDrivers)
        return BW_FAILED_MALLOC;

    if(driver_name)
        strncpy(active_device.name, driver_name, 32);
    else {
        char* drv_name = (char*)malloc(32);
        if(!asioDrivers->getDriverNames(&drv_name, 1)) {
            free(drv_name);
            return BW_FAILED;
        }
        strncpy(active_device.name, drv_name, 32);
        free(drv_name);
    }

    BW_LOG_GEN("Current Device: %s", active_device.name);

    if(!asioDrivers->loadDriver(active_device.name))
        return BW_FAILED;
    active_device.driver_info.sysRef = GetCurrentProcess();
    active_device.driver_info.asioVersion = 2;
    if(ASIOInit(&active_device.driver_info) != ASE_OK) {
        memset(active_device.name, 0, 128);
        asioDrivers->removeCurrentDriver();
        return BW_FAILED;
    }

    // If these fail then magic happened so I would rather crash than check the return value
    ASIOGetChannels(&active_device.num_input_channels, &active_device.num_output_channels);
    if(active_device.num_input_channels > ASIO_MAX_INPUT_CHANNELS)
        active_device.num_input_channels = ASIO_MAX_INPUT_CHANNELS;
    if(active_device.num_output_channels > ASIO_MAX_OUTPUT_CHANNELS)
        active_device.num_output_channels = ASIO_MAX_OUTPUT_CHANNELS;
    active_device.num_channels = active_device.num_output_channels + active_device.num_input_channels;

    ASIOGetBufferSize(&active_device.min_buffer_size, &active_device.max_buffer_size,
                      &active_device.preferred_buffer_size, &active_device.buffer_granularity);

    // Initializing buffer info structs
    for(int i = 0; i < active_device.num_input_channels; i++) {
        active_device.buffer_infos[i].isInput = ASIOTrue;
        active_device.buffer_infos[i].channelNum = i;
        active_device.buffer_infos[i].buffers[0] = 0;
        active_device.buffer_infos[i].buffers[1] = 0;
    }
    for(int i = 0; i < active_device.num_output_channels; i++) {
        const long nic = active_device.num_input_channels;
        active_device.buffer_infos[i + nic].isInput = ASIOTrue;
        active_device.buffer_infos[i + nic].channelNum = i;
        active_device.buffer_infos[i + nic].buffers[0] = 0;
        active_device.buffer_infos[i + nic].buffers[1] = 0;
    }

    if(ASIOGetSampleRate(&active_device.sample_rate) != ASE_OK) {
        BW_LOG_ERR("Could not determine sample rate, attempting to set the sample rate at 48000Hz");
        const double req_smp_rate = 48000.f;
        if(ASIOSetSampleRate(req_smp_rate) != ASE_OK) {
            BW_LOG_ERR("Sample rate could not be determined or set");
            asioDrivers->removeCurrentDriver();
            memset(&active_device, 0, sizeof(active_device));
            return BW_FAILED;
        } else {
            BW_LOG_GEN("Sample rate was set to %lf", req_smp_rate);
            active_device.sample_rate = req_smp_rate;
        }
    }

    BW_PRINT("Sample Rate: %f", active_device.sample_rate);
    BW_PRINT("Input Channels: %ld", active_device.num_input_channels);
    BW_PRINT("Output Channels: %ld", active_device.num_output_channels);
    BW_PRINT("Buffer Size: %ld", active_device.preferred_buffer_size);

    if(ASIOCreateBuffers(active_device.buffer_infos, active_device.num_channels,
                         active_device.preferred_buffer_size, &asioCallbacks) != ASE_OK) {
        BW_LOG_ERR("Failed to generate buffer infos");
        memset(&active_device, 0, sizeof(active_device));
        asioDrivers->removeCurrentDriver();
        ASIOExit();
        return BW_FAILED_MALLOC;
    }

    // Match the Channel Info Structures to the Buffer Info Structures
    for(int i = 0; i < active_device.num_channels; i++) {
        active_device.channel_infos[i].channel = active_device.buffer_infos[i].channelNum;
        active_device.channel_infos[i].channel = active_device.buffer_infos[i].isInput;
        ASIOGetChannelInfo(&active_device.channel_infos[i]);
        BW_PRINT("Channel %d Info", i);
        BW_PRINT("    Channel Number: %ld", active_device.channel_infos[i].channel);
        BW_PRINT("    Is Input: %ld", active_device.channel_infos[i].isInput);
        BW_PRINT("    Is Input of Assoc. Buf: %ld", active_device.buffer_infos[i].isInput);
        BW_PRINT("    Is Active: %ld", active_device.channel_infos[i].isActive);
        BW_PRINT("    Channel Group: %ld", active_device.channel_infos[i].channelGroup);
        BW_PRINT("    Sample Type: %ld", active_device.channel_infos[i].type);
        BW_PRINT("    Name: %s\n", active_device.channel_infos[i].name);
    }

    /*
        *WARN:
        * It isn't specified in the ASIOSDK that all of the devices' channels must have the
        * same input / output sample type format, for the time being all of the channels will
        * be given the same conversion function, if there is an interface which requires different
        * functions per channel this will have to be assigned on a channel by channel basis
    */

    switch(active_device.channel_infos[0].type) {
        case ASIOSTInt16LSB:
            active_device.sample_format = INT_16_BIT;
            break;
        case ASIOSTInt24LSB:
            active_device.sample_format = INT_24_BIT;
            break;
        case ASIOSTInt32LSB:
            active_device.sample_format = INT_32_BIT;
            break;
        case ASIOSTFloat32LSB:
            active_device.sample_format = FLOAT_32_BIT;
            break;
        default:
            //TODO: Add support for more sample types (especially Big Endian Types for mobile port)
            assert(1 && "Device sample type not supported\n");
            break;
    }

    ASIOGetLatencies(&active_device.input_latency, &active_device.output_latency);
    BW_LOG_GEN("Input Latency | Output Latency: %ld | %ld", active_device.input_latency, active_device.output_latency);

    if(ASIOOutputReady() == ASE_OK)
        active_device.post_output = 1;
    else
        active_device.post_output = 0;

    active_device.post_output ? BW_PRINT("Supports Post Output") : BW_PRINT("Does not support Post Output");

    if(ASIOStart() != ASE_OK) {
        ASIODisposeBuffers();
        ASIOExit();
        TerminateAsioEnv();
    }

    return BW_OK;
}

void BWAudioBackend::Asio::Deactivate() {
    if(asioDrivers) {
        ASIOStop();
        ASIODisposeBuffers();
        ASIOExit();
    }
    TerminateAsioEnv();
}

std::vector<BWAudioDevice> BWAudioBackend::Asio::QueryDevices() {
    long num_drivers = 0;
    char* driver_names[ASIO_MAX_DRIVERS];
    std::vector<BWAudioDevice> out = {};

    InitAsioEnv();
    if(!asioDrivers)
        return {};

    for(int i = 0; i < ASIO_MAX_DRIVERS; i++) {
        driver_names[i] = (char*)malloc(32); // 32 characters in each driver name
        if(!driver_names[i]) {
            for(int j = 0; j < i; j++)
                free(driver_names[j]);
            return {};
        }
    }

    num_drivers = asioDrivers->getDriverNames(driver_names, ASIO_MAX_DRIVERS);
    out.reserve(num_drivers);
    for(int i = 0; i < num_drivers; i++) {
        out.push_back({});
        memcpy(out[out.size() - 1].device_name, driver_names[i], 32);
    }

    for(int i = 0; i < ASIO_MAX_DRIVERS; i++) {
        free(driver_names[i]);
    }

    return out;
}

void bufferSwitch(long doubleBufferIndex, ASIOBool directProcess) {
    //Backdoor into the actual audio callback (bufferSwitchTimeInfo)

    //Taken from the ASIO2 SDK, explanation in ASIOSDK/host/sample/hostsample.cpp
    ASIOTime timeInfo = {0};

	if(ASIOGetSamplePosition(&timeInfo.timeInfo.samplePosition, &timeInfo.timeInfo.systemTime) == BW_OK)
		timeInfo.timeInfo.flags = kSystemTimeValid | kSamplePositionValid;

    bufferSwitchTimeInfo(&timeInfo, doubleBufferIndex, directProcess);
}

void sampleRateDidChange(ASIOSampleRate sRate) {
    //TODO: This, likely just update a conversion algorithm and always use a single internal sample rate
    //      (likely 48000)
}

long asioMessage(long selector, long value, void* message, double* opt) {
    //Unused as of ASIO 2.3
    (void)message;
    (void)opt;

    switch(selector) {
        case kAsioSelectorSupported:
            if(value == kAsioResetRequest
                || value == kAsioEngineVersion
                || value == kAsioResyncRequest
                || value == kAsioLatenciesChanged
                // the following three were added for ASIO 2.0, you don't necessarily have to support them
                || value == kAsioSupportsTimeInfo
                || value == kAsioSupportsTimeCode
                || value == kAsioSupportsInputMonitor)
                return 1L;
            else return 0;
        case kAsioBufferSizeChange:
            BWAudioBackend::Asio::Deactivate();
            BWAudioBackend::Asio::Activate(0);
            return 1L;
        case kAsioResetRequest:
            //TODO: Handle reset request, currently we will just crash
            assert(1 && "ASIO Reset Request was sent");
            break;
        case kAsioResyncRequest:
            return 1L;
        case kAsioLatenciesChanged:
            //NOTE: Ignored for the time being, should trigger a latency tracking update if
            //      latency is being tracked
            return 1L;
        case kAsioEngineVersion:
            return 2L;
        case kAsioSupportsTimeInfo:
            return 1L;
        case kAsioSupportsTimeCode:
            return 0; //NOTE: May need to keep track of this info, needs more research
        case kAsioSupportsInputMonitor:
            return 0;
    }
    return 0;
}

ASIOTime* bufferSwitchTimeInfo(ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess) {
    (void) directProcess; //Processing should never be deferred

    long buf_size = active_device.preferred_buffer_size;
    long num_inputs = active_device.num_input_channels;
    long num_outputs = active_device.num_output_channels;
    BWSampleTypes native_format = active_device.sample_format;
    ASIOBufferInfo* infos = active_device.buffer_infos;

    //Internal buffer declaration
    //FIX: Make these two buffers into a constant storage size
    float input_bufs[num_inputs][buf_size];
    float output_bufs[num_outputs][buf_size];

    //Convert the input buffers into floats
    for(int i = 0; i < num_inputs; i++, infos++) {
        BWUtil_ConvertToFloat(infos->buffers[doubleBufferIndex], input_bufs[i], native_format, buf_size);
    }

    //Processing

    /*
    *   At this point all of the derived parameters should be passed to the processing function,
    *   namely input_bufs, output_bufs, num_inputs, num_outputs; There should also be a timing
    *   function to roughly measure input overflow and output underflows
    *
    *   process_buffers(input_bufs, output_bufs, num_inputs, num_outputs, buf_size);
    */
    //Zero Initialize all of the output buffers
    for(int i = 0; i < num_outputs; i++) {
        memset(output_bufs[i], 0, buf_size * sizeof(float));

        //Add the inputs to the outputs
        for(int j = 0; j < num_inputs; j++) {
            for(int k = 0; k < buf_size; k++) {
                output_bufs[i][k] += input_bufs[j][k];
                if(output_bufs[i][k] > 1.0) output_bufs[i][k] = 1.0;
                if(output_bufs[i][k] < -1.0) output_bufs[i][k] = -1.0;
            }
        }
    }

    //Convert the output buffers to the native format
    for(int i = 0; i < num_outputs; i++, infos++) {
        BWUtil_ConvertFromFloat(output_bufs[i], infos->buffers[doubleBufferIndex], native_format, buf_size);
    }

    if(active_device.post_output)
        ASIOOutputReady();

    return 0;
}
