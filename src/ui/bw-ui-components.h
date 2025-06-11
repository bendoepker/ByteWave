#ifndef BW_UI_COMPONENTS_H
#define BW_UI_COMPONENTS_H

#include "bw-ui-types.h"

//SECTION: VERTICAL SLIDER
void BWUI_CreateVSlider(BWVertSlider* slider, int pos_x, int pos_y, int bar_length);
void BWUI_UpdateVSlider(BWVertSlider* slider);
Rectangle BWUI_GetVSliderRec(BWVertSlider* slider);
void BWUI_VSliderHandleMouse(BWVertSlider* slider, BWMouseState state, int button, Vector2 mouse_pos);

//SECTION: HORIZONTAL SLIDER
void BWUI_CreateHSlider(BWHorizSlider* slider, int pos_x, int pos_y, int bar_length);
void BWUI_UpdateHSlider(BWHorizSlider* slider);
Rectangle BWUI_GetHSliderRec(BWHorizSlider* slider);
void BWUI_HSliderHandleMouse(BWHorizSlider* slider, BWMouseState state, int button, Vector2 mouse_pos);

//SECTION: ROUND SLIDER
void BWUI_CreateRSlider(BWRingSlider* slider, int pos_x, int pos_y, int radius);
void BWUI_UpdateRSlider(BWRingSlider* slider);
Rectangle BWUI_GetRSliderRec(BWRingSlider* slider);
void BWUI_RSliderHandleMouse(BWRingSlider* slider, BWMouseState state, int button, Vector2 mouse_pos);

//SECTION: TEXT BUTTON
void BWUI_CreateTextButton(BWTextButton* button, int pos_x, int pos_y, int width, int height, char* text, void(*callback), void* callback_params);
void BWUI_UpdateTextButton(BWTextButton* button);
Rectangle BWUI_GetTextButtonRec(BWTextButton* button);
void BWUI_TextButtonHandleMouse(BWTextButton* button, BWMouseState state, int mouse_button, Vector2 mouse_pos);

//SECTION: IMAGE BUTTON
void BWUI_CreateImageButton(BWImageButton* button, int pos_x, int pos_y, int width, int height, Image image, Image image_clicked, void(*callback), void* callback_params);
void BWUI_DestroyImageButton(BWImageButton* button);
void BWUI_UpdateImageButton(BWImageButton* button);
Rectangle BWUI_GetImageButtonRec(BWImageButton* button);
void BWUI_ImageButtonHandleMouse(BWImageButton* button, BWMouseState state, int mouse_button, Vector2 mouse_pos);

#endif //BW_UI_COMPONENTS_H
