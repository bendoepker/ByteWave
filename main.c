#include <stdio.h>
#include <stdlib.h>
//#include "src/hostapi/bw_hostapi.h"

#include "include/bw_win_consts.h"

#include <windows.h>
#include <initguid.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>

void handleHResult(HRESULT hres) {
    if(hres != S_OK) { printf("%ld\n", hres); exit(0); }
}

int main(void) {
    HRESULT hres = S_OK;
    IMMDeviceEnumerator* device_enumerator = NULL;
    IMMDeviceCollection* device_collection = NULL;
    IMMDevice* device = NULL;

    hres = CoInitialize(NULL);
    handleHResult(hres);

    hres = CoCreateInstance(&CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, &IID_IMMDeviceEnumerator, (void**)&device_enumerator);
    handleHResult(hres);

    hres = device_enumerator->lpVtbl->EnumAudioEndpoints(device_enumerator, eCapture, DEVICE_STATE_ACTIVE, &device_collection);
    
    handleHResult(hres);

    UINT num_devices = 0;
    hres = device_collection->lpVtbl->GetCount(device_collection, &num_devices);
    handleHResult(hres);

    for(int i = 0; i < num_devices; i++) {
        hres = device_collection->lpVtbl->Item(device_collection, i, &device);
        handleHResult(hres);
        IPropertyStore* props;
        hres = device->lpVtbl->OpenPropertyStore(device, STGM_READ, &props);
        handleHResult(hres);

        PROPVARIANT name;
        PropVariantInit(&name);
        hres = props->lpVtbl->GetValue(props, &PKEY_Device_FriendlyName, &name);
        handleHResult(hres);

        if(name.vt != VT_EMPTY) printf("%d: %ls\n", i, name.pwszVal);
    }
}
