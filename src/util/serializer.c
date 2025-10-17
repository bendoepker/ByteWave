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

#include <serializer.h>

/* For Logging */
#include <log.h>

/* For file I/O */
#include <stdio.h>

/* For strncmp() */
#include <string.h>

#include <stdlib.h>

/* For File System */
#ifdef _WIN32
#include <windows.h>
#endif

BWError BWConfig_Write(const char* file_name, BWConfigData* data) {
    uint32_t version = 1; //This is version one of the config file
    FILE* file = fopen(file_name, "w");
    if(file == 0) return BW_FILE_OPEN_ERROR;

    //SECTION: File ID
    size_t num_written = fwrite("BWCD", 1, 4, file);
    if(num_written != 4) return BW_FILE_WRITE_ERROR;

    //SECTION: Version Tag
    num_written = fwrite(&version, 4, 1, file);
    if(num_written != 1) return BW_FILE_WRITE_ERROR;

    //SECTION: Host API Tag
    switch(data->host_api) {
        case UNKNOWN:
            num_written = fwrite("UNKN", 1, 4, file);
            break;
        case ASIO:
            num_written = fwrite("ASIO", 1, 4, file);
            break;
        case WASAPI:
            num_written = fwrite("WAPI", 1, 4, file);
            break;
        case JACK:
            num_written = fwrite("JACK", 1, 4, file);
            break;
        case ALSA:
            num_written = fwrite("ALSA", 1, 4, file);
            break;
        case CORE:
            num_written = fwrite("CORE", 1, 4, file);
            break;
    }
    if(num_written != 4) return BW_FILE_WRITE_ERROR;

    fclose(file);
    return BW_OK;
}

BWError BWConfig_Read(const char* file_name, BWConfigData* data) {
    FILE* file = fopen(file_name, "r");
    uint32_t version; //Should be 1
    if(file == 0) return BW_FILE_OPEN_ERROR;

    //SECTION: Verify File ID
    char file_id[4];
    size_t num_read = fread(file_id, 1, 4, file);
    if(num_read != 4) return BW_FILE_READ_ERROR;

    //SECTION: Verify Version Tag
    num_read = fread(&version, 4, 1, file);
    if(num_read != 1) return BW_FILE_READ_ERROR;
    if(version != 1) return BW_VERSION_INCOMPATIBLE;

    //SECTION: Host API
    char host_api[4];
    num_read = fread(host_api, 1, 4, file);
    if(num_read != 4) return BW_FILE_READ_ERROR;
    if(strncmp(host_api, "UNKN", 4) == 0) data->host_api = UNKNOWN;
    else if(strncmp(host_api, "WAPI", 4) == 0) data->host_api = WASAPI;
    else if(strncmp(host_api, "ASIO", 4) == 0) data->host_api = ASIO;
    else if(strncmp(host_api, "JACK", 4) == 0) data->host_api = JACK;
    else if(strncmp(host_api, "ALSA", 4) == 0) data->host_api = ALSA;
    else if(strncmp(host_api, "CORE", 4) == 0) data->host_api = CORE;
    else return BW_FILE_READ_ERROR; //NOTE: Add other hostapis before this line

    fclose(file);

    return BW_OK;
}
