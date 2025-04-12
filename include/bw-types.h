#ifndef BW_TYPES_H
#define BW_TYPES_H
#include <stdint.h>

typedef enum BWError {
    BW_OK = 0,
    BW_FAILED,
    BW_UNINITIALIZED,
    BW_WINDOWS_ERROR,
    BW_FAILED_MALLOC,
    BW_BAD_PARAMS,
    BW_HARDWARE_MALFUNCTION,
    BW_FILE_WRITE_ERROR,
    BW_FILE_READ_ERROR,
    BW_FILE_OPEN_ERROR,
    BW_VERSION_INCOMPATIBLE,
} BWError;

typedef enum {
    BW_FALSE = 0,
    BW_TRUE = 1,
} BWBool;

//At the moment I will only be supporting these sample rates, this may
//change in the future, especially if the implementation is trivial
const static uint32_t BWSampleRates[] = {
    //8000,
    //11025,
    //16000,
    //22050,
    44100,
    48000,
    88200,
    96000,
    176400,
    192000,
    //352800,
    //384000,
};

typedef struct {
    uint16_t num_input_channels;
    uint16_t num_output_channels;

    uint8_t  audio_device_usable;

    uint32_t buffer_size;

    uint16_t input_latency;
    uint16_t output_latency;

} BWAudioStream;

typedef struct {
    char host_api[4];             //The last host api that was used
    char* device_name;          //The name of the device within the host api that was used
    uint32_t device_name_length;
    uint32_t window_width;
    uint32_t window_height;

} BWConfigData;

typedef struct {
    BWConfigData* config_data;
} BWUIData;

void BWError_Handle(BWError err);

#endif /* BW_TYPES_H */
