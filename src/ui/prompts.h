#ifndef PROMPT_H
#define PROMPT_H
#include <types.h>
#include "application.h"

void ProcessPopups(BWConfigData* config_data, UITriggers* triggers);
void PromptForAudioBackend(BWConfigData* config_data, UITriggers* triggers);
void AudioBackendPopup();
void PromptForClose();

#endif //PROMPT_H
