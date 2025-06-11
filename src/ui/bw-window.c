#include "bw-window.h"
#include <string.h>
#include "bw-ui-components.h"
#include "bw-mouse.h"

#include <bw-log.h>

void _close_button_callback(void* params) {
    if(params != 0) {
        unsigned char* exit_requested = (unsigned char*)params;
        *exit_requested = 1;
    } else {
        BW_LOG_ERR("Could not safely exit");
        CloseWindow();
    }
}

void _maximize_button_callback(void* params) {
    if(!IsWindowMaximized())
        MaximizeWindow();
    else
        RestoreWindow();
}

void _minimize_button_callback(void* params) {
    MinimizeWindow();
}

void BWUI_CreateTitleBar(BWTitleBar* title_bar, void* exit_request) {
    memset(title_bar, 0, sizeof(*title_bar));
    memcpy(title_bar->title, "ByteWave", 8);
    title_bar->hitbox.height = 32;
    title_bar->hitbox.x = 0;
    title_bar->hitbox.y = 0;
    title_bar->hitbox.width = GetScreenWidth();

    Image titlebar_close_img = LoadImage("../res/assets/buttons/titlebar_close.png");
    Image titlebar_close_clicked_img = LoadImage("../res/assets/buttons/titlebar_close_clicked.png");

    Image titlebar_maximize_img = LoadImage("../res/assets/buttons/titlebar_maximize.png");
    Image titlebar_maximize_clicked_img = LoadImage("../res/assets/buttons/titlebar_maximize_clicked.png");

    Image titlebar_minimize_img = LoadImage("../res/assets/buttons/titlebar_minimize.png");
    Image titlebar_minimize_clicked_img = LoadImage("../res/assets/buttons/titlebar_minimize_clicked.png");

    BWUI_CreateImageButton(&title_bar->close_button, GetScreenWidth() - 32, 0, 32, 32, titlebar_close_img, titlebar_close_clicked_img, _close_button_callback, exit_request);
    BWUI_CreateImageButton(&title_bar->maximize_button, GetScreenWidth() - 64, 0, 32, 32, titlebar_maximize_img, titlebar_maximize_clicked_img, _maximize_button_callback, 0);
    BWUI_CreateImageButton(&title_bar->minimize_button, GetScreenWidth() - 96, 0, 32, 32, titlebar_minimize_img, titlebar_minimize_clicked_img, _minimize_button_callback, 0);
}

void BWUI_UpdateTitleBar(BWTitleBar* title_bar, Vector2 mouse_pos, Font* font) {

    //Draw the titlebar
    DrawRectangleRec((Rectangle){.x = 0, .y = 0, .width = GetScreenWidth(), .height = 32}, DARKGRAY);
    DrawTextEx(*font, title_bar->title, (Vector2){6, 6}, 20, 4, BLACK);
    if(GetScreenWidth() != title_bar->hitbox.width) {
        //Window has been horizontally resized, move the buttons
        title_bar->hitbox.width = GetScreenWidth();
        title_bar->minimize_button.hitbox.x = title_bar->hitbox.width - 96;
        title_bar->maximize_button.hitbox.x = title_bar->hitbox.width - 64;
        title_bar->close_button.hitbox.x = title_bar->hitbox.width - 32;
    }
    BWUI_UpdateImageButton(&title_bar->close_button);
    BWUI_UpdateImageButton(&title_bar->maximize_button);
    BWUI_UpdateImageButton(&title_bar->minimize_button);
}

Rectangle BWUI_GetTitleBarRec(BWTitleBar* title_bar) {
    return title_bar->hitbox;
}

void BWUI_TitleBarHandleMouse(BWTitleBar* title_bar, BWMouseState state, int button) {
    //TODO: THIS
}

/*
*
*   +--+--------------------+--+ < ns_frame.y
*   |p1|         p2         |p3|
*   +--+--------------------+--+ < ew_frame.y
*   |  |                    |  |
*   |p4|         p5         |p6|
*   |  |                    |  |
*   |  |                    |  |
*   +--+--------------------+--+ ew_frame.height
*   |p7|         p8         |p9|
*   +--+--------------------+--+ < ns_frame.height
*   ^  ^                    ^  ^
*   |  ns_frame.x           |  ew_frame.width
*   ew_frame.x              ns_frame.width
*
*   Using the 8 points above we should be able to determine where the mouse position is (p1 - p9)
*
*   mp = GetMousePositionAbsolute()
*   if(!(mp.x < ew_frame.x) && !(mp.x > ew_frame.x + ew_frame.width) &&
*      !(mp.y < ns_frame.y) && !(mp.y > ns_frame.y + ns_frame.height)
*
*/
void BWUI_CreateWindowFrame(BWWindowFrame* frame) {
    memset(frame, 0, sizeof(*frame));
    Vector2 window_pos = GetWindowPosition();
    Vector2 window_size = {.x = GetScreenWidth(), .y = GetScreenHeight()};

    const float half_frame_size = 5.0;

    frame->ns_frame.x = window_pos.x + half_frame_size;
    frame->ns_frame.width = window_size.x - half_frame_size * 2.0;
    frame->ns_frame.y = window_pos.y - half_frame_size;
    frame->ns_frame.height = window_size.y + half_frame_size * 2.0;

    frame->ew_frame.x = window_pos.x - half_frame_size;
    frame->ew_frame.width = window_size.x + half_frame_size * 2.0;
    frame->ew_frame.y = window_pos.y + half_frame_size;
    frame->ew_frame.height = window_size.y - half_frame_size * 2.0;

    frame->clicked = false;
}

