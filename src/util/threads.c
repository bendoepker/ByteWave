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

#include "threads.h"
#include "log.h"
#include <stdio.h>

size_t BWUtil_GetThreadPriority() {
#ifdef _WIN32
    HANDLE cur_thr = GetCurrentThread();
    return GetThreadPriority(cur_thr);
#endif
#ifdef __linux__
    pthread_t cur_thr = pthread_self();
    struct sched_param sp;
    int policy;
    if(pthread_getschedparam(cur_thr, &policy, &sp) != 0) {
        BW_LOG_ERR("Failed to get pthread scheduling params.");
    }
    return sp.sched_priority;
#endif
    return SIZE_MAX; //No operating system defined, thats up to you to figure out
}

//Sets the priority of the thread
void BWUtil_SetThreadPriority(BWThreadPriority priority) {
#ifdef _WIN32
    HANDLE cur_thr = GetCurrentThread();
    SetThreadPriority(cur_thr, priority);
    return;
#endif
#ifdef __linux__
    pthread_t cur_thr = pthread_self();
    struct sched_param sp;
    sp.sched_priority = priority;
    if(pthread_setschedparam(cur_thr, SCHED_FIFO, &sp) != 0) {
        BW_LOG_ERR("Failed to set pthread scheduling (requires root privilege).");
    }
    return;
#endif
}

//Intermediate function that is passed to the create_thread function
//May need to be used in other OS's (Mac)?
#ifdef _WIN32
static inline DWORD WINAPI _bw_win_threading_function(void* func_data) {
    //TODO: Test that this works with a return value of (void*) as opposed to (void)
    BWFunctionData* data = (BWFunctionData*)func_data;
    data->function(data->data);
    return 0;
}
#endif //Windows

//Returns a thread ID of the new thread on succes, returns 0 on fail
BWThread BWUtil_CreateThread(BWFunctionData* func_data, BWThreadPriority priority) {
#ifdef _WIN32
    HANDLE t_out = CreateThread(NULL, 0, _bw_win_threading_function, (void*)func_data, 0, 0);
    if(!t_out) return 0;
    if(SetThreadPriority(t_out, priority) == 0) return 0;
    return t_out; //Returns 0 if the handle is invalid, else returns the thread
#endif //Windows
#ifdef __linux__
    pthread_t t_out = 0;
    if(pthread_create(&t_out, NULL, func_data->function, func_data->data) != 0){
        BW_LOG_ERR("Failed to create a new thread.");
    }
    return t_out;
#endif //Linux
    return 0; //No operating system defined, that's above my pay grade
}

size_t BWUtil_WaitForThreads(BWThread* threads, size_t thread_count) {
#ifdef _WIN32
    //TODO: Implement a meaningful return value here
    return WaitForMultipleObjects(thread_count, threads, TRUE, INFINITE);
#endif
#ifdef __linux__
    for(int _i = 0; _i < thread_count; _i++) {
        pthread_join(threads[_i], NULL);
    }
    return 0;
#endif
}

size_t BWUtil_WaitForThread(BWThread thread) {
#ifdef _WIN32
    return WaitForSingleObject(thread, INFINITE);
#endif
#ifdef __linux__
    pthread_join(thread, NULL);
    return 0;
#endif
}
