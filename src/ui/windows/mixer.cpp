#include <imgui.h>
#include <vector>

namespace {

struct MixerUITrack {
    float level_left = 0.f;
    float level_right = 0.f;
    float gain_level = 1.f;
};

}

namespace BWUI {
void ShowMixer(ImGuiWindowFlags flags, bool* show_mixer) {

    static std::vector<MixerUITrack> tracks;
    if(!tracks.size()) {
        tracks.push_back(MixerUITrack { 1.0, 1.0, .5 });
        tracks.push_back(MixerUITrack { 0.5, 0.5, .3 });
        tracks.push_back(MixerUITrack { 0.3, 0.3, .2 });
        tracks.push_back(MixerUITrack { 0.2, 0.2, .1 });
    }

    ImGui::Begin("Mixer", show_mixer, flags);

    ImGui::Begin("MixerScrollContainer", 0, ImGuiWindowFlags_HorizontalScrollbar);

    int idx = 0;
    for(auto track : tracks) {
        if(idx > 0) ImGui::SameLine();
        ImGui::Text("Val %d: %f", idx, track.gain_level);
        ImGui::SameLine();
        ImGui::Spacing();
        idx++;
    }

    ImGui::End(); // Mixer Scroll Container

    ImGui::End();
}
}
