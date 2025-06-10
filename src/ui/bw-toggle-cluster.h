#ifndef BW_TOGGLE_CLUSTER_H
#define BW_TOGGLE_CLUSTER_H

#include "bw-ui-components.h"

typedef struct {
    //Mixer
    BWImageButton mixer_toggle;

    //Playback
    BWImageButton pb_pause;
    BWImageButton pb_play;
    BWImageButton pb_stop;
} BWToggleCluster;

void BWUI_CreateToggleCluster(BWToggleCluster* toggle_cluster);
void BWUI_UpdateToggleCluster(BWToggleCluster* toggle_cluster);

#endif //BW_TOGGLE_CLUSTER_H
