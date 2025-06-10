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

//SECTION: TEXT BUTTON
typedef struct {
    Rectangle button;
    void (*callback)(void);
    char text[64];

    int font_size;
    Vector2 text_offset;

    bool clicked;
} BWTextButton;

void BWUI_CreateTextButton(BWTextButton* button, int pos_x, int pos_y, int width, int height, char* text, void(*callback));
void BWUI_UpdateTextButton(BWTextButton* button);

//SECTION: IMAGE BUTTON
typedef struct {
    Rectangle hitbox;
    void (*callback)(void);
    Texture texture;
    Texture texture_clicked;

    Vector2 texture_offset;
    float scale_factor;

    bool clicked;
} BWImageButton;

void BWUI_CreateImageButton(BWImageButton* button, int pos_x, int pos_y, int width, int height, Image image, Image image_clicked, void(*callback));
void BWUI_UpdateImageButton(BWImageButton* button);

#endif //BW_UI_COMPONENTS_H
