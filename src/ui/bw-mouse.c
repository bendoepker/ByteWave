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

#include "bw-mouse.h"

/* For assert() */
#include <assert.h>

/* For logging */
#include <bw-log.h>

/* For xyzHandleMouse() */
#include "bw-toplevel-ui.h"

#ifdef _WIN32
#include <win-undef.h>
#include <windows.h>
typedef BOOL (WINAPI *GetCursorPos_t)(LPPOINT);
GetCursorPos_t pGetCursorPos = 0;
#endif

BWMouseState BWUI_GetMouseState(int button) {
    if(IsMouseButtonPressed(button)) {
        if(IsKeyPressed(KEY_LEFT_CONTROL) ||
           IsKeyPressed(KEY_RIGHT_CONTROL) ||
           IsKeyDown(KEY_LEFT_CONTROL) ||
           IsKeyDown(KEY_RIGHT_CONTROL))
            return MOUSE_CTRL_PRESSED;
        else if(IsKeyPressed(KEY_LEFT_ALT) ||
                IsKeyPressed(KEY_RIGHT_ALT) ||
                IsKeyDown(KEY_LEFT_ALT) ||
                IsKeyDown(KEY_RIGHT_ALT))
            return MOUSE_ALT_PRESSED;
        else if(IsKeyPressed(KEY_LEFT_SHIFT) ||
                IsKeyPressed(KEY_RIGHT_SHIFT) ||
                IsKeyDown(KEY_LEFT_SHIFT) ||
                IsKeyDown(KEY_RIGHT_SHIFT))
            return MOUSE_SHIFT_PRESSED;
        else
            return MOUSE_PRESSED;
    } else if(IsMouseButtonDown(button)) {
        return MOUSE_DOWN;
    } else if(IsMouseButtonReleased(button)) {
        return MOUSE_RELEASED;
    } else
        return MOUSE_UP;
}

BWMouseLocale BWUI_GetMouseLocale(Vector2 mouse_pos, BWToplevelUI* toplevel) {
    if(!IsWindowFocused()) {
        return ML_NONE;
    }

    if(BWUI_CheckWindowFrameCollision(toplevel->window_frame)) {
        return ML_WINDOW_FRAME;
    }
    if(CheckCollisionPointRec(mouse_pos, BWUI_GetTitleBarRec(toplevel->title_bar))) {
        return ML_TITLE_BAR;
    }
    if(CheckCollisionPointRec(mouse_pos, BWUI_GetToggleClusterRec(toplevel->toggle_cluster))) {
        return ML_TOGGLE_CLUSTER;
    }
    if(toplevel->mixer->is_open) {
        if(CheckCollisionPointRec(mouse_pos, toplevel->mixer->hitbox)) {
            return ML_MIXER;
        }
    }
    /*
    *   TODO:
    *   if(CheckCollisionPointRec(mouse_pos, BWUI_GetPlaylistRec(toplevel->playlist))) {
    *       return ML_PLAYLIST;
    *   }
    */

    return ML_NONE;
}

void BWUI_HandleMouseByLocale(BWMouseState state, int button, BWMouseLocale locale, Vector2 mouse_pos, BWToplevelUI* toplevel) {
    switch(locale) {
        //The first two are special cases which need to see the mouse from outside of the
        //window, so they resolve the mouse position from GetMousePositionAbsolute()
        case ML_WINDOW_FRAME:
            BWUI_WindowFrameHandleMouse(toplevel->window_frame, state, button);
            return;
        case ML_TITLE_BAR:
            BWUI_TitleBarHandleMouse(toplevel->title_bar, state, button);
            return;
        case ML_TOGGLE_CLUSTER:
            BWUI_ToggleClusterHandleMouse(toplevel->toggle_cluster, state, button, mouse_pos);
            return;
        case ML_SYSTEM_INFO:
            assert(1 && "TODO: Implement this");
            return;
        case ML_TOOL_SELECT:
            assert(1 && "TODO: Implement this");
            return;
        case ML_EFFECT_WINDOW:
            assert(1 && "TODO: Implement this");
            return;
        case ML_PLAYLIST:
            assert(1 && "TODO: Implement this");
            return;
        case ML_MIXER:
            assert(1 && "TODO: Implement this");
            return;
        case ML_NONE:
            return;
    }
}

void BWUI_EnableOSMousePos() {
#ifdef _WIN32
    HMODULE hUser32 = LoadLibraryA("user32.dll");
    if(!hUser32) {
        BW_PRINT("Could not load User32.dll");
        return;
    }
    pGetCursorPos = (GetCursorPos_t)GetProcAddress(hUser32, "GetCursorPos");
    if(!pGetCursorPos) {
        BW_PRINT("Could not load GetCursorPos() from User32.dll");
        return;
    }
#endif //Windows
}

Vector2 GetMousePositionAbsolute() {
#ifdef _WIN32
    if(pGetCursorPos != 0) {
        POINT mouse_pos;
        if(pGetCursorPos(&mouse_pos)) {
            return (Vector2) {
                .x = (float)mouse_pos.x,
                .y = (float)mouse_pos.y
            };
        }
    }
    return (Vector2){0, 0};
#endif //Windows
}
