#include <bw-ui.h>
#include <stdio.h>

/* For config data type */
#include <bw-types.h>

/* For keybind handling */
#include "bw-kb.h"

/* For UI Components */
#include "bw-ui-components.h"

/* For Logging */
#include <bw-log.h>

/*
#ifdef _WIN32
#include <win-undef.h>
#include <windows.h>
#endif
*/

#define MINIMUM_SCREEN_WIDTH 1200
#define MINIMUM_SCREEN_HEIGHT 800

void bcb() {
    BW_PRINT("PRESSED");
}

void* BWUI_UIMain(void* ui_data) {

    BWUIData* data = (BWUIData*)ui_data;

    if(data->config_data->window_width < MINIMUM_SCREEN_WIDTH) data->config_data->window_width = 800;
    if(data->config_data->window_height < MINIMUM_SCREEN_HEIGHT) data->config_data->window_height = 500;

    InitWindow(data->config_data->window_width, data->config_data->window_height, "ByteWave");
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);

    SetWindowMinSize(1200, 800);
    SetWindowMaxSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));

    SetTargetFPS(60);

    SetExitKey(KEY_NULL);

    Font fonts[1] = {0};
    fonts[0] = LoadFontEx("../res/Open_Sans/static/OpenSans-Regular.ttf", 48, 0, 400);
    SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);


    char key_press_list[32];
    const int MAX_NUM_KEYS = 32;

    BWVertSlider vert_slider;
    BWUI_CreateVSlider(&vert_slider, 100, 100, 80);

    BWHorizSlider horiz_slider;
    BWUI_CreateHSlider(&horiz_slider, 100, 200, 80);

    BWRingSlider ring_slider;
    BWUI_CreateRSlider(&ring_slider, 100, 300, 25);

    BWButton button;
    BWUI_CreateButton(&button, 100, 400, 100, 50, "Hello", bcb);

    //Main render loop
    while(!WindowShouldClose()) {
        //Keyboard Handles
        get_key_press_list(key_press_list, MAX_NUM_KEYS);


        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        //Text Display
        DrawText(key_press_list, 400, 400, 40, BLACK);

        //Vertical Slider Test
        BWUI_UpdateVSlider(&vert_slider);
        char arr[32];
        sprintf(arr, "%f", vert_slider.value);
        DrawText(arr, 150, 100, 40, BLACK);

        //Horizontal Slider Test
        BWUI_UpdateHSlider(&horiz_slider);
        char arr2[32];
        sprintf(arr2, "%f", horiz_slider.value);
        DrawText(arr2, 250, 200, 40, BLACK);

        //Ring Slider Test
        BWUI_UpdateRSlider(&ring_slider);
        char arr3[32];
        sprintf(arr3, "%f", ring_slider.value);
        DrawText(arr3, 150, 275, 40, BLACK);

        //Button Test
        BWUI_UpdateButton(&button);

        EndDrawing();
    }

    return 0;
}
