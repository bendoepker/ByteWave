#include "bw-ui-components.h"

/* For Logging */
#include <bw-log.h>

/* For trig funcs */
#include <math.h>

/* For strncpy() */
#include <string.h>

void BWUI_CreateVSlider(BWVertSlider* slider, int pos_x, int pos_y, int bar_length) {
    slider->bar.x = pos_x;
    slider->bar.y = pos_y;
    slider->bar.width = 23;
    slider->bar.height = 10;
    slider->track.x = pos_x + 10;
    slider->track.y = pos_y + 5;
    slider->track.width = 3;
    slider->track.height = bar_length;
    slider->value = 1.0f;
}
void BWUI_UpdateVSlider(BWVertSlider* slider) {
    //Update the slider
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !slider->clicked){
        Vector2 mouse_pos = GetMousePosition();
        if(mouse_pos.x >= slider->bar.x && mouse_pos.x <= slider->bar.x + slider->bar.width &&
           mouse_pos.y >= slider->bar.y && mouse_pos.y <= slider->bar.y + slider->bar.height)
            slider->clicked = true;
    } else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && slider->clicked) {
        //Holding down and moving the slider
        Vector2 mouse_pos = GetMousePosition();
        if(mouse_pos.y >= slider->track.y + slider->track.height) {
            //Mouse moved below the slider
            slider->value = 0.0f;
        } else if(mouse_pos.y <= slider->track.y) {
            //Mouse moved above the slider
            slider->value = 1.0f;
        } else {
            //Mouse moved within the slider range
            slider->value = 1.0f - ((float)(mouse_pos.y - slider->track.y) / (float) slider->track.height);
        }
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && slider->clicked) {
        // mouse released after hold
#if 0
        Vector2 mouse_pos = GetMousePosition();
        if(mouse_pos.y >= slider->track.y + slider->track.height) {
            //Mouse released below the slider
            slider->value = 0.0f;
        } else if(mouse_pos.y <= slider->track.y) {
            //Mouse releaseed above the slider
            slider->value = 1.0f;
        } else {
            //Mouse released within the slider range
            slider->value = 1.0f - ((float)(mouse_pos.y - slider->track.y) / (float) slider->track.height);
        }
#endif //This extra bit of processing seems unnecessary
        slider->clicked = false;
    }

    //Move the bar
    slider->bar.y = slider->track.y - 5 + ((slider->track.height - (int)(slider->track.height * slider->value)));

    //Draw the slider
    DrawRectangleRounded(slider->track, 1.0, 2, DARKGRAY);
    if(slider->clicked) DrawRectangleRounded(slider->bar, 10.0, 5, RED);
    else DrawRectangleRounded(slider->bar, 10.0, 5, BLACK);
}

void BWUI_CreateHSlider(BWHorizSlider* slider, int pos_x, int pos_y, int bar_length) {
    slider->bar.x = pos_x;
    slider->bar.y = pos_y;
    slider->bar.height = 23;
    slider->bar.width = 10;
    slider->track.x = pos_x + 5;
    slider->track.y = pos_y + 10;
    slider->track.width = bar_length;
    slider->track.height = 3;
    slider->value = 1.0f;
}

void BWUI_UpdateHSlider(BWHorizSlider* slider) {
    //Update the slider
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !slider->clicked){
        Vector2 mouse_pos = GetMousePosition();
        if(mouse_pos.x >= slider->bar.x && mouse_pos.x <= slider->bar.x + slider->bar.width &&
           mouse_pos.y >= slider->bar.y && mouse_pos.y <= slider->bar.y + slider->bar.height)
            slider->clicked = true;
    } else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && slider->clicked) {
        //Holding down and moving the slider
        Vector2 mouse_pos = GetMousePosition();
        if(mouse_pos.x <= slider->track.x) {
            //Mouse moved to the left of the slider
            slider->value = 0.0f;
        } else if(mouse_pos.x >= slider->track.x + slider->track.width) {
            //Mouse moved to the right of the slider
            slider->value = 1.0f;
        } else {
            //Mouse moved within the slider range
            slider->value = (float)((mouse_pos.x - slider->track.x) / slider->track.width);
        }
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && slider->clicked) {
        // mouse released after hold
#if 0 //This extra bit of processing seems unnecessary
        Vector2 mouse_pos = GetMousePosition();
        if(mouse_pos.x <= slider->track.x) {
            //Mouse released to the left of the slider
            slider->value = 0.0f;
        } else if(mouse_pos.x >= slider->track.x + slider->track.width) {
            //Mouse released to the right of the slider
            slider->value = 1.0f;
        } else {
            //Mouse released within the slider range
            slider->value = (float)((mouse_pos.x - slider->track.x) / slider->track.width);
        }
#endif
        slider->clicked = false;
    }

    //Move the bar
    slider->bar.x = slider->track.x - 5 + (int)(slider->track.width * slider->value);

    //Draw the slider
    DrawRectangleRounded(slider->track, 1.0, 2, DARKGRAY);
    if(slider->clicked) DrawRectangleRounded(slider->bar, 10.0, 5, RED);
    else DrawRectangleRounded(slider->bar, 10.0, 5, BLACK);
}

