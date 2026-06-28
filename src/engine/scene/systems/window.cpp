/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/scene/systems/window.hpp>
#include <iostream>

// LCOV_EXCL_START
bool Engine::Scene::Systems::GLFWWindowImpl::init()
{
    return glfwInit();
}
void Engine::Scene::Systems::GLFWWindowImpl::terminate()
{
    glfwTerminate();
}
GLFWwindow *Engine::Scene::Systems::GLFWWindowImpl::createWindow(int width, int height, const char *title)
{
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return glfwCreateWindow(width, height, title, nullptr, nullptr);
}
void Engine::Scene::Systems::GLFWWindowImpl::makeContextCurrent(GLFWwindow *window)
{
    glfwMakeContextCurrent(window);
}
bool Engine::Scene::Systems::GLFWWindowImpl::loadGlad()
{
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}
// LCOV_EXCL_STOP

Engine::Scene::Systems::Window::Window(std::unique_ptr<IWindowImpl<GLFWwindow *>> impl)
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
Engine::Scene::Systems::Window::~Window()
{
    if (impl)
    {
        impl->terminate();
    }
}

GLFWwindow *Engine::Scene::Systems::Window::get() const
{
    return this->window;
}
bool Engine::Scene::Systems::Window::is_open() const
{
    return !glfwWindowShouldClose(this->window);
}
void Engine::Scene::Systems::Window::close() const
{
    glfwSetWindowShouldClose(this->window, true);
}
bool Engine::Scene::Systems::Window::isInitialized() const
{
    return this->window != nullptr;
}
void Engine::Scene::Systems::Window::glDeclarations() const
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}
void Engine::Scene::Systems::Window::refresh() const
{
    glfwSwapBuffers(this->window);
    glfwPollEvents();
}