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

#ifndef BW_WASAPI_H
#define BW_WASAPI_H

#include <bw-types.h>

typedef struct {
    char name[128];
    uint8_t device_usable;
} _wasapi_device;

/*
*   Activate a connection and start I/O with a device
*/
BWError BWWASAPI_Activate(BWHostApi_AudioDevice* audio_device);

/*
*   Close the connection to the active device
*/
BWError BWWASAPI_Deactivate();

/*
*   Query all of the devices available to WASAPI
*/
BWError BWWASAPI_QueryDevices(_wasapi_device** devices, uint32_t* num_devices);

#endif //BW_WASAPI_H
