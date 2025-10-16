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

#ifndef CIRC_BUF_H
#define CIRC_BUF_H

#include <types.h>
#include <stddef.h>

typedef struct {
    float* array;
    size_t capacity;
    size_t write_ptr;
    size_t read_ptr;
} BWCircBufFloat;

//Allocate the internal buffers
BWError BWCBF_Create(BWCircBufFloat* circ_buf, size_t capacity);

//Deallocate the internal buffers
void BWCBF_Destroy(BWCircBufFloat* circ_buf);

//Append num items from buf to the circular buffer circ_buf
void BWCBF_Append(BWCircBufFloat* circ_buf, float* buf, size_t num);

//Copy the most recently appended
void BWCBF_GetMostRecent(BWCircBufFloat* circ_buf, float* buf, size_t num);


#endif //CIRC_BUF_H
