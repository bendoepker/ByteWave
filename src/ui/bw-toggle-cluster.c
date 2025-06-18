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

#include "bw-toggle-cluster.h"

#include "bw-ui-components.h"

/* For Logging */
#include <bw-log.h>

void _mt_cb(void* params) {
    BWMixerUI* mixer = params;
    mixer->is_open = !mixer->is_open;
}

void _pb_pause_cb(void* params) {
    (void)params;
    BW_PRINT("TODO: Playback Pause Pressed");
}

void _pb_stop_cb(void* params) {
    (void)params;
    BW_PRINT("TODO: Playback Stop Pressed");
}

void _pb_play_cb(void* params) {
    (void)params;
    BW_PRINT("TODO: Playback Play Pressed");
}

void BWUI_CreateToggleCluster(BWToggleCluster* toggle_cluster, BWMixerUI* mixer_ui) {
    toggle_cluster->mixer_ui_ref = mixer_ui;

    //Toggle Cluster
    //Mixer Toggle
    Image mixer_toggle_img = LoadImage("../res/assets/buttons/mixer_toggle.png");
    Image mixer_toggle_clicked_img = LoadImage("../res/assets/buttons/mixer_toggle_clicked.png");

    //Playback Stop
    Image playback_stop_img = LoadImage("../res/assets/buttons/playback_stop.png");
    Image playback_stop_clicked_img = LoadImage("../res/assets/buttons/playback_stop_clicked.png");

    //Playback Play
    Image playback_play_img = LoadImage("../res/assets/buttons/playback_play.png");
    Image playback_play_clicked_img = LoadImage("../res/assets/buttons/playback_play_clicked.png");

    //Playback Pause
    Image playback_pause_img = LoadImage("../res/assets/buttons/playback_pause.png");
    Image playback_pause_clicked_img = LoadImage("../res/assets/buttons/playback_pause_clicked.png");

    BWUI_CreateImageButton(&toggle_cluster->mixer_toggle, 0, 32, 32, 32, mixer_toggle_img, mixer_toggle_clicked_img, _mt_cb, mixer_ui);
    BWUI_CreateImageButton(&toggle_cluster->pb_stop, 32, 32, 32, 32, playback_stop_img, playback_stop_clicked_img, _pb_stop_cb, 0);
    BWUI_CreateImageButton(&toggle_cluster->pb_play, 64, 32, 32, 32, playback_play_img, playback_play_clicked_img, _pb_play_cb, 0);
    BWUI_CreateImageButton(&toggle_cluster->pb_pause, 96, 32, 32, 32, playback_pause_img, playback_pause_clicked_img, _pb_pause_cb, 0);

    toggle_cluster->hitbox.x = 0;
    toggle_cluster->hitbox.y = 32;
    toggle_cluster->hitbox.width = 128;
    toggle_cluster->hitbox.height = 32;

    //Images are stored in gpu memory so they can be unloaded from ram
    UnloadImage(mixer_toggle_img);
    UnloadImage(mixer_toggle_clicked_img);
    UnloadImage(playback_stop_img);
    UnloadImage(playback_stop_clicked_img);
    UnloadImage(playback_play_img);
    UnloadImage(playback_play_clicked_img);
    UnloadImage(playback_pause_img);
    UnloadImage(playback_pause_clicked_img);
}

void BWUI_DestroyToggleCluster(BWToggleCluster* toggle_cluster) {
    BWUI_DestroyImageButton(&toggle_cluster->mixer_toggle);
    BWUI_DestroyImageButton(&toggle_cluster->pb_stop);
    BWUI_DestroyImageButton(&toggle_cluster->pb_play);
    BWUI_DestroyImageButton(&toggle_cluster->pb_pause);
}

void BWUI_UpdateToggleCluster(BWToggleCluster* toggle_cluster) {
    BWUI_UpdateImageButton(&toggle_cluster->mixer_toggle);
    BWUI_UpdateImageButton(&toggle_cluster->pb_stop);
    BWUI_UpdateImageButton(&toggle_cluster->pb_play);
    BWUI_UpdateImageButton(&toggle_cluster->pb_pause);
}

Rectangle BWUI_GetToggleClusterRec(BWToggleCluster* toggle_cluster) {
    return toggle_cluster->hitbox;
}

void BWUI_ToggleClusterHandleMouse(BWToggleCluster* toggle_cluster, BWMouseState state, int button, Vector2 mouse_pos) {
    if(CheckCollisionPointRec(mouse_pos, BWUI_GetImageButtonRec(&toggle_cluster->mixer_toggle))) {
        BWUI_ImageButtonHandleMouse(&toggle_cluster->mixer_toggle, state, button, mouse_pos);
    } else if(CheckCollisionPointRec(mouse_pos, BWUI_GetImageButtonRec(&toggle_cluster->pb_pause))) {
        BWUI_ImageButtonHandleMouse(&toggle_cluster->pb_pause, state, button, mouse_pos);
    } else if(CheckCollisionPointRec(mouse_pos, BWUI_GetImageButtonRec(&toggle_cluster->pb_stop))) {
        BWUI_ImageButtonHandleMouse(&toggle_cluster->pb_stop, state, button, mouse_pos);
    } else if(CheckCollisionPointRec(mouse_pos, BWUI_GetImageButtonRec(&toggle_cluster->pb_play))) {
        BWUI_ImageButtonHandleMouse(&toggle_cluster->pb_play, state, button, mouse_pos);
    }
}
