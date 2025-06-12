#include <bw-ui.h>
#include <stdio.h>

/* For config data type */
#include <bw-types.h>

/* For trig funcs */
#include <math.h>

/* For colors */
#include "bw-colors.h"

/* For keybind handling */
#include "bw-kb.h"

/* For UI Components */
#include "bw-toplevel-ui.h"

/* For Mouse Functions */
#include "bw-mouse.h"

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

void* BWUI_UIMain(void* ui_data) {

    //Should we exit the application:
    unsigned char exit_requested = 0;


    BWUIData* data = (BWUIData*)ui_data;

    if(data->config_data->window_width < MINIMUM_SCREEN_WIDTH) data->config_data->window_width = MINIMUM_SCREEN_WIDTH;
    if(data->config_data->window_height < MINIMUM_SCREEN_HEIGHT) data->config_data->window_height = MINIMUM_SCREEN_HEIGHT;

    InitWindow(data->config_data->window_width, data->config_data->window_height, "ByteWave");

    SetWindowState(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_RESIZABLE);

    Image bytewave_icon = LoadImage("../res/bytewave-icon.png");
    SetWindowIcon(bytewave_icon);
    UnloadImage(bytewave_icon);

    SetWindowMinSize(MINIMUM_SCREEN_WIDTH, MINIMUM_SCREEN_HEIGHT);
    SetWindowMaxSize(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));

    SetTargetFPS(60);

    SetExitKey(KEY_NULL);

    Font fonts[1] = {0};
    fonts[0] = LoadFontEx("../res/Open_Sans/static/OpenSans-Regular.ttf", 48, 0, 400);
    SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);

    BWTitleBar title_bar;
    BWUI_CreateTitleBar(&title_bar, &exit_requested);

    BWWindowFrame window_frame;
    BWUI_CreateWindowFrame(&window_frame, (Vector2){.x = MINIMUM_SCREEN_WIDTH, .y = MINIMUM_SCREEN_HEIGHT});

    BWVertSlider vert_slider;
    BWUI_CreateVSlider(&vert_slider, 100, 100, 80);

    BWHorizSlider horiz_slider;
    BWUI_CreateHSlider(&horiz_slider, 100, 200, 80);

    BWRingSlider ring_slider;
    BWUI_CreateRSlider(&ring_slider, 100, 300, 15);

    BWMixerUI mixer_ui;
    BWUI_CreateMixer(&mixer_ui, 0, 0);

    BWToggleCluster toggle_cluster;
    BWUI_CreateToggleCluster(&toggle_cluster, &mixer_ui);


    BWToplevelUI toplevel;
    toplevel.toggle_cluster = &toggle_cluster;
    toplevel.mixer = &mixer_ui;
    toplevel.title_bar = &title_bar;
    toplevel.window_frame = &window_frame;

    BWUI_EnableOSMousePos();

    BWMouseState left_mouse_state;
    BWMouseState right_mouse_state;
    BWMouseLocale mouse_locale;

    //Main render loop
    while(!exit_requested) {

        Vector2 mouse_pos = GetMousePosition();
        left_mouse_state = BWUI_GetMouseState(MOUSE_BUTTON_LEFT);
        right_mouse_state = BWUI_GetMouseState(MOUSE_BUTTON_RIGHT);
        mouse_locale = BWUI_GetMouseLocale(mouse_pos, &toplevel);

        if(left_mouse_state != MOUSE_UP) {
            BWUI_HandleMouseByLocale(left_mouse_state, MOUSE_BUTTON_LEFT, mouse_locale, mouse_pos, &toplevel);
        }
        if(right_mouse_state != MOUSE_UP) {
            BWUI_HandleMouseByLocale(left_mouse_state, MOUSE_BUTTON_RIGHT, mouse_locale, mouse_pos, &toplevel);
        }

        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        //Background
        float width = GetScreenWidth();
        float height = GetScreenHeight();
        float hypotenuse = ceilf(sqrt(width * width + height * height));
        DrawCircleGradient(width / 2.0, height / 2.0, hypotenuse, GRAY5, GRAY0);

        //This function sets the cursor to MOUSE_CURSOR_RESIZE_XXXX or MOUSE_CURSOR_DEFAULT depending on cursor location
        BWUI_UpdateWindowFrame(&window_frame);

        BWUI_UpdateTitleBar(&title_bar, fonts);

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

        BWUI_UpdateToggleCluster(&toggle_cluster);
        BWUI_UpdateMixer(&mixer_ui);

        EndDrawing();

        //If a close request comes from elsewher (Likely a wayland workaround)
        if(WindowShouldClose()) exit_requested = 1;
    }

    BWUI_DestroyTitleBar(&title_bar);
    BWUI_DestroyToggleCluster(&toggle_cluster);

    return 0;
}
