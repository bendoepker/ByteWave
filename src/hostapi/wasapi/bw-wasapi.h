#ifndef BW_WASAPI_H
#define BW_WASAPI_H

#include <windows.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>

//ByteWave
#include <bw-types.h>

typedef struct {
    IMMDevice* capture_device;
    IMMDevice* render_device;
    IAudioClient* audio_client;
    IAudioCaptureClient* capture_client;
    IAudioRenderClient* render_client;
    WAVEFORMATEX* audio_format;
    UINT32 buffer_frame_count;
} _wasapi_stream_params;

typedef struct {
    unsigned int num_capture_devices;
    unsigned int num_render_devices;
    IMMDevice** capture_devices;
    IMMDevice** render_devices;
} _wasapi_devices;

/*
    * TODO: Add description, return types, parameters
*/
BWError BWWASAPI_Initialize();

/*
    * TODO: Add description, return types, parameters
*/
BWError BWWASAPI_Terminate();

/*
    * TODO: Add description, return types, parameters
*/
BWError BWWASAPI_QueryDevices(_wasapi_devices* devices);

/*
    * TODO: Add description, return types, parameters
*/
BWError BWWASAPI_OpenStream(_wasapi_stream_params** stream_params);

/*
    * TODO: Add description, return types, parameters
*/
BWError BWWASAPI_CloseStream(_wasapi_stream_params** stream_params);

#endif //BW_WASAPI_H
