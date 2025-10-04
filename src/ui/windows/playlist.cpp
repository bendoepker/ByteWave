#include <imgui.h>

namespace BWUI {
void ShowPlaylist(ImGuiWindowFlags flags, bool* show_playlist) {
    // Playlist Code
    ImGui::Begin("Playlist", show_playlist, flags);

    ImGui::End();
}
}
