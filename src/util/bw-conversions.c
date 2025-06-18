/*
*	Bytewave: A Lightweight Digital Audio Workstation
*	Copyright (C) 2025  Ben Doepker
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <bw-conversions.h>
#include <string.h>
#include <assert.h>

//TODO: Add Conversions for non LSB byte ordered types

//Conversion to float
static inline void _int16_to_float32(int16_t* input_buffer, float* output_buffer, uint32_t buffer_size) {
    for(uint32_t i = 0; i < buffer_size; i++) {
        output_buffer[i] = input_buffer[i] / 32768.0f;
    }
}

static inline void _int24_to_float32(void* input_buffer, float* output_buffer, uint32_t buffer_size) {
    uint8_t* ib = (uint8_t*)input_buffer;
    int32_t tmp[buffer_size];
    uint32_t tmp1, tmp2, tmp3;
    //NOTE: The only supported architecture is little endian
    for(uint32_t i = 0; i < buffer_size; i++) {
        //Unpack each byte of the 24 bit ints into individual variables
        tmp1 = ib[(i * 3) + 0];
        tmp2 = ib[(i * 3) + 1];
        tmp3 = ib[(i * 3) + 2];
        // Pack each of the bytes into a 32 bit int in the appropriate order
        tmp[i] = (tmp1 << 16) | (tmp2 << 8) | tmp3;
        if(tmp[i] & 0x800000) tmp[i] |= 0xFF000000; //Negative values are translated to 32 bit int
        //At this point the values are stored as 32 bit ints, but the values are still in the range
        //of -8,388,608 <-> 8,388,607 (Signed 24 Bit Int Range)
        output_buffer[i] = tmp[i] / 8388608.0f;
        //The constant in the line above will compress the range of positive values by a small
        //amount, but is a small enough amount that the processing saved is worth it
    }
}

static inline void _int32_to_float32(int32_t* input_buffer, float* output_buffer, uint32_t buffer_size) {
    for(uint32_t i = 0; i < buffer_size; i++) {
        output_buffer[i] = input_buffer[i] / 2147483648.0f;
    }
}


static inline void _float32_to_int16(float* input_buffer, int16_t* output_buffer, uint32_t buffer_size) {
    for(uint32_t i = 0; i < buffer_size; i++) {
        output_buffer[i] = input_buffer[i] * 32767.f; //Slight loss of precision for negative samples
    }
}

static inline void _float32_to_int24(float* input_buffer, void* output_buffer, uint32_t buffer_size) {
    //Reinterpret the output buffer as a byte array
    uint8_t* ob = (uint8_t*)output_buffer;
    int32_t tmp;
    for(uint32_t i = 0; i < buffer_size; i++) {
        //Scale the number to the 24 Bit range
        tmp = input_buffer[i] * 8388607.f;

        //Pack each of the bytes of the output array (little endian)
        ob[(i * 3) + 0] = tmp & 0xFF;
        ob[(i * 3) + 1] = (tmp >> 8) & 0xFF;
        ob[(i * 3) + 2] = (tmp >> 16) & 0xFF;
    }
}

static inline void _float32_to_int32(float* input_buffer, int32_t* output_buffer, uint32_t buffer_size) {
    for(uint32_t i = 0; i < buffer_size; i++) {
        output_buffer[i] = input_buffer[i] * 2147483647.f;
    }
}

void BWUtil_ConvertToFloat(void* input_buffer, float* output_buffer,
                           BWSampleTypes input_type, uint32_t buffer_size) {
    switch(input_type) {
        case INT_16_BIT:
            _int16_to_float32(input_buffer, output_buffer, buffer_size);
            return;
        case INT_24_BIT:
            _int24_to_float32(input_buffer, output_buffer, buffer_size);
            return;
        case INT_32_BIT:
            _int32_to_float32(input_buffer, output_buffer, buffer_size);
            return;
        case FLOAT_32_BIT:
            memcpy(output_buffer, input_buffer, buffer_size * sizeof(float));
            return;
    }
}

void BWUtil_ConvertFromFloat(float* input_buffer, void* output_buffer,
                             BWSampleTypes output_type, uint32_t buffer_size) {
    switch(output_type) {
        case INT_16_BIT:
            _float32_to_int16(input_buffer, output_buffer, buffer_size);
            return;
        case INT_24_BIT:
            _float32_to_int24(input_buffer, output_buffer, buffer_size);
            return;
        case INT_32_BIT:
            _float32_to_int32(input_buffer, output_buffer, buffer_size);
            return;
        case FLOAT_32_BIT:
            memcpy(output_buffer, input_buffer, buffer_size * sizeof(float));
            return;
    }
}