void BWUI_CreateRSlider(BWRingSlider* slider, int pos_x, int pos_y, int radius) {
    slider->pos.x = pos_x;
    slider->pos.y = pos_y;
    slider->radius = radius;

    slider->bar.x = pos_x;
    slider->bar.y = pos_y;
    slider->bar.width = 24;
    slider->bar.height = 10;

    slider->value = 0.0;
}

static inline bool _check_collision_ring_slider(BWRingSlider* slider, Vector2 pos) {
    Vector2 p1 = {0};
    Vector2 p2 = {0};
    Vector2 p3 = {0};
    Vector2 p4 = {0};

    //HACK: magic numbers as described below
    float rotation = 240 * slider->value + 150;

    float sinRotation = sinf(rotation*DEG2RAD);
    float cosRotation = cosf(rotation*DEG2RAD);
    float x = slider->pos.x;
    float y = slider->pos.y;
    //HACK: magic numbers again
    float dx = -(11 - slider->radius);
    float dy = -(5);

    //'Top Left'
    p1.x = x + dx*cosRotation - dy*sinRotation;
    p1.y = y + dx*sinRotation + dy*cosRotation;

    //'Top Right'
    p2.x = x + (dx + slider->bar.width)*cosRotation - dy*sinRotation;
    p2.y = y + (dx + slider->bar.width)*sinRotation + dy*cosRotation;

    //'Bottom Left'
    p3.x = x + dx*cosRotation - (dy + slider->bar.height)*sinRotation;
    p3.y = y + dx*sinRotation + (dy + slider->bar.height)*cosRotation;

    //'Bottom Right'
    p4.x = x + (dx + slider->bar.width)*cosRotation - (dy + slider->bar.height)*sinRotation;
    p4.y = y + (dx + slider->bar.width)*sinRotation + (dy + slider->bar.height)*cosRotation;

    /*
    *  p1   p2
    *   +---+
    *   |\  |
    *   | \ |
    *   |  \|
    *   +---+
    *  p3   p4
    *
    *  now we can use the raylib triangle collision function to test for collision
    */
    if(CheckCollisionPointTriangle(pos, p1, p2, p4)) return true;
    else if(CheckCollisionPointTriangle(pos, p1, p3, p4)) return true;
    else return false;
}

static inline float _get_angle_ring_slider(BWRingSlider* slider, Vector2 pos) {
    double y = (double)pos.y - (double)slider->pos.y;
    double x = (double)pos.x - (double)slider->pos.x;

    return atan2(y, x);
}

void BWUI_UpdateRSlider(BWRingSlider* slider) {

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !slider->clicked) {
        Vector2 mouse_pos = GetMousePosition();
        if(_check_collision_ring_slider(slider, mouse_pos)) {
            slider->clicked = true;
        }
    } else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && slider->clicked) {
        //Holding down and moving the slider
        Vector2 mouse_pos = GetMousePosition();
        //Get the angle of the mouse from the center of the ring
        float theta = _get_angle_ring_slider(slider, mouse_pos) * RAD2DEG;
        if(theta >= 30.0 && theta <= 90) {
            //Slider is between the right side and straight down
            slider->value = 1.0f;
        } else if (theta >= 90 && theta <= 150) {
            //Slider is between the left side and straight down
            slider->value = 0.0f;
        } else {
            //Slider is a valid value
            if(theta >= 150.0) theta -= 150; //std treats bottom half as negative and top half as positive
            else theta += 210;
            slider->value = theta / 240.f;
        }
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && slider->clicked) {
        slider->clicked = false;
    }

    //Draw the ring slider
    //HACK: Lots of magic numbers here, so this is the rundown:
    //      For DrawRing():
    //          -208: minimum angle is -210, -208 will keep the end of the ring near the middle of the bar at extremes
    //          30: maximum angle is 30, same as -210
    //          32: number of segments rendered
    //      For DrawRectanglePro():
    //          in (Vector2):
    //              11 - slider->radius: offsets the bar 11 pixels from the center of radius (center of circle)
    //              5: half of the height of the bar, aligns the rectangle to the circle
    //          in (240...):
    //              240 * slider->value: angle of the arc is 240°
    //              150: the clockwise offset of the beginning of the arc is 150 degrees (inverse of -210)
    DrawRing(slider->pos, slider->radius, slider->radius + 3, -208, 28, 32, DARKGRAY);
    if(slider->clicked) DrawRectanglePro(slider->bar, (Vector2){11 - slider->radius, 5}, (240 * slider->value + 150), RED);
    else DrawRectanglePro(slider->bar, (Vector2){11 - slider->radius, 5}, (240 * slider->value + 150), BLACK);
}

