#include "bw-wasapi.h"

/* For Logging */
#include <bw-log.h>

/* For wcstombs() */
#include <stdlib.h>

/* Windows Macros and Headers */
#include <bw-win-consts.h>
#include <windows.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>

/* For increasing thread priority */
#include <avrt.h>

/* For spawning the audio thread */
#include <bw-threads.h>

/* These two must be included wherever the PKEY constants are used */
#include <initguid.h>
#include <functiondiscoverykeys_devpkey.h>

typedef struct {
    IMMDevice* output_device;
    IMMDevice* input_device;
    uint16_t num_input_channels;
    uint16_t num_output_channels;
    uint16_t num_channels;

    uint32_t sample_rate;
    uint32_t buffer_size;

} _wasapi_device_attributes;

//Globals
_wasapi_device_attributes device_attr = {0};

void _wasapi_internal_process_loop() {
    //WARN: This runs in a separate thread with higher priority!

    DWORD task_index = 0;
    HANDLE mmcss_handle = AvSetMmThreadCharacteristics("Pro Audio", &task_index);
    if(mmcss_handle == 0) {
        //Failed to set thread characteristics
        //TODO:
        return;
    }
    AvSetMmThreadPriority(mmcss_handle, AVRT_PRIORITY_HIGH);

    while(1 /* Add a Way Out */ ) {
        //TODO: Processing
    }


    AvRevertMmThreadCharacteristics(mmcss_handle);
}

static inline BWError _translate_win_err(HRESULT hres) {
    switch(hres) {
        case S_OK:
            return BW_OK;
        case S_FALSE:
            return BW_FAILED;
        case E_OUTOFMEMORY:
            return BW_FAILED_MALLOC;
        case E_INVALIDARG:
            return BW_BAD_PARAMS;
        default:
            return BW_FAILED;
    }
}

BWError BWWASAPI_Activate(BWHostApi_AudioDevice* _audio_device) {
    HRESULT hres;
    IMMDeviceEnumerator* enumerator = 0;
    IMMDeviceCollection* collection = 0;
    IMMDevice* input_device = 0;
    IPropertyStore* props = 0;

    hres = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if(hres != S_OK) return BW_WINDOWS_ERROR;

    //NOTE: We will always use the default output endpoint and the selected device will
    //      contain the input device, these should be opened in exclusive mode for minimum latency
    //SECT: Find the input and output device
    hres = CoCreateInstance(&BW_CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL,
                            &BW_IID_IMMDeviceEnumerator, (void**)&enumerator);
    if(hres != S_OK) goto error;

    //Output device
    hres = enumerator->lpVtbl->GetDefaultAudioEndpoint(enumerator, eRender, DEVICE_STATE_ACTIVE, &device_attr.output_device);
    if(hres != S_OK) goto error;

    //Input device collection
    hres = enumerator->lpVtbl->EnumAudioEndpoints(enumerator, eCapture, DEVICE_STATE_ACTIVE, &collection);
    if(hres != S_OK) goto error;

    //Find the device in the collection
    UINT count = 0;
    hres = collection->lpVtbl->GetCount(collection, &count);
    if(hres != S_OK) goto error;

    for(int i = 0; i < count; i++) {
        hres = collection->lpVtbl->Item(collection, i, &input_device);
        if(hres != 0) goto error;

        input_device->lpVtbl->OpenPropertyStore(input_device, STGM_READ, &props);
        if(hres != 0) goto error;

        PROPVARIANT name;
        PropVariantInit(&name);

        hres = props->lpVtbl->GetValue(props, &PKEY_Device_FriendlyName, &name);
        if(hres != 0) goto error;

        if(name.vt != VT_EMPTY) {
            char buf[128] = {0};
            wcstombs(buf, name.pwszVal, 128);
            if(strncmp(_audio_device->device_name, buf, 128) == 0) {
                //We found the device, put it into the device_attr struct
                device_attr.input_device = input_device;
                PropVariantClear(&name);
                break;
            }
        }
        PropVariantClear(&name);
        input_device->lpVtbl->Release(input_device);
    }

    //Release all temporary heap variables
    if(props != 0) props->lpVtbl->Release(props);
    if(enumerator != 0) enumerator->lpVtbl->Release(enumerator);
    if(collection != 0) collection->lpVtbl->Release(collection);
    return BW_OK;

    //On error release all heap variables including those exposed globally
error:
    if(props != 0) props->lpVtbl->Release(props);
    if(device_attr.output_device != 0) device_attr.output_device->lpVtbl->Release(device_attr.output_device);
    device_attr.output_device = 0;
    if(device_attr.input_device != 0) device_attr.input_device->lpVtbl->Release(device_attr.input_device);
    device_attr.input_device = 0;
    if(enumerator != 0) enumerator->lpVtbl->Release(enumerator);
    if(collection != 0) collection->lpVtbl->Release(collection);
    return _translate_win_err(hres);
}

