#ifndef BWAAPI_H
#define BWAAPI_H

#define BW_NO_AUDIO_API 0x0000
#define BW_WASAPI 0x0001
#define BW_ASIO 0x0002
#define BW_PULSE_AUDIO 0x0003
#define BW_PIPEWIRE 0x0004
#define BW_CORE_AUDIO 0x0005

#include "wasapi/WASAPI.h"
#include "../error/error.h"

class BWAAPI {
public:
    //Initialize an audio stream with the host api
    BWERROR initialize (unsigned int host_api);

    BWERROR write_buffer(const CircularBufferFloat32*);
    BWERROR write_buffer(const CircularBufferFloat64*);

    BWERROR read_buffer(CircularBufferFloat32*);
    BWERROR read_buffer(CircularBufferFloat64*);

    //Purge the initialized audio stream
    BWERROR purge();
private:

};

#endif
