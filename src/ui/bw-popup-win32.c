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

#include "bw-popup.h"
#include <windows.h>
#include <bw-log.h>

HWND popup_handle = 0;
unsigned char ec = 0; //Exit confirmed;

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_DESTROY:
            DestroyWindow(popup_handle);
            popup_handle = 0;
            return 0;
        case WM_PAINT:
            //TODO: ^
            return 0;
        case WM_LBUTTONDOWN:
            ec = 1;
            return 0;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

void BWUI_ClosePopup(void* handle, unsigned char* exit_confirm) {
    const char CLASS_NAME[] = "CloseWindowPopup";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = wnd_proc;
    wc.hInstance = NULL;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    RegisterClass(&wc);

    RECT rect;
    GetWindowRect(handle, &rect);
    int x = rect.right - rect.left;
    x /= 2;
    int y = rect.bottom - rect.top;
    y /= 2;

    popup_handle = CreateWindowEx(0, CLASS_NAME, "Close Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                  x, y, 400, 200, handle, NULL, NULL, NULL);
    if(popup_handle == 0) {
        *exit_confirm = 0;
        BW_LOG_ERR("Failed to create close popup window");
        return;
    }

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if(popup_handle == 0) break;
        if(ec) {
            DestroyWindow(popup_handle);
            *exit_confirm = 1;
            popup_handle = 0;
            break;
        }
    }

    return;
}
