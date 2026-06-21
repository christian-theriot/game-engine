/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/window.hpp>
#include <iostream>

// LCOV_EXCL_START
bool Engine::GLFWWindowImpl::init()
{
    return glfwInit();
}
void Engine::GLFWWindowImpl::terminate()
{
    glfwTerminate();
}
GLFWwindow *Engine::GLFWWindowImpl::createWindow(int width, int height, const char *title)
{
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return glfwCreateWindow(width, height, title, nullptr, nullptr);
}
void Engine::GLFWWindowImpl::makeContextCurrent(GLFWwindow *window)
{
    glfwMakeContextCurrent(window);
}
bool Engine::GLFWWindowImpl::loadGlad()
{
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}
// LCOV_EXCL_STOP

Engine::Window::Window(std::unique_ptr<IWindowImpl<GLFWwindow *>> impl)
    : impl(std::move(impl)),
      window(nullptr)
{
    if (!this->impl->init())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return;
    }

    this->window = this->impl->createWindow(1024, 768, "Game Engine");
    if (this->window == nullptr)
    {
        std::cout << "Failed to open GLFW window" << std::endl;
        return;
    }

    this->impl->makeContextCurrent(this->window);

    if (!this->impl->loadGlad())
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
}
Engine::Window::~Window()
{
    if (impl)
    {
        impl->terminate();
    }
}

GLFWwindow *Engine::Window::get() const
{
    return this->window;
}
bool Engine::Window::is_open() const
{
    return !glfwWindowShouldClose(this->window);
}
void Engine::Window::close() const
{
    glfwSetWindowShouldClose(this->window, true);
}
bool Engine::Window::isInitialized() const
{
    return this->window != nullptr;
}