#include "application.h"
#include "prompts.h"

void Application::RenderFrame() {
    ImGui::Begin("Main Viewport");
    if(this->triggers.open_backend_popup)
        PromptForAudioBackend(this->bw_config, &this->triggers);

    AudioBackendPopup();
    ImGui::End(); //Main Viewport
}
