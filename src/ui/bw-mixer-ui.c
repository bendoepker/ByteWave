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

#include "bw-mixer-ui.h"

/* For Logging */
#include <bw-log.h>

#define MIXER_HEIGHT 256

Rectangle _get_mixer_hitbox() {
    Rectangle out;
    out.x = 0;
    out.y = GetScreenHeight() - MIXER_HEIGHT;
    out.height = MIXER_HEIGHT;
    out.width = GetScreenWidth();
    return out;
}

void BWUI_CreateMixer(BWMixerUI* mixer_ui, Mixer* mixers_init, int num_mixers) {
    (void) mixer_ui;
    (void) num_mixers;
    mixer_ui->is_open = false;
    mixer_ui->hitbox = _get_mixer_hitbox();
}

void BWUI_UpdateMixer(BWMixerUI* mixer) {
    
    if(IsWindowResized())
        mixer->hitbox = _get_mixer_hitbox();

    //Draw the Mixer
    if(mixer->is_open) {
        DrawRectangleRec(mixer->hitbox, DARKGRAY);
    }
}
