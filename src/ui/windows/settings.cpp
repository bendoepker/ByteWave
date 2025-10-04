#include <imgui.h>

namespace BWUI {
void ShowSettings(ImGuiWindowFlags flags, bool* show_settings) {
    ImGui::Begin("ByteWave Settings", show_settings, flags);

    ImGui::End();
}
}
