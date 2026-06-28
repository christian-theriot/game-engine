#include <engine/v2/scene/systems/input.hpp>
#include <engine/v2/scene/systems/window.hpp>

Engine::Scene::Systems::EventBus *Engine::Scene::Systems::Input::events = nullptr;

Engine::Scene::Systems::Input::Input(const Window *window, EventBus *events)
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
void Engine::Scene::Systems::Input::handleKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    events->publish(KeyEvent(key, scancode, action, mods));
}
void Engine::Scene::Systems::Input::handleCharacterEvent(GLFWwindow *window, unsigned int codepoint)
{
    events->publish(CharacterEvent(codepoint));
}
void Engine::Scene::Systems::Input::handleCursorEvent(GLFWwindow *window, double xpos, double ypos)
{
    events->publish(CursorEvent(xpos, ypos));
}
void Engine::Scene::Systems::Input::handleCursorEnterEvent(GLFWwindow *window, int entered)
{
    events->publish(CursorEnterEvent(entered));
}
void Engine::Scene::Systems::Input::handleMouseButtonEvent(GLFWwindow *window, int button, int action, int mods)
{
    events->publish(MouseButtonEvent(button, action, mods));
}
void Engine::Scene::Systems::Input::handleScrollEvent(GLFWwindow *window, double xoffset, double yoffset)
{
    events->publish(ScrollEvent(xoffset, yoffset));
}
void Engine::Scene::Systems::Input::handleJoystickEvent(int jid, int event)
{
    events->publish(JoystickEvent(jid, event));
}
void Engine::Scene::Systems::Input::handleDropEvent(GLFWwindow *window, int count, const char **paths)
{
    events->publish(DropEvent(count, paths));
}
