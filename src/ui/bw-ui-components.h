#ifndef BW_UI_COMPONENTS_H
#define BW_UI_COMPONENTS_H

/* For bool */
#include <stdbool.h>

/* For Raylib Types */
#include "../../lib/ui/raylib.h"

//SECTION: VERTICAL SLIDER
typedef struct {
    Rectangle bar;
    Rectangle track;

    float value;

    bool clicked;
} BWVertSlider;

void BWUI_CreateVSlider(BWVertSlider* slider, int pos_x, int pos_y, int bar_length);
void BWUI_UpdateVSlider(BWVertSlider* slider);

//SECTION: HORIZONTAL SLIDER
typedef struct {
    Rectangle bar;
    Rectangle track;

    float value;

    bool clicked;

} BWHorizSlider;

void BWUI_CreateHSlider(BWHorizSlider* slider, int pos_x, int pos_y, int bar_length);
void BWUI_UpdateHSlider(BWHorizSlider* slider);

//SECTION: ROUND SLIDER
typedef struct {
    Rectangle bar;
    Vector2 pos;
    int radius;

    float value;

    bool clicked;
} BWRingSlider;

void BWUI_CreateRSlider(BWRingSlider* slider, int pos_x, int pos_y, int radius);
void BWUI_UpdateRSlider(BWRingSlider* slider);

//SECTION: BUTTON
typedef struct {
    Rectangle button;
    void (*callback)(void);
    char text[64];

    int font_size;
    Vector2 text_offset;

    bool clicked;
} BWButton;

void BWUI_CreateButton(BWButton* button, int pos_x, int pos_y, int width, int height, char* text, void(*callback));
void BWUI_UpdateButton(BWButton* button);

#endif //BW_UI_COMPONENTS_H
