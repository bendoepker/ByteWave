#include "application.h"
#include "prompts.h"

void Application::RenderFrame() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGuiWindowFlags main_viewport_flags = ImGuiWindowFlags_NoResize |
                                           ImGuiWindowFlags_NoDocking |
                                           ImGuiWindowFlags_NoTitleBar;
    ImGui::Begin("Main Viewport", 0, main_viewport_flags);
    ProcessPopups(this->bw_config, &this->triggers);
    ImGui::End(); //Main Viewport
}
