#ifndef CONVERSIONS_H
#define CONVERSIONS_H
#include <types.h>

//Provide a preallocated buffer of buffer_size * sample_size for both
//the input and output, 24 bit integers should be packed into two six-byte packs
void BWUtil_ConvertToFloat(void* input_buffer, float* output_buffer,
                           BWSampleTypes input_type, uint32_t buffer_size);
void BWUtil_ConvertFromFloat(float* input_buffer, void* output_buffer,
                            BWSampleTypes output_type, uint32_t buffer_size);

#endif //CONVERSIONS_H
