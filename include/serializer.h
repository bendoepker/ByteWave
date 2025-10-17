#ifndef SERIALIZER_H
#define SERIALIZER_H
#include <types.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

BWError BWConfig_Write(const char* file_name, BWConfigData* data);
BWError BWConfig_Read(const char* file_name, BWConfigData* data);

#ifdef __cplusplus
}
#endif

#endif //SERIALIZER_H