BWError BWWASAPI_Deactivate() {

    CoUninitialize();
    device_attr.output_device->lpVtbl->Release(device_attr.output_device);
    device_attr.input_device->lpVtbl->Release(device_attr.input_device);

    return BW_OK;
}

BWError BWWASAPI_QueryDevices(_wasapi_device** devices, uint32_t* num_devices) {
    HRESULT hres;
    IMMDeviceEnumerator* enumerator = 0;
    IMMDeviceCollection* dev_collection = 0;
    IMMDevice* dev = 0;
    IPropertyStore* props = 0;

    hres = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if(hres != S_OK) return _translate_win_err(hres);

    hres = CoCreateInstance(&BW_CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL,
                            &BW_IID_IMMDeviceEnumerator, (void**)&enumerator);
    if(hres != S_OK || enumerator == 0) goto exit;

    hres = enumerator->lpVtbl->EnumAudioEndpoints(enumerator, eCapture, DEVICE_STATE_ACTIVE, &dev_collection);
    if(hres != S_OK || dev_collection == 0) goto exit;

    hres = dev_collection->lpVtbl->GetCount(dev_collection, num_devices);
    if(hres != S_OK) goto exit;
    if(*num_devices == 0) {
        *devices = 0;
        return BW_OK; //NOTE: Maybe this should be BW_NO_DEVICES, but that would require refactoring BWHostApi_Initizalize()
    }

    *devices = malloc(sizeof(**devices) * *num_devices);
    if(*devices == 0) {
        free(enumerator);
        free(dev_collection);
        return BW_FAILED_MALLOC;
    }
    memset(*devices, 0, sizeof(**devices) * *num_devices);

    for(int i = 0; i < *num_devices; i++) {
        hres = dev_collection->lpVtbl->Item(dev_collection, i, &dev);
        if(hres != S_OK || dev == 0) goto exit;

        hres = dev->lpVtbl->OpenPropertyStore(dev, STGM_READ, &props);
        if(hres != S_OK || props == 0) goto exit;

        PROPVARIANT name;
        PropVariantInit(&name);
        hres = props->lpVtbl->GetValue(props, &PKEY_Device_FriendlyName, &name);
        if(hres != S_OK) goto exit;

        if(name.vt != VT_EMPTY) {
            //Convert the name from wide char set to char
            wcstombs((*devices)[i].name, name.pwszVal, 128);
        }
        PropVariantClear(&name);
        dev->lpVtbl->Release(dev);
    }

exit:
    if(props != 0) props->lpVtbl->Release(props);
    if(dev != 0) dev->lpVtbl->Release(dev);
    if(dev_collection != 0) dev_collection->lpVtbl->Release(dev_collection);
    if(enumerator != 0) enumerator->lpVtbl->Release(enumerator);
    CoUninitialize();
    if(hres != S_OK) return _translate_win_err(hres);
    return BW_OK;
}
