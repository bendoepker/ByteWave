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
