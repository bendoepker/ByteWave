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

#include "mixer.h"

//WARN: THIS IS PSEUDOCODE
void mixer_begin(Mixer* mixer) {
    //SECT: Check that all input mixers have the done processing flag set, if not return
    //      (Another mixer track will call it if not)
    if(mixer->num_mixer_inputs > 1) {
        for(int i = 0; i < mixer->num_mixer_inputs; i++) {
            if(!mixer->mixer_inputs[i].done_processing) return;
        }
    }

    //SECT: Set input mixer done processing flags to false and add mixer inputs together
    for(int i = 0; i < mixer->num_mixer_inputs; i++) {
        for(int j = 0; j < mixer->buffer_size; j++) {
            mixer->left_buffer[j] += mixer->mixer_inputs[i].left_buffer[j];
            mixer->right_buffer[j] += mixer->mixer_inputs[i].right_buffer[j];
        }
    }

    //SECT: Apply limiter to the signal (Keep signal in the range -1.0 <-> 1.0)
    for(int i = 0; i < mixer->buffer_size; i++) {
        if(mixer->left_buffer[i] > 1.0) mixer->left_buffer[i] = 1.0;
        else if(mixer->left_buffer[i] < -1.0) mixer->left_buffer[i] = -1.0;
        if(mixer->right_buffer[i] >1.0) mixer->right_buffer[i] = 1.0;
        else if(mixer->right_buffer[i] < -1.0) mixer->right_buffer[i] = -1.0;
    }

    //SECT: Call mixer effects
    for(int i = 0; i < MIXER_MAX_EFFECTS; i++) {
        //TODO: CALL PLUGIN
    }

    //SECT: Adjust gain level
    for(int i = 0; i < mixer->buffer_size; i++) {
        mixer->left_buffer[i] *= mixer->gain_level;
        mixer->right_buffer[i] *= mixer->gain_level;
    }

    mixer->done_processing = 1;
    for(int i = 0; i < mixer->num_mixer_outputs; i++) {
        mixer_begin(&mixer->mixer_outputs[i]);
    }
}
