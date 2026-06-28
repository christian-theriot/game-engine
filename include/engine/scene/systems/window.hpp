#ifndef __ENGINE_SCENE_SYSTEMS_WINDOW_HPP
#define __ENGINE_SCENE_SYSTEMS_WINDOW_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/scene/system.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

namespace Engine::Scene::Systems
{
    template <typename WindowHandle>
    class IWindowImpl
    {
    public:
        virtual ~IWindowImpl() = default;
        virtual bool init() = 0;
        virtual void terminate() = 0;
        virtual WindowHandle createWindow(int width, int height, const char *title) = 0;
        virtual void makeContextCurrent(WindowHandle window) = 0;
        virtual bool loadGlad() { return true; }
    };

    class GLFWWindowImpl : public IWindowImpl<GLFWwindow *>
    {
    public:
        bool init() override;
        void terminate() override;
        GLFWwindow *createWindow(int width, int height, const char *title) override;
        void makeContextCurrent(GLFWwindow *window) override;
        bool loadGlad() override;
    };

    class Window : public Scene::System
    {
        std::unique_ptr<IWindowImpl<GLFWwindow *>> impl;
        GLFWwindow *window;

    public:
        Window(std::unique_ptr<IWindowImpl<GLFWwindow *>> impl = std::make_unique<GLFWWindowImpl>());
        ~Window();

        bool is_open() const;
        GLFWwindow *get() const;
        void close() const;
        void glDeclarations() const;
        void refresh() const;
        void update(Scene::World *world, float deltaTime) override { refresh(); };

        bool isInitialized() const;
    };
}

#endif