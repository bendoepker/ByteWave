#include <stdlib.h>
#include <bw-log.h>
#include <string.h>
#include <bw-threads.h>
#include <bw-ui.h>
#include <stdio.h>

#include <bw-dynamic-array.h>

//These two must be together in this order in whatever file uses the latter constants
//in order to link to uuid without errors from GCC
//#include <initguid.h>
//#include <functiondiscoverykeys_devpkey.h>

#include "src/hostapi/bw-hostapi.h"

//WARN: Config Testing
#include <bw-config.h>

BWThread thread_pool[1];

void print_da(d_array* da) {
    BW_PRINT("-----------------------");
    BW_PRINT("Array Capacity: %d", da->array_capacity);
    BW_PRINT("Array Length: %d", da->array_length);
    BW_PRINT("Array Empty Space: %d", da->array_empty_space);
    BW_PRINT("Element Size: %d", da->element_size);
    BW_PRINT("Array:");
    for(int i = 0; i < da->array_length; i++) {
        BW_PRINT("Char %d: %c", i, *(char*)DArrayAt(da, i));
    }
    BW_PRINT("-----------------------");
}

int main(void) {

    d_array da = {0};
    BWError te = DArrayCreate(&da, sizeof(char), 5);
    if(te) return -1;

    BW_PRINT("Append 'a' 'b' 'c'");
    DArrayAppend(&da, (char[]){'a', 'b', 'c'}, 3);
    print_da(&da);

    BW_PRINT("Append 'd' 'e' 'f'");
    DArrayAppend(&da, (char[]){'d', 'e', 'f'}, 3);
    print_da(&da);

    BW_PRINT("PopBack 4");
    DArrayPopBack(&da, 4);
    print_da(&da);

    BW_PRINT("Insert 'g' at index 1");
    DArrayInsert(&da, (char[]){'g', 'h'}, 2, 1);
    print_da(&da);

    BW_PRINT("Append 'i' 'j' 'k' 'l' 'm'");
    DArrayAppend(&da, (char[]){'i', 'j', 'k', 'l', 'm'}, 5);
    print_da(&da);

    BW_PRINT("Insert 'n' 'o' at index 3");
    DArrayInsert(&da, (char[]){'n', 'o'}, 2, 3);
    print_da(&da);

    BW_PRINT("Remove 'g' 'h'");
    DArrayRemove(&da, 2, 1);
    print_da(&da);

    BW_PRINT("Search for 'a' 'm' 'i' 'x'");
    BW_PRINT("-----------------------");
    BW_PRINT("'a': %d", DArraySearch(&da, (char[]){'a'}));
    BW_PRINT("'m': %d", DArraySearch(&da, (char[]){'m'}));
    BW_PRINT("'i': %d", DArraySearch(&da, (char[]){'i'}));
    BW_PRINT("'x': %d", DArraySearch(&da, (char[]){'x'}));
    BW_PRINT("-----------------------");
    BW_PRINT("Print Array Memory as hex");
    BW_PRINT("-----------------------");
    for(int i = 0; i < da.array_capacity; i++) {
        printf("0x%.2x ", *(char*)(da.array + i));
    }

    exit(0);
    BWUIData* ui_data = malloc(sizeof *ui_data);

    BWConfigData* conf_data = malloc(sizeof *conf_data);
    BWError_Handle(BWConfig_Read("test-config.bwc", conf_data));
    ui_data->config_data = conf_data;
    BW_LOG_GEN("%s", conf_data->device_name);

    //SECTION: Create the UI thread
    BWFunctionData func_data = {.function = BWUI_UIMain, .data = (void*)ui_data};
    BWThread ui_thread = BWUtil_CreateThread(&func_data, BW_NORMAL_PRIORITY);
    thread_pool[0] = ui_thread;

    BW_PRINT("Beginning ASIO Test");
    BWError_Handle(BWHostApi_Initialize(*conf_data));

    BWError_Handle(BWHostApi_Terminate());

    BW_PRINT("DONE");
    BWError_Handle(BWConfig_Write("test-config.bwc", conf_data));
    BWUtil_WaitForThreads(thread_pool, 1);
    free(conf_data->device_name);
    free(conf_data);
}

/*
exit(0);

    //NOTE: Testing WASAPI
    BWError res = BWWASAPI_Initialize();

    //NOTE: Testing WASAPIQueryDevices()
    _wasapi_devices devices;
    res = BWWASAPI_QueryDevices(&devices);
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

    _wasapi_stream_params* stream_params = NULL; //malloc(sizeof *_wasapi_stream_params);
    res = BWWASAPI_OpenStream(&stream_params);
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

    res = BWWASAPI_CloseStream(&stream_params);
    if(res != BW_OK) printf("Error: %d\n", res);

    res = BWWASAPI_Terminate();
    if(res != BW_OK) printf("Error: %d\n", res);
    printf("end\n");
    */
