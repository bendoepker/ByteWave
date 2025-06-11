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
    ML_WINDOW_FRAME,
    ML_TITLE_BAR,
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
    Rectangle ns_frame;
    Rectangle ew_frame;
    Vector2 prev_mouse_pos;
    bool clicked;
} BWWindowFrame;

typedef struct {
    Rectangle hitbox;
    char title[9];
    BWImageButton minimize_button;
    BWImageButton maximize_button;
    BWImageButton close_button;
} BWTitleBar;

typedef struct {
    Rectangle hitbox;

    //TODO: Replace this with a scrollbar object
    /*Rectangle sb_thumb; //Scroll bar thumb */ 

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
    BWWindowFrame* window_frame;
    BWTitleBar* title_bar;
    BWToggleCluster* toggle_cluster;

    //TODO:
    //BWSystemInfo* system_info;
    //BWToolSelect* tool_select;
    //BWPlaylist* playlist

    BWMixerUI* mixer;
} BWToplevelUI;

/*
*   UI Hierarchy (Tree)
*
*   BWToplevelUI
*   |
*   +--BWWindowFrame
*   |
*   +--BWTitleBar
*   |  +--BWImageButton minimize_button
*   |  +--BWImageButton maximize_button
*   |  +--BWImageButton close_button
*   |
*   +--BWToggleCluster
*   |  +--BWImageButton mixer_toggle
*   |  +--BwImageButton playback_pause (pb_pause)
*   |  +--BwImageButton playback_play (pb_play)
*   |  +--BwImageButton playback_stop (pb_stop)
*   |
*   +--BWSystemInfo
*   |
*   +--BWToolSelect
*   |
*   +--BWPlaylist
*   |
*   +--BWMixer
*      +--BWVertSlider gain_slider
*      +--BWRingSlider pan_slider
*      +--BWImageButton mute_button
*      +--BWImageButton solo_button
*      +--BWImageButton routing_button
*
*
*
*/

#endif //BW_UI_TYPES_H
