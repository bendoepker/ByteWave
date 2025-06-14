#ifndef BW_WINDOW_H
#define BW_WINDOW_H
#include "../../lib/ui/raylib.h"
#include "bw-ui-types.h"

/*
*
*   Window Functions to create custom frame and title bar
*
*/

#ifdef BW_CUSTOM_WINDOW
#   define BW_WINDOW_DECORATED FLAG_WINDOW_UNDECORATED
#else
#   define BW_WINDOW_DECORATED 0
#endif

void BWUI_CreateTitleBar(BWTitleBar* title_bar, void* exit_request);
void BWUI_UpdateTitleBar(BWTitleBar* title_bar, Font* font);
Rectangle BWUI_GetTitleBarRec(BWTitleBar* title_bar);
void BWUI_TitleBarHandleMouse(BWTitleBar* title_bar, BWMouseState state, int button);
void BWUI_DestroyTitleBar(BWTitleBar* title_bar);

void BWUI_CreateWindowFrame(BWWindowFrame* frame, Vector2 min_window_size);
void BWUI_UpdateWindowFrame(BWWindowFrame* frame);
bool BWUI_CheckWindowFrameCollision(BWWindowFrame* frame);
void BWUI_WindowFrameHandleMouse(BWWindowFrame* window_frame, BWMouseState state, int button);

#endif //BW_WINDOW_H
