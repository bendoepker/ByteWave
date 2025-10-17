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

#ifndef BW_JACK_H
#define BW_JACK_H

#include <types.h>
#include <vector>

typedef struct {
    char name[128];

    uint32_t input_latency;
    uint32_t output_latency;
} _jack_device;

// Initialize Jack Library
BWError BWJack_Activate();

// Terminate Jack Library
BWError BWJack_Deactivate();

std::vector<BWAudioDevice> BWJack_QueryDevices();

#endif //BW_JACK_H
