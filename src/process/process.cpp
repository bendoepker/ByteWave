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

#include "process.h"
#include <string.h>

void process_buffers(float** input_buffers, float** output_buffers,
                uint32_t num_input_buffers, uint32_t num_output_buffers,
                uint32_t buffer_size) {
    if(num_input_buffers == 2 && num_output_buffers == 2) {
        memcpy(output_buffers[0], input_buffers[0], sizeof(float) * buffer_size);
        memcpy(output_buffers[1], input_buffers[1], sizeof(float) * buffer_size);
    }
}
