#ifndef BW_SCHEDULE_H
#define BW_SCHEDULE_H
#include <stdint.h>

/*
*
*   Multi-platform high resolution timing functions
*
*/

#ifdef _WIN32
#include <windows.h>
#endif //Windows

typedef struct {
#ifdef _WIN32
    LARGE_INTEGER tick_frequency;
    LARGE_INTEGER start_time;
#endif //Windows
} BWTimeInfo;

/*Timer in microseconds
*
*   Intended Usage:
*   BWTimeInfo time_info = BWTimer_StartTimer();
*
*   Do Some Processing...
*
*   uint64_t elapsed_time = BWTimer_EndTimer(time_info);
*
*   Do Something with the time...
*
*/
BWTimeInfo BWSched_StartTimer();
uint64_t BWSched_EndTimer(BWTimeInfo time_info);

//Sleep
void BWSched_Sleep(unsigned long milliseconds);

//Real Time Callback
void BWSched_CreateRTCallback(void (*callback()), void* params);

#endif //BW_SCHEDULE_H
