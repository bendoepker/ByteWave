#ifndef AUDIO_THREAD_MGR_H
#define AUDIO_THREAD_MGR_H

#include <types.h>

namespace BWAudioBackend {

uint32_t GetCPUUsage();
void SetCPUUsage(uint32_t CPUUsage);

void SendStopCode();

void* ManageThreads(void* unused);

}

#endif //AUDIO_THREAD_MGR_H
