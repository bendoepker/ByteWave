#include "bw-wasapi.h"
#include <bw-win-consts.h>

#include <stdio.h>

BWError BWWASAPIInitialize() {
    BWError result = BW_OK;
    HRESULT hres;
    hres = CoInitialize(NULL);
    if(hres != S_OK) return BW_WINDOWS_ERROR;

    return result;
}

BWError BWWASAPITerminate() {
    BWError result = BW_OK;

    CoUninitialize();

    return result;
}

//PERF: Working as intended
BWError BWWASAPIQueryDevices(wasapi_devices* devices) {
    //Initial Variables
    BWError result = BW_OK;
    HRESULT hres = S_OK;

    //Device enumerator to get all devices
    IMMDeviceEnumerator* device_enumerator = NULL;
    hres = CoCreateInstance(&BW_CLSID_MMDeviceEnumerator, 
                     NULL,
                     CLSCTX_ALL, 
                     &BW_IID_IMMDeviceEnumerator, 
                     (void**)&device_enumerator);
    if(hres != S_OK) {
        device_enumerator->lpVtbl->Release(device_enumerator);
        return BW_WINDOWS_ERROR;
    }

    //Device collection holding all capture devices
    IMMDeviceCollection* capture_device_collection = NULL;
    hres = device_enumerator->lpVtbl->EnumAudioEndpoints(device_enumerator,
                                                         eCapture,
                                                         DEVICE_STATE_ACTIVE,
                                                         &capture_device_collection);
    if(hres != S_OK) {
        device_enumerator->lpVtbl->Release(device_enumerator);
        capture_device_collection->lpVtbl->Release(capture_device_collection);
        return BW_WINDOWS_ERROR;
    }

    //Device collection holding all render devices
    IMMDeviceCollection* render_device_collection = NULL;
    hres = device_enumerator->lpVtbl->EnumAudioEndpoints(device_enumerator,
                                                         eRender,
                                                         DEVICE_STATE_ACTIVE,
                                                         &render_device_collection);
    if(hres != S_OK) {
        device_enumerator->lpVtbl->Release(device_enumerator);
        capture_device_collection->lpVtbl->Release(capture_device_collection);
        render_device_collection->lpVtbl->Release(render_device_collection);
        return BW_WINDOWS_ERROR;
    }

    //At this point 'render_device_collection' and 'capture_device_collection'
    //contain references to output and input devices respectively

    //Populate the output struct with number of devices
    devices->num_render_devices = 0;
    hres = render_device_collection->lpVtbl->GetCount(render_device_collection,
                                                      &devices->num_render_devices);
    if(hres != S_OK) {
        device_enumerator->lpVtbl->Release(device_enumerator);
        capture_device_collection->lpVtbl->Release(capture_device_collection);
        render_device_collection->lpVtbl->Release(render_device_collection);
        return BW_WINDOWS_ERROR;
    }

    devices->num_capture_devices = 0;
    hres = capture_device_collection->lpVtbl->GetCount(capture_device_collection,
                                                       &devices->num_capture_devices);
    if(hres != S_OK) {
        device_enumerator->lpVtbl->Release(device_enumerator);
        capture_device_collection->lpVtbl->Release(capture_device_collection);
        render_device_collection->lpVtbl->Release(render_device_collection);
        return BW_WINDOWS_ERROR;
    }

    //Populate the output with references to the devices
    devices->render_devices = (IMMDevice**)malloc(devices->num_render_devices * sizeof(IMMDevice*));
    if(devices->render_devices == NULL) return BW_FAILED;
    for(int i = 0; i < devices->num_render_devices; i++) {
        IMMDevice* device = NULL;
        hres = render_device_collection->lpVtbl->Item(render_device_collection,
                                                      i,
                                                      &device);
        if(hres != S_OK) {
            device_enumerator->lpVtbl->Release(device_enumerator);
            capture_device_collection->lpVtbl->Release(capture_device_collection);
            render_device_collection->lpVtbl->Release(render_device_collection);
            return BW_WINDOWS_ERROR;
        }
        devices->render_devices[i] = device;
    }

    devices->capture_devices = (IMMDevice**)malloc(devices->num_capture_devices * sizeof(IMMDevice*));
    if(devices->capture_devices == NULL) return BW_FAILED;
    for(int i = 0; i < devices->num_capture_devices; i++){
        IMMDevice* device = NULL;
        hres = capture_device_collection->lpVtbl->Item(capture_device_collection,
                                                       i,
                                                       &device);
        if(hres != S_OK) {
            device_enumerator->lpVtbl->Release(device_enumerator);
            capture_device_collection->lpVtbl->Release(capture_device_collection);
            render_device_collection->lpVtbl->Release(render_device_collection);
            return BW_WINDOWS_ERROR;
        }
        devices->capture_devices[i] = device;
    }

    //Release Querying COM objects
    device_enumerator->lpVtbl->Release(device_enumerator);
    capture_device_collection->lpVtbl->Release(capture_device_collection);
    render_device_collection->lpVtbl->Release(render_device_collection);
    return result;
}

