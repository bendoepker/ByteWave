#include "audio-thread-mgr.h"
#include <atomic>
#include <thread>
#include <windows.h>

typedef struct {
    std::atomic<uint32_t> CPUUsage;
    std::atomic<bool> StopCode;
} GlobalAudioBackendData;

static GlobalAudioBackendData audio_thread_data = {0};

uint32_t BWAudioBackend::GetCPUUsage() {
    return audio_thread_data.CPUUsage;
}

void BWAudioBackend::SetCPUUsage(uint32_t CPUUsage) {
    audio_thread_data.CPUUsage = CPUUsage;
}

void BWAudioBackend::SendStopCode() {
    audio_thread_data.StopCode = true;
}

void* BWAudioBackend::ManageThreads(void* unused) {
    while(!audio_thread_data.StopCode) {

        Sleep(1);
    }
    return 0;
}
