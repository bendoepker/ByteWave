#include "application.h"
#include "style.h"
#include "windows/mixer.cpp"
#include <stdlib.h>
#include <serializer.h>
#include <hostapi.h>

const char* config_location = "test-config.bwc";

static void glfw_error_callback(int error, const char* description)
{
    BW_LOG_ERR("GLFW Error %d: %s\n", error, description);
}

void Application::ConfigStartup() {
    this->bw_config = (BWConfigData*)malloc(sizeof(*this->bw_config));
    if(!this->bw_config)
        (void)0; //TODO: Handle out of memory error
    BWError ret = BWConfig_Read(config_location, this->bw_config);
    BWHostApi_Initialize(this->bw_config);
    if(ret != BW_OK || this->bw_config->host_api == UNKNOWN) {
        this->triggers.open_backend_popup = true;
    }
}

void Application::ConfigShutdown() {
    if(BWHostApi_IsActivated())
        BWHostApi_Deactivate();
    BWHostApi_Terminate();
    BWConfig_Write(config_location, this->bw_config);
    free(this->bw_config);
}

Application::Application() {
    //TODO: Load a config here with any video settings
    const char* glsl_version = "#version 130";

    glfwSetErrorCallback(glfw_error_callback);
    if(!glfwInit()) {
        failed_init = true;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //NOTE: This will be changed when i create a custom titlebar
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

    window = glfwCreateWindow((int)(1280 * scale), (int)(800 * scale), "ByteWave", nullptr, nullptr);
    if(!window) {
        failed_init = true;
        return;
    }
    glfwMakeContextCurrent(window);
    if(enable_vsync)
        glfwSwapInterval(1);

    if(!gladLoadGL(glfwGetProcAddress)) {
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard |
                      //ImGuiConfigFlags_ViewportsEnable |
                      ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    if(dark_mode)
        BWUI::CreateDarkStyle(ImGui::GetStyle(), *this);
    else
        BWUI::CreateLightStyle(ImGui::GetStyle(), *this);

    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ConfigStartup();
}

Application::~Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if(window) glfwDestroyWindow(window);
    glfwTerminate();
}

// The Run function should be used to create hooks such as OnUpdate(), OnXYZ(), Render(), etc. there should not be
// any user facing UI code within this function
void Application::Run() {
    if(failed_init)
        return;

    ImGui::EndFrame();
    ImGui::UpdatePlatformWindows();

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if(glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // All of the UI Code should be put inside of this function
        this->RenderFrame();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);

        glfwSwapBuffers(window);
    }
}
