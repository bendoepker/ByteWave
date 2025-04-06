#include <bw-wasapi.h>
#include <stdio.h>
#include <bw-log.h>

//These two must be together in this order in whatever file uses the latter constants
//in order to link to uuid without errors from GCC
#include <initguid.h>
#include <functiondiscoverykeys_devpkey.h>

//WARN: ASIO TESTING
#include "lib/ASIOSDK/common/asio.h"
#include <bw-asio.h>
#include <bw-asio-il.h>

int main(void) {
    BW_PRINT("Beginning ASIO Test");

    BWAsioInitialize();
    asio_device* devs;
    uint32_t num_devs;
    BWError err = BWAsioQueryDevices(&devs, &num_devs);
    if(err != BW_OK) {
        BW_LOG_ERR("BW Error: %d", err);
        return -1;
    }
    BW_PRINT("Numer of Asio Devices: %d", num_devs);
    for(int i = 0; i < num_devs; i++) {
        BW_PRINT("Device %d: %s", devs[i].device_index, devs[i].name);
    }

    BWAsioTerminate();

//PERF: THIS IS THE END OF ASIO TESTING
exit(0);

    //NOTE: Testing WASAPI
    BWError res = BWWASAPIInitialize();

    //NOTE: Testing WASAPIQueryDevices()
    wasapi_devices devices;
    res = BWWASAPIQueryDevices(&devices);
    if(res != BW_OK) printf("Error: %d", res);
    for(int i = 0; i < devices.num_capture_devices; i++) {
        IPropertyStore* dev_props = NULL;
        HRESULT hres = devices.capture_devices[i]->lpVtbl->OpenPropertyStore(devices.capture_devices[i],
                                                                             STGM_READ,
                                                                             &dev_props);
        if(hres != S_OK) {
            printf("Windows Error: %ld", hres);
            exit(0);
        }
        PROPVARIANT name;
        hres = dev_props->lpVtbl->GetValue(dev_props, &PKEY_Device_FriendlyName, &name);
        if(hres != S_OK) {
            printf("Windows Error: %ld", hres);
            exit(0);
        }
        if(name.vt != VT_EMPTY) printf("%d: %ls\n", i, name.pwszVal);
    }

    for(int i = 0; i < devices.num_render_devices; i++) {
        IPropertyStore* dev_props = NULL;
        HRESULT hres = devices.render_devices[i]->lpVtbl->OpenPropertyStore(devices.render_devices[i],
                                                                            STGM_READ,
                                                                            &dev_props);
        if(hres != S_OK) {
            printf("Windows Error: %ld", hres);
            exit(0);
        }
        PROPVARIANT name;
        hres = dev_props->lpVtbl->GetValue(dev_props, &PKEY_Device_FriendlyName, &name);
        if(hres != S_OK) {
            printf("Windows Error: %ld", hres);
            exit(0);
        }
        if(name.vt != VT_EMPTY) printf("%d: %ls\n", i, name.pwszVal);
    }
    //NOTE: End testing WASAPIQueryDevices()

    //TODO: Begin testing input / output
    //      Create capture / render functions

    wasapi_stream_params* stream_params = NULL; //malloc(sizeof(wasapi_stream_params));
    res = BWWASAPIOpenStream(&stream_params);
    if(res != BW_OK) printf("Error: %d\n", res);

    //NOTE: Testing WAVEFORMAT PARAMS
    printf("Num Channels: %d\n", stream_params->audio_format->nChannels);
    printf("Bit Depth: %d\n", stream_params->audio_format->wBitsPerSample);
    printf("Format Tag (1 is PCM): %d\n", stream_params->audio_format->wFormatTag);
    printf("Sample Rate: %ld\n", stream_params->audio_format->nSamplesPerSec);
    printf("Block Align: %d\n", stream_params->audio_format->nBlockAlign);
    printf("Buffer Frame Count: %d\n", stream_params->buffer_frame_count);

    //Calculate start time in nsec
    //
    //Process buffers
    //
    //Calculate end time in nsec
    //
    //end - start = duration of audio processing
    //
    //buffer_len / sample_rate = frame time
    //
    //frame time - duration time = sleep time
    //
    //sleep for sleep time

    IPropertyStore* dev_props;
    HRESULT hres = stream_params->capture_device->lpVtbl->
        OpenPropertyStore(stream_params->capture_device,
                          STGM_READ,
                          &dev_props);
    if(hres != S_OK) {
        printf("Failed open property store\n");
        hres = S_OK;
    }
    else {
        PROPVARIANT name;
        dev_props->lpVtbl->GetValue(dev_props, &PKEY_Device_FriendlyName, &name);
        if(name.vt != VT_EMPTY) printf("Current Input Device: %ls\n", name.pwszVal);
    }


    unsigned int packet_size = 0;
    unsigned int buffer_frames_available = 0;
    REFERENCE_TIME duration = 0;
    const REFERENCE_TIME rt_one_second = 10000000;
    const REFERENCE_TIME rt_one_millisecond = 10000;

    while(1) {
        //TODO: calculate sleep duration
        Sleep(0);
        //Clears the current line and moves cursor to the beginning
        printf("\r");
        printf("\x1b[2K");
        int num_blocks = 0;

        HRESULT hres = stream_params->capture_client->lpVtbl->GetNextPacketSize(stream_params->capture_client,
                                                                                &packet_size);


        for(int i = 0; i < num_blocks; i++) {
            printf("0");
        }
    }

    res = BWWASAPICloseStream(&stream_params);
    if(res != BW_OK) printf("Error: %d\n", res);

    res = BWWASAPITerminate();
    if(res != BW_OK) printf("Error: %d\n", res);
    printf("end\n");
}
