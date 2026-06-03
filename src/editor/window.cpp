#include <editor/window.hpp>
#include <iostream>

Window::Window()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow(1024, 768, "Game Engine", nullptr, nullptr);
    if (this->window == nullptr)
    {
        std::cout << "Failed to open GLFW window" << std::endl;
        return;
    }

    glfwMakeContextCurrent(this->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
}
Window::~Window()
{
    glfwTerminate();
}

GLFWwindow *Window::get() const
{
    return this->window;
}
bool Window::is_open() const
{
    return !glfwWindowShouldClose(this->window);
}