void BWUI_UpdateWindowFrame(BWWindowFrame* frame) {
    if(IsWindowFocused() && !IsWindowMaximized()) {
        //Check for collision
        Rectangle* ew = &frame->ew_frame;           //East-West Frame
        Rectangle* ns = &frame->ns_frame;           //Nort-South Frame
        Vector2 mp = GetMousePositionAbsolute();    //Mouse Position
        //BW_PRINT("%f %f", mp.x, mp.y);

        //Check that the mouse is within the window ( + padding) area
        if(!(mp.x < ew->x) && !(mp.x > ew->x + ew->width) &&
           !(mp.y < ns->y) && !(mp.y > ns->y + ns->height)) {
            if(mp.x < ns->x) {
                //We are in one of the left 3 squares
                if(mp.y < ew->y) {
                    //Top Left
                    SetMouseCursor(MOUSE_CURSOR_RESIZE_NWSE);
                    return;
                } else if(mp.y < ew->y + ew->height) {
                    //Middle Left
                    SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
                    return;
                } else {
                    //Bottom Left
                    SetMouseCursor(MOUSE_CURSOR_RESIZE_NESW);
                    return;
                }
            } else if(mp.x < ns->x + ns->width) {
                //We are in one of the middle 3 squares
                if(mp.y < ew->y) {
                    //Top Middle
                    SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
                    return;
                } else if(mp.y < ew->y + ew->height) {
                    //Middle middle => set to default cursor
                    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                    return;
                } else {
                    //Bottom middle
                    SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
                    return;
                }
            } else {
                //We are in one of the right squares
                if(mp.y < ew->y) {
                    //Top Right
                    SetMouseCursor(MOUSE_CURSOR_RESIZE_NESW);
                    return;
                } else if(mp.y < ew->y + ew->height) {
                    //Middle Right
                    SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
                    return;
                } else {
                    //Bottom Right
                    SetMouseCursor(MOUSE_CURSOR_RESIZE_NWSE);
                    return;
                }
            }
        }
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        return;
    }
}

bool BWUI_CheckWindowFrameCollision(BWWindowFrame* frame) {
    //Check for collision
    Rectangle* ew = &frame->ew_frame;           //East-West Frame
    Rectangle* ns = &frame->ns_frame;           //Nort-South Frame
    Vector2 mp = GetMousePositionAbsolute();    //Mouse Position
    //BW_PRINT("%f %f", mp.x, mp.y);

    //Check that the mouse is within the window ( + padding) area
    if(!(mp.x < ew->x) && !(mp.x > ew->x + ew->width) &&
       !(mp.y < ns->y) && !(mp.y > ns->y + ns->height)) {
        if(mp.x < ns->x) {
            //We are in one of the left 3 squares
            if(mp.y < ew->y) {
                //Top Left
                return true;
            } else if(mp.y < ew->y + ew->height) {
                //Middle Left
                return true;
            } else {
                //Bottom Left
                return true;
            }
        } else if(mp.x < ns->x + ns->width) {
            //We are in one of the middle 3 squares
            if(mp.y < ew->y) {
                //Top Middle
                return true;
            } else if(mp.y < ew->y + ew->height) {
                //Middle middle => set to default cursor
                return false;
            } else {
                //Bottom middle
                return true;
            }
        } else {
            //We are in one of the right squares
            if(mp.y < ew->y) {
                //Top Right
                return true;
            } else if(mp.y < ew->y + ew->height) {
                //Middle Right
                return true;
            } else {
                //Bottom Right
                return true;
            }
        }
    }
    return false;
}

void BWUI_WindowFrameHandleMouse(BWWindowFrame* window_frame, BWMouseState state, int button) {
    if(button == MOUSE_BUTTON_LEFT) {
        if(state == MOUSE_PRESSED) {
            window_frame->clicked = true;
        } else if(state == MOUSE_DOWN) {
            //Resize the window here

        } else {
            window_frame->clicked = false;
        }
    }
}
