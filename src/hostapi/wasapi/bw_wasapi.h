#include <windows.h>
#include <bw_types.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>

typedef struct {
    IMMDevice* capture_device;
    IMMDevice* render_device;
    IAudioCaptureClient* capture_client;
    IAudioRenderClient* render_client;
} wasapi_stream_params;

typedef struct {
    unsigned int num_capture_devices;
    unsigned int num_render_devices;
    IMMDevice** capture_devices;
    IMMDevice** render_devices;
} wasapi_devices;

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
BWError BWWASAPIQueryDevices(wasapi_devices* devices);