//TODO: Create mode selection via parameters for input and output
BWError BWWASAPIOpenStream(wasapi_stream_params** stream_params) {
    BWError result = BW_OK;

    //WARN: This may need to change depending on implementation
    //      But we're gonna roll with it for the time being
    (*stream_params) = malloc(sizeof(wasapi_stream_params));
    if(*stream_params == NULL) return BW_FAILED;

    (*stream_params)->audio_client = NULL;
    (*stream_params)->render_client = NULL;
    (*stream_params)->capture_client = NULL;
    (*stream_params)->capture_device = NULL;
    (*stream_params)->render_device = NULL;
    (*stream_params)->audio_format = NULL;
    (*stream_params)->buffer_frame_count = 0;

    IMMDeviceEnumerator* enumerator = NULL;

    HRESULT hres = CoCreateInstance(&BW_CLSID_MMDeviceEnumerator,
                                    NULL,
                                    CLSCTX_ALL,
                                    &BW_IID_IMMDeviceEnumerator,
                                    (void**)&enumerator);
    if(hres != S_OK) {
        enumerator->lpVtbl->Release(enumerator);
        return BW_FAILED;
    }

    //TODO: Using the default output, make it selectable
    //      Set to eCapture, enable one for eRender
    hres = enumerator->lpVtbl->GetDefaultAudioEndpoint(enumerator,
                                                       eCapture,
                                                       eConsole,
                                                       &(*stream_params)->capture_device);
    if(hres != S_OK) {
        enumerator->lpVtbl->Release(enumerator);
        return BW_FAILED;
    }

    hres = (*stream_params)->capture_device->lpVtbl->Activate((*stream_params)->capture_device,
                                                              &BW_IID_IAudioClient,
                                                              CLSCTX_ALL,
                                                              NULL,
                                                              (void**)&(*stream_params)->audio_client);
    if(hres != S_OK) {
        enumerator->lpVtbl->Release(enumerator);
        return BW_FAILED;
    }

    hres = (*stream_params)->audio_client->lpVtbl->GetMixFormat((*stream_params)->audio_client,
                                                                &(*stream_params)->audio_format);
    if(hres != S_OK) {
        enumerator->lpVtbl->Release(enumerator);
        return BW_FAILED;
    }

    hres = (*stream_params)->audio_client->lpVtbl->Initialize((*stream_params)->audio_client,
                                                              AUDCLNT_SHAREMODE_SHARED,
                                                              0,
                                                              10000000, //Requested Duration
                                                              0, //Must be zero in shared mode
                                                              (*stream_params)->audio_format,
                                                              NULL);
    if(hres != S_OK) {
        enumerator->lpVtbl->Release(enumerator);
        return BW_FAILED;
    }

    hres = (*stream_params)->audio_client->lpVtbl->GetBufferSize((*stream_params)->audio_client,
                                                                 &(*stream_params)->buffer_frame_count);
    if(hres != S_OK) {
        enumerator->lpVtbl->Release(enumerator);
        return BW_FAILED;
    }

    hres = (*stream_params)->audio_client->lpVtbl->GetService((*stream_params)->audio_client,
                                                              &BW_IID_IAudioCaptureClient,
                                                              (void**)&(*stream_params)->capture_client);
    if(hres != S_OK) {
        enumerator->lpVtbl->Release(enumerator);
        return BW_FAILED;
    }

    //Free the helper objects
    enumerator->lpVtbl->Release(enumerator);

    hres = (*stream_params)->audio_client->lpVtbl->Start((*stream_params)->audio_client);
    if(hres != S_OK) result = BW_FAILED;

    return result;
}

//PERF: Done
//NOTE: releases all windows resources and frees the memory from the heap afterwards
//      Not thread safe (alloc / dealloc)
BWError BWWASAPICloseStream(wasapi_stream_params** stream_params) {
    BWError result = BW_OK;


    if((*stream_params) == NULL) {
        return BW_UNINITIALIZED;
    }

    HRESULT hres = S_OK;

    hres = (*stream_params)->audio_client->lpVtbl->Stop((*stream_params)->audio_client);
    if(hres != S_OK) return BW_FAILED;

    //Render Device
    if((*stream_params)->render_device != NULL) {
        (*stream_params)->render_device->lpVtbl->Release((*stream_params)->render_device);
        (*stream_params)->render_device = NULL;
    }
    //Capture Device
    if((*stream_params)->capture_device != NULL) {
        (*stream_params)->capture_device->lpVtbl->Release((*stream_params)->capture_device);
        (*stream_params)->capture_device = NULL;
    }
    //Audio Client
    if((*stream_params)->audio_client != NULL) {
        (*stream_params)->audio_client->lpVtbl->Release((*stream_params)->audio_client);
        (*stream_params)->audio_client = NULL;
    }
    //Render Client
    if((*stream_params)->render_client != NULL) {
        (*stream_params)->render_client->lpVtbl->Release((*stream_params)->render_client);
        (*stream_params)->render_client = NULL;
    }
    //Capture Client
    if((*stream_params)->capture_client != NULL) {
        (*stream_params)->capture_client->lpVtbl->Release((*stream_params)->capture_client);
        (*stream_params)->capture_client = NULL;
    }

    //NOTE: this also frees the waveformatex nested struct
    //      hence it is not freed above
    free((*stream_params));

    return result;
}
