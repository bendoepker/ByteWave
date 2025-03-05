#include <windows.h>
#include <bw_types.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>

typedef struct {
    IMMDevice* device_ptr;
    IAudioCaptureClient* capture_client;
    IAudioRenderClient* render_client;
} wasapi_clients;

/*
    * TODO: Add description, return types, parameters
*/
BWError BWWASAPIInitialize();

/*
    * TODO: Add description, return types, parameters
*/
BWError BWWASAPITerminate();

/*
    * TODO: Add description, return types, parameters
*/
BWError BWWASAPIQueryDevices();
