#include "prompts.h"
#include <imgui.h>
#include <log.h>
#include <hostapi.h>

void PromptForAudioBackend(BWConfigData* config_data, UITriggers* triggers) {
    ImGui::OpenPopup("backend_select_popup");
    triggers->open_backend_popup = false;
}

void AudioBackendPopup() {
    static BWHostApi selected_hostapi = UNKNOWN;
    static const ImGuiWindowFlags win_flags = ImGuiWindowFlags_NoResize |
                                              ImGuiWindowFlags_NoMove;
    if(ImGui::BeginPopupModal("backend_select_popup", NULL, win_flags)) {
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 center(io.DisplaySize.x * .5f - 200, io.DisplaySize.y * .5f - 150);
        ImGui::SetWindowSize(ImVec2(400, 300), ImGuiCond_Always);
        ImGui::SetWindowPos(center, ImGuiCond_Always);

        ImGui::SeparatorText("Select an audio backend");
    #ifdef BW_JACK
        if(ImGui::Button("JACK")) {
            selected_hostapi = JACK;
        }
    #endif
    #ifdef BW_ALSA
        if(ImGui::Button("ALSA")) {
            selected_hostapi = ALSA;
        }
    #endif
    #ifdef BW_ASIO
        if(ImGui::Button("Asio")) {
            selected_hostapi = ASIO;
        }
    #endif

        if(ImGui::Button("Confirm")) {
            if(BWAudioBackend::GetCurrentApi() != selected_hostapi) {
                if(BWAudioBackend::IsActivated())
                    BWAudioBackend::Deactivate();
                BWAudioBackend::ChangeHostApi(selected_hostapi);
            } else {
                BWAudioBackend::Activate();
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void PromptForClose() {

}
