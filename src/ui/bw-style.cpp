#include "bw-style.h"
#include <bw-log.h>

static void CreateStyleBase(ImGuiStyle& s, Application& a) {
    s.FontSizeBase = 16;
    s.ScaleAllSizes(a.scale);
    s.FontScaleDpi = a.scale;
    s.WindowRounding = 0.f;
}

void BWUI::CreateDarkStyle(ImGuiStyle& s, Application& a) {
    CreateStyleBase(s, a);
    // Colors
    // Text
    s.Colors[ImGuiCol_Text] = HexCol(0xEAEAEAFF);
    s.Colors[ImGuiCol_TextDisabled] = HexCol(0xA9A9A9FF);

    // Background
    s.Colors[ImGuiCol_WindowBg] = HexCol(0x1E1E1EFF);
    s.Colors[ImGuiCol_ChildBg] = HexCol(0x1E1E1EFF);
    s.Colors[ImGuiCol_PopupBg] = HexCol(0x1E1E1EFF);

    s.Colors[ImGuiCol_Border] = HexCol(0x3C3C3CFF);
    s.Colors[ImGuiCol_FrameBg] = HexCol(0x3C3C3CFF);
    s.Colors[ImGuiCol_FrameBgHovered] = HexCol(0x3C3C3CFF);
    s.Colors[ImGuiCol_FrameBgActive] = HexCol(0x3C3C3CFF);

    // Buttons
    //s.Colors[ImGuiCol_Button] = HexCol(0x327898FF);
    s.Colors[ImGuiCol_Button] = HexCol(0x3C3C3CFF);
    //s.Colors[ImGuiCol_ButtonHovered] = HexCol(0x3E94BBFF);
    s.Colors[ImGuiCol_ButtonHovered] = HexCol(0x327898FF);
    s.Colors[ImGuiCol_ButtonActive] = HexCol(0x265B74FF);

    //s.Colors[BWCol_HazardButton] = HexCol(0xF44336FF);
    s.Colors[BWCol_HazardButton] = s.Colors[ImGuiCol_Button];
    s.Colors[BWCol_HazardButtonHovered] = HexCol(0xC73227FF);
    s.Colors[BWCol_HazardButtonClicked] = HexCol(0x912018FF);

    //s.Colors[BWCol_OkButton] = HexCol(0x4CAF50FF);
    s.Colors[BWCol_OkButton] = s.Colors[ImGuiCol_Button];
    //s.Colors[BWCol_OkButtonHovered] = HexCol(0x3B923EFF);
    s.Colors[BWCol_OkButtonHovered] = HexCol(0x4CAF50FF);
    s.Colors[BWCol_OkButtonClicked] = HexCol(0x29752CFF);
}

void BWUI::CreateLightStyle(ImGuiStyle& s, Application& a) {
    CreateStyleBase(s, a);
    //TODO:
    // Colors
    // Text
}
