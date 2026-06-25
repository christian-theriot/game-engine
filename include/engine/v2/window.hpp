#ifndef __ENGINE_V2_WINDOW_HPP
#define __ENGINE_V2_WINDOW_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

namespace Engine
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

    class Window
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

        bool isInitialized() const;
    };
}

#endif