#ifndef BW_FFT_H
#define BW_FFT_H

#include <complex.h>

#ifdef _MSC_VER
#define ComplexFloat _Fcomplex
#else
#define ComplexFloat float complex
#endif

//WARN: num_elements must be a power of 2
void fft(float* input_buffer, float* output_buffer, int num_elements, ComplexFloat* internal_buffer);

#endif //BW_FFT_H
