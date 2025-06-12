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

    //Buttons are inset 5 pixels from the border
    BWUI_CreateImageButton(&title_bar->close_button, GetScreenWidth() - 32, 3, 27, 27, titlebar_close_img, titlebar_close_clicked_img, _close_button_callback, exit_request);
    BWUI_CreateImageButton(&title_bar->maximize_button, GetScreenWidth() - 64, 3, 27, 27, titlebar_maximize_img, titlebar_maximize_clicked_img, _maximize_button_callback, 0);
    BWUI_CreateImageButton(&title_bar->minimize_button, GetScreenWidth() - 96, 3, 27, 27, titlebar_minimize_img, titlebar_minimize_clicked_img, _minimize_button_callback, 0);

    //Unload images from ram
    UnloadImage(titlebar_close_img);
    UnloadImage(titlebar_close_clicked_img);

    UnloadImage(titlebar_maximize_img);
    UnloadImage(titlebar_maximize_clicked_img);

    UnloadImage(titlebar_minimize_img);
    UnloadImage(titlebar_minimize_clicked_img);
}

void BWUI_UpdateTitleBar(BWTitleBar* title_bar, Vector2 mouse_pos, Font* font) {

    //Draw the titlebar
    DrawRectangleRec(title_bar->hitbox, DARKGRAY);
    DrawTextEx(*font, title_bar->title, (Vector2){6, 6}, 20, 4, BLACK);
    if(GetScreenWidth() != title_bar->hitbox.width) {
        //Window has been horizontally resized, move the buttons (Inset by 5 pixels)
        title_bar->hitbox.width = GetScreenWidth();
        title_bar->minimize_button.hitbox.x = title_bar->hitbox.width - 96;
        title_bar->maximize_button.hitbox.x = title_bar->hitbox.width - 64;
        title_bar->close_button.hitbox.x = title_bar->hitbox.width - 32;
    }
    BWUI_UpdateImageButton(&title_bar->close_button);
    BWUI_UpdateImageButton(&title_bar->maximize_button);
    BWUI_UpdateImageButton(&title_bar->minimize_button);
}

