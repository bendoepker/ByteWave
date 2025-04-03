#include "bw-threads.h"
#include "bw-log.h"
#include <stdio.h>

size_t get_thread_priority() {
#ifdef _WIN32
    HANDLE cur_thr = GetCurrentThread();
    return GetThreadPriority(cur_thr);
#endif
#ifdef __linux__
    pthread_t cur_thr = pthread_self();
    struct sched_param sp;
    int policy;
    if(pthread_getschedparam(cur_thr, &policy, &sp) != 0) {
        BW_LOG_ERROR("Failed to get pthread scheduling params.");
    }
    return sp.sched_priority;
#endif
    return SIZE_MAX; //No operating system defined, thats up to you to figure out
}

//Sets the priority of the thread
void set_thread_priority(BW_THREAD_PRIORITY priority) {
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
        BW_LOG_ERROR("Failed to set pthread scheduling (requires root privilege).");
    }
    return;
#endif
}

//Intermediate function that is passed to the create_thread function
//May need to be used in other OS's?
#ifdef _WIN32
static inline DWORD WINAPI __intermediate_function(void* func_data) {
    //TODO: Test that this works with a return value of (void*) as opposed to (void)
    function_data* data = (function_data*)func_data;
    data->function(data->data);
    return 0;
}
#endif //Windows

//Returns a thread ID of the new thread on succes, returns 0 on fail
bw_thread create_thread(function_data* func_data, BW_THREAD_PRIORITY priority) {
#ifdef _WIN32
    HANDLE t_out = CreateThread(NULL, 0, __intermediate_function, (void*)func_data, 0, 0);
    if(!t_out) return 0;
    if(SetThreadPriority(t_out, priority) == 0) return 0;
    return t_out; //Returns 0 if the handle is invalid, else returns the thread
#endif //Windows
#ifdef __linux__
    pthread_t t_out = 0;
    if(pthread_create(&t_out, NULL, func_data->function, func_data->data) != 0){
        BW_LOG_ERROR("Failed to create a new thread.");
    }
    return t_out;
#endif //Linux
    return 0; //No operating system defined, that's above my pay grade
}

size_t wait_for_threads(bw_thread* threads, size_t thread_count) {
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

size_t wait_for_thread(bw_thread thread) {
#ifdef _WIN32
    return WaitForSingleObject(thread, INFINITE);
#endif
#ifdef __linux__
    pthread_join(thread, NULL);
    return 0;
#endif
}
