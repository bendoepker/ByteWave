#include <bw-ui.h>

/* For Logging */
#include <bw-log.h>

#define CLAY_IMPLEMENTATION
#include "../../lib/ui/clay.h"
#include "../../lib/ui/clay_renderer.c"

/*
#ifdef _WIN32
#include <win-undef.h>
#include <windows.h>
#endif
*/

void _bw_ui_handle_clay_errors(Clay_ErrorData error_data) {
    BW_LOG_ERR("%s", error_data.errorText.chars);
}

void* BWUI_UIMain(void* ui_data) {

    Clay_Raylib_Initialize(1024, 768, "ByteWave", FLAG_WINDOW_RESIZABLE
                                                  | FLAG_WINDOW_HIGHDPI
                                                  | FLAG_MSAA_4X_HINT
                                                  | FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena clayMemory = (Clay_Arena) {
        .memory = malloc(clayRequiredMemory),
        .capacity = clayRequiredMemory
    };

    Clay_Initialize(clayMemory, (Clay_Dimensions){
        .width = GetScreenWidth(),
        .height = GetScreenHeight()
    }, (Clay_ErrorHandler) { _bw_ui_handle_clay_errors });

    Font fonts[1] = {0};
    fonts[0] = LoadFontEx("../res/Open_Sans/static/OpenSans-Regular.ttf", 48, 0, 400);
    SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);
    Clay_SetMeasureTextFunction(Raylib_MeasureText, &fonts[0]);

    SetExitKey(KEY_NULL);

    //Main render loop
    while(!WindowShouldClose()) {
        //Keyboard Handles
        if(IsKeyPressed(KEY_SPACE)) {
            //TODO: Pause / Resume playback
        }


        Clay_SetLayoutDimensions((Clay_Dimensions) {
            .width = GetScreenWidth(),
            .height = GetScreenHeight()
        });

        Clay_BeginLayout();

        CLAY({.id = CLAY_ID("Container"),
            .layout = {
                .sizing = {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_GROW()
            }
            },
             .backgroundColor = {
                140, 140, 140, 255
             }
        }){ /* Child Components in here */ }

        Clay_RenderCommandArray renderCommands = Clay_EndLayout();

        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(renderCommands, fonts);
        EndDrawing();
    }

    return 0;
}
