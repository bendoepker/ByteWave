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
    Vector2 min_window_size;
    enum ResizeDirection {
        NE,
        N,
        NW,
        W,
        SW,
        S,
        SE,
        E,
    } resize_direction;
    bool clicked;
} BWWindowFrame;

typedef struct {
    Rectangle hitbox;
    Rectangle render_box;

    char title[9];
    BWImageButton minimize_button;
    BWImageButton maximize_button;
    BWImageButton close_button;

    Vector2 prev_mouse_pos;
    bool clicked;
    bool double_clicked;
    int frames_since_click;
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

typedef struct {
    Texture spectrum_analyzer;
    Texture cpu_load_measure;
    float cpu_load;
} BWSystemInfo;

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
    BWSystemInfo* system_info;

    //TODO:
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
