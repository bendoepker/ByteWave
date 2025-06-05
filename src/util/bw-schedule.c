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
