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

#ifndef PROCESS_H
#define PROCESS_H

/*
*
*   ByteWave Processing function
*
*   These are the functions responsible for processing the audio,
*   routing the audio through the mixer tracks, and reporting the outputs
*   back to the Host APIs as well as any UI functions that will require it
*
*
*/

#include <stdint.h>
#include "mixer.h"

//The main processing function, every host api will call this function in their own thread
//and the audio will be routed through the mixer structures
void process_buffers(float* input_buffers, float* output_buffers,
                     uint32_t num_input_buffers, uint32_t num_output_buffers,
                     uint32_t buffer_size);

#endif //PROCESS_H
