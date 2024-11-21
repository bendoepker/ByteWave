#include <audiopolicy.h>
#include <Audioclient.h>
#include <mmdeviceapi.h>
#include <cmath>

void testResult(HRESULT);

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient3 = __uuidof(IAudioClient3);
const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);

int main() {
    HRESULT res = S_OK;
    REFERENCE_TIME requested_duration = 10000000;
    REFERENCE_TIME actual_duration;
    UINT32 buffer_frame_count;
    UINT32 frames_available;
    IMMDeviceEnumerator* audio_device_enumerator = nullptr;
    IMMDevice* audio_device = nullptr;
    IAudioClient3* audio_client = nullptr;
    IAudioCaptureClient* capture_client = nullptr;
    WAVEFORMATEX* wave_format = nullptr;
    UINT32 packet_length = 0;
    BOOL done = false;
    BYTE* data;
    DWORD flags;

    //Initialize COM
    res = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    testResult(res);

    //Initialize Device Enumerator
    res = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&audio_device_enumerator);
    testResult(res);

    
}

void testResult(HRESULT err) {
    if(err != S_OK) exit(0);
}
