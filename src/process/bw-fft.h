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
