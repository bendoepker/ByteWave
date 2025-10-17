#ifndef APPLICATION_H
#define APPLICATION_H
#include <imgui.h>
#include <glad/include/glad/gl.h>
#include <backend/imgui_impl_glfw.h>
#include <backend/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <types.h>
#include <log.h>

typedef struct _UITriggers {
    bool open_backend_popup = false;
} UITriggers;

class Application {
    GLFWwindow* window;
    bool failed_init = false;
    bool enable_vsync = false;
    bool dark_mode = true;
    ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 1.f);
    BWConfigData* bw_config = 0;
    UITriggers triggers;

    void ConfigStartup();
    void ConfigShutdown();
    void RenderFrame();
public:

    void Run();
    Application();
    ~Application();

    float scale = 1.0;
};
#endif //APLICATION_H
