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

#ifndef BW_MOUSE_H
#define BW_MOUSE_H

#include "bw-ui-types.h"

BWMouseState BWUI_GetMouseState(int button);
BWMouseLocale BWUI_GetMouseLocale(Vector2 mouse_pos, BWToplevelUI* toplevel);
void BWUI_HandleMouseByLocale(BWMouseState state, int button, BWMouseLocale locale, Vector2 mouse_pos, BWToplevelUI* toplevel);
void BWUI_EnableOSMousePos();
Vector2 GetMousePositionAbsolute();

#endif //BW_MOUSE_H
