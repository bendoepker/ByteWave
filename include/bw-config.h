#ifndef BW_CONFIG_H
#define BW_CONFIG_H
#include <bw-types.h>
#include <stdint.h>

#define BW_HOST_API_TAG_WASAPI "WAPI"
#define BW_HOST_API_TAG_ASIO "ASIO"
#define BW_HOST_API_TAG_JACK "JACK"
#define BW_HOST_API_TAG_ALSA "ALSA"
#define BW_HOST_API_TAG_CORE_AUDIO "CORE"

BWError BWConfig_Write(char* file_name, BWConfigData* data);
BWError BWConfig_Read(char* file_name, BWConfigData* data);

#endif //BW_CONFIG_H
