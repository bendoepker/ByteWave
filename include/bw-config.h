#ifndef BW_CONFIG_H
#define BW_CONFIG_H
#include <bw-types.h>
#include <stdint.h>

BWError BWConfig_Write(char* file_name, BWConfigData* data);
BWError BWConfig_Read(char* file_name, BWConfigData* data);

#endif //BW_CONFIG_H
