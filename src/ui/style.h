#ifndef STYLE_H
#define STYLE_H

#include <stdint.h>
#include <imgui.h>
#include "application.h"

/*
 *  Takes a 32 bit hex value and converts it to a vec4 of floats in range 0.0 => 1.0
 *  0x12345678 => x:0x12, y:0x34, z:0x56, w:0x78
 */
consteval ImVec4 HexCol(const uint32_t in) {
    return ImVec4 (
        ((in & 0xff000000) >> 24) / 255.f,
        ((in & 0x00ff0000) >> 16) / 255.f,
        ((in & 0x0000ff00) >> 8) / 255.f,
        (in & 0x000000ff) / 255.f
    );
}

namespace BWUI {

/*
 *  Sets the style passed into the function to a dark mode
 */
void CreateDarkStyle(ImGuiStyle& style, Application& app);

/*
 *  Sets the style passed into the function to a light mode
 */
void CreateLightStyle(ImGuiStyle& style, Application& app);

} //namespace BWUI

#endif //STYLE_H
