#ifndef BW_THREADS_H
#define BW_THREADS_H

#ifdef _WIN32
    #include <windows.h>
    #include <processthreadsapi.h>
#endif //Windows
#ifdef __linux__
    #include <pthread.h>
    #include <sys/resource.h>
#endif //Linux
#ifdef __APPLE__
    //Yeah I might skip this one for a while
#endif //Mac

#include <stdint.h>

typedef enum {
#ifdef _WIN32
    BW_HIGH_PRIORITY = THREAD_PRIORITY_TIME_CRITICAL,
    BW_NORMAL_PRIORITY = THREAD_PRIORITY_NORMAL,
    BW_LOW_PRIORITY = THREAD_PRIORITY_LOWEST,
#endif //Windows
#ifdef __linux__
    //'nice' values not priority values
    BW_HIGH_PRIORITY = 99,
    BW_NORMAL_PRIORITY = 50,
    BW_LOW_PRIORITY = 1,
#endif //Linux
} BW_THREAD_PRIORITY;

#ifdef _WIN32
    typedef HANDLE bw_thread;
#endif
#ifdef __linux__
    typedef pthread_t bw_thread;
#endif

//Struct to pass function pointer and data to the create_thread function
typedef struct {
    void* (*function)(void*);
    void* data;
} function_data;

//Get / Set the priority of the current thread
size_t get_thread_priority();
void set_thread_priority(BW_THREAD_PRIORITY priority);

//Creates a new thread and executes the function in func_data using the data in func_data
//returns the thread id of the new thread
bw_thread create_thread(function_data* func_data, BW_THREAD_PRIORITY priority);

//Waits for all of the threads in the 'threads' array to finish, thread count
//is the number of thread ids in 'threads'
size_t wait_for_threads(bw_thread* threads, size_t thread_count);

//Waits for the thread 'thread' to complete before continuing in the function
size_t wait_for_thread(bw_thread thread);

#endif //BW_THREADS_H
