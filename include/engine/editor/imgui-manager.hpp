#ifndef __ENGINE_EDITOR_IMGUI_MANAGER_HPP
#define __ENGINE_EDITOR_IMGUI_MANAGER_HPP
/*
 * Copyright (C) 2026 Christian Theriot
 */

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Engine::Editor
{
    class ImGuiManager
    {
        GLFWwindow *window;

    public:
        ImGuiManager(GLFWwindow *window, const char *glsl_version);
        ~ImGuiManager();

        void beginFrame();
        void endFrame();
        void render();
    };
}

#endif