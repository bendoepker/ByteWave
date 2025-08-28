#include <bw-ui.h>
#include <imgui.h>
#include <backend/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <bw-log.h>

void* BWUI_UIMain(void* data) {
    BW_PRINT("HERE");
    if(!glfwInit()) {
        return 0;
    }
    GLFWwindow* window = glfwCreateWindow(1280, 800, "ByteWave", NULL, NULL);
    glfwMakeContextCurrent(window);

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
