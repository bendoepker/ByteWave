#ifndef BW_WASAPI_H
#define BW_WASAPI_H

#include <windows.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>

//ByteWave
#include <bw_types.h>

typedef struct {
    IMMDevice* capture_device;
    IMMDevice* render_device;
    IAudioClient* audio_client;
    IAudioCaptureClient* capture_client;
    IAudioRenderClient* render_client;
    WAVEFORMATEX* audio_format;
    UINT32 buffer_frame_count;
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

/*
    * TODO: Add description, return types, parameters
*/
BWError BWWASAPIOpenStream(wasapi_stream_params** stream_params);

/*
    * TODO: Add description, return types, parameters
*/
BWError BWWASAPICloseStream(wasapi_stream_params** stream_params);

#endif //BW_WASAPI_H
