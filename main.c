#include <bw_wasapi.h>
#include <stdio.h>

#include <functiondiscoverykeys_devpkey.h>

int main(void) {

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

    //Open audio stream
    //
    //Something (looped)
    //
    //Close stream
    wasapi_stream_params* stream_params = NULL;
    res = BWWASAPIOpenStream(stream_params);
    if(res != BW_OK) printf("Error: %d\n", res);

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
    while(1) {

    }

    res = BWWASAPITerminate();
    if(res != BW_OK) printf("Error: %d\n", res);
}
