#ifndef __ENGINE_V2_SCENE_SYSTEMS_INPUT_HPP
#define __ENGINE_V2_SCENE_SYSTEMS_INPUT_HPP
/**
 * Copyright (C) 2026 Christian Theriot
 */

#include <engine/v2/events.hpp>
#include <engine/v2/scene/system.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine
{
    class Window;
    class KeyEvent : public Event
    {
        int key;
        int scancode;
        int action;
        int mods;

    public:
        inline KeyEvent(int key, int scancode, int action, int mods)
            : key(key), scancode(scancode), action(action), mods(mods) {}
        inline int getKey() const { return key; }
        inline int getScancode() const { return scancode; }
        inline int getAction() const { return action; }
        inline int getMods() const { return mods; }
    };

    class CharacterEvent : public Event
    {
        unsigned int codepoint;

    public:
        inline CharacterEvent(unsigned int codepoint) : codepoint(codepoint) {}
        inline unsigned int getCodepoint() const { return codepoint; }
    };

    class CursorEvent : public Event
    {
        double xpos;
        double ypos;

    public:
        inline CursorEvent(double xpos, double ypos) : xpos(xpos), ypos(ypos) {}
        inline double getX() const { return xpos; }
        inline double getY() const { return ypos; }
    };

    class CursorEnterEvent : public Event
    {
        bool entered;

    public:
        inline CursorEnterEvent(bool entered) : entered(entered) {}
        inline bool get() const { return entered; }
    };

    class MouseButtonEvent : public Event
    {
        int button;
        int action;
        int mods;

    public:
        inline MouseButtonEvent(int button, int action, int mods)
            : button(button), action(action), mods(mods) {}
        inline int getButton() const { return button; }
        inline int getAction() const { return action; }
        inline int getMods() const { return mods; }
    };

    class ScrollEvent : public Event
    {
        double xoffset;
        double yoffset;

    public:
        inline ScrollEvent(double xoffset, double yoffset) : xoffset(xoffset), yoffset(yoffset) {}
        inline double getXOffset() const { return xoffset; }
        inline double getYOffset() const { return yoffset; }
    };

    class JoystickEvent : public Event
    {
        int jid;
        int event;

    public:
        inline JoystickEvent(int jid, int event) : jid(jid), event(event) {}
        inline int getID() const { return jid; }
        inline int getEvent() const { return event; }
    };

    class DropEvent : public Event
    {
        std::vector<std::string> paths;

    public:
        inline DropEvent(int count, const char **paths)
        {
            for (int i = 0; i < count; i++)
                this->paths.emplace_back(paths[i]);
        }
        inline const std::vector<std::string> &getPaths() const { return paths; }
    };

}

namespace Engine::Scene::Systems
{
    class InputSystem : public Scene::System
    {
        static EventBus *events;

    public:
        InputSystem(const Window *window, EventBus *events);
        static void setEventBus(EventBus *eventBus) { events = eventBus; }
        static void handleKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void handleCharacterEvent(GLFWwindow *window, unsigned int codepoint);
        static void handleCursorEvent(GLFWwindow *window, double xpos, double ypos);
        static void handleCursorEnterEvent(GLFWwindow *window, int entered);
        static void handleMouseButtonEvent(GLFWwindow *window, int button, int action, int mods);
        static void handleScrollEvent(GLFWwindow *window, double xoffset, double yoffset);
        static void handleJoystickEvent(int jid, int event);
        static void handleDropEvent(GLFWwindow *window, int count, const char **paths);
        inline void update(World *world, float deltaTime) override {}
    };
}

#endif