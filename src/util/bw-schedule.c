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

#include <bw-schedule.h>

#ifdef _WIN32
#endif //Windows

BWTimeInfo BWSched_StartTimer() {
    BWTimeInfo out = {0};
#ifdef _WIN32
    //For windows we will use the QueryPerformanceCounter to get as high resolution as possible
    //this will mean we have to convert from ticks (in 100ns / .1us units) to 1us units
    QueryPerformanceFrequency(&out.tick_frequency);
    QueryPerformanceCounter(&out.start_time);
#endif
    return out;
}

uint64_t BWSched_EndTimer(BWTimeInfo time_info) {
#ifdef _WIN32
    LARGE_INTEGER end_time = {0};
    LARGE_INTEGER elapsed_time = {0};

    //Get the elapsed ticks
    QueryPerformanceCounter(&end_time);
    elapsed_time.QuadPart = end_time.QuadPart - time_info.start_time.QuadPart;

    //Convert the elapsed time to micro-ticks to conserve precision
    elapsed_time.QuadPart *= 1000000;

    //Divide the elapsed ticks by the frequency to get micro-seconds
    return elapsed_time.QuadPart /= time_info.tick_frequency.QuadPart;
#endif
}

void BWSched_CreateRTCallback(void (*callback()), void* params) {

}
