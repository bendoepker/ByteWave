#ifndef BW_MIXER_UI_H
#define BW_MIXER_UI_H

#include "../process/mixer.h"
#include "bw-ui-types.h"

/*
*   @param mixer_ui: the BWMixerUI object that will store the state of the mixers
*   @param mixers_init: an array of the mixer objects that will contain the initial state of the mixers
*   @param num_mixers: the number of mixers to be initialized
*
*/
void BWUI_CreateMixer(BWMixerUI* mixer_ui, Mixer* mixers_init, int num_mixers);
void BWUI_UpdateMixer(BWMixerUI* mixer);

#endif //BW_UI_MIXER_H
