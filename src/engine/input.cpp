#include <engine/input.hpp>
#include <engine/window.hpp>

Engine::EventBus *Engine::Input::events = nullptr;

Engine::Input::Input(const Window *window, EventBus *events)
{
    setEventBus(events);
    glfwSetKeyCallback(window->get(), handleKeyEvent);
    glfwSetCharCallback(window->get(), handleCharacterEvent);
    glfwSetCursorPosCallback(window->get(), handleCursorEvent);
    glfwSetCursorEnterCallback(window->get(), handleCursorEnterEvent);
    glfwSetMouseButtonCallback(window->get(), handleMouseButtonEvent);
    glfwSetScrollCallback(window->get(), handleScrollEvent);
    glfwSetJoystickCallback(handleJoystickEvent);
    glfwSetDropCallback(window->get(), handleDropEvent);
}
void Engine::Input::handleKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    events->publish(KeyEvent(key, scancode, action, mods));
}
void Engine::Input::handleCharacterEvent(GLFWwindow *window, unsigned int codepoint)
{
    events->publish(CharacterEvent(codepoint));
}
void Engine::Input::handleCursorEvent(GLFWwindow *window, double xpos, double ypos)
{
    events->publish(CursorEvent(xpos, ypos));
}
void Engine::Input::handleCursorEnterEvent(GLFWwindow *window, int entered)
{
    events->publish(CursorEnterEvent(entered));
}
void Engine::Input::handleMouseButtonEvent(GLFWwindow *window, int button, int action, int mods)
{
    events->publish(MouseButtonEvent(button, action, mods));
}
void Engine::Input::handleScrollEvent(GLFWwindow *window, double xoffset, double yoffset)
{
    events->publish(ScrollEvent(xoffset, yoffset));
}
void Engine::Input::handleJoystickEvent(int jid, int event)
{
    events->publish(JoystickEvent(jid, event));
}
void Engine::Input::handleDropEvent(GLFWwindow *window, int count, const char **paths)
{
    events->publish(DropEvent(count, paths));
}
