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

#ifndef BW_TOGGLE_CLUSTER_H
#define BW_TOGGLE_CLUSTER_H

#include "bw-ui-types.h"

void BWUI_CreateToggleCluster(BWToggleCluster* toggle_cluster, BWMixerUI* mixer_ui);
void BWUI_DestroyToggleCluster(BWToggleCluster* toggle_cluster);
void BWUI_UpdateToggleCluster(BWToggleCluster* toggle_cluster);

Rectangle BWUI_GetToggleClusterRec(BWToggleCluster* toggle_cluster);
void BWUI_ToggleClusterHandleMouse(BWToggleCluster* toggle_cluster, BWMouseState state, int button, Vector2 mouse_pos);

#endif //BW_TOGGLE_CLUSTER_H
