#ifndef __ENGINE_WINDOW_HPP
#define __ENGINE_WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine
{
    class Window
    {
        GLFWwindow *window;

    public:
        Window();
        ~Window();

        bool is_open() const;
        GLFWwindow *get() const;
        void close() const;
    };
}

#endif