void BWUI_DestroyTitleBar(BWTitleBar* title_bar) {
    BWUI_DestroyImageButton(&title_bar->close_button);
    BWUI_DestroyImageButton(&title_bar->maximize_button);
    BWUI_DestroyImageButton(&title_bar->minimize_button);
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
void BWUI_CreateWindowFrame(BWWindowFrame* frame, Vector2 min_window_size) {
    memset(frame, 0, sizeof(*frame));
    Vector2 window_pos = GetWindowPosition();
    Vector2 window_size = {.x = GetScreenWidth(), .y = GetScreenHeight()};

    const float half_frame_size = 4.0;

    frame->ns_frame.x = window_pos.x + half_frame_size;
    frame->ns_frame.width = window_size.x - half_frame_size * 2.0;
    frame->ns_frame.y = window_pos.y - half_frame_size;
    frame->ns_frame.height = window_size.y + half_frame_size * 2.0;

    frame->ew_frame.x = window_pos.x - half_frame_size;
    frame->ew_frame.width = window_size.x + half_frame_size * 2.0;
    frame->ew_frame.y = window_pos.y + half_frame_size;
    frame->ew_frame.height = window_size.y - half_frame_size * 2.0;

    frame->clicked = false;
    frame->resize_direction = N;

    frame->min_window_size = min_window_size;
}

void BWUI_UpdateWindowFrame(BWWindowFrame* frame) {
    //No need to process zeros
    if(IsWindowFocused() && !IsWindowMaximized()) {
        if(frame->clicked && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            frame->clicked = false;
        //Check for collision if the frame is not currently being clicked (resized)
        if(!frame->clicked && !IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
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
                        frame->resize_direction = NW;
                        return;
                    } else if(mp.y < ew->y + ew->height) {
                        //Middle Left
                        SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
                        frame->resize_direction = W;
                        return;
                    } else {
                        //Bottom Left
                        SetMouseCursor(MOUSE_CURSOR_RESIZE_NESW);
                        frame->resize_direction = SW;
                        return;
                    }
                } else if(mp.x < ns->x + ns->width) {
                    //We are in one of the middle 3 squares
                    if(mp.y < ew->y) {
                        //Top Middle
                        SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
                        frame->resize_direction = N;
                        return;
                    } else if(mp.y < ew->y + ew->height) {
                        //Middle middle => set to default cursor
                        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                        return;
                    } else {
                        //Bottom middle
                        SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
                        frame->resize_direction = S;
                        return;
                    }
                } else {
                    //We are in one of the right squares
                    if(mp.y < ew->y) {
                        //Top Right
                        SetMouseCursor(MOUSE_CURSOR_RESIZE_NESW);
                        frame->resize_direction = NE;
                        return;
                    } else if(mp.y < ew->y + ew->height) {
                        //Middle Right
                        SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
                        frame->resize_direction = E;
                        return;
                    } else {
                        //Bottom Right
                        SetMouseCursor(MOUSE_CURSOR_RESIZE_NWSE);
                        frame->resize_direction = SE;
                        return;
                    }
                }
            }
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            return;
        } else if(frame->clicked){
            //Resize the window here
            Vector2 new_mouse_pos = GetMousePositionAbsolute();
            Vector2 window_pos = GetWindowPosition();
            Vector2 window_size = {.x = GetScreenWidth(), .y = GetScreenHeight()};
            Vector2 new_window_size = {.x = window_size.x, .y = window_size.y};
            Vector2 mouse_diff = {new_mouse_pos.x - frame->prev_mouse_pos.x, new_mouse_pos.y - frame->prev_mouse_pos.y};
            switch(frame->resize_direction) {
                case N:
                    //Moving the top of window
                    if(window_size.y == frame->min_window_size.y) {
                        //Only allow increase in height
                        if(new_mouse_pos.y < window_pos.y) {
                            new_window_size.y -= (new_mouse_pos.y - window_pos.y);
                            SetWindowPosition(window_pos.x, new_mouse_pos.y);
                            SetWindowSize(new_window_size.x, new_window_size.y);
                        }
                    } else {
                        if(mouse_diff.y > 0) {
                            //Shrinking window
                            float max_difference = window_size.y - frame->min_window_size.y;
                            if(mouse_diff.y > max_difference)
                                mouse_diff.y = max_difference;
                            new_window_size.y -= mouse_diff.y;
                            SetWindowPosition(window_pos.x, window_pos.y + mouse_diff.y);
                            SetWindowSize(new_window_size.x, new_window_size.y);
                        } else {
                            new_window_size.y -= mouse_diff.y;
                            SetWindowPosition(window_pos.x, window_pos.y + mouse_diff.y);
                            SetWindowSize(new_window_size.x, new_window_size.y);
                        }
                    }
                    break;
                case NE:
                    //Moving the top and right of window
                    //TODO:
                    break;
                case E:
                    //Moving the right of window
                    if(window_size.x == frame->min_window_size.x) {
                        //Only allow increase in width
                        if(new_mouse_pos.x > window_pos.x + window_size.x) {
                            new_window_size.x = (new_mouse_pos.x - window_pos.x);
                            SetWindowSize(new_window_size.x, new_window_size.y);
                        }
                    } else {
                        if(mouse_diff.x < 0) {
                            //Shrinking window
                            float max_difference = window_size.x - frame->min_window_size.x;
                            if(mouse_diff.x < -max_difference)
                                mouse_diff.x = -max_difference;
                            new_window_size.x += mouse_diff.x;
                            SetWindowSize(new_window_size.x, new_window_size.y);
                        } else {
                            new_window_size.x += mouse_diff.x;
                            SetWindowSize(new_window_size.x, new_window_size.y);
                        }
                    }
                    break;
                case SE:
                    //Moving the bottom and right of window
                    //TODO:
                    break;
                case S:
                    //Moving the bottom of window
                    if(window_size.y == frame->min_window_size.y) {
                        //Only allow increase in height
                        if(new_mouse_pos.y > window_pos.y + window_size.y) {
                            new_window_size.y = (new_mouse_pos.y - window_pos.y);
                            SetWindowSize(new_window_size.x, new_window_size.y);
                        }
                    } else {
                        if(mouse_diff.y < 0) {
                            //Shrinking window
                            float max_difference = window_size.y - frame->min_window_size.y;
                            if(mouse_diff.y < -max_difference)
                                mouse_diff.y = -max_difference;
                            new_window_size.y += mouse_diff.y;
                            SetWindowSize(new_window_size.x, new_window_size.y);
                        } else {
                            new_window_size.y += mouse_diff.y;
                            SetWindowSize(new_window_size.x, new_window_size.y);
                        }
                    }
                    break;
                case SW:
                    //Moving the bottom and left of window
                    //TODO:
                    break;
                case W:
                    //Moving the left of window
                    if(window_size.x == frame->min_window_size.x) {
                        //Only allow increase in width
                        if(new_mouse_pos.x < window_pos.x) {
                            new_window_size.x -= (new_mouse_pos.x - window_pos.x);
                            SetWindowPosition(new_mouse_pos.x, window_pos.y);
                            SetWindowSize(new_window_size.x, new_window_size.y);
                        }
                    } else {
                        if(mouse_diff.x > 0) {
                            //Shrinking window
                            float max_difference = window_size.x - frame->min_window_size.x;
                            if(mouse_diff.x > max_difference)
                                mouse_diff.x = max_difference;
                            new_window_size.x -= mouse_diff.x;
                            SetWindowPosition(window_pos.x + mouse_diff.x, window_pos.y);
                            SetWindowSize(new_window_size.x, new_window_size.y);
                        } else {
                            new_window_size.x -= mouse_diff.x;
                            SetWindowPosition(window_pos.x + mouse_diff.x, window_pos.y);
                            SetWindowSize(new_window_size.x, new_window_size.y);
                        }
                    }
                    break;
                case NW:
                    //Moving the top and left of window
                    //TODO:
                    break;
            }
            frame->prev_mouse_pos = new_mouse_pos;

            //Recalculate frame hitbox
            window_pos = GetWindowPosition();
            window_size = new_window_size;

            const float half_frame_size = 5.0;

            frame->ns_frame.x = window_pos.x + half_frame_size;
            frame->ns_frame.width = window_size.x - half_frame_size * 2.0;
            frame->ns_frame.y = window_pos.y - half_frame_size;
            frame->ns_frame.height = window_size.y + half_frame_size * 2.0;

            frame->ew_frame.x = window_pos.x - half_frame_size;
            frame->ew_frame.width = window_size.x + half_frame_size * 2.0;
            frame->ew_frame.y = window_pos.y + half_frame_size;
            frame->ew_frame.height = window_size.y - half_frame_size * 2.0;
        }
    }
}

bool BWUI_CheckWindowFrameCollision(BWWindowFrame* frame) {
    //Check for collision
    Rectangle* ew = &frame->ew_frame;           //East-West Frame
    Rectangle* ns = &frame->ns_frame;           //Nort-South Frame
    Vector2 mp = GetMousePositionAbsolute();    //Mouse Position

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
            window_frame->prev_mouse_pos = GetMousePositionAbsolute();
        }
    }
}
