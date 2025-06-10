#ifndef BW_MOUSE_H
#define BW_MOUSE_H

#include "bw-ui-types.h"

BWMouseState _get_mouse_state(int button);
BWMouseLocale _get_mouse_locale(Vector2 mouse_pos, BWToggleCluster* toggle_cluster, int num_open_effects);
void BWUI_HandleMouseByLocale(BWMouseState state, int button, BWMouseLocale locale, Vector2 mouse_pos, BWToplevelUI* toplevel);

#endif //BW_MOUSE_H
