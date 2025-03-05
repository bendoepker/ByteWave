#include "bw_wasapi.h"
#include <stdio.h>

#include <bw_win_consts.h>
#include <mmdeviceapi.h>
#include <windows.h>

BWError BWWASAPIInitialize() {
    BWError result = BW_OK;
    HRESULT hres;
    hres = CoInitialize(NULL);
    if(hres != S_OK) return BW_WINDOWS_ERROR;

    return result;
}

BWError BWWASAPITerminate() {
    BWError result = BW_OK;
    //TODO: Release all resources allocated in Initialize()

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