void BWUI_CreateTextButton(BWTextButton* button, int pos_x, int pos_y, int width, int height, char* text, void(*callback)) {
    button->button.x = pos_x;
    button->button.y = pos_y;
    button->button.width = width;
    button->button.height = height;
    strncpy(button->text, text, 64);
    if(callback != 0)
        button->callback = callback;

    //Find the font size that will fill the box but not overflow it
    button->font_size = 0;
    while(true) {
        ++button->font_size;
        Vector2 real_size = MeasureTextEx(GetFontDefault(), text, button->font_size, ceilf(button->font_size / 10.0));

        //HACK: Magic number 3: im giving a default padding of 3 pixels (so there should be 6 pixels extra)
        if(real_size.x >= button->button.width - 6.0) break;
        if(real_size.y >= button->button.height - 6.0) break;
    }
    --button->font_size;

    //Get the text offset so that it is centered on the button
    Vector2 real_size = MeasureTextEx(GetFontDefault(), text, button->font_size, ceilf(button->font_size / 10.0));
    button->text_offset.x = ceilf((button->button.width - real_size.x) / 2.0);
    button->text_offset.y = ceilf((button->button.height - real_size.y) / 2.0);
}

void BWUI_UpdateTextButton(BWTextButton* button) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse_pos = GetMousePosition();
        if(CheckCollisionPointRec(mouse_pos, button->button)) {
            button->clicked = true;
        }
    } else if(button->clicked && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse_pos = GetMousePosition();
        if(button->callback != 0)
            if(CheckCollisionPointRec(mouse_pos, button->button))
                button->callback();
        button->clicked = false;
    }

    //Draw Button
    if(button->clicked) DrawRectangleRec(button->button, RED);
    else DrawRectangleRec(button->button, DARKGRAY);
    DrawText(button->text, button->button.x + button->text_offset.x, button->button.y + button->text_offset.y, button->font_size, BLACK);
}

void BWUI_CreateImageButton(BWImageButton* button, int pos_x, int pos_y, int width, int height, Image image, Image image_clicked, void(*callback)) {
    button->hitbox.x = pos_x;
    button->hitbox.y = pos_y;
    button->hitbox.width = width;
    button->hitbox.height = height;
    button->texture = LoadTextureFromImage(image);
    button->texture_clicked = LoadTextureFromImage(image_clicked);
    button->callback = callback;

    if(button->texture.width > width || button->texture.height > height) {
        //Scale the texture down to fit within the hitbox
        float width_scale = (float)button->texture.width / (float)width;
        float height_scale = (float)button->texture.height / (float)height;
        button->scale_factor = (1.0 / fmax(width_scale, height_scale));
    } else if (button->texture.width < width || button->texture.height < height) {
        //Scale the texture up to fit the hitbox
        float width_scale = (float)button->texture.width / (float)width;
        float height_scale = (float)button->texture.height / (float)height;
        button->scale_factor = (1.0 / fmax(width_scale, height_scale));
    } else {
        button->scale_factor = 1.0;
    }

    if(button->texture.width < button->hitbox.width)
        button->texture_offset.x = (button->hitbox.width - button->texture.width) / 2.0;

    if(button->texture.height < button->hitbox.height)
        button->texture_offset.y = (button->hitbox.height - button->texture.height) / 2.0;
}

void BWUI_UpdateImageButton(BWImageButton* button) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse_pos = GetMousePosition();
        if(CheckCollisionPointRec(mouse_pos, button->hitbox))
            button->clicked = true;
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && button->clicked) {
        Vector2 mouse_pos = GetMousePosition();
        if(CheckCollisionPointRec(mouse_pos, button->hitbox))
            button->callback();
        button->clicked = false;
    }

    //Draw button
    if(button->clicked)
        DrawTextureEx(button->texture_clicked,
                      (Vector2){button->hitbox.x + button->texture_offset.x, button->hitbox.y + button->texture_offset.y},
                      0, button->scale_factor, WHITE);
    else
        DrawTextureEx(button->texture,
                      (Vector2){button->hitbox.x + button->texture_offset.x, button->hitbox.y + button->texture_offset.y},
                      0, button->scale_factor, WHITE);
}
