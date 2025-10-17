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

namespace BWAudioBackend {
namespace Jack {
// Initialize Jack Library
BWError Activate();

// Terminate Jack Library
void Deactivate();

// Get a list of Jack Audio Devices
std::vector<BWAudioDevice> QueryDevices();
}
}

#endif //BW_JACK_H
