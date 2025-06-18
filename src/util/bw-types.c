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

#include <bw-types.h>
#include <bw-log.h>

const char* _bw_get_error_message(BWError err) {
    switch(err) {
        case BW_OK:
            return "No Error";
        case BW_FAILED:
            return "Failed, unspecified";
        case BW_NO_DEVICES:
            return "No device found";
        case BW_UNINITIALIZED:
            return "Host API / Device was not initialized";
        case BW_WINDOWS_ERROR:
            return "Error with Windows";
        case BW_FAILED_MALLOC:
            return "Failed to execute malloc(), system out of memory";
        case BW_BAD_PARAMS:
            return "Incorrect parameters were passed to the function";
        case BW_HARDWARE_MALFUNCTION:
            return "External hardware device malfunction";
        case BW_FILE_WRITE_ERROR:
            return "Error writing to a file";
        case BW_FILE_READ_ERROR:
            return "Error reading a file";
        case BW_FILE_OPEN_ERROR:
            return "Error opening a file";
        case BW_VERSION_INCOMPATIBLE:
            return "Incompatible version found";
        default:
            return "Error message unspecified";
    }
}

void BWError_Handle(BWError err) {
    if(err != BW_OK) {
        BW_LOG_ERR("%s", _bw_get_error_message(err));
    }
}
