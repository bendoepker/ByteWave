#ifndef BW_UI_TYPES_H
#define BW_UI_TYPES_H
#include "../../lib/ui/raylib.h"
#include <stdbool.h>

typedef enum {
    MOUSE_UP,
    MOUSE_PRESSED,
    MOUSE_DOWN,
    MOUSE_RELEASED,
    MOUSE_CTRL_PRESSED,
    MOUSE_ALT_PRESSED,
    MOUSE_SHIFT_PRESSED,
} BWMouseState;

typedef enum {
    ML_TOGGLE_CLUSTER,
    ML_SYSTEM_INFO, //Temp name for proposed diagnostics block
    ML_TOOL_SELECT, //Temp name for proposed tool selection
    ML_PLAYLIST,
    ML_EFFECT_WINDOW,
    ML_MIXER,
    ML_NONE,
} BWMouseLocale;

typedef struct {
    Rectangle bar;
    Rectangle track;

    float value;

    bool clicked;
} BWVertSlider;

typedef struct {
    Rectangle bar;
    Rectangle track;

    float value;

    bool clicked;

} BWHorizSlider;

typedef struct {
    Rectangle bar;
    Rectangle bounding_box;
    Vector2 bar_offset;
    Vector2 pos;
    int radius;

    float value;

    bool clicked;
} BWRingSlider;

typedef struct {
    Rectangle hitbox;
    char text[64];

    void (*callback)(void*);
    void* callback_params;

    int font_size;
    Vector2 text_offset;

    bool clicked;
} BWTextButton;

typedef struct {
    Rectangle hitbox;
    Texture texture;
    Texture texture_clicked;

    void (*callback)(void*);
    void* callback_params;

    Vector2 texture_offset;
    float scale_factor;

    bool clicked;
} BWImageButton;

typedef struct {
    Rectangle hitbox;
    Rectangle sb_thumb; //Scroll bar thumb

    bool is_open;

    int num_tracks;
} BWMixerUI;

typedef struct {
    //Total area of the toggle cluster
    Rectangle hitbox;

    //Mixer
    BWImageButton mixer_toggle;
    BWMixerUI* mixer_ui_ref;

    //Playback
    BWImageButton pb_pause;
    BWImageButton pb_play;
    BWImageButton pb_stop;
} BWToggleCluster;

/*
*
*   Generally how the UI should look:
*
*   +-----------------------------------------------------------+
*   | Toggle Cluster      | System Info      | Tool Select      |
*   |-----------------------------------------------------------|
*   | Playlist Track                                            |
*   | - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
*   | Playlist Track                                            |
*   | - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
*   | Playlist Track                                            |
*   | - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
*   | Playlist Track                                            |
*   | - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
*   | Playlist Track                                            |
*   | - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
*   | Playlist Track                                            |
*   |-----------------------------------------------------------|
*   | Mixer Tracks|      |      |      |      |      |      |   |
*   | (Only Shown when toggled) |      |      |      |      |   |
*   |      |      |      |      |      |      |      |      |   |
*   |      |      |      |      |      |      |      |      |   |
*   |      |      |      |      |      |      |      |      |   |
*   |-----------------------------------------------------------|
*
*/

//This struct should generally adhere to the above template
typedef struct {
    BWToggleCluster* toggle_cluster;

    //TODO:
    //BWSystemInfo* system_info;
    //BWToolSelect* tool_select;
    //BWPlaylist* playlist

    BWMixerUI* mixer;
} BWToplevelUI;

#endif //BW_UI_TYPES_H
