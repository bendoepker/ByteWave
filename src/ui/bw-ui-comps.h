#ifndef BW_UI_COMPS_H
#define BW_UI_COMPS_H

#include <imgui.h>

namespace BWUI {
    bool HazardButton(const char* label, const ImVec2& size = ImVec2(0, 0), ImGuiButtonFlags flags = 0);
    bool OkButton(const char* label, const ImVec2& size = ImVec2(0, 0), ImGuiButtonFlags flags = 0);
}

#endif //BW_UI_COMPS_H
