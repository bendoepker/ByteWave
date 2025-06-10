#ifndef BW_TOGGLE_CLUSTER_H
#define BW_TOGGLE_CLUSTER_H

#include "bw-ui-types.h"

void BWUI_CreateToggleCluster(BWToggleCluster* toggle_cluster, BWMixerUI* mixer_ui);
void BWUI_UpdateToggleCluster(BWToggleCluster* toggle_cluster);
Rectangle BWUI_GetToggleClusterRec(BWToggleCluster* toggle_cluster);
void BWUI_ToggleClusterHandleMouse(BWToggleCluster* toggle_cluster, BWMouseState state, int button, Vector2 mouse_pos);

#endif //BW_TOGGLE_CLUSTER_H
