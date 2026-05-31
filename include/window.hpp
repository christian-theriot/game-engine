#ifndef __WINDOW_HPP
#define __WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
    GLFWwindow *window;

public:
    Window();
    ~Window();

    bool is_open() const;
    GLFWwindow *get() const;
};

#endif