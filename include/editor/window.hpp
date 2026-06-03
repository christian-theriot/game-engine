#ifndef __EDITOR_WINDOW_HPP
#define __EDITOR_WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Editor
{
    class Window
    {
        GLFWwindow *window;

    public:
        Window();
        ~Window();

        bool is_open() const;
        GLFWwindow *get() const;
    };
}

#endif