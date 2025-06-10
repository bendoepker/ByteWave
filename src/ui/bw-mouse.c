#include "bw-mouse.h"

/* For assert() */
#include <assert.h>

#include "bw-toplevel-ui.h"

BWMouseState _get_mouse_state(int button) {
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

BWMouseLocale _get_mouse_locale(Vector2 mouse_pos, BWToggleCluster* toggle_cluster, int num_open_effects) {
    if(!IsWindowFocused()) {
        return ML_NONE;
    }

    (void)num_open_effects; //TODO: Implement with effect windows

    if(CheckCollisionPointRec(mouse_pos, BWUI_GetToggleClusterRec(toggle_cluster))) {
        return ML_TOGGLE_CLUSTER;
    }

    return ML_NONE;
}

void BWUI_HandleMouseByLocale(BWMouseState state, int button, BWMouseLocale locale, Vector2 mouse_pos, BWToplevelUI* toplevel) {
    switch(locale) {
        case ML_TOGGLE_CLUSTER:
            BWUI_ToggleClusterHandleMouse(toplevel->toggle_cluster, state, button, mouse_pos);
            break;
        case ML_SYSTEM_INFO:
            assert(1 && "TODO: Implement this");
            break;
        case ML_TOOL_SELECT:
            assert(1 && "TODO: Implement this");
            break;
        case ML_EFFECT_WINDOW:
            assert(1 && "TODO: Implement this");
            break;
        case ML_PLAYLIST:
            assert(1 && "TODO: Implement this");
            break;
        case ML_MIXER:
            assert(1 && "TODO: Implement this");
            break;
        case ML_NONE:
            break;
    }
}
