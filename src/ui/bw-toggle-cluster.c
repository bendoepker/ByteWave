#include "bw-toggle-cluster.h"

/* For Logging */
#include <bw-log.h>

void _mt_cb() {
    BW_PRINT("TODO: Mixer Toggle Pressed");
}

void _pb_pause_cb() {
    BW_PRINT("TODO: Playback Pause Pressed");
}

void _pb_stop_cb() {
    BW_PRINT("TODO: Playback Stop Pressed");
}

void _pb_play_cb() {
    BW_PRINT("TODO: Playback Play Pressed");
}

void BWUI_CreateToggleCluster(BWToggleCluster* toggle_cluster) {
    //Toggle Cluster
    Image mixer_toggle_img = LoadImage("../res/assets/buttons/mixer_toggle.png");
    Image mixer_toggle_clicked_img = LoadImage("../res/assets/buttons/mixer_toggle_clicked.png");
    BWUI_CreateImageButton(&toggle_cluster->mixer_toggle, 0, 0, 32, 32, mixer_toggle_img, mixer_toggle_clicked_img, _mt_cb);

    Image playback_stop_img = LoadImage("../res/assets/buttons/playback_stop.png");
    Image playback_stop_clicked_img = LoadImage("../res/assets/buttons/playback_stop_clicked.png");
    BWUI_CreateImageButton(&toggle_cluster->pb_stop, 32, 0, 32, 32, playback_stop_img, playback_stop_clicked_img, _pb_stop_cb);

    Image playback_play_img = LoadImage("../res/assets/buttons/playback_play.png");
    Image playback_play_clicked_img = LoadImage("../res/assets/buttons/playback_play_clicked.png");
    BWUI_CreateImageButton(&toggle_cluster->pb_play, 64, 0, 32, 32, playback_play_img, playback_play_clicked_img, _pb_play_cb);

    Image playback_pause_img = LoadImage("../res/assets/buttons/playback_pause.png");
    Image playback_pause_clicked_img = LoadImage("../res/assets/buttons/playback_pause_clicked.png");
    BWUI_CreateImageButton(&toggle_cluster->pb_pause, 96, 0, 32, 32, playback_pause_img, playback_pause_clicked_img, _pb_pause_cb);

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

void BWUI_UpdateToggleCluster(BWToggleCluster* toggle_cluster) {
    BWUI_UpdateImageButton(&toggle_cluster->mixer_toggle);
    BWUI_UpdateImageButton(&toggle_cluster->pb_pause);
    BWUI_UpdateImageButton(&toggle_cluster->pb_stop);
    BWUI_UpdateImageButton(&toggle_cluster->pb_play);
}
