#include <imgui.h>
#include "bw-ui-comps.h"

void ShowPlaylist(ImGuiWindowFlags flags, bool* show_playlist) {
    // Playlist Code
    ImGui::Begin("Playlist", show_playlist, flags);

    ImGui::End();
}
