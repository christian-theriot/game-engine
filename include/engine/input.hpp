#ifndef __ENGINE_INPUT_HPP
#define __ENGINE_INPUT_HPP

#include <engine/window.hpp>
#include <engine/events.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

namespace Engine
{
    class KeyEvent : public Event
    {
        int key;
        int scancode;
        int action;
        int mods;

    public:
        KeyEvent(int key, int scancode, int action, int mods);

        int getKey() const;
        int getScancode() const;
        int getAction() const;
        int getMods() const;
    };

    class CharacterEvent : public Event
    {
        unsigned int codepoint;

    public:
        CharacterEvent(unsigned int codepoint);

        unsigned int getCodepoint() const;
    };

    class CursorEvent : public Event
    {
        double xpos;
        double ypos;

    public:
        CursorEvent(double xpos, double ypos);

        double getX() const;
        double getY() const;
    };

    class CursorEnterEvent : public Event
    {
        bool entered;

    public:
        CursorEnterEvent(bool entered);

        bool get() const;
    };

    class MouseButtonEvent : public Event
    {
        int button;
        int action;
        int mods;

    public:
        MouseButtonEvent(int button, int action, int mods);

        int getButton() const;
        int getAction() const;
        int getMods() const;
    };

    class ScrollEvent : public Event
    {
        double xoffset;
        double yoffset;

    public:
        ScrollEvent(double xoffset, double yoffset);

        double getX() const;
        double getY() const;
    };

    class JoystickEvent : public Event
    {
        int jid;
        int event;

    public:
        JoystickEvent(int jid, int event);

        int getID() const;
        int getEvent() const;
    };

    class DropEvent : public Event
    {
        std::vector<std::string> paths;

    public:
        DropEvent(int count, const char **paths);

        const std::vector<std::string> &getPaths() const;
    };

    class Input
    {
        static EventBus *events;

    public:
        Input(const Window &window, EventBus &events);

        static void setEventBus(EventBus *events);

        static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void characterCallback(GLFWwindow *window, unsigned int codepoint);
        static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
        static void cursorEnterCallback(GLFWwindow *window, int entered);
        static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
        static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
        static void joystickCallback(int jid, int event);
        static void dropCallback(GLFWwindow *window, int count, const char **paths);
    };
}

#endif