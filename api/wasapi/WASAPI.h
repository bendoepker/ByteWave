#ifndef WASAPI_H
#define WASAPI_H

#include <audiopolicy.h>
#include <Audioclient.h>
#include "../../error/error.h"
#include "../../structures/circular_buffer.h"

class WASAPIHost {
public:
    WASAPIHost() = delete;

    HRESULT initialize(AUDCLNT_SHAREMODE share_mode, DWORD stream_flags, REFERENCE_TIME buffer_duration, REFERENCE_TIME periodicity, const WAVEFORMATEX p_format, LPCGUID audio_session_guid);

    void get_buffer_size();

    void read_buffer(CircularBufferFloat32 buffer);
    void read_buffer(CircularBufferFloat64 buffer);

    void write_buffer(CircularBufferFloat32 buffer);
    void write_buffer(CircularBufferFloat64 buffer);

private:

};

#endif
