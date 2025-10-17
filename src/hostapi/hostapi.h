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

#ifndef HOSTAPI_H
#define HOSTAPI_H

#ifdef BW_ASIO
    #include "asio/bw-asio.h"
#endif
#ifdef BW_WASAPI
    #include "wasapi/bw-wasapi.h"
#endif
#ifdef BW_DSOUND
    #include "dsound/bw-dsound.h"
#endif
#ifdef BW_JACK
    #include "jack/bw-jack.h"
#endif
#include <types.h>

// Startup the available audio backends, get a list of available devices
BWError BWHostApi_Initialize(BWConfigData* conf_data);

// Close down the audio subsystem
BWError BWHostApi_Terminate();

// Start a specific audio backend
BWError BWHostApi_Activate(BWHostApi hostapi);

// Close the active audio backend
BWError BWHostApi_Deactivate();

// Check if any of the specific audio backends have been activated
bool BWHostApi_IsActivated();

#endif //HOSTAPI_H
