#ifndef BW_MOUSE_H
#define BW_MOUSE_H

#include "bw-ui-types.h"

BWMouseState BWUI_GetMouseState(int button);
BWMouseLocale BWUI_GetMouseLocale(Vector2 mouse_pos, BWToplevelUI* toplevel);
void BWUI_HandleMouseByLocale(BWMouseState state, int button, BWMouseLocale locale, Vector2 mouse_pos, BWToplevelUI* toplevel);
void BWUI_EnableOSMousePos();
Vector2 GetMousePositionAbsolute();

#endif //BW_MOUSE_H
