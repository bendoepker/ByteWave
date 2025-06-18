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

/* For malloc() and free() */
#include <stdlib.h>

/* For memset() */
#include <string.h>

/* Logging functions */
#include <bw-log.h>

/* for spawning the UI thread */
#include <bw-threads.h>
#include <bw-ui.h>

/* Audio thread */
#include "src/hostapi/bw-hostapi.h"

/* Save data for the UI and audio threads */
#include <bw-config.h>

BWThread thread_pool[1];

int main(void) {
    BWUIData* ui_data = malloc(sizeof *ui_data);

    BWConfigData* conf_data = malloc(sizeof *conf_data);
    memset(conf_data, 0, sizeof(*conf_data));
    if(BWConfig_Read("test-config.bwc", conf_data) != BW_OK) {
        //Set some defaults, the file doesn't exist or is malformed
        conf_data->window_height = 500;
        conf_data->window_width = 800;
    }
    ui_data->config_data = conf_data;

    //SECTION: Create the UI thread
    BWFunctionData func_data = {.function = BWUI_UIMain, .data = (void*)ui_data};
    BWThread ui_thread = BWUtil_CreateThread(&func_data, BW_NORMAL_PRIORITY);
    thread_pool[0] = ui_thread;

    BWError_Handle(BWHostApi_Initialize(conf_data));
    BWError_Handle(BWHostApi_Activate());

    BWUtil_WaitForThreads(thread_pool, 1);

    //Threads are finished, close the audio thread
    BWError_Handle(BWHostApi_Deactivate());
    BWError_Handle(BWHostApi_Terminate());
    BWError_Handle(BWConfig_Write("test-config.bwc", conf_data));
    free(conf_data);
}
