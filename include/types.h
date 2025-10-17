#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>

typedef enum BWError {
    BW_OK = 0,
    BW_FAILED,
    BW_UNINITIALIZED,
    BW_NO_DEVICES,
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

//If byte ordering is not specified it is assumed to be little endian (x86_64 native)
typedef enum {
    //Int types
    INT_8_BIT,

    INT_16_BIT,
    INT_24_BIT,
    INT_32_BIT,

    INT_16_BIT_MSB, //TODO:
    INT_24_BIT_MSB, //TODO:
    INT_32_BIT_MSB, //TODO:

    //Floating point types
    FLOAT_32_BIT,
    FLOAT_64_BIT, //TODO:

    FLOAT_32_BIT_MSB, //TODO:
    FLOAT_64_BIT_MSB, //TODO:

} BWSampleTypes;

//Integer type minimum and maximum values
#define UINT_8_BIT_MAX 255
#define UINT_8_BIT_MIN 0
#define INT_16_BIT_MAX 32767
#define INT_16_BIT_MIN -32768
#define INT_24_BIT_MAX 8388607
#define INT_24_BIT_MIN -8388608
#define INT_32_BIT_MAX 2147483647
#define INT_32_BIT_MIN -2147483648

//Floating point minimum and maximum values
#define FLOAT_32_MAX 1.0
#define FLOAT_32_MIN -1.0
#define FLOAT_64_MAX 1.0
#define FLOAT_64_MIN -1.0

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

typedef enum {
    UNKNOWN = 0,
    ASIO    = 1,
    ALSA    = 2,
    JACK    = 3,
    WASAPI  = 4,
    CORE    = 5,
} BWHostApi;

typedef struct {
    BWHostApi host_api;             //The last host api that was used
} BWConfigData;

typedef struct {
    BWConfigData* config_data;
} BWUIData;

typedef struct {
    char device_name[128];
    BWHostApi host_api;
    bool is_input, is_output;
} BWAudioDevice;

//This is intended to be allocated on the stack of the main thread
typedef struct {
    char device_name[128]; //Max of 128 characters including the '\0' for the name
    BWHostApi host_api;

    //Internal buffers use 32 bit floats
    float* left_buffer;
    float* right_buffer;

    uint32_t buffer_size;

    uint32_t input_latency;
    uint32_t output_latency;

} BWHostApi_AudioDevice;

void BWError_Handle(BWError err);

#endif //TYPES_H
