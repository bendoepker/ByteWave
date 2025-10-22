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

#ifndef BW_ASIO_H
#define BW_ASIO_H

#include <types.h>
#include <vector>

namespace BWAudioBackend {
namespace Asio {
/*
*   Activate a connection with a device
*/
BWError Activate(const char* driver_name);

/*
*   Close the connection to the device
*/
void Deactivate();

/*
*   Query all of the devices available to ASIO
*/
std::vector<BWAudioDevice> QueryDevices();
}
}

#endif //BW_ASIO_H
