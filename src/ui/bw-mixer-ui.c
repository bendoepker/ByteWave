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
