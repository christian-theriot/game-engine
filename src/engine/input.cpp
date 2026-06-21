/**
 * Copyright (C) 2026 Christian Theriot
 */
#include <engine/input.hpp>
#include <iostream>

Engine::EventBus *Engine::Input::events = nullptr;

Engine::KeyEvent::KeyEvent(int key, int scancode, int action, int mods)
    : key(key),
      scancode(scancode),
      action(action),
      mods(mods)
{
}
int Engine::KeyEvent::getKey() const
{
    return key;
}
int Engine::KeyEvent::getScancode() const
{
    return scancode;
}
int Engine::KeyEvent::getAction() const
{
    return action;
}
int Engine::KeyEvent::getMods() const
{
    return mods;
}

Engine::CharacterEvent::CharacterEvent(unsigned int codepoint)
    : codepoint(codepoint)
{
}
unsigned int Engine::CharacterEvent::getCodepoint() const
{
    return codepoint;
}

Engine::CursorEvent::CursorEvent(double xpos, double ypos)
    : xpos(xpos),
      ypos(ypos)
{
}
double Engine::CursorEvent::getX() const
{
    return xpos;
}
double Engine::CursorEvent::getY() const
{
    return ypos;
}

Engine::CursorEnterEvent::CursorEnterEvent(bool entered)
    : entered(entered)
{
}
bool Engine::CursorEnterEvent::get() const
{
    return entered;
}

Engine::MouseButtonEvent::MouseButtonEvent(int button, int action, int mods)
    : button(button),
      action(action),
      mods(mods)
{
}
int Engine::MouseButtonEvent::getButton() const
{
    return button;
}
int Engine::MouseButtonEvent::getAction() const
{
    return action;
}
int Engine::MouseButtonEvent::getMods() const
{
    return mods;
}

Engine::ScrollEvent::ScrollEvent(double xoffset, double yoffset)
    : xoffset(xoffset),
      yoffset(yoffset)
{
}
double Engine::ScrollEvent::getX() const
{
    return xoffset;
}
double Engine::ScrollEvent::getY() const
{
    return yoffset;
}

Engine::JoystickEvent::JoystickEvent(int jid, int event)
    : jid(jid),
      event(event)
{
}
int Engine::JoystickEvent::getID() const
{
    return jid;
}
int Engine::JoystickEvent::getEvent() const
{
    return event;
}

Engine::DropEvent::DropEvent(int count, const char **paths)
{
    for (int i = 0; i < count; i++)
    {
        this->paths.emplace_back(paths[i]);
    }
}
const std::vector<std::string> &Engine::DropEvent::getPaths() const
{
    return paths;
}

void Engine::Input::setEventBus(EventBus *events)
{
    Input::events = events;
}

Engine::Input::Input(const Window &window, EventBus &events)
{
    setEventBus(&events);
    glfwSetInputMode(window.get(), GLFW_STICKY_KEYS, GL_TRUE);

    glfwSetKeyCallback(window.get(), keyCallback);
    glfwSetCharCallback(window.get(), characterCallback);
    glfwSetCursorPosCallback(window.get(), cursorPosCallback);
    glfwSetCursorEnterCallback(window.get(), cursorEnterCallback);
    glfwSetMouseButtonCallback(window.get(), mouseButtonCallback);
    glfwSetScrollCallback(window.get(), scrollCallback);
    glfwSetJoystickCallback(joystickCallback);
    glfwSetDropCallback(window.get(), dropCallback);
}
void Engine::Input::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    events->publish(KeyEvent(key, scancode, action, mods));
}
void Engine::Input::characterCallback(GLFWwindow *window, unsigned int codepoint)
{
    events->publish(CharacterEvent(codepoint));
}
void Engine::Input::cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    events->publish(CursorEvent(xpos, ypos));
}
void Engine::Input::cursorEnterCallback(GLFWwindow *window, int entered)
{
    events->publish(CursorEnterEvent(entered));
}
void Engine::Input::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    events->publish(MouseButtonEvent(button, action, mods));
}
void Engine::Input::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    events->publish(ScrollEvent(xoffset, yoffset));
}
void Engine::Input::joystickCallback(int jid, int event)
{
    events->publish(JoystickEvent(jid, event));
}
void Engine::Input::dropCallback(GLFWwindow *window, int count, const char **paths)
{
    events->publish(DropEvent(count, paths));